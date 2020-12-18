
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
#include "pfmLoadHelp.hpp"
#include "version.hpp"


static QProgressBar *prog;


#ifdef NVLinux


static int32_t substitute_cnt;
static char substitute_path[10][3][1024];


static void sub_in ()
{
  char        varin[1024], info[1024];
  FILE        *fp;


  substitute_cnt = 0;


  //  If the startup file was found...
    
  if ((fp = find_startup (".pfm_cfg")) != NULL)
    {
      //  Read each entry.
        
      while (ngets (varin, sizeof (varin), fp) != NULL)
        {
	  if (varin[0] != '#')
	    {
	      //  Check input for matching strings and load values if found.
            
	      if (strstr (varin, "[SUBSTITUTE PATH]") != NULL && substitute_cnt < 10)
		{
		  //  Put everything to the right of the equals sign in 'info'.
            
		  get_string (varin, info);


		  /*  Throw out malformed substitute paths.  */

		  if (strchr (info, ','))
		    {
		      /*  Check for more than 1 UNIX substitute path.  */

		      if (strchr (info, ',') == strrchr (info, ','))
			{
			  strcpy (substitute_path[substitute_cnt][0], strtok (info, ","));
			  strcpy (substitute_path[substitute_cnt][1], strtok (NULL, ","));
			  substitute_path[substitute_cnt][2][0] = 0;
			}
		      else
			{
			  strcpy (substitute_path[substitute_cnt][0], strtok (info, ","));
			  strcpy (substitute_path[substitute_cnt][1], strtok (NULL, ","));
			  strcpy (substitute_path[substitute_cnt][2], strtok (NULL, ","));
			}

		      substitute_cnt++;
		    }
		}
	    }
	}

      fclose (fp);
    }
}

#endif



