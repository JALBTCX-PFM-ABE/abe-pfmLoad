
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



#include "pfmPage.hpp"
#include "pfmPageHelp.hpp"


pfmPage::pfmPage (QWidget *parent, PFM_DEFINITION *pfmDef, PFM_GLOBAL *pfmg, int32_t page_num):
  QWizardPage (parent)
{
  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmLoadWatermark.png"));

  pfm_def = pfmDef;
  pfm_global = pfmg;
  l_page_num = page_num;
  pfmIndex.sprintf ("%02d", page_num - 1);
  prev_mbin = pfm_def->mbin_size;
  prev_gbin = pfm_def->gbin_size;

  QString title;
  title = tr ("PFM %1 Options").arg (pfmIndex);
  setTitle (title);


  QVBoxLayout *pageLayout = new QVBoxLayout (this);
  pageLayout->setMargin (5);
  pageLayout->setSpacing (5);


  QHBoxLayout *pfm_file_box = new QHBoxLayout;
  pfm_file_box->setSpacing (5);

  pageLayout->addLayout (pfm_file_box);


  QString pfl = tr ("PFM file %1").arg (pfmIndex);
  pfm_file_label = new QLabel (pfl, this);

  pfm_file_box->addWidget (pfm_file_label, 1);

  pfm_file_edit = new QLineEdit (this);
  pfm_file_edit->setToolTip (tr ("Set the PFM file name manually"));
  connect (pfm_file_edit, SIGNAL (textChanged (const QString &)), this, SLOT (slotPFMFileEdit (const QString &)));


  pfm_file_box->addWidget (pfm_file_edit, 10);

  pfm_file_browse = new QPushButton (tr ("Browse..."), this);
  pfm_file_browse->setToolTip (tr ("Select a preexisting PFM file to append to or create file in new directory"));

  pfm_file_label->setWhatsThis (pfm_fileText);
  pfm_file_edit->setWhatsThis (pfm_fileText);
  pfm_file_browse->setWhatsThis (pfm_fileBrowseText);

  connect (pfm_file_browse, SIGNAL (clicked ()), this, SLOT (slotPFMFileBrowse ()));

  pfm_file_box->addWidget (pfm_file_browse, 1);


  QGroupBox *limBox = new QGroupBox (tr ("Limits"), this);
  QHBoxLayout *limBoxLayout = new QHBoxLayout;
  limBox->setLayout (limBoxLayout);
  limBoxLayout->setSpacing (10);


  QGroupBox *mBinsBox = new QGroupBox (tr ("Bin size (meters)"), this);
  QHBoxLayout *mBinsBoxLayout = new QHBoxLayout;
  mBinsBox->setLayout (mBinsBoxLayout);
  mBinsBoxLayout->setSpacing (10);

  mBinSize = new QDoubleSpinBox (this);
  mBinSize->setDecimals (2);
  mBinSize->setRange (0.0, 1000.0);
  mBinSize->setSingleStep (1.0);
  mBinSize->setValue (pfm_def->mbin_size);
  mBinSize->setWrapping (true);
  mBinSize->setToolTip (tr ("Set the PFM bin size in meters"));
  mBinSize->setWhatsThis (mBinSizeText);
  connect (mBinSize, SIGNAL (valueChanged (double)), this, SLOT (slotMBinSizeChanged (double)));
  mBinsBoxLayout->addWidget (mBinSize);


  limBoxLayout->addWidget (mBinsBox);


  QGroupBox *gBinsBox = new QGroupBox (tr ("Bin size (minutes)"), this);
  QHBoxLayout *gBinsBoxLayout = new QHBoxLayout;
  gBinsBox->setLayout (gBinsBoxLayout);
  gBinsBoxLayout->setSpacing (10);

  gBinSize = new QDoubleSpinBox (this);
  gBinSize->setDecimals (3);
  gBinSize->setRange (0.0, 200.0);
  gBinSize->setSingleStep (0.05);
  gBinSize->setValue (pfm_def->gbin_size);
  gBinSize->setWrapping (true);
  gBinSize->setToolTip (tr ("Set the PFM bin size in minutes"));
  gBinSize->setWhatsThis (gBinSizeText);
  connect (gBinSize, SIGNAL (valueChanged (double)), this, SLOT (slotGBinSizeChanged (double)));
  gBinsBoxLayout->addWidget (gBinSize);


  limBoxLayout->addWidget (gBinsBox);


  QGroupBox *minDBox = new QGroupBox (tr ("Minimum depth"), this);
  QHBoxLayout *minDBoxLayout = new QHBoxLayout;
  minDBox->setLayout (minDBoxLayout);
  minDBoxLayout->setSpacing (10);

  minDepth = new QDoubleSpinBox (this);
  minDepth->setDecimals (1);
  minDepth->setRange (-10000.0, 12000.0);
  minDepth->setSingleStep (1000.0);
  minDepth->setValue (pfm_def->min_depth);
  minDepth->setWrapping (false);
  minDepth->setToolTip (tr ("Set the minimum allowable depth for the PFM structure"));
  minDepth->setWhatsThis (minDepthText);
  connect (minDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMinDepthChanged (double)));
  minDBoxLayout->addWidget (minDepth);


  limBoxLayout->addWidget (minDBox);


  QGroupBox *maxDBox = new QGroupBox (tr ("Maximum depth"), this);
  QHBoxLayout *maxDBoxLayout = new QHBoxLayout;
  maxDBox->setLayout (maxDBoxLayout);
  maxDBoxLayout->setSpacing (10);

  maxDepth = new QDoubleSpinBox (this);
  maxDepth->setDecimals (1);
  maxDepth->setRange (-10000.0, 12000.0);
  maxDepth->setSingleStep (1000.0);
  maxDepth->setValue (pfm_def->max_depth);
  maxDepth->setWrapping (false);
  maxDepth->setToolTip (tr ("Set the maximum allowable depth for the PFM structure"));
  maxDepth->setWhatsThis (maxDepthText);
  connect (maxDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMaxDepthChanged (double)));
  maxDBoxLayout->addWidget (maxDepth);


  limBoxLayout->addWidget (maxDBox);


  QGroupBox *precBox = new QGroupBox (tr ("Precision"), this);
  QHBoxLayout *precBoxLayout = new QHBoxLayout;
  precBox->setLayout (precBoxLayout);
  precBoxLayout->setSpacing (10);

  precision = new QComboBox (this);
  precision->setToolTip (tr ("Set the PFM structure depth precision"));
  precision->setWhatsThis (precisionText);
  precision->setEditable (false);
  precision->addItem (tr ("%L1 (one centimeter)").arg (0.01, 0, 'f', 2));
  precision->addItem (tr ("%L1 (one decimeter)").arg (0.10, 0, 'f', 2));
  precision->addItem (tr ("%L1 (one meter)").arg (1.00, 0, 'f', 2));
  precBoxLayout->addWidget (precision);


  limBoxLayout->addWidget (precBox);


  pageLayout->addWidget (limBox);


  QGroupBox *areaBox = new QGroupBox (tr ("Area file"), this);
  QHBoxLayout *areaBoxLayout = new QHBoxLayout;
  areaBox->setLayout (areaBoxLayout);
  areaBoxLayout->setSpacing (10);

  area_edit = new QLineEdit (this);
  area_edit->setReadOnly (true);
  area_edit->setToolTip (tr ("Area file name for this PFM")); 
  area_edit->setWhatsThis (areaText);
  areaBoxLayout->addWidget (area_edit);

  area_browse = new QPushButton (tr ("Browse..."), this);
  area_browse->setToolTip (tr ("Select an area file to define the PFM area"));
  area_browse->setWhatsThis (areaBrowseText);
  connect (area_browse, SIGNAL (clicked ()), this, SLOT (slotAreaFileBrowse ()));
  areaBoxLayout->addWidget (area_browse);

  area_map = new QPushButton (tr ("Map..."), this);
  area_map->setToolTip (tr ("Create an area file using areaCheck")); 
  area_map->setWhatsThis (area_mapText);
  connect (area_map, SIGNAL (clicked ()), this, SLOT (slotAreaMap ()));
  areaBoxLayout->addWidget (area_map);

  area_pfm = new QPushButton (tr ("PFM..."), this);
  area_pfm->setToolTip (tr ("Use the area in an already existing PFM structure")); 
  area_pfm->setWhatsThis (area_PFMText);
  connect (area_pfm, SIGNAL (clicked ()), this, SLOT (slotAreaPFM ()));
  areaBoxLayout->addWidget (area_pfm);

  area_nsew = new QPushButton (tr ("NSEW..."), this);
  area_nsew->setToolTip (tr ("Create an area file by defining North, South, East, and West bounds")); 
  area_nsew->setWhatsThis (area_nsewText);
  connect (area_nsew, SIGNAL (clicked ()), this, SLOT (slotAreaNSEW ()));
  areaBoxLayout->addWidget (area_nsew);


  pageLayout->addWidget (areaBox, 1);



  QGroupBox *optBox = new QGroupBox (tr ("Optional files"), this);
  QHBoxLayout *optBoxLayout = new QHBoxLayout;
  optBox->setLayout (optBoxLayout);
  optBoxLayout->setSpacing (10);


  QGroupBox *mosaicBox = new QGroupBox (tr ("Mosaic file"), this);
  QHBoxLayout *mosaicBoxLayout = new QHBoxLayout;
  mosaicBox->setLayout (mosaicBoxLayout);
  mosaicBoxLayout->setSpacing (10);

  mosaic_edit = new QLineEdit (this);
  mosaic_edit->setReadOnly (true);
  mosaic_edit->setToolTip (tr ("Mosaic file name for this PFM"));
  mosaic_edit->setWhatsThis (mosaicText);
  mosaicBoxLayout->addWidget (mosaic_edit);

  mosaic_browse = new QPushButton (tr ("Browse..."), this);
  mosaic_browse->setToolTip (tr ("Select a mosaic file for this PFM"));
  mosaic_browse->setWhatsThis (mosaicBrowseText);
  mosaic_edit->setText (pfm_def->mosaic);
  connect (mosaic_browse, SIGNAL (clicked ()), this, SLOT (slotMosaicFileBrowse ()));
  mosaicBoxLayout->addWidget (mosaic_browse);


  optBoxLayout->addWidget (mosaicBox);


  QGroupBox *featureBox = new QGroupBox (tr ("Feature file"), this);
  QHBoxLayout *featureBoxLayout = new QHBoxLayout;
  featureBox->setLayout (featureBoxLayout);
  featureBoxLayout->setSpacing (10);

  feature_edit = new QLineEdit (this);
  feature_edit->setReadOnly (true);
  feature_edit->setToolTip (tr ("Feature file name for this PFM"));
  feature_edit->setWhatsThis (featureText);
  featureBoxLayout->addWidget (feature_edit);

  feature_browse = new QPushButton (tr ("Browse..."), this);
  feature_browse->setToolTip (tr ("Select a feature file for this PFM"));
  feature_browse->setWhatsThis (featureBrowseText);
  feature_edit->setText (pfm_def->feature);
  connect (feature_browse, SIGNAL (clicked ()), this, SLOT (slotFeatureFileBrowse ()));
  featureBoxLayout->addWidget (feature_browse);


  optBoxLayout->addWidget (featureBox);


  pageLayout->addWidget (optBox, 1);


  QGroupBox *filtBox = new QGroupBox (tr ("Area filter settings"), this);
  QHBoxLayout *filtBoxLayout = new QHBoxLayout;
  filtBox->setLayout (filtBoxLayout);
  filtBoxLayout->setSpacing (10);


  QGroupBox *aBox = new QGroupBox (tr ("Apply area filter"), this);
  QHBoxLayout *aBoxLayout = new QHBoxLayout;
  aBox->setLayout (aBoxLayout);
  aBoxLayout->setSpacing (10);

  applyFilter = new QCheckBox (this);
  applyFilter->setToolTip (tr ("Apply the area filter for this PFM"));
  applyFilter->setWhatsThis (applyFilterText);
  applyFilter->setChecked (pfm_def->apply_area_filter);
  connect (applyFilter, SIGNAL (stateChanged (int)), this, SLOT (slotApplyFilterStateChanged (int)));
  aBoxLayout->addWidget (applyFilter);


  filtBoxLayout->addWidget (aBox);


  QGroupBox *dBox = new QGroupBox (tr ("Deep filter only"), this);
  QHBoxLayout *dBoxLayout = new QHBoxLayout;
  dBox->setLayout (dBoxLayout);
  dBoxLayout->setSpacing (10);

  deepFilter = new QCheckBox (this);
  deepFilter->setToolTip (tr ("Only filter values deeper than the average surface"));
  deepFilter->setWhatsThis (deepFilterText);
  deepFilter->setChecked (pfm_def->deep_filter_only);
  if (!pfm_def->apply_area_filter) deepFilter->setEnabled (false);
  dBoxLayout->addWidget (deepFilter);


  filtBoxLayout->addWidget (dBox);


  QGroupBox *bBox = new QGroupBox (tr ("Bin standard deviation"), this);
  QHBoxLayout *bBoxLayout = new QHBoxLayout;
  bBox->setLayout (bBoxLayout);
  bBoxLayout->setSpacing (10);

  stdSpin = new QDoubleSpinBox (this);
  stdSpin->setDecimals (2);
  stdSpin->setRange (0.3, 3.0);
  stdSpin->setSingleStep (0.1);
  stdSpin->setValue (pfm_def->cellstd);
  stdSpin->setWrapping (true);
  stdSpin->setToolTip (tr ("Set the area filter standard deviation"));
  stdSpin->setWhatsThis (stdText);
  if (!pfm_def->apply_area_filter) stdSpin->setEnabled (false);
  bBoxLayout->addWidget (stdSpin);


  filtBoxLayout->addWidget (bBox);


  QGroupBox *tBox = new QGroupBox (tr ("Feature Radius"), this);
  QHBoxLayout *tBoxLayout = new QHBoxLayout;
  tBox->setLayout (tBoxLayout);
  tBoxLayout->setSpacing (10);

  featureRadius = new QDoubleSpinBox (this);
  featureRadius->setDecimals (2);
  featureRadius->setRange (0.0, 200.0);
  featureRadius->setSingleStep (10.0);
  featureRadius->setValue (pfm_def->radius);
  featureRadius->setWrapping (true);
  featureRadius->setToolTip (tr ("Set the radius of the area around features to exclude from filtering"));
  featureRadius->setWhatsThis (featureRadiusText);
  if (!pfm_def->apply_area_filter) featureRadius->setEnabled (false);
  tBoxLayout->addWidget (featureRadius);


  filtBoxLayout->addWidget (tBox);


  pageLayout->addWidget (filtBox, 1);


  //  Register fields.

  pfm_file_edit_field = "pfm_file_edit" + pfmIndex;
  registerField (pfm_file_edit_field, pfm_file_edit);

  area_edit_field = "area_edit" + pfmIndex;
  registerField (area_edit_field, area_edit);

  mBinSizeField = "mBinSize" + pfmIndex;
  registerField (mBinSizeField, mBinSize, "value", "valueChanged");

  gBinSizeField = "gBinSize" + pfmIndex;
  registerField (gBinSizeField, gBinSize, "value", "valueChanged");

  minDepthField = "minDepth" + pfmIndex;
  registerField (minDepthField, minDepth, "value", "valueChanged");

  maxDepthField = "maxDepth" + pfmIndex;
  registerField (maxDepthField, maxDepth, "value", "valueChanged");

  precisionField = "precision" + pfmIndex;
  registerField (precisionField, precision);

  mosaic_edit_field = "mosaic_edit" + pfmIndex;
  registerField (mosaic_edit_field, mosaic_edit);

  feature_edit_field = "feature_edit" + pfmIndex;
  registerField (feature_edit_field, feature_edit);

  applyFilterField = "applyFilter" + pfmIndex;
  registerField (applyFilterField, applyFilter);

  deepFilterField = "deepFilter" + pfmIndex;
  registerField (deepFilterField, deepFilter);

  stdSpinField = "stdSpin" + pfmIndex;
  registerField (stdSpinField, stdSpin, "value", "valueChanged");

  featureRadiusField = "featureRadius" + pfmIndex;
  registerField (featureRadiusField, featureRadius, "value", "valueChanged");


  setFields (pfmDef);
}



