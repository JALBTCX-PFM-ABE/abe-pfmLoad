
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! or / / ! are being used by Doxygen to
    document the software.  Dashes in these comment blocks are used to create bullet lists.
    The lack of blank lines after a block of dash preceeded comments means that the next
    block of dash preceeded comments is a new, indented bullet list.  I've tried to keep the
    Doxygen formatting to a minimum but there are some other items (like <br> and <pre>)
    that need to be left alone.  If you see a comment that starts with / * ! or / / ! and
    there is something that looks a bit weird it is probably due to some arcane Doxygen
    syntax.  Be very careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "pfmLoad.hpp"


uint8_t readParameterFile (QString parameter_file, QStringList *input_files, PFM_DEFINITION *pfm_def, PFM_GLOBAL *pfm_global, FLAGS *flags)
{
  char file[1024];
  strcpy (file, parameter_file.toLatin1 ());
  FILE *fp;

  if ((fp = fopen (file, "r")) == NULL)
    {
      QString tmp = pfmLoad::tr ("Error opening parameter file : %1\nReason: %2").arg (parameter_file).arg (strerror (errno));
      QMessageBox::warning (0, pfmLoad::tr ("pfmLoad load parameter file"), tmp);
      return (NVFalse);
    }


  char string[1024];
  int32_t pfm_index = 0;
  uint8_t input_started = NVFalse;
  QString qstring, cut, half_cut;


  //  Clear the attributes that we picked up from our application defaults in envin_out.cpp

  pfm_global->attribute_count = 0;

  pfm_global->time_attribute_num = 0;
  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++) pfm_global->gsf_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++) pfm_global->hof_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++) pfm_global->tof_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++) pfm_global->czmil_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++) pfm_global->las_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++) pfm_global->bag_attribute_num[i] = 0;


  while (fgets (string, sizeof (string), fp) != NULL)
    {
      qstring = string;

      if (qstring.contains ("End Input Files")) break;


      if (qstring.contains ("PFM Definition "))
        {
          cut = qstring.section ("PFM Definition ", 1, 1);
          pfm_index = cut.mid (0, 3).toInt ();
        }


      //  The [PFM Directory] = string allows us to use old .prm files without barfing.
 
      if (qstring.contains ("[PFM Handle File] = ") || qstring.contains ("[PFM Directory] = "))
        {
          cut = qstring.section (" = ", 1, 1);
          pfm_def[pfm_index].name = cut.trimmed ();


          //  Check to see if this is a pre-existing PFM.  If so, set the flag.

          PFM_OPEN_ARGS open_args;
          int32_t hnd;

          strcpy (open_args.list_path, pfm_def[pfm_index].name.toLatin1 ());
          open_args.checkpoint = 0;

          if ((hnd = open_existing_pfm_file (&open_args)) >= 0)
            {
              close_pfm_file (hnd);
              pfm_def[pfm_index].existing = NVTrue;
            }
        }

      if (qstring.contains ("[Bin Size Meters] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].mbin_size = cut.toFloat ();
        }

      if (qstring.contains ("[Bin Size Minutes] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].gbin_size = cut.toFloat ();
        }

      if (qstring.contains ("[Minimum Depth] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].min_depth = cut.toFloat ();
        }

      if (qstring.contains ("[Maximum Depth] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].max_depth = cut.toFloat ();
        }

      if (qstring.contains ("[Depth Precision] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].precision = cut.toFloat ();
        }

      if (qstring.contains ("[Area File] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].area = cut.trimmed ();
        }

      if (qstring.contains ("[Polygon Latitude,Longitude] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          half_cut = cut.section (',', 0, 0);
          pfm_def[pfm_index].polygon[pfm_def[pfm_index].polygon_count].y = half_cut.toDouble ();
          half_cut = cut.section (',', 1, 1);
          pfm_def[pfm_index].polygon[pfm_def[pfm_index].polygon_count].x = half_cut.toDouble ();
          pfm_def[pfm_index].polygon_count++;

          pfm_def[pfm_index].area = "Defined in parameter file";
        }

      if (qstring.contains ("[Mosaic File] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].mosaic = cut.trimmed ();
        }

      if (qstring.contains ("[Feature File] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].feature = cut.trimmed ();
        }

      if (qstring.contains ("[Apply Area Filter Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].apply_area_filter = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Deep Filter Only Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].deep_filter_only = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Area Filter Bin Standard Deviation] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].cellstd = cut.toFloat ();
        }

      if (qstring.contains ("[Area Filter Feature Radius] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_def[pfm_index].radius = cut.toFloat ();
        }

      /*
      if (qstring.contains ("[Maximum Input Files] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->max_files = cut.toInt ();
        }

      if (qstring.contains ("[Maximum Input Lines] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->max_lines = cut.toInt ();
        }

      if (qstring.contains ("[Maximum Input Pings] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->max_pings = cut.toInt ();
        }

      if (qstring.contains ("[Maximum Input Beams] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->max_beams = cut.toInt ();
        }
      */


      if (qstring.contains ("[Cached Memory Size] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->cache_mem = cut.toInt ();
        }


      //  Check for the attribute flags which will be saved as [Attribute name] without an equals sign.
      //  If we are reading parameter files we want to override the saved QSettings for attributes.

      if (qstring.contains (pfm_global->time_attribute.name))
        {
          pfm_global->attribute_count++;
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->time_attribute_num = cut.toInt ();
        }


      for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->gsf_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->gsf_attribute_num[i] = cut.toInt ();
              break;
            }
        }

      for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->hof_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->hof_attribute_num[i] = cut.toInt ();
              break;
            }
        }

      for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->tof_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->tof_attribute_num[i] = cut.toInt ();
              break;
            }
        }

      for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->czmil_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->czmil_attribute_num[i] = cut.toInt ();
              break;
            }
        }

      for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->las_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->las_attribute_num[i] = cut.toInt ();
              break;
            }
        }

      for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
        {
          //  Add the brackets so we don't get a partial hit if the names are similar.

          QString bracketed_string = "[" + QString (pfm_global->bag_attribute[i].name) + "]";


          if (qstring.contains (bracketed_string))
            {
              pfm_global->attribute_count++;
              cut = qstring.section (" = ", 1, 1).trimmed ();
              pfm_global->bag_attribute_num[i] = cut.toInt ();
              break;
            }
        }
          

      if (qstring.contains ("[Load GSF Nominal Depth Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->nom = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Insert CUBE Attributes Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->attr = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Reference Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->ref = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Invert Substitute Paths Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->sub = (uint8_t) (cut.toInt ());
        }



      if (qstring.contains ("[Load HOF GCS Compatible Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->old = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[HOF Load Null Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->hof = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Invalidate HOF Land Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->lnd = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Invalidate HOF Secondary Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->sec = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Reference TOF First Return Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->tof = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Reference Water Surface Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->ref_water_surface = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Water Surface Load Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_water_surface = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Land Load Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_land = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Water Load Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_water = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL HydroFusion Filter Invalid Load Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->HF_filt = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 1 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 2 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 3 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 4 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 5 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 6 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Shallow Channel 7 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL IR Channel Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_IR_CHANNEL] = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[CZMIL Deep Channel Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->czmil_chan[CZMIL_DEEP_CHANNEL] = (uint8_t) (cut.toInt ());
        }


      if (qstring.contains ("[Load Best SRTM Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtmb = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Load 1 Second SRTM Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtm1 = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Load 3 Second SRTM Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtm3 = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Load 30 Second SRTM Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtm30 = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Load SRTM As Reference Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtmr = (uint8_t) (cut.toInt ());
        }

      if (qstring.contains ("[Exclude SRTM2 Data Flag] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          flags->srtme = (uint8_t) (cut.toInt ());
        }



      if (qstring.contains ("[Default Horizontal Error] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->horizontal_error = cut.toInt ();
        }

      if (qstring.contains ("[Default Vertical Error] = "))
        {
          cut = qstring.section (" = ", 1, 1).trimmed ();
          pfm_global->vertical_error = cut.toInt ();
        }



      if (input_started) input_files->append (qstring.trimmed ());
                  
      if (qstring.contains ("**  Input Files  **")) input_started = NVTrue;
    }


  fclose (fp);


  return (NVTrue);
}