pfmLoad::pfmLoad (int *argc __attribute__ ((unused)), char **argv, QWidget *parent, QString parm_file)
  : QWizard (parent)
{
  uint8_t readParameterFile (QString parameter_file, QStringList *input_files, PFM_DEFINITION *pfm_def, PFM_GLOBAL *pfm_global, FLAGS *flags);


  strcpy (pfm_global.progname, argv[0]);
  parameter_file = parm_file;
  upr_file = NVFalse;
  csf_attr = NVFalse;
  pfm_global.input_dirs.clear ();


  QResource::registerResource ("/icons.rcc");


  //  Set the main icon

  setWindowIcon (QIcon (":/icons/pfmLoadWatermark.png"));


  //  Set the window title (for now).

  setWindowTitle (VERSION);


  /*  Override the HDF5 version check so that we can read BAGs created with an older version of HDF5.  */

  putenv ((char *) "HDF5_DISABLE_VERSION_CHECK=2");


  global_dialog.gsfD = global_dialog.hofD = global_dialog.tofD = global_dialog.czmilD = global_dialog.lasD = global_dialog.bagD = NULL;


  //  Dangerous things should always default to the least dangerous mode so we don't save and restore
  //  these through envin/envout

  pfm_global.max_files = NINT (pow (2.0, (double) PFM_FILE_BITS)) - 1;
  pfm_global.max_lines = NINT (pow (2.0, (double) PFM_LINE_BITS)) - 1;
  pfm_global.max_pings = NINT (pow (2.0, (double) PFM_PING_BITS)) - 1;
  pfm_global.max_beams = NINT (pow (2.0, (double) PFM_BEAM_BITS)) - 1;
  flags.ref = NVFalse;


  //  Set the timezone to GMT, we'll use this later.

#ifdef NVWIN3X
 #ifdef __MINGW64__
  putenv((char *) "TZ=GMT");
  tzset();
 #else
  _putenv((char *) "TZ=GMT");
  _tzset();
#endif

  strcpy (loader_name, "pfmLoader.exe");
  strcpy (cube_name, "cube_pfm.exe");
#else
  putenv((char *) "TZ=GMT");
  tzset();

  strcpy (loader_name, "pfmLoader");
  strcpy (cube_name, "cube_pfm");
#endif


  cube_available = NVFalse;
  if (find_startup_name (cube_name) != NULL) cube_available = NVTrue;


  if (!find_startup_name (loader_name))
    {
      QMessageBox::critical (this, "pfmLoad", tr ("Cannot find the loader executable %1!\nTerminating!").arg (loader_name));
      exit (-1);
    }


  setTimeAttributes (&pfm_global.time_attribute);
  setGSFAttributes (pfm_global.gsf_attribute);
  setHOFAttributes (pfm_global.hof_attribute);
  setTOFAttributes (pfm_global.tof_attribute);
  setCZMILAttributes (pfm_global.czmil_attribute, pfm_global.czmil_attribute_description);
  setLASAttributes (pfm_global.las_attribute);
  setBAGAttributes (pfm_global.bag_attribute);


  envin (&pfm_global, &flags, &ref_def, &window_x, &window_width, &window_y, &window_height, &inputFilter);


  // Set the application font

  QApplication::setFont (pfm_global.font);

  /*
  countTimeAttributes (pfm_global.time_attribute_num, &pfm_global.attribute_count);
  countGSFAttributes (pfm_global.gsf_attribute_num, &pfm_global.attribute_count);
  countHOFAttributes (pfm_global.hof_attribute_num, &pfm_global.attribute_count);
  countTOFAttributes (pfm_global.tof_attribute_num, &pfm_global.attribute_count);
  countCZMILAttributes (pfm_global.czmil_attribute_num, &pfm_global.attribute_count);
  countLASAttributes (pfm_global.las_attribute_num, &pfm_global.attribute_count);
  countBAGAttributes (pfm_global.bag_attribute_num, &pfm_global.attribute_count);
  */

  setWizardStyle (QWizard::ClassicStyle);


  setOption (HaveHelpButton, true);
  setOption (ExtendedWatermarkPixmap, false);

  connect (this, SIGNAL (helpRequested ()), this, SLOT (slotHelpClicked ()));


  //  Set the window size and location from the defaults

  this->resize (window_width, window_height);
  this->move (window_x, window_y);


  //  Initialize all of the PFM definitions.

  for (int32_t i = 0 ; i < MAX_LOAD_FILES ; i++)
    {
      pfm_def[i].name = "";
      pfm_def[i].area = "";
      pfm_def[i].polygon_count = 0;
      pfm_def[i].index = i;
      pfm_def[i].mbin_size = ref_def.mbin_size;
      pfm_def[i].gbin_size = ref_def.gbin_size;
      pfm_def[i].min_depth = ref_def.min_depth;
      pfm_def[i].max_depth = ref_def.max_depth;
      pfm_def[i].precision = ref_def.precision;
      pfm_def[i].mosaic = "NONE";
      pfm_def[i].feature = "NONE";
      pfm_def[i].apply_area_filter = ref_def.apply_area_filter;
      pfm_def[i].deep_filter_only = ref_def.deep_filter_only;
      pfm_def[i].cellstd = ref_def.cellstd;
      pfm_def[i].radius = ref_def.radius;
      pfm_def[i].existing = NVFalse;
      memset (&pfm_def[i].open_args, 0, sizeof (PFM_OPEN_ARGS));
    }


  input_files.clear ();


  //  Check for an input parameter file (since we might get the cache memory size).
 
  if (!parameter_file.isEmpty ()) readParameterFile (parameter_file, &input_files, pfm_def, &pfm_global, &flags);


  //  Set up the wizard pages.

  setPage (0, new startPage (this, parameter_file, &pfm_global));

  for (int32_t i = 0 ; i < MAX_LOAD_FILES ; i++)
    {
      int32_t page_num = i + 1;

      setPage (page_num, new pfmPage (this, &pfm_def[i], &pfm_global, page_num));
    }

  setPage (MAX_LOAD_FILES + 2, new inputPage (this, &inputFilter, &inputFiles, &pfm_global));
  setPage (MAX_LOAD_FILES + 3, globalPg = new globalPage (this, &pfm_global, &flags, &global_dialog, cube_available));
  setPage (MAX_LOAD_FILES + 4, new runPage (this, &progress, &checkList));


  //  If we read any input files from a parameter file, add them to the file list.

  if (!input_files.empty ()) 
    {
      for (int32_t i = 0 ; i < input_files.size () ; i++) inputFiles->append (input_files.at (i));
    }


  prog = progress.fbar;


  setButtonText (QWizard::CustomButton1, tr ("&Load"));
  setOption (QWizard::HaveCustomButton1, true);
  button (QWizard::CustomButton1)->setToolTip (tr ("Start the PFM load process"));
  button (QWizard::CustomButton1)->setWhatsThis (runText);
  setButtonText (QWizard::CustomButton2, tr ("&Save"));
  setOption (QWizard::HaveCustomButton2, true);
  button (QWizard::CustomButton2)->setToolTip (tr ("Save the input parameters to a parameter (.prm or .upr)"));
  button (QWizard::CustomButton2)->setWhatsThis (saveText);
  setButtonText (QWizard::CustomButton3, tr ("&Save status"));
  setOption (QWizard::HaveCustomButton3, true);
  button (QWizard::CustomButton3)->setToolTip (tr ("Save the process status in the list view above (.txt)"));
  button (QWizard::CustomButton3)->setWhatsThis (saveStatus);
  connect (this, SIGNAL (customButtonClicked (int)), this, SLOT (slotCustomButtonClicked (int)));


  setStartId (0);
}



pfmLoad::~pfmLoad ()
{
}