int32_t 
pfmPage::nextId () const
{
  if (pfm_file_edit->text ().isEmpty () || area_edit->text ().isEmpty ()) return (MAX_LOAD_FILES + 2);

  return (l_page_num + 1);
}



bool 
pfmPage::validatePage ()
{
  if (l_page_num == 1 && (pfm_file_edit->text ().isEmpty () || area_edit->text ().isEmpty ()))
    {
      QMessageBox::critical (this, "pfmLoad", tr ("You must enter a PFM name and an area file name to continue!"));

      return (false);
    }

  return (true);
}



void 
pfmPage::slotMBinSizeChanged (double value)
{
  pfm_def->mbin_size = value;


  //  Disconnect the geographic bin size value changed signal so we don't bounce back and forth.  Reconnect after
  //  setting the value.

  disconnect (gBinSize, SIGNAL (valueChanged (double)), 0, 0);


  //  We don't ever want both gbin and mbin to be 0.0.

  if (fabs (value < 0.0000001))
    {
      pfm_def->gbin_size = prev_gbin;
    }
  else
    {
      prev_mbin = value;
      pfm_def->gbin_size = 0.0;
    }
  gBinSize->setValue (pfm_def->gbin_size);

  connect (gBinSize, SIGNAL (valueChanged (double)), this, SLOT (slotGBinSizeChanged (double)));
}



