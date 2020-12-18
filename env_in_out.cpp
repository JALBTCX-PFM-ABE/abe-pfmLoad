
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


double settings_version = 7.01;



/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called pfmLoad.ini and will be stored in a directory called ABE.config.  On Linux, the ABE.config
  directory will be stored in your $HOME directory.  On Windows, it will be stored in your $USERPROFILE folder.  If
  you make a change to the way a variable is used and you want to force the defaults to be restored just change the
  settings_version to a newer number (I've been using the program version number from version.hpp - which you should
  be updating EVERY time you make a change to the program!).  You don't need to change the settings_version though
  unless you want to force the program to go back to the defaults (which can annoy your users).  So, the
  settings_version won't always match the program version.
*/

void envin (PFM_GLOBAL *pfm_global, FLAGS *flags, PFM_DEFINITION *ref_def, int32_t *window_x, int32_t *window_width,
            int32_t *window_y, int32_t *window_height, QString *inputFilter)
{
  //  We need to get the font from the global settings.

#ifdef NVWIN3X
  QString ini_file2 = QString (getenv ("USERPROFILE")) + "/ABE.config/" + "globalABE.ini";
#else
  QString ini_file2 = QString (getenv ("HOME")) + "/ABE.config/" + "globalABE.ini";
#endif

  pfm_global->font = QApplication::font ();

  QSettings settings2 (ini_file2, QSettings::IniFormat);
  settings2.beginGroup ("globalABE");


  QString defaultFont = pfm_global->font.toString ();
  QString fontString = settings2.value (QString ("ABE map GUI font"), defaultFont).toString ();
  pfm_global->font.fromString (fontString);


  settings2.endGroup ();


  double saved_version = 0.0;


  // Set defaults so that if keys don't exist the parameters are defined

  pfm_global->horizontal_error = 1.5;
  pfm_global->vertical_error = 0.25;


  pfm_global->appending = NVFalse;
  pfm_global->attribute_count = 0;

  pfm_global->time_attribute_num = 1;
  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++) pfm_global->gsf_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++) pfm_global->hof_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++) pfm_global->tof_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++) pfm_global->czmil_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++) pfm_global->las_attribute_num[i] = 0;
  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++) pfm_global->bag_attribute_num[i] = 0;

  pfm_global->input_dir = ".";
  pfm_global->output_dir = ".";
  pfm_global->area_dir = ".";
  pfm_global->mosaic_dir = ".";
  pfm_global->feature_dir = ".";
  pfm_global->cache_mem = 400000000;
  pfm_global->data_type = 0;

  flags->old = NVFalse;
  flags->hof = NVFalse;
  flags->sub = NVFalse;
  flags->lnd = NVFalse;
  flags->tof = NVFalse;
  flags->nom = NVFalse;
  flags->sec = NVFalse;
  flags->srtmb = NVFalse;
  flags->srtm1 = NVFalse;
  flags->srtm3 = NVFalse;
  flags->srtm30 = NVFalse;
  flags->srtmr = NVFalse;
  flags->srtme = NVTrue;
  flags->attr = NVFalse;
  flags->expand = NVFalse;
  flags->czmil_land = NVTrue;
  flags->czmil_water = NVTrue;
  flags->czmil_water_surface = NVTrue;
  flags->ref_water_surface = NVTrue;
  flags->HF_filt = NVFalse;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = NVTrue;
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = NVTrue;
  flags->czmil_chan[CZMIL_IR_CHANNEL] = NVFalse;
  flags->czmil_chan[CZMIL_DEEP_CHANNEL] = NVTrue;
  *inputFilter = QString ("GSF (*.d[0-9][0-9] *.gsf)");
  ref_def->mbin_size = 2.0;
  ref_def->gbin_size = 0.0;
  ref_def->min_depth = -10000.0;
  ref_def->max_depth = 12000.0;
  ref_def->precision = 0.01;
  ref_def->apply_area_filter = NVFalse;
  ref_def->deep_filter_only = NVTrue;
  ref_def->cellstd = 2.4;
  ref_def->radius = 20.0;
  *window_x = 0;
  *window_y = 0;
  *window_width = 880;
  *window_height = 520;


  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmLoad.ini";