void pfmLoad::initializePage (int id)
{
  QString regField, pfmIndex;


  button (QWizard::HelpButton)->setIcon (QIcon (":/icons/contextHelp.png"));
  button (QWizard::CustomButton1)->setEnabled (false);
  button (QWizard::CustomButton2)->setEnabled (false);
  button (QWizard::CustomButton3)->setEnabled (false);


  //  Start page

  if (!id)
    {

    }


  //  PFM pages.

  else if (id >= 1 && id < MAX_LOAD_FILES + 2)
    {
      if (id == 1) pfm_global.cache_mem = field ("mem").toDouble ();


      if (!hasVisitedPage (id))
        {
          int32_t pid = id - 1;
          int32_t prev_id = pid - 1;

          if (pid)
            {
              pfmIndex.sprintf ("%02d", prev_id);
              
              regField = "mBinSize" + pfmIndex;
              pfm_def[prev_id].mbin_size = field (regField).toDouble ();

              regField = "gBinSize" + pfmIndex;
              pfm_def[prev_id].gbin_size = field (regField).toDouble ();

              regField = "minDepth" + pfmIndex;
              pfm_def[prev_id].min_depth = field (regField).toDouble ();

              regField = "maxDepth" + pfmIndex;
              pfm_def[prev_id].max_depth = field (regField).toDouble ();

              regField = "precision" + pfmIndex;
              int32_t j = field (regField).toInt ();

              switch (j)
                {
                case 0:
                  pfm_def[prev_id].precision = 0.01;
                  break;

                case 1:
                  pfm_def[prev_id].precision = 0.10;
                  break;

                case 2:
                  pfm_def[prev_id].precision = 1.00;
                  break;
                }

              regField = "mosaic_edit" + pfmIndex;
              pfm_def[prev_id].mosaic = field (regField).toString ();

              regField = "feature_edit" + pfmIndex;
              pfm_def[prev_id].feature = field (regField).toString ();

              regField = "applyFilter" + pfmIndex;
              pfm_def[prev_id].apply_area_filter = field (regField).toBool ();

              regField = "deepFilter" + pfmIndex;
              pfm_def[prev_id].deep_filter_only = field (regField).toBool ();

              regField = "stdSpin" + pfmIndex;
              pfm_def[prev_id].cellstd = field (regField).toDouble ();

              regField = "featureRadius" + pfmIndex;
              pfm_def[prev_id].radius = field (regField).toDouble ();
            }


          //  Populate the next definition with whatever you put in the previous one unless it's already been 
          //  done (i.e. it has a PFM structure name).

          if (pid && pfm_def[pid].name.isEmpty ()) 
            {
              pfm_def[pid].max_depth = pfm_def[prev_id].max_depth;
              pfm_def[pid].min_depth = pfm_def[prev_id].min_depth;
              pfm_def[pid].precision = pfm_def[prev_id].precision;
              pfm_def[pid].mbin_size = pfm_def[prev_id].mbin_size;
              pfm_def[pid].gbin_size = pfm_def[prev_id].gbin_size;
              pfm_def[pid].apply_area_filter = pfm_def[prev_id].apply_area_filter;
              pfm_def[pid].deep_filter_only = pfm_def[prev_id].deep_filter_only;
              pfm_def[pid].cellstd = pfm_def[prev_id].cellstd;
              pfm_def[pid].radius = pfm_def[prev_id].radius;


              //  Save the latest definitions for writing to the settings file.

              ref_def.mbin_size = pfm_def[pid].mbin_size;
              ref_def.gbin_size = pfm_def[pid].gbin_size;
              ref_def.min_depth = pfm_def[pid].min_depth;
              ref_def.max_depth = pfm_def[pid].max_depth;
              ref_def.precision = pfm_def[pid].precision;
              ref_def.apply_area_filter = pfm_def[pid].apply_area_filter;
              ref_def.deep_filter_only = pfm_def[pid].deep_filter_only;
              ref_def.cellstd = pfm_def[pid].cellstd;
              ref_def.radius = pfm_def[pid].radius;
            }

          pfmPage *nextPage = (pfmPage *) page (id);
          nextPage->setFields (&pfm_def[pid]);
        }

      if (id > 1 && pfm_def[id].name.isEmpty () && pfm_def[id].area.isEmpty ())
        {
          page (id)->setButtonText (QWizard::NextButton, tr ("Finish"));
        }
      else
        {
          page (id)->setButtonText (QWizard::NextButton, tr ("Next"));
        }
    }


  //  Input page.

  else if (id == MAX_LOAD_FILES + 2)
    {
    }


  //  Global page

  else if (id == MAX_LOAD_FILES + 3)
    {
      countTimeAttributes (pfm_global.time_attribute_num, &pfm_global.attribute_count);

      switch (pfm_global.data_type)
        {
        case L_GSF:
          countGSFAttributes (pfm_global.gsf_attribute_num, &pfm_global.attribute_count);
          break;

        case L_HOF:
          countHOFAttributes (pfm_global.hof_attribute_num, &pfm_global.attribute_count);
          break;

        case L_TOF:
          countTOFAttributes (pfm_global.tof_attribute_num, &pfm_global.attribute_count);
          break;

        case L_CZMIL:
          countCZMILAttributes (pfm_global.czmil_attribute_num, &pfm_global.attribute_count);
          break;

        case L_LAS:
          countLASAttributes (pfm_global.las_attribute_num, &pfm_global.attribute_count);
          break;

        case L_BAG:
          countBAGAttributes (pfm_global.bag_attribute_num, &pfm_global.attribute_count);
          break;
        }


      //  Get the file count

      pfm_file_count = 0;
      for (int32_t i = 0 ; i < MAX_LOAD_FILES ; i++) 
        {
          if (pfm_def[i].name == "")
            {
              pfm_file_count = i;
              break;
            }
        }


      //  Since we're actually going to shell pfmLoader we don't need to check for pre-existing attributes here.  It will be done in 
      //  pfmLoader.  We do need to test to see if we are appending and to see if there are any CSF attributes in new PFMs.

      for (int32_t i = 0 ; i < pfm_file_count ; i++)
        {
          //  First, just check to see if it exists.

          FILE *fp;
          char fname[1024];

          strcpy (fname, pfm_def[i].name.toLatin1 ());


          //  Try to open the file.

          pfm_def[i].existing = NVFalse;
          if ((fp = fopen (fname, "r")) != NULL)
            {
              fclose (fp);
              pfm_def[i].existing = NVTrue;
            }


          //  Test for CSF attributes.

          for (int32_t k = 15 ; k < 20 ; k++)
            {
              if (pfm_global.czmil_attribute_num[k])
                {
                  csf_attr = NVTrue;
                  break;
                }
            }
        }


      if (flags.old) 
        QMessageBox::warning (this, tr ("pfmLoad GCS Compatible Mode"),
                              tr ("Do not use GCS compatible mode unless you are going to reprocess the data in the PFM using Optech's GCS program!"));

      globalPg->setFields (&pfm_global, &flags);


      if (check_srtm2_topo () && !flags.srtme && (flags.srtmb || flags.srtm1)) 
        QMessageBox::information (this, tr ("pfmLoad SRTM2 data"),
                                  tr ("WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n\n"
                                      "If your PFM file includes any areas outside of the United States it will be limited "
                                      "distribution due to the inclusion of NGA SRTM2 topography data.  If you do not want to "
                                      "include the SRTM2 data please select 'Exclude SRTM2 data' from the 'Optional flag "
                                      "settings' on the next (Global Options) page.\n\n"
                                      "WARNING WARNING WARNING WARNING WARNING WARNING WARNING"));
    }


  //  Run page.

  else if (id == MAX_LOAD_FILES + 4)
    {
      //  Shut down any of the global page dialogs if they were left open.

      if (global_dialog.gsfD) global_dialog.gsfD->close ();
      if (global_dialog.hofD) global_dialog.hofD->close ();
      if (global_dialog.tofD) global_dialog.tofD->close ();
      if (global_dialog.czmilD) global_dialog.czmilD->close ();
      if (global_dialog.lasD) global_dialog.lasD->close ();
      if (global_dialog.bagD) global_dialog.bagD->close ();

      global_dialog.gsfD = global_dialog.hofD = global_dialog.tofD = global_dialog.czmilD = global_dialog.lasD = global_dialog.bagD = NULL;


      button (QWizard::CustomButton1)->setEnabled (true);
      button (QWizard::CustomButton2)->setEnabled (true);


      for (int32_t i = 0 ; i < MAX_LOAD_FILES ; i++)
        {
          pfmIndex.sprintf ("%02d", i);

          regField = "pfm_file_edit" + pfmIndex;
          pfm_def[i].name = field (regField).toString ();


          //  Empty PFM name means we're done with defining PFM structures.

          if (pfm_def[i].name.isEmpty ()) break;


          //  We may have to append the .pfm

          if (!pfm_def[i].name.endsWith (".pfm")) pfm_def[i].name.append (".pfm");


          //  Save the output directory.  It might have been input manually instead of browsed.

          pfm_global.output_dir = QFileInfo (pfm_def[i].name).absoluteDir ().absolutePath ();


          regField = "area_edit" + pfmIndex;
          pfm_def[i].area = field (regField).toString ();


          //  Save the area directory.  The area file might have been created using areaCheck instead of browsed.

          pfm_global.area_dir = QFileInfo (pfm_def[i].area).absoluteDir ().absolutePath ();


          regField = "mBinSize" + pfmIndex;
          pfm_def[i].mbin_size = field (regField).toDouble ();

          regField = "gBinSize" + pfmIndex;
          pfm_def[i].gbin_size = field (regField).toDouble ();

          regField = "minDepth" + pfmIndex;
          pfm_def[i].min_depth = field (regField).toDouble ();

          regField = "maxDepth" + pfmIndex;
          pfm_def[i].max_depth = field (regField).toDouble ();

          regField = "precision" + pfmIndex;
          int32_t j = field (regField).toInt ();

          switch (j)
            {
            case 0:
              pfm_def[i].precision = 0.01;
              break;

            case 1:
              pfm_def[i].precision = 0.10;
              break;

            case 2:
              pfm_def[i].precision = 1.00;
              break;
            }

          regField = "mosaic_edit" + pfmIndex;
          pfm_def[i].mosaic = field (regField).toString ();

          regField = "feature_edit" + pfmIndex;
          pfm_def[i].feature = field (regField).toString ();

          regField = "applyFilter" + pfmIndex;
          pfm_def[i].apply_area_filter = field (regField).toBool ();

          regField = "deepFilter" + pfmIndex;
          pfm_def[i].deep_filter_only = field (regField).toBool ();

          regField = "stdSpin" + pfmIndex;
          pfm_def[i].cellstd = field (regField).toDouble ();

          regField = "featureRadius" + pfmIndex;
          pfm_def[i].radius = field (regField).toDouble ();


          flags.old = field ("hofOld").toBool ();
          flags.hof = field ("hofNull").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = field ("czmilChan1").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = field ("czmilChan2").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = field ("czmilChan3").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = field ("czmilChan4").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = field ("czmilChan5").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = field ("czmilChan6").toBool ();
          flags.czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = field ("czmilChan7").toBool ();
          flags.czmil_chan[CZMIL_IR_CHANNEL] = field ("czmilIRChan").toBool ();
          flags.czmil_chan[CZMIL_DEEP_CHANNEL] = field ("czmilDeepChan").toBool ();
          flags.sub = field ("subPath").toBool ();
          flags.lnd = field ("invLand").toBool ();
          flags.sec = field ("invSec").toBool ();
          flags.ref = field ("refData").toBool ();
          flags.expand = field ("expCheck").toBool ();
          flags.tof = field ("tofRef").toBool ();
          flags.nom = field ("gsfNominal").toBool ();
          flags.srtmb = field ("srtmBest").toBool ();
          flags.srtm1 = field ("srtmOne").toBool ();
          flags.srtm3 = field ("srtmThree").toBool ();
          flags.srtm30 = field ("srtmThirty").toBool ();
          flags.srtmr = field ("srtmRef").toBool ();
          flags.srtme = field ("srtm2Ex").toBool ();
          flags.attr = field ("cubeAttributes").toBool ();
          flags.ref_water_surface = field ("ref_ws").toBool ();
          flags.czmil_water_surface = field ("wsClass").toBool ();
          flags.czmil_land = field ("landClass").toBool ();
          flags.czmil_water = field ("waterClass").toBool ();
          flags.HF_filt = field ("HFfilt").toBool ();

          pfm_global.horizontal_error = field ("horizontal").toDouble ();
          pfm_global.vertical_error = field ("vertical").toDouble ();
        }


      //  If the CUBE attributes is checked and the default H/V errors are set to 0.0, set them to a reasonable default.

      if (flags.attr)
	{
	  if (pfm_global.horizontal_error == 0.0) pfm_global.horizontal_error = 1.5;
	  if (pfm_global.vertical_error == 0.0) pfm_global.vertical_error = 0.25;
	}


      //  Save the settings.

      //  Use frame geometry to get the absolute x and y.

      QRect tmp = this->frameGeometry ();
      window_x = tmp.x ();
      window_y = tmp.y ();


      //  Use geometry to get the width and height.

      tmp = this->geometry ();
      window_width = tmp.width ();
      window_height = tmp.height ();


      envout (&pfm_global, &flags, &ref_def, &window_x, &window_width, &window_y, &window_height, &inputFilter);
    }
}