void 
pfmPage::slotGBinSizeChanged (double value)
{
  pfm_def->gbin_size = value;


  //  Disconnect the meter bin size value changed signal so we don't bounce back and forth.  Reconnect after setting
  //  the value.

  disconnect (mBinSize, SIGNAL (valueChanged (double)), 0, 0);


  //  We don't ever want both gbin and mbin to be 0.0.

  if (fabs (value < 0.0000001))
    {
      pfm_def->mbin_size = prev_mbin;
    }
  else
    {
      prev_gbin = value;
      pfm_def->mbin_size = 0.0;
    }
  mBinSize->setValue (pfm_def->mbin_size);

  connect (mBinSize, SIGNAL (valueChanged (double)), this, SLOT (slotMBinSizeChanged (double)));
}



void 
pfmPage::slotMinDepthChanged (double value)
{
  //  Check against max depth so that we don't set the min depth greater than or equal to the max depth.

  if (value >= pfm_def->max_depth)
    {
      QMessageBox::warning (this, "pfmLoad", tr ("You cannot set the minimum depth greater than or equal to the maximum depth!"));


      //  Disconnect the min depth spin box so we can set the value back to what it was last set to.

      disconnect (minDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMinDepthChanged (double)));

      minDepth->setValue (pfm_def->min_depth);

      connect (minDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMinDepthChanged (double)));


      return;
    }


  pfm_def->min_depth = value;
}