#else
   QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmLoad.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmLoad");

  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return;


  flags->old = settings.value (QString ("load HOF in GCS form"), flags->old).toBool ();

  flags->hof = settings.value (QString ("load HOF null values"), flags->hof).toBool ();

  flags->sub = settings.value (QString ("invert substitute paths"), flags->sub).toBool ();

  flags->lnd = settings.value (QString ("invalidate HOF land values"), flags->lnd).toBool ();

  flags->tof = settings.value (QString ("make TOF first returns reference"), flags->tof).toBool ();

  flags->sec = settings.value (QString ("invalidate HOF secondary depths"), flags->sec).toBool ();

  flags->nom = settings.value (QString ("load nominal depths"), flags->nom).toBool ();

  flags->srtmb = settings.value (QString ("load best SRTM"), flags->srtmb).toBool ();

  flags->srtm1 = settings.value (QString ("load 1 second SRTM"), flags->srtm1).toBool ();

  flags->srtm3 = settings.value (QString ("load 3 second SRTM"), flags->srtm3).toBool ();

  flags->srtm30 = settings.value (QString ("load 30 second SRTM"), flags->srtm30).toBool ();

  flags->srtmr = settings.value (QString ("load SRTM as reference"), flags->srtmr).toBool ();

  flags->srtme = settings.value (QString ("Exclude SRTM2 data"), flags->srtme).toBool ();

  flags->expand = settings.value (QString ("Half bin expansion"), flags->expand).toBool ();

  flags->attr = settings.value (QString ("Save CUBE placeholders"), flags->attr).toBool ();


  flags->ref_water_surface = settings.value (QString ("load CZMIL water surface data as reference"), flags->ref_water_surface).toBool ();

  flags->czmil_water_surface = settings.value (QString ("load CZMIL water surface data"), flags->czmil_water_surface).toBool ();

  flags->czmil_land = settings.value (QString ("load CZMIL land data"), flags->czmil_land).toBool ();

  flags->czmil_water = settings.value (QString ("load CZMIL water data"), flags->czmil_water).toBool ();

  flags->HF_filt = settings.value (QString ("load HydroFusion filter invalid"), flags->HF_filt).toBool ();

  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1] = settings.value (QString ("load CZMIL shallow channel 1"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2] = settings.value (QString ("load CZMIL shallow channel 2"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3] = settings.value (QString ("load CZMIL shallow channel 3"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4] = settings.value (QString ("load CZMIL shallow channel 4"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5] = settings.value (QString ("load CZMIL shallow channel 5"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6] = settings.value (QString ("load CZMIL shallow channel 6"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]).toBool ();
  flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7] = settings.value (QString ("load CZMIL shallow channel 7"),
                                                               flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]).toBool ();
  flags->czmil_chan[CZMIL_IR_CHANNEL] = settings.value (QString ("load CZMIL IR channel"), flags->czmil_chan[CZMIL_IR_CHANNEL]).toBool ();
  flags->czmil_chan[CZMIL_DEEP_CHANNEL] = settings.value (QString ("load CZMIL DEEP channel"), flags->czmil_chan[CZMIL_DEEP_CHANNEL]).toBool ();

  *inputFilter = settings.value (QString ("input filter"), *inputFilter).toString ();

  ref_def->max_depth = (float) settings.value (QString ("max depth"), (double) ref_def->max_depth).toDouble ();

  ref_def->min_depth = (float) settings.value (QString ("min depth"), (double) ref_def->min_depth).toDouble ();

  ref_def->precision = (float) settings.value (QString ("precision"), (double) ref_def->precision).toDouble ();

  ref_def->mbin_size = (float) settings.value (QString ("bin size meters"), (double) ref_def->mbin_size).toDouble ();
  ref_def->gbin_size = (float) settings.value (QString ("bin size minutes"), (double) ref_def->gbin_size).toDouble ();

  ref_def->apply_area_filter = settings.value (QString ("apply area filter"), ref_def->apply_area_filter).toBool ();
  ref_def->deep_filter_only = settings.value (QString ("deep filter only"), ref_def->deep_filter_only).toBool ();

  ref_def->cellstd = (float) settings.value (QString ("bin standard deviation"), (double) ref_def->cellstd).toDouble ();

  ref_def->radius = (float) settings.value (QString ("feature radius"), (double) ref_def->radius).toDouble ();

  pfm_global->horizontal_error = (float) settings.value (QString ("horizontal error"), 
							     (double) pfm_global->horizontal_error).toDouble ();
  pfm_global->vertical_error = (float) settings.value (QString ("vertical error"), 
							     (double) pfm_global->vertical_error).toDouble ();

  pfm_global->time_attribute_num = settings.value (pfm_global->time_attribute.name, pfm_global->time_attribute_num).toInt ();

  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    pfm_global->gsf_attribute_num[i] = settings.value (pfm_global->gsf_attribute[i].name, pfm_global->gsf_attribute_num[i]).toInt ();

  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    pfm_global->hof_attribute_num[i] = settings.value (pfm_global->hof_attribute[i].name, pfm_global->hof_attribute_num[i]).toInt ();

  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    pfm_global->tof_attribute_num[i] = settings.value (pfm_global->tof_attribute[i].name, pfm_global->tof_attribute_num[i]).toInt ();

  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    pfm_global->czmil_attribute_num[i] = settings.value (pfm_global->czmil_attribute[i].name, pfm_global->czmil_attribute_num[i]).toInt ();

  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    pfm_global->las_attribute_num[i] = settings.value (pfm_global->las_attribute[i].name, pfm_global->las_attribute_num[i]).toInt ();

  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    pfm_global->bag_attribute_num[i] = settings.value (pfm_global->bag_attribute[i].name, pfm_global->bag_attribute_num[i]).toInt ();


  pfm_global->input_dir = settings.value (QString ("input directory"), pfm_global->input_dir).toString ();
  pfm_global->output_dir = settings.value (QString ("output directory"), pfm_global->output_dir).toString ();
  pfm_global->area_dir = settings.value (QString ("area directory"), pfm_global->area_dir).toString ();
  pfm_global->mosaic_dir = settings.value (QString ("mosaic directory"), pfm_global->mosaic_dir).toString ();
  pfm_global->feature_dir = settings.value (QString ("feature directory"), pfm_global->feature_dir).toString ();


  pfm_global->cache_mem = settings.value (QString ("PFM cache memory"), pfm_global->cache_mem).toInt ();


  *window_width = settings.value (QString ("width"), *window_width).toInt ();
  *window_height = settings.value (QString ("height"), *window_height).toInt ();
  *window_x = settings.value (QString ("x position"), *window_x).toInt ();
  *window_y = settings.value (QString ("y position"), *window_y).toInt ();


  if (pfm_global->horizontal_error == 0.0) pfm_global->horizontal_error = 1.5;
  if (pfm_global->vertical_error == 0.0) pfm_global->vertical_error = 0.25;


  settings.endGroup ();
}



//  Save the users defaults.

void envout (PFM_GLOBAL *pfm_global, FLAGS *flags, PFM_DEFINITION *ref_def, int32_t *window_x, int32_t *window_width,
             int32_t *window_y, int32_t *window_height, QString *inputFilter)
{
  //  Get the INI file name

#ifdef NVWIN3X
  QString ini_file = QString (getenv ("USERPROFILE")) + "/ABE.config/pfmLoad.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/ABE.config/pfmLoad.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("pfmLoad");


  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("load HOF in GCS form"), flags->old);

  settings.setValue (QString ("load HOF null values"), flags->hof);

  settings.setValue (QString ("invert substitute paths"), flags->sub);

  settings.setValue (QString ("invalidate HOF land values"), flags->lnd);

  settings.setValue (QString ("make TOF first returns reference"), flags->tof);

  settings.setValue (QString ("invalidate HOF secondary depths"), flags->sec);

  settings.setValue (QString ("load nominal depths"), flags->nom);

  settings.setValue (QString ("load best SRTM"), flags->srtmb);

  settings.setValue (QString ("load 1 second SRTM"), flags->srtm1);

  settings.setValue (QString ("load 3 second SRTM"), flags->srtm3);

  settings.setValue (QString ("load 30 second SRTM"), flags->srtm30);

  settings.setValue (QString ("load SRTM as reference"), flags->srtmr);

  settings.setValue (QString ("Exclude SRTM2 data"), flags->srtme);

  settings.setValue (QString ("Half bin expansion"), flags->expand);

  settings.setValue (QString ("Save CUBE placeholders"), flags->attr);


  settings.setValue (QString ("load CZMIL water surface data as reference"), flags->ref_water_surface);
  settings.setValue (QString ("load CZMIL water surface data"), flags->czmil_water_surface);
  settings.setValue (QString ("load CZMIL land data"), flags->czmil_land);
  settings.setValue (QString ("load CZMIL water data"), flags->czmil_water);
  settings.setValue (QString ("load HydroFusion filter invalid"), flags->HF_filt);

  settings.setValue (QString ("load CZMIL shallow channel 1"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  settings.setValue (QString ("load CZMIL shallow channel 2"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  settings.setValue (QString ("load CZMIL shallow channel 3"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  settings.setValue (QString ("load CZMIL shallow channel 4"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  settings.setValue (QString ("load CZMIL shallow channel 5"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  settings.setValue (QString ("load CZMIL shallow channel 6"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  settings.setValue (QString ("load CZMIL shallow channel 7"), flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  settings.setValue (QString ("load CZMIL IR channel"), flags->czmil_chan[CZMIL_IR_CHANNEL]);
  settings.setValue (QString ("load CZMIL DEEP channel"), flags->czmil_chan[CZMIL_DEEP_CHANNEL]);

  settings.setValue (QString ("input filter"), *inputFilter);

  settings.setValue (QString ("max depth"), (double) ref_def->max_depth);

  settings.setValue (QString ("min depth"), (double) ref_def->min_depth);

  settings.setValue (QString ("precision"), (double) ref_def->precision);

  settings.setValue (QString ("bin size meters"), (double) ref_def->mbin_size);
  settings.setValue (QString ("bin size minutes"), (double) ref_def->gbin_size);

  settings.setValue (QString ("apply area filter"), ref_def->apply_area_filter);

  settings.setValue (QString ("deep filter only"), ref_def->deep_filter_only);

  settings.setValue (QString ("bin standard deviation"), (double) ref_def->cellstd);

  settings.setValue (QString ("feature radius"), (double) ref_def->radius);

  settings.setValue (QString ("horizontal error"), (double) pfm_global->horizontal_error);
  settings.setValue (QString ("vertical error"), (double) pfm_global->vertical_error);


  settings.setValue (pfm_global->time_attribute.name, pfm_global->time_attribute_num);

  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->gsf_attribute[i].name, pfm_global->gsf_attribute_num[i]);

  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->hof_attribute[i].name, pfm_global->hof_attribute_num[i]);

  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->tof_attribute[i].name, pfm_global->tof_attribute_num[i]);

  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->czmil_attribute[i].name, pfm_global->czmil_attribute_num[i]);

  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->las_attribute[i].name, pfm_global->las_attribute_num[i]);

  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    settings.setValue (pfm_global->bag_attribute[i].name, pfm_global->bag_attribute_num[i]);


  settings.setValue (QString ("input directory"), pfm_global->input_dir);
  settings.setValue (QString ("output directory"), pfm_global->output_dir);
  settings.setValue (QString ("area directory"), pfm_global->area_dir);
  settings.setValue (QString ("mosaic directory"), pfm_global->mosaic_dir);
  settings.setValue (QString ("feature directory"), pfm_global->feature_dir);

  settings.setValue (QString ("PFM cache memory"), pfm_global->cache_mem);

  settings.setValue (QString ("width"), *window_width);
  settings.setValue (QString ("height"), *window_height);
  settings.setValue (QString ("x position"), *window_x);
  settings.setValue (QString ("y position"), *window_y);

  settings.endGroup ();
}