void pfmLoad::cleanupPage (int id)
{
  if (!id)
    {
    }
  else if (id >= 1 && id <= MAX_LOAD_FILES + 1)
    {
    }
  else if (id == MAX_LOAD_FILES + 2)
    {
    }
  else if (id == MAX_LOAD_FILES + 3)
    {
    }
  else if (id == MAX_LOAD_FILES + 4)
    {
    }
}



//  This is where the actual load is shelled to pfmLoader.

void 
pfmLoad::slotCustomButtonClicked (int id)
{
  char string[1024];
  QString tmp, tmpFile;
  PFM_LOAD_PARAMETERS load_parms[MAX_LOAD_FILES];
  QStringList arguments;
  FILE *fp = NULL;
  QStringList files, filters;
  QString file;
  QSet<QUrl> set;
  QList<QUrl> urls;
  QFileDialog *fd;



  uint8_t writeParameterFile (QString parameter_file, QStringList input_files, PFM_DEFINITION *pfm_def, PFM_GLOBAL pfm_global, FLAGS flags);
  int32_t get_file_type (char *);
  uint8_t clear_features (PFM_GLOBAL *pfm_global, int32_t, NV_I32_COORD2, char *, double);
  void remove_dir (QString dir);


  //  Get the files from the QTextEdit box on the inputPage.

  QTextCursor inputCursor = inputFiles->textCursor ();

  inputCursor.setPosition (0);


  QStringList sort_files;

  sort_files.clear ();


  //  First we need to look for list files (*.lst), remove them from the input file list, and then expand them into the sort file list.

  do
    {
      if (inputCursor.block ().text ().endsWith (".lst"))
        {
          char file[512], string[512];
          FILE *fp;

          strcpy (file, inputCursor.block ().text ().toLatin1 ());

          if ((fp = fopen (file, "r")) != NULL)
            {
              while (ngets (string, sizeof (string), fp) != NULL) sort_files.append (QString (string));

              fclose (fp);
            }
        }
      else
        {
          sort_files << inputCursor.block ().text ();
        }
    } while (inputCursor.movePosition (QTextCursor::NextBlock));


  //  Sort so we can remove dupes.

  sort_files.sort ();


  //  Remove dupes and place into input_files.

  QString name, prev_name = "";
  input_files.clear ();

  for (int32_t i = 0 ; i < sort_files.size () ; i++)
    {
      name = sort_files.at (i);

      if (name != prev_name)
        {
          input_files.append (name);
          prev_name = name;
        }
    }


  //  If the user selected directories in inputPage we need to sort and de-dupe that list as well just in case 
  //  the user wants to save a pfmLoader "update" parameters file.

  if (pfm_global.input_dirs.size ())
    {
      sort_files.clear ();
      sort_files = pfm_global.input_dirs;


      //  Sort so we can remove dupes.

      sort_files.sort ();


      pfm_global.input_dirs.clear ();

      for (int32_t i = 0 ; i < sort_files.size () ; i++)
        {
          name = sort_files.at (i);

          if (name != prev_name)
            {
              pfm_global.input_dirs.append (name);
              prev_name = name;
            }
        }
    }


  //  We need to get the polygon points either from the input area file, the input parameter file or the
  //  existing PFM because we have to write the temporary (or possibly permanent if the save button is pressed) 
  //  parameter file before we load the data.

  for (int32_t i = 0 ; i < pfm_file_count ; i++)
    {
      if (pfm_def[i].existing)
        {
          strcpy (pfm_def[i].open_args.list_path, pfm_def[i].name.toLatin1 ());
          pfm_def[i].open_args.checkpoint = 0;


          //  Opening the file will load the polygon points in the header.

          if ((pfm_def[i].hnd = open_existing_pfm_file (&pfm_def[i].open_args)) < 0)
            {
              QString tmp = tr ("An error occurred while trying to open file\n%1\nThe error message was :\n%2").arg (pfm_def[i].open_args.list_path).arg 
                (pfm_error_str (pfm_error));
              QMessageBox::warning (this, "pfmLoad", tmp);

              qDebug () << load_parms[0].pfm_global.progname << __FILE__ << __FUNCTION__ << __LINE__ << tmp;
              exit (-1);
            }

          close_pfm_file (pfm_def[i].hnd);
        }
      else
        {
          //  Load the area file unless we're appending.

          if (!pfm_def[i].area.contains (tr ("Defined in PFM structure")))
            {
              //  If we read in a parameter file with [Polygon Latitude,Longitude] points we're going
              //  to transfer that to the open_args structure, otherwise we'll read in an area file.

              if (pfm_def[i].polygon_count)
                {
                  pfm_def[i].open_args.head.mbr.min_y = pfm_def[i].open_args.head.mbr.min_x = 999.0;
                  pfm_def[i].open_args.head.mbr.max_y = pfm_def[i].open_args.head.mbr.max_x = -999.0;

                  pfm_def[i].open_args.head.polygon_count = pfm_def[i].polygon_count;
                  for (int32_t j = 0 ; j < pfm_def[i].polygon_count ; j++)
                    {
                      pfm_def[i].open_args.head.polygon[j].y = pfm_def[i].polygon[j].y;
                      pfm_def[i].open_args.head.polygon[j].x = pfm_def[i].polygon[j].x;

                      pfm_def[i].open_args.head.mbr.min_y = qMin (pfm_def[i].open_args.head.mbr.min_y, pfm_def[i].open_args.head.polygon[j].y);
                      pfm_def[i].open_args.head.mbr.min_x = qMin (pfm_def[i].open_args.head.mbr.min_x, pfm_def[i].open_args.head.polygon[j].x);
                      pfm_def[i].open_args.head.mbr.max_y = qMax (pfm_def[i].open_args.head.mbr.max_y, pfm_def[i].open_args.head.polygon[j].y);
                      pfm_def[i].open_args.head.mbr.max_x = qMax (pfm_def[i].open_args.head.mbr.max_x, pfm_def[i].open_args.head.polygon[j].x);
                    }
                }
              else
                {
                  strcpy (string, pfm_def[i].area.toLatin1 ());


                  get_area_mbr2 (string, &pfm_def[i].open_args.head.polygon_count, pfm_def[i].open_args.head.polygon,
                                 &pfm_def[i].open_args.head.mbr);


                  //  If we asked for a half-bin expansion of the area, do it here.

                  if (flags.expand && pfm_def[i].open_args.head.polygon_count == 4)
                    {
                      //  But *ONLY* if the area is a rectangle!!!

                      int32_t lt = 0, ln = 0;
                      for (int32_t ck = 0 ; ck < 4 ; ck++)
                        {
                          if (pfm_def[i].open_args.head.polygon[ck].y == pfm_def[i].open_args.head.mbr.min_y ||
                              pfm_def[i].open_args.head.polygon[ck].y == pfm_def[i].open_args.head.mbr.max_y) lt++;
                          if (pfm_def[i].open_args.head.polygon[ck].x == pfm_def[i].open_args.head.mbr.min_x ||
                              pfm_def[i].open_args.head.polygon[ck].x == pfm_def[i].open_args.head.mbr.max_x) ln++;
                        }


                      //  If all four polygon points matched the MBR edges, adjust the MBR and the polygon.

                      if (lt == 4 && ln == 4)
                        {
                          pfm_def[i].open_args.head.mbr.min_x = pfm_def[i].open_args.head.mbr.min_x - (pfm_def[i].gbin_size / 60.0) / 2.0;
                          pfm_def[i].open_args.head.mbr.min_y = pfm_def[i].open_args.head.mbr.min_y - (pfm_def[i].gbin_size / 60.0) / 2.0;
                          pfm_def[i].open_args.head.mbr.max_x = pfm_def[i].open_args.head.mbr.max_x + (pfm_def[i].gbin_size / 60.0) / 2.0;
                          pfm_def[i].open_args.head.mbr.max_y = pfm_def[i].open_args.head.mbr.max_y + (pfm_def[i].gbin_size / 60.0) / 2.0;


                          //  Adjust the polygon to match the changed MBR

                          pfm_def[i].open_args.head.polygon[0].y = pfm_def[i].open_args.head.mbr.min_y;
                          pfm_def[i].open_args.head.polygon[0].x = pfm_def[i].open_args.head.mbr.min_x;
                          pfm_def[i].open_args.head.polygon[1].y = pfm_def[i].open_args.head.mbr.max_y;
                          pfm_def[i].open_args.head.polygon[1].x = pfm_def[i].open_args.head.mbr.min_x;
                          pfm_def[i].open_args.head.polygon[2].y = pfm_def[i].open_args.head.mbr.max_y;
                          pfm_def[i].open_args.head.polygon[2].x = pfm_def[i].open_args.head.mbr.max_x;
                          pfm_def[i].open_args.head.polygon[3].y = pfm_def[i].open_args.head.mbr.min_y;
                          pfm_def[i].open_args.head.polygon[3].x = pfm_def[i].open_args.head.mbr.max_x;
                        }
                    }
                }
            }
        }
    }


  switch (id)
    {
    case QWizard::CustomButton1:
      button (QWizard::BackButton)->setEnabled (false);
      button (QWizard::CustomButton1)->setEnabled (false);


      //  Set the window title to be the first PFM file name.

      setWindowTitle (pfm_def[0].name);


      checkList->clear ();


      QApplication::setOverrideCursor (Qt::WaitCursor);


#ifdef NVLinux
      if (flags.sub) sub_in ();
#endif


      input_file_count = input_files.size ();


      progress.fbar->setWhatsThis (tr ("Progress of input file loading"));

      tmp = tr ("Reading input files");
      progress.fbar->setRange (0, input_file_count);
      progress.fbox->setTitle (tmp);
      qApp->processEvents ();


      //  Write the temporary parameter file that we will use to run pfmLoader.

      tmpFile = pfm_global.output_dir + QString (SEPARATOR) + QString ("pfmLoad_temp_%1.prm").arg (getpid ());
      strcpy (tmp_file, tmpFile.toLatin1 ());

      if (!writeParameterFile (tmp_file, input_files, pfm_def, pfm_global, flags))
        {
          QMessageBox::warning (this, "pfmLoad", tr ("Error opening parameters file %1 : %2").arg (tmp_file).arg (strerror (errno)));
          exit (-1);
        }


      loadProc = new QProcess (this);

      arguments.clear ();
      arguments += QString (tmp_file);


      connect (loadProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, SLOT (slotLoadDone (int, QProcess::ExitStatus)));
      connect (loadProc, SIGNAL (readyReadStandardError ()), this, SLOT (slotLoadReadyReadStandardError ()));
      connect (loadProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotLoadError (QProcess::ProcessError)));


      loadProc->start (loader_name, arguments);


      break;


    case QWizard::CustomButton2:
      fd = new QFileDialog (this, tr ("pfmLoad save parameter file"));
      fd->setViewMode (QFileDialog::List);


      //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.

      urls = fd->sidebarUrls ();
      urls += QUrl::fromLocalFile (QDir (pfm_global.output_dir).absolutePath ());
      urls += QUrl::fromLocalFile (QDir (".").absolutePath ());
      set = urls.toSet ();
      urls = set.toList ();
      qSort (urls);
      fd->setSidebarUrls (urls);

      fd->setDirectory (QDir (pfm_global.output_dir).absolutePath ());
      connect (fd, SIGNAL (filterSelected (const QString)), this, SLOT (slotParameterFilterSelected (const QString)));

      filters << tr ("pfmLoad parameter file (*.prm)");

      if (pfm_global.input_dirs.size ()) filters << tr ("pfmLoader \"update\" parameter file (*.upr)");

      fd->setNameFilters (filters);
      fd->setFileMode (QFileDialog::AnyFile);
      fd->selectNameFilter (tr ("pfmLoad parameter file (*.prm)"));


      if (fd->exec () == QDialog::Accepted)
        {
          QStringList files = fd->selectedFiles ();

          QString file = files.at (0);

          if (!file.isEmpty())
            {
              //  Add .prm or .upr to filename if not there.

              if (upr_file)
                {
                  if (!file.endsWith (".upr")) file.append (".upr");
                }
              else
                {
                  if (!file.endsWith (".prm")) file.append (".prm");
                }

              if (!writeParameterFile (file, input_files, pfm_def, pfm_global, flags))
                {
                  QString tmp;
                  tmp.sprintf (tr ("Error opening parameters file : %s").toLatin1 (), strerror (errno));
                  QMessageBox::warning (this, tr ("pfmLoad Save load parameters"), tmp);
                }
            }
        }

      button (QWizard::FinishButton)->setEnabled (true);

      break;


    case QWizard::CustomButton3:
      fd = new QFileDialog (this, tr ("pfmLoad Save process status"));
      fd->setViewMode (QFileDialog::List);


      //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
      //  This function is in the nvutility library.

      setSidebarUrls (fd, pfm_global.output_dir);

      filters << QString ("Text (*.txt)");

      fd->setNameFilters (filters);
      fd->setFileMode (QFileDialog::AnyFile);
      fd->selectNameFilter ("Text (*.txt)");

      if (fd->exec () == QDialog::Accepted)
        {
          files = fd->selectedFiles ();

          file = files.at (0);


          if (!file.isEmpty())
            {
              //  Add .txt to filename if not there.

              if (!file.endsWith (".txt")) file.append (".txt");
 
              char fname[1024];
              strcpy (fname, file.toLatin1 ());


              if ((fp = fopen (fname, "w")) != NULL)
                {
                  for (int32_t i = 0 ; i < checkList->count () ; i++)
                    {
                      char string[2048];
                      strcpy (string, checkList->item(i)->text ().toLatin1 ());

                      fprintf (fp, "%s\n", string);
                    }

                  fclose (fp);
                }
            }
        }

      break;
    }
}