void 
pfmPage::slotMaxDepthChanged (double value)
{
  //  Check against min depth so that we don't set the max depth less than or equal to the min depth.

  if (value <= pfm_def->min_depth)
    {
      QMessageBox::warning (this, "pfmLoad", tr ("You cannot set the maximum depth less than or equal to the minimum depth!"));


      //  Disconnect the max depth spin box so we can set the value back to what it was last set to.

      disconnect (maxDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMaxDepthChanged (double)));

      maxDepth->setValue (pfm_def->max_depth);

      connect (maxDepth, SIGNAL (valueChanged (double)), this, SLOT (slotMaxDepthChanged (double)));


      return;
    }


  pfm_def->max_depth = value;
}



void 
pfmPage::slotApplyFilterStateChanged (int state)
{
  if (state == Qt::Unchecked)
    {
      deepFilter->setEnabled (false);
      stdSpin->setEnabled (false);
      featureRadius->setEnabled (false);
    }
  else
    {
      deepFilter->setEnabled (true);
      stdSpin->setEnabled (true);
      featureRadius->setEnabled (true);
    }
}



void 
pfmPage::slotPFMFileEdit (const QString &string)
{
  pfm_def->name = string;


  if (!pfm_def->name.endsWith (".pfm")) pfm_def->name += ".pfm";


  mBinSize->setEnabled (true);
  gBinSize->setEnabled (true);
  minDepth->setEnabled (true);
  maxDepth->setEnabled (true);
  precision->setEnabled (true);
  area_browse->setEnabled (true);
  area_map->setEnabled (true);
  area_pfm->setEnabled (true);
  area_nsew->setEnabled (true);


  //  Watch out for directory names that might be typed in.

  if (QFileInfo (string).isDir ()) return;


  FILE *fp;
  int32_t hnd;
  PFM_OPEN_ARGS open_args;


  strcpy (open_args.list_path, pfm_def->name.toLatin1 ());


  //  Try to open the file.  If it exists we will not allow the user to change the bin size, 
  //  depth precision, etc.

  pfm_def->existing = NVFalse;
  if ((fp = fopen (open_args.list_path, "r")) != NULL)
    {
      fclose (fp);

      open_args.checkpoint = 0;

      if ((hnd = open_existing_pfm_file (&open_args)) >= 0)
        {
          char file[512];
          get_target_file (hnd, open_args.list_path, file);
          feature_edit->setText (QString (file));
          get_mosaic_file (hnd, open_args.list_path, file);
          mosaic_edit->setText (QString (file));

          close_pfm_file (hnd);


          area_edit->setText (tr ("Defined in PFM structure"));


          mBinSize->setEnabled (false);
          gBinSize->setEnabled (false);
          minDepth->setEnabled (false);
          maxDepth->setEnabled (false);
          precision->setEnabled (false);
          area_browse->setEnabled (false);
          area_map->setEnabled (false);
          area_pfm->setEnabled (false);
          area_nsew->setEnabled (false);

          pfm_def->existing = NVTrue;
        }
      else
        {
          QString tmp = tr ("The file \n%1\n is not a PFM structure or there was an error reading the file.\nThe error message returned was:\n%2").arg 
                                (QDir::toNativeSeparators (QString (open_args.list_path))).arg (pfm_error_str (pfm_error));
          QMessageBox::warning (this, "pfmLoad", tmp);

          if (pfm_error == CHECKPOINT_FILE_EXISTS_ERROR)
            {
              qDebug () << pfm_global->progname << __FILE__ << __FUNCTION__ << __LINE__ << tmp;
              exit (-1);
            }
          return;
        }
    }
  else
    {
      if (area_edit->text () == tr ("Defined in PFM structure")) area_edit->clear ();

      feature_edit->setText ("NONE");
      mosaic_edit->setText ("NONE");

      QFileInfo fi = QFileInfo (pfm_def->name);
      if (fi.isRelative ())
        {
          //  Get the absolute path name

          QDir dir;

          pfm_def->name.prepend ("/");
          pfm_def->name.prepend (dir.canonicalPath());
        }
    }

  if (!pfm_file_edit->text ().isEmpty () && !area_edit->text ().isEmpty ())
    {
      setButtonText (QWizard::NextButton, tr ("Next"));
    }
  else
    {
      if (l_page_num > 1) setButtonText (QWizard::NextButton, tr ("Finish"));
    }
}



void 
pfmPage::slotPFMFileBrowse ()
{
  QStringList         files, filters;
  QString             file;


  QFileDialog *fd = new QFileDialog (this, tr ("pfmLoad Open PFM Structure"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, pfm_global->output_dir);


  filters << QString ("PFM (*.pfm)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::AnyFile);
  fd->selectNameFilter ("PFM (*.pfm)");

  if (fd->exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a file.

      pfm_global->output_dir = fd->directory ().absolutePath ();

      files = fd->selectedFiles ();

      pfm_def->name = files.at (0);


      if (!pfm_def->name.isEmpty())
        {
          if (!pfm_def->name.endsWith (".pfm")) pfm_def->name.append (".pfm");


          //  This will trigger slotPFMFileEdit.

          pfm_file_edit->setText (pfm_def->name);
        }
    }
}



void 
pfmPage::slotAreaFileBrowse ()
{
  SHPHandle shpHandle;
  SHPObject *shape = NULL;
  int32_t type, numShapes;
  double minBounds[4], maxBounds[4];


  QFileDialog fd (this, tr ("pfmLoad Area File"));
  fd.setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (&fd, pfm_global->area_dir);


  QStringList filters;
  filters << tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)");

  fd.setNameFilters (filters);
  fd.setFileMode (QFileDialog::ExistingFile);
  fd.selectNameFilter (tr ("Area file (*.ARE *.are *.afs *.shp *.SHP)"));


  QStringList files;
  if (fd.exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a file.

      pfm_global->area_dir = fd.directory ().absolutePath ();

      files = fd.selectedFiles ();


      pfm_def->area = files.at (0);


      if (!pfm_def->area.isEmpty())
        {
          if (pfm_def->area.endsWith (".shp", Qt::CaseInsensitive))
            {
              char shpname[1024];
              strcpy (shpname, pfm_def->area.toLatin1 ());


              //  Open shape file

              shpHandle = SHPOpen (shpname, "rb");

              if (shpHandle == NULL)
                {
                  QMessageBox::warning (this, "pfmLoad", tr ("Cannot open shape file %1!").arg (pfm_def->area));
                  return;
                }
              else
                {
                  //  Get shape file header info

                  SHPGetInfo (shpHandle, &numShapes, &type, minBounds, maxBounds);


                  if (type != SHPT_POLYGON && type != SHPT_POLYGONZ && type != SHPT_POLYGONM &&
                      type != SHPT_ARC && type != SHPT_ARCZ && type != SHPT_ARCM)
                    {
                      QMessageBox::warning (this, "pfmLoad", tr ("Shape file %1 is not a polygon or polyline file!").arg (pfm_def->area));
                      return;
                    }
                  else
                    {
                      //  Read only the first shape.

                      shape = SHPReadObject (shpHandle, 0);


                      //  Check the number of vertices.

                      if (shape->nVertices < 3)
                        {
                          SHPClose (shpHandle);
                          QMessageBox::warning (this, "pfmLoad", tr ("Number of vertices (%1) of shape file %2 is too few for a polygon!").arg
                                                (shape->nVertices).arg (pfm_def->area));
                          return;
                        }


                      //  Read the vertices to take a shot at determining that this is a geographic polygon.

                      for (int32_t j = 0 ; j < shape->nVertices ; j++)
                        {
                          if (shape->padfX[j] < -360.0 || shape->padfX[j] > 360.0 || shape->padfY[j] < -90.0 || shape->padfY[j] > 90.0)
                            {
                              SHPDestroyObject (shape);
                              SHPClose (shpHandle);
                              QMessageBox::warning (this, "pfmLoad", tr ("Shape file %1 does not appear to be geographic!").arg (pfm_def->area));
                              return;
                            }
                        }


                      SHPDestroyObject (shape);
                      SHPClose (shpHandle);
                    }
                }
            }

          area_edit->setText (pfm_def->area);

          if (!pfm_file_edit->text ().isEmpty () && !area_edit->text ().isEmpty ()) setButtonText (QWizard::NextButton, tr ("Next"));
        }
    }
}



void 
pfmPage::slotMosaicFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("pfmLoad Mosaic File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, pfm_global->mosaic_dir);


  QStringList filters;
  filters << tr ("Mosaic file (*.mos, *.tif)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Mosaic file (*.mos, *.tif)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a file.

      pfm_global->mosaic_dir = fd->directory ().absolutePath ();

      files = fd->selectedFiles ();

      pfm_def->mosaic = files.at (0);


      if (!pfm_def->mosaic.isEmpty())
        {
          mosaic_edit->setText (pfm_def->mosaic);
        }
    }
}



void 
pfmPage::slotFeatureFileBrowse ()
{
  QFileDialog *fd = new QFileDialog (this, tr ("pfmLoad Feature File"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, pfm_global->feature_dir);


  QStringList filters;
  filters << tr ("Binary Feature Data (*.bfd)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (tr ("Binary Feature Data (*.bfd)"));


  QStringList files;
  if (fd->exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a file.

      pfm_global->feature_dir = fd->directory ().absolutePath ();

      files = fd->selectedFiles ();

      pfm_def->feature = files.at (0);


      if (!pfm_def->feature.isEmpty())
        {
          feature_edit->setText (pfm_def->feature);
        }
    }
}



void 
pfmPage::slotMapReadyReadStandardError ()
{
  QByteArray response = mapProc->readAllStandardError ();
  char *res = response.data ();

  fprintf (stderr, "%s %s %s %d - %s\n", pfm_global->progname, __FILE__, __FUNCTION__, __LINE__, res);
  fflush (stderr);
}



void 
pfmPage::slotMapReadyReadStandardOutput ()
{
  QByteArray response = mapProc->readAllStandardOutput ();
  char *res = response.data ();

  fprintf (stderr, "%s %s %s %d - %s\n", pfm_global->progname, __FILE__, __FUNCTION__, __LINE__, res);
  fflush (stderr);
}



void 
pfmPage::slotMapError (QProcess::ProcessError error)
{
  switch (error)
    {
    case QProcess::FailedToStart:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("Unable to start the map editor process!"));
      break;

    case QProcess::Crashed:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("The map editor process crashed!"));
      break;

    case QProcess::Timedout:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("The map editor process timed out!"));
      break;

    case QProcess::WriteError:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("There was a write error from the map editor process!"));
      break;

    case QProcess::ReadError:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("There was a read error from the map editor process!"));
      break;

    case QProcess::UnknownError:
      QMessageBox::critical (this, tr ("pfmLoad map editor"), tr ("The map editor process died with an unknown error!"));
      break;
    }
}