void 
pfmLoad::slotParameterFilterSelected (const QString &filter)
{
  if (filter == tr ("pfmLoad parameter file (*.prm)"))
    {
      upr_file = NVFalse;
    }
  else
    {
      upr_file = NVTrue;
    }
}



//  Signal from the Cancel button.

void 
pfmLoad::reject ()
{
  exit (-1);
}



void 
pfmLoad::slotLoadReadyReadStandardError ()
{
  static QString resp_string = "";
  static uint8_t cr_lf = NVFalse, wcr = NVFalse;


  QByteArray response = loadProc->readAllStandardError ();


  //  Parse the return response for carriage returns and line feeds

  for (int32_t i = 0 ; i < response.length () ; i++)
    {
      if (response.at (i) == '\n')
        {
	  //  Protecting against Windows CR/LF stupidity.  We don't want to process the LF if the immediately
	  //  preceeding character was a CR.

	  if (!wcr)
	    {
	      if (!cr_lf) checkList->takeItem (checkList->currentRow ());

	      if (resp_string.contains ("Reading file "))
		{
		  checkList->addItem ("\n");

		  int32_t fnum, numf;
		  char tmp[1024];
		  strcpy (tmp, resp_string.toLatin1 ());
		  sscanf (tmp, "Reading file %d of %d", &fnum, &numf);

		  progress.fbar->setValue (fnum - 1);
		}
	      else if (resp_string.contains ("Reading input files complete"))
		{
		  progress.fbar->setValue (input_file_count);
		}


              //  Well-known Text abort from pfmLoader.

	      else if (resp_string.contains ("LAS_WKT closed by user"))
                {
                  resp_string = tr ("The load process was canceled by the user when asked for Well-known Text for a LAS file!\n\nPress Cancel to exit.");
                  button (QWizard::CustomButton3)->setEnabled (true);
                  button (QWizard::FinishButton)->setEnabled (false);
                  remove (tmp_file);
                }

              QListWidgetItem *cur = new QListWidgetItem (resp_string);

	      checkList->addItem (cur);
	      checkList->setCurrentItem (cur);
	      checkList->scrollToItem (cur);
	    }

	  wcr = NVFalse;
	  cr_lf = NVTrue;

          resp_string = "";
        }
      else if (response.at (i) == '\r')
        {
          if (!cr_lf) checkList->takeItem (checkList->currentRow ());


	  //  We have to do this here as well as above since we'll be blanking it on Windows.

	  if (resp_string.contains ("Reading file "))
	    {
	      checkList->addItem ("\n");

	      int32_t fnum, numf;
	      char tmp[1024];
	      strcpy (tmp, resp_string.toLatin1 ());
	      sscanf (tmp, "Reading file %d of %d", &fnum, &numf);

	      progress.fbar->setValue (fnum - 1);
	    }
	  else if (resp_string.contains ("Reading input files complete"))
	    {
	      progress.fbar->setValue (input_file_count);
	    }

          QListWidgetItem *cur = new QListWidgetItem (resp_string);

          checkList->addItem (cur);
          checkList->setCurrentItem (cur);
          checkList->scrollToItem (cur);

	  wcr = NVTrue;
          cr_lf = NVFalse;

          resp_string = "";
        }
      else
        {
	  wcr = NVFalse;
          resp_string += response.at (i);
        }
    }
}