void 
pfmPage::slotMapDone (int exitCode __attribute__ ((unused)), QProcess::ExitStatus exitStatus __attribute__ ((unused)))
{
  pfm_def->area = pfm_def->name + ".are";

  rename (temp_area_name.toLatin1 (), pfm_def->area.toLatin1 ());

  area_edit->setText (pfm_def->area);

  if (!pfm_def->name.isEmpty () && !pfm_def->area.isEmpty ()) setButtonText (QWizard::NextButton, tr ("Next"));
}



void 
pfmPage::slotAreaMap ()
{
  if (pfm_def->name.isEmpty ())
    {
      QMessageBox::warning (this, "pfmLoad", tr ("You must first set a PFM file name before trying to create an area file."));
    }
  else
    {
      temp_area_name.sprintf ("TEMPORARY_AREA_FILE_%d_%d.are", l_page_num - 1, getpid ());


      mapProc = new QProcess (this);
      QStringList arguments;


      if (pfm_def->area.isEmpty ())
        {
          arguments += "--empty_file";
          arguments += temp_area_name;

 
          connect (mapProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, 
                   SLOT (slotMapDone (int, QProcess::ExitStatus)));
          connect (mapProc, SIGNAL (readyReadStandardError ()), this, SLOT (slotMapReadyReadStandardError ()));
          connect (mapProc, SIGNAL (readyReadStandardOutput ()), this, SLOT (slotMapReadyReadStandardOutput ()));
          connect (mapProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotMapError (QProcess::ProcessError)));

          mapProc->start ("areaCheck", arguments);
        }
      else
        {
          arguments += "--file";
          arguments += pfm_def->area;

 
          connect (mapProc, SIGNAL (finished (int, QProcess::ExitStatus)), this, 
                   SLOT (slotMapDone (int, QProcess::ExitStatus)));
          connect (mapProc, SIGNAL (readyReadStandardError ()), this, SLOT (slotMapReadyReadStandardError ()));
          connect (mapProc, SIGNAL (readyReadStandardOutput ()), this, SLOT (slotMapReadyReadStandardOutput ()));
          connect (mapProc, SIGNAL (error (QProcess::ProcessError)), this, SLOT (slotMapError (QProcess::ProcessError)));

          mapProc->start ("areaCheck", arguments);
        }
    }
}



void 
pfmPage::slotAreaPFM ()
{
  QStringList         files, filters;
  QString             file;
  int32_t             hnd;
  PFM_OPEN_ARGS       open_args;


  if (pfm_def->name.isEmpty ())
    {
      QMessageBox::warning (this, "pfmLoad", tr ("You must first set a PFM file name before trying to use a pre-existing PFM area definition."));
    }
  else
    {
      QFileDialog *fd = new QFileDialog (this, tr ("pfmLoad Open PFM Structure (area)"));
      fd->setViewMode (QFileDialog::List);


      //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
      //  This function is in the nvutility library.

      setSidebarUrls (fd, pfm_global->area_dir);


      filters << QString ("PFM (*.pfm)");

      fd->setNameFilters (filters);
      fd->setFileMode (QFileDialog::ExistingFile);
      fd->selectNameFilter ("PFM (*.pfm)");

      if (fd->exec () == QDialog::Accepted)
	{
          //  Save the directory that we were in when we selected a file.

          pfm_global->area_dir = fd->directory ().absolutePath ();

	  files = fd->selectedFiles ();

	  QString pfm_name = files.at (0);


	  if (!pfm_name.isEmpty())
	    {
	      strcpy (open_args.list_path, pfm_name.toLatin1 ());
	      open_args.checkpoint = 0;

	      if ((hnd = open_existing_pfm_file (&open_args)) >= 0)
		{
		  temp_area_name.sprintf ("TEMPORARY_AREA_FILE_%d_%d.are", l_page_num - 1, getpid ());

		  FILE *fp;

		  if ((fp = fopen (temp_area_name.toLatin1 (), "w")) != NULL)
		    {
		      char ltstring[25], lnstring[25], hem;
		      double deg, min, sec;


		      for (int32_t i = 0 ; i < open_args.head.polygon_count ; i++)
			{
			  strcpy (ltstring, fixpos (open_args.head.polygon[i].y, &deg, &min, &sec, &hem, POS_LAT, POS_HDMS));
			  strcpy (lnstring, fixpos (open_args.head.polygon[i].x, &deg, &min, &sec, &hem, POS_LON, POS_HDMS));

			  fprintf (fp, "%s, %s\n", ltstring, lnstring);
			}

		      fclose (fp);


		      pfm_def->area = pfm_def->name + ".are";

		      rename (temp_area_name.toLatin1 (), pfm_def->area.toLatin1 ());

		      area_edit->setText (pfm_def->area);

		      if (!pfm_def->name.isEmpty () && !pfm_def->area.isEmpty ()) setButtonText (QWizard::NextButton, tr ("Next"));
		    }

		  close_pfm_file (hnd);
		}
	    }
	}
    }
}



void 
pfmPage::slotAreaNSEW ()
{
  if (pfm_def->name.isEmpty ())
    {
      QMessageBox::warning (this, "pfmLoad", tr ("You must first set a PFM file name before trying to use a pre-existing PFM area definition."));
    }
  else
    {
      nsewD = new QDialog (this, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
      nsewD->setWindowTitle (tr ("pfmLoad NSEW bounds"));


      QVBoxLayout *nsewBox = new QVBoxLayout (nsewD);
      nsewBox->setMargin (5);
      nsewBox->setSpacing (5);


      QGroupBox *northBox = new QGroupBox (tr ("North Latitude"), nsewD);
      QHBoxLayout *northBoxLayout = new QHBoxLayout;
      northBox->setLayout (northBoxLayout);
      northBoxLayout->setSpacing (10);

      north = new QLineEdit (nsewD);
      north->setToolTip (tr ("Set the northern boundary of the area"));
      north->setWhatsThis (northText);

      northBoxLayout->addWidget (north, 1);

      nsewBox->addWidget (northBox);


      QGroupBox *southBox = new QGroupBox (tr ("South Latitude"), nsewD);
      QHBoxLayout *southBoxLayout = new QHBoxLayout;
      southBox->setLayout (southBoxLayout);
      southBoxLayout->setSpacing (10);

      south = new QLineEdit (nsewD);
      south->setToolTip (tr ("Set the southern boundary of the area"));
      south->setWhatsThis (southText);

      southBoxLayout->addWidget (south, 1);

      nsewBox->addWidget (southBox);


      QGroupBox *westBox = new QGroupBox (tr ("West Longitude"), nsewD);
      QHBoxLayout *westBoxLayout = new QHBoxLayout;
      westBox->setLayout (westBoxLayout);
      westBoxLayout->setSpacing (10);

      west = new QLineEdit (nsewD);
      west->setToolTip (tr ("Set the western boundary of the area"));
      west->setWhatsThis (westText);

      westBoxLayout->addWidget (west, 1);

      nsewBox->addWidget (westBox);


      QGroupBox *eastBox = new QGroupBox (tr ("East Longitude"), nsewD);
      QHBoxLayout *eastBoxLayout = new QHBoxLayout;
      eastBox->setLayout (eastBoxLayout);
      eastBoxLayout->setSpacing (10);

      east = new QLineEdit (nsewD);
      east->setToolTip (tr ("Set the eastern boundary of the area"));
      east->setWhatsThis (eastText);

      eastBoxLayout->addWidget (east, 1);

      nsewBox->addWidget (eastBox);


      QHBoxLayout *actions = new QHBoxLayout (0);
      nsewBox->addLayout (actions);

      QPushButton *bHelp = new QPushButton (nsewD);
      bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
      bHelp->setToolTip (tr ("Enter What's This mode for help"));
      connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
      actions->addWidget (bHelp);

      actions->addStretch (10);


      QPushButton *applyButton = new QPushButton (tr ("OK"), nsewD);
      applyButton->setToolTip (tr ("Accept values and close dialog"));
      applyButton->setWhatsThis (applyNSEWText);
      connect (applyButton, SIGNAL (clicked ()), this, SLOT (slotApplyNSEW ()));
      actions->addWidget (applyButton);

      QPushButton *closeButton = new QPushButton (tr ("Cancel"), nsewD);
      closeButton->setToolTip (tr ("Discard values and close dialog"));
      closeButton->setWhatsThis (closeNSEWText);
      connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseNSEW ()));
      actions->addWidget (closeButton);


      nsewD->show ();
    }
}