void 
pfmLoad::slotLoadError (QProcess::ProcessError error)
{
  switch (error)
    {
    case QProcess::FailedToStart:
      QMessageBox::critical (this, "pfmLoad", tr ("Unable to start the load process!"));
      break;

    case QProcess::Crashed:
      QMessageBox::critical (this, "pfmLoad", tr ("The load process crashed!"));
      break;

    case QProcess::Timedout:
      QMessageBox::critical (this, "pfmLoad", tr ("The load process timed out!"));
      break;

    case QProcess::WriteError:
      QMessageBox::critical (this, "pfmLoad", tr ("There was a write error from the load process!"));
      break;

    case QProcess::ReadError:
      QMessageBox::critical (this, "pfmLoad", tr ("There was a read error from the load process!"));
      break;

    case QProcess::UnknownError:
      QMessageBox::critical (this, "pfmLoad", tr ("The load process died with an unknown error!"));
      break;
    }


  checkList->addItem (" ");
  button (QWizard::CustomButton3)->setEnabled (true);
}



void 
pfmLoad::slotLoadDone (int exitCode __attribute__ ((unused)), QProcess::ExitStatus exitStatus)
{
  if (exitStatus == QProcess::CrashExit)
    {
      checkList->addItem (QString (loader_name) + " did not exit normally!");
      QListWidgetItem *cur = new QListWidgetItem (tr ("\nLoading aborted, press Cancel to exit.\n"));

      checkList->addItem (cur);
      checkList->setCurrentItem (cur);
      checkList->scrollToItem (cur);

      button (QWizard::CustomButton3)->setEnabled (true);
    }
  else
    {
      checkList->addItem (" ");
      QListWidgetItem *cur = new QListWidgetItem (tr ("\nLoading complete, press Finish to exit.\n"));

      checkList->addItem (cur);
      checkList->setCurrentItem (cur);
      checkList->scrollToItem (cur);

      button (QWizard::FinishButton)->setEnabled (true);
    }


  remove (tmp_file);


  QApplication::restoreOverrideCursor ();
}



void 
pfmLoad::slotHelpClicked ()
{
  QWhatsThis::enterWhatsThisMode ();
}