void
pfmPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void 
pfmPage::slotApplyNSEW ()
{
  if (north->text ().isEmpty () || south->text ().isEmpty () || west->text ().isEmpty () || east->text ().isEmpty ())
    {
      QMessageBox::warning (this, tr ("pfmLoad NSEW bounds"), tr ("You must set all four fields or use the Cancel button."));
    }
  else
    {
      char nlat[50], slat[50], elon[50], wlon[50], file[512], hem;
      double ndegs, sdegs, wdegs, edegs, deg, min, sec, tmp;

      strcpy (nlat, north->text ().toLatin1 ());
      strcpy (slat, south->text ().toLatin1 ());
      strcpy (elon, east->text ().toLatin1 ());
      strcpy (wlon, west->text ().toLatin1 ());

      posfix (nlat, &ndegs, POS_LAT);
      posfix (slat, &sdegs, POS_LAT);
      posfix (elon, &edegs, POS_LON);
      posfix (wlon, &wdegs, POS_LON);

      if (ndegs < sdegs)
	{
	  tmp = sdegs;
	  sdegs = ndegs;
	  ndegs = tmp;
	}

      if (edegs < wdegs)
	{
	  if ((edegs < 0.0 && wdegs < 0.0) || (edegs >= 0.0 && wdegs >= 0.0))
	    {
	      tmp = wdegs;
	      wdegs = edegs;
	      edegs = tmp;
	    }
	}


      pfm_def->area = pfm_def->name + ".are";

      strcpy (file, pfm_def->area.toLatin1 ());

      FILE *fp;
      
      if ((fp = fopen (file, "w")) == NULL)
	{
	  QMessageBox::warning (this, tr ("pfmLoad NSEW bounds"), tr ("Unable to open area file %1").arg (pfm_def->area));
	}
      else
	{
	  strcpy (nlat, fixpos (ndegs, &deg, &min, &sec, &hem, POS_LAT, POS_HDMS));
	  strcpy (slat, fixpos (sdegs, &deg, &min, &sec, &hem, POS_LAT, POS_HDMS));
	  strcpy (wlon, fixpos (wdegs, &deg, &min, &sec, &hem, POS_LON, POS_HDMS));
	  strcpy (elon, fixpos (edegs, &deg, &min, &sec, &hem, POS_LON, POS_HDMS));

	  fprintf (fp, "%s, %s\n", slat, wlon);
	  fprintf (fp, "%s, %s\n", nlat, wlon);
	  fprintf (fp, "%s, %s\n", nlat, elon);
	  fprintf (fp, "%s, %s\n", slat, elon);

	  fclose (fp);

	  area_edit->setText (pfm_def->area);

	  if (!pfm_def->name.isEmpty () && !pfm_def->area.isEmpty ()) setButtonText (QWizard::NextButton, tr ("Next"));
	}

      nsewD->close ();
    }
}



void 
pfmPage::slotCloseNSEW ()
{
  nsewD->close ();
}



void 
pfmPage::setFields (PFM_DEFINITION *pfmDef)
{
  pfm_def = pfmDef;

  if (pfm_def->name != "") setField (pfm_file_edit_field, pfm_def->name);

  setField (area_edit_field, pfm_def->area);
                                     
  setField (mBinSizeField, pfm_def->mbin_size);
  setField (gBinSizeField, pfm_def->gbin_size);
  setField (minDepthField, pfm_def->min_depth);
  setField (maxDepthField, pfm_def->max_depth);

  if (pfm_def->precision < 0.1)
    {
      setField (precisionField, 0);
    }
  else if (pfm_def->precision < 1.0)
    {
      setField (precisionField, 1);
    }
  else
    {
      setField (precisionField, 2);
    }

  setField (mosaic_edit_field, pfm_def->mosaic);
  setField (feature_edit_field, pfm_def->feature);

  setField (applyFilterField, pfm_def->apply_area_filter);
  setField (deepFilterField, pfm_def->deep_filter_only);

  setField (stdSpinField, pfm_def->cellstd);

  setField (featureRadiusField, pfm_def->radius);
}
