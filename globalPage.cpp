
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



#include "globalPage.hpp"
#include "globalPageHelp.hpp"


globalPage::globalPage (QWidget *prnt, PFM_GLOBAL *pfm_glb, FLAGS *fl, GLOBAL_DIALOG *gd, uint8_t cube_available):
  QWizardPage (prnt)
{
  pfm_global = pfm_glb;
  global_dialog = gd;
  parent = prnt;
  flags = fl;
  global_dialog->gsfD = NULL;
  global_dialog->hofD = NULL;
  global_dialog->tofD = NULL;
  global_dialog->czmilD = NULL;
  global_dialog->lasD = NULL;
  global_dialog->bagD = NULL;


  setTitle (tr ("Global Load Options"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmLoadWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QGroupBox *optBox = new QGroupBox (tr ("Data Type Options"), this);
  QHBoxLayout *optBoxLayout = new QHBoxLayout;
  optBox->setLayout (optBoxLayout);

  globalTab = new QTabWidget ();
  globalTab->setTabPosition (QTabWidget::North);

  optBoxLayout->addWidget (globalTab);


  // *********************************  Global Flag Settings *********************************

  QGroupBox *globalBox = new QGroupBox (this);
  QHBoxLayout *globalBoxLayout = new QHBoxLayout;
  globalBox->setLayout (globalBoxLayout);
  QVBoxLayout *globalBoxLLayout = new QVBoxLayout;
  QVBoxLayout *globalBoxRLayout = new QVBoxLayout;
  globalBoxLayout->addLayout (globalBoxLLayout);
  globalBoxLayout->addLayout (globalBoxRLayout);


  optGrp = new QButtonGroup (this);
  optGrp->setExclusive (false);
  connect (optGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotOptClicked (int)));


  refData = new QCheckBox (tr ("Mark data as reference"), this);
  refData->setChecked (flags->ref);
  refData->setToolTip (tr ("Mark input data as reference only"));
  refData->setWhatsThis (refDataText);
  globalBoxLLayout->addWidget (refData);


  subPath = new QCheckBox (tr ("Invert paths"), this);
  subPath->setChecked (flags->sub);
  subPath->setToolTip (tr ("Invert paths from UNIX to Windows syntax"));
  subPath->setWhatsThis (subPathText);
  globalBoxLLayout->addWidget (subPath);

  if ((find_startup_name (".pfm_cfg")) == NULL) subPath->setEnabled (false);

#ifndef NVLinux
  subPath->setEnabled (false);
#endif


  expCheck = new QCheckBox (tr ("Half bin area expansion"), this);
  expCheck->setChecked (flags->expand);
  expCheck->setToolTip (tr ("Expand MBR by one-half bin in all directions"));
  expCheck->setWhatsThis (expCheckText);
  globalBoxLLayout->addWidget (expCheck);


  cubeAttributes = new QCheckBox (tr ("Insert CUBE bin attributes"), this);
  cubeAttributes->setChecked (flags->attr);
  cubeAttributes->setToolTip (tr ("Save space for the CUBE bin attributes"));
  cubeAttributes->setWhatsThis (cubeAttributesText);
  globalBoxLLayout->addWidget (cubeAttributes);

  if (!cube_available)
    {
      cubeAttributes->setVisible (false);
      cubeAttributes->setChecked (false);
      cubeAttributes->setEnabled (false);
    }


  //  The SRTM stuff is set up as a pseudo radio box via the slot.

  srtm1 = check_srtm1_topo ();
  srtm2 = check_srtm2_topo ();
  srtm3 = check_srtm3_topo ();
  srtm30 = check_srtm30_topo ();


  srtmBest = new QCheckBox (tr ("Load best resolution SRTM data"), this);
  srtmBest->setChecked (flags->srtmb);
  srtmBest->setToolTip (tr ("Load the best available resolution SRTM data if available"));
  srtmBest->setWhatsThis (srtmBestText);
  optGrp->addButton (srtmBest, 0);
  globalBoxRLayout->addWidget (srtmBest);

  if (!srtm1 && !srtm2 && !srtm3 &&!srtm30) srtmBest->setEnabled (false);


  srtmOne = new QCheckBox (tr ("Load 1 second resolution SRTM data"), this);
  srtmOne->setChecked (flags->srtm1);
  srtmOne->setToolTip (tr ("Load the 1 second resolution SRTM data if available"));
  srtmOne->setWhatsThis (srtmOneText);
  optGrp->addButton (srtmOne, 1);
  globalBoxRLayout->addWidget (srtmOne);

  srtmOne->setEnabled (srtm1);


  srtmThree = new QCheckBox (tr ("Load 3 second resolution SRTM data"), this);
  srtmThree->setChecked (flags->srtm3);
  srtmThree->setToolTip (tr ("Load the 3 second resolution SRTM data if available"));
  srtmThree->setWhatsThis (srtmThreeText);
  optGrp->addButton (srtmThree, 2);
  globalBoxRLayout->addWidget (srtmThree);

  srtmThree->setEnabled (srtm3);


  srtmThirty = new QCheckBox (tr ("Load 30 second resolution SRTM data"), this);
  srtmThirty->setChecked (flags->srtm30);
  srtmThirty->setToolTip (tr ("Load the 30 second resolution SRTM data if available"));
  srtmThirty->setWhatsThis (srtmThirtyText);
  optGrp->addButton (srtmThirty, 3);
  globalBoxRLayout->addWidget (srtmThirty);

  srtmThirty->setEnabled (srtm30);


  srtmRef = new QCheckBox (tr ("Load SRTM data as reference"), this);
  srtmRef->setChecked (flags->srtmr);
  srtmRef->setToolTip (tr ("Mark any loaded SRTM data as reference"));
  srtmRef->setWhatsThis (srtmRefText);
  globalBoxRLayout->addWidget (srtmRef);

  if (!srtm1 && !srtm2 && !srtm3 &&!srtm30) srtmRef->setEnabled (false);


  srtm2Ex = new QCheckBox (tr ("Exclude SRTM2 data"), this);
  srtm2Ex->setChecked (flags->srtme);
  srtm2Ex->setToolTip (tr ("Do not load any limited distribution SRTM2 data"));
  srtm2Ex->setWhatsThis (srtm2ExText);
  optGrp->addButton (srtm2Ex, 4);
  globalBoxRLayout->addWidget (srtm2Ex);

  srtm2Ex->setEnabled (srtm2);



  if (getenv ("ABE_DATA") == NULL)
    {
      srtmBest->setEnabled (false);
      srtmOne->setEnabled (false);
      srtmThree->setEnabled (false);
      srtmThirty->setEnabled (false);
      srtmRef->setEnabled (false);
    }
  else
    {
      if (flags->srtmb)
        {
          srtmOne->setChecked (false);
          srtmThree->setChecked (false);
          srtmThirty->setChecked (false);
        }
      if (flags->srtm1)
        {
          srtmBest->setChecked (false);
          srtmThree->setChecked (false);
          srtmThirty->setChecked (false);
        }

      if (flags->srtm3)
        {
          srtmBest->setChecked (false);
          srtmOne->setChecked (false);
          srtmThirty->setChecked (false);
        }

      if (flags->srtm30)
        {
          srtmBest->setChecked (false);
          srtmOne->setChecked (false);
          srtmThree->setChecked (false);
        }
    }


  // *********************************  CZMIL Flag Settings *********************************

  czmilBox = new QGroupBox (this);
  QHBoxLayout *czmilBoxLayout = new QHBoxLayout;
  czmilBox->setLayout (czmilBoxLayout);
  QVBoxLayout *czmilBoxLLayout = new QVBoxLayout;
  QVBoxLayout *czmilBoxRLayout = new QVBoxLayout;
  czmilBoxLayout->addLayout (czmilBoxLLayout);
  czmilBoxLayout->addLayout (czmilBoxRLayout);


  czmilLoadGrp = new QButtonGroup (this);
  czmilLoadGrp->setExclusive (false);


  czmilChan[CZMIL_SHALLOW_CHANNEL_1] = new QCheckBox (tr ("Load Shallow Channel 1 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setToolTip (tr ("Load shallow channel 1 (central channel) data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_1]->setWhatsThis (czmil1Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_1], CZMIL_SHALLOW_CHANNEL_1);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_1]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_2] = new QCheckBox (tr ("Load Shallow Channel 2 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setToolTip (tr ("Load shallow channel 2 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_2]->setWhatsThis (czmil2Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_2], CZMIL_SHALLOW_CHANNEL_2);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_2]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_3] = new QCheckBox (tr ("Load Shallow Channel 3 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setToolTip (tr ("Load shallow channel 3 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_3]->setWhatsThis (czmil3Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_3], CZMIL_SHALLOW_CHANNEL_3);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_3]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_4] = new QCheckBox (tr ("Load Shallow Channel 4 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setToolTip (tr ("Load shallow channel 4 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_4]->setWhatsThis (czmil4Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_4], CZMIL_SHALLOW_CHANNEL_4);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_4]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_5] = new QCheckBox (tr ("Load Shallow Channel 5 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setToolTip (tr ("Load shallow channel 5 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_5]->setWhatsThis (czmil5Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_5], CZMIL_SHALLOW_CHANNEL_5);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_5]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_6] = new QCheckBox (tr ("Load Shallow Channel 6 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setToolTip (tr ("Load shallow channel 6 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_6]->setWhatsThis (czmil6Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_6], CZMIL_SHALLOW_CHANNEL_6);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_6]);

  czmilChan[CZMIL_SHALLOW_CHANNEL_7] = new QCheckBox (tr ("Load Shallow Channel 7 data"), this);
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setChecked (flags->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setToolTip (tr ("Load shallow channel 7 data"));
  czmilChan[CZMIL_SHALLOW_CHANNEL_7]->setWhatsThis (czmil7Text);
  czmilLoadGrp->addButton (czmilChan[CZMIL_SHALLOW_CHANNEL_7], CZMIL_SHALLOW_CHANNEL_7);
  czmilBoxLLayout->addWidget (czmilChan[CZMIL_SHALLOW_CHANNEL_7]);

  czmilChan[CZMIL_IR_CHANNEL] = new QCheckBox (tr ("Load IR data"), this);
  czmilChan[CZMIL_IR_CHANNEL]->setChecked (flags->czmil_chan[CZMIL_IR_CHANNEL]);
  czmilChan[CZMIL_IR_CHANNEL]->setToolTip (tr ("Load IR data (not a good idea)"));
  czmilChan[CZMIL_IR_CHANNEL]->setWhatsThis (czmilIRText);
  czmilLoadGrp->addButton (czmilChan[CZMIL_IR_CHANNEL], CZMIL_IR_CHANNEL);
  czmilBoxRLayout->addWidget (czmilChan[CZMIL_IR_CHANNEL]);

  czmilChan[CZMIL_DEEP_CHANNEL] = new QCheckBox (tr ("Load Deep Channel data"), this);
  czmilChan[CZMIL_DEEP_CHANNEL]->setChecked (flags->czmil_chan[CZMIL_DEEP_CHANNEL]);
  czmilChan[CZMIL_DEEP_CHANNEL]->setToolTip (tr ("Load Deep Channel data"));
  czmilChan[CZMIL_DEEP_CHANNEL]->setWhatsThis (czmilDeepText);
  czmilLoadGrp->addButton (czmilChan[CZMIL_DEEP_CHANNEL], CZMIL_DEEP_CHANNEL);
  czmilBoxRLayout->addWidget (czmilChan[CZMIL_DEEP_CHANNEL]);


  ref_ws = new QCheckBox (tr ("Reference water surface data"), this);
  ref_ws->setChecked (flags->ref_water_surface);
  ref_ws->setToolTip (tr ("Load water surface data as reference points"));
  ref_ws->setWhatsThis (ref_wsText);
  connect (ref_ws, SIGNAL (stateChanged (int)), this, SLOT (slotRefWsClicked (int)));
  czmilBoxRLayout->addWidget (ref_ws);

  wsClass = new QCheckBox (tr ("Load water surface data"), this);
  wsClass->setChecked (flags->czmil_water_surface);
  wsClass->setToolTip (tr ("Load data classified as water surface"));
  wsClass->setWhatsThis (wsClassText);
  czmilBoxRLayout->addWidget (wsClass);

  landClass = new QCheckBox (tr ("Load land data"), this);
  landClass->setChecked (flags->czmil_land);
  landClass->setToolTip (tr ("Load data classified as land"));
  landClass->setWhatsThis (landClassText);
  czmilBoxRLayout->addWidget (landClass);

  waterClass = new QCheckBox (tr ("Load water data"), this);
  waterClass->setChecked (flags->czmil_water);
  waterClass->setToolTip (tr ("Load data classified as water"));
  waterClass->setWhatsThis (waterClassText);
  czmilBoxRLayout->addWidget (waterClass);

  HFfilt = new QCheckBox (tr ("Load HydroFusion filter invalidated data"), this);
  HFfilt->setChecked (flags->HF_filt);
  HFfilt->setToolTip (tr ("Load data that has been invalidated by HydroFusion filters"));
  HFfilt->setWhatsThis (HFfiltText);
  czmilBoxRLayout->addWidget (HFfilt);


  // *********************************  GSF Flag Settings *********************************

  gsfBox = new QGroupBox (this);
  QHBoxLayout *gsfBoxLayout = new QHBoxLayout;
  gsfBox->setLayout (gsfBoxLayout);
  QVBoxLayout *gsfBoxLLayout = new QVBoxLayout;
  QVBoxLayout *gsfBoxRLayout = new QVBoxLayout;
  gsfBoxLayout->addLayout (gsfBoxLLayout);
  gsfBoxLayout->addLayout (gsfBoxRLayout);


  gsfNominal = new QCheckBox (tr ("Load nominal depths from GSF files"), this);
  gsfNominal->setChecked (flags->nom);
  gsfNominal->setToolTip (tr ("Load the nominal depths from GSF files if available"));
  gsfNominal->setWhatsThis (gsfNominalText);
  gsfBoxLLayout->addWidget (gsfNominal);


  // *********************************  HOF Flag Settings *********************************

  hofBox = new QGroupBox (this);
  QHBoxLayout *hofBoxLayout = new QHBoxLayout;
  hofBox->setLayout (hofBoxLayout);
  QVBoxLayout *hofBoxLLayout = new QVBoxLayout;
  QVBoxLayout *hofBoxRLayout = new QVBoxLayout;
  hofBoxLayout->addLayout (hofBoxLLayout);
  hofBoxLayout->addLayout (hofBoxRLayout);


  hofOld = new QCheckBox (tr ("Load HOF in GCS compatible form"), this);
  hofOld->setChecked (flags->old);
  hofOld->setToolTip (tr ("Load HOF data in the old PFM_SHOALS_1K_DATA (GCS compatible) form"));
  hofOld->setWhatsThis (hofOldText);
  connect (hofOld, SIGNAL (stateChanged (int)), this, SLOT (slotHofOldClicked (int)));
  hofBoxLLayout->addWidget (hofOld);

  invLand = new QCheckBox (tr ("Invalidate HOF land values"), this);
  invLand->setChecked (flags->lnd);
  invLand->setToolTip (tr ("Invalidate all HOF values that are marked as Land"));
  invLand->setWhatsThis (invLandText);
  hofBoxLLayout->addWidget (invLand);

  invSec = new QCheckBox (tr ("Invalidate HOF secondary values"), this);
  invSec->setChecked (flags->sec);
  invSec->setToolTip (tr ("Invalidate all HOF secondary return values"));
  invSec->setWhatsThis (invSecText);
  hofBoxLLayout->addWidget (invSec);

  if (flags->old) invSec->setEnabled (false);

  hofNull = new QCheckBox (tr ("Load HOF null values"), this);
  hofNull->setChecked (flags->hof);
  hofNull->setToolTip (tr ("Load HOF null values into the PFM structure"));
  hofNull->setWhatsThis (hofNullText);
  hofBoxLLayout->addWidget (hofNull);


  // *********************************  TOF Flag Settings *********************************

  tofBox = new QGroupBox (this);
  QHBoxLayout *tofBoxLayout = new QHBoxLayout;
  tofBox->setLayout (tofBoxLayout);
  QVBoxLayout *tofBoxLLayout = new QVBoxLayout;
  QVBoxLayout *tofBoxRLayout = new QVBoxLayout;
  tofBoxLayout->addLayout (tofBoxLLayout);
  tofBoxLayout->addLayout (tofBoxRLayout);


  tofRef = new QCheckBox (tr ("Load TOF first returns as reference"), this);
  tofRef->setChecked (flags->tof);
  tofRef->setToolTip (tr ("Load TOF first return values as reference points"));
  tofRef->setWhatsThis (tofRefText);
  tofBoxLLayout->addWidget (tofRef);


  globalTab->addTab (globalBox, tr ("Global"));
  globalTab->addTab (czmilBox, "CZMIL");
  globalTab->addTab (gsfBox, "GSF");
  globalTab->addTab (hofBox, "HOF");
  globalTab->addTab (tofBox, "TOF");


  vbox->addWidget (optBox, 1);


  //  Index file attribute selection.

  QGroupBox *attrBox = new QGroupBox (tr ("Attributes"), this);
  QHBoxLayout *attrBoxLayout = new QHBoxLayout;
  attrBox->setLayout (attrBoxLayout);
  attrBoxLayout->setSpacing (10);


  QGroupBox *tbox = new QGroupBox (tr ("Time (POSIX minutes)"), this);
  tbox->setWhatsThis (timeCheckText);
  QHBoxLayout *tboxLayout = new QHBoxLayout;
  tbox->setLayout (tboxLayout);

  timeCheck = new QCheckBox (tbox);
  timeCheck->setToolTip (tr ("Insert time (in POSIX minutes) as an attribute for any data type"));
  timeCheck->setWhatsThis (timeCheckText);
  if (pfm_global->time_attribute_num)
    {
      timeCheck->setChecked (true);
    }
  else
    {
      timeCheck->setChecked (false);
    }
  tboxLayout->addWidget (timeCheck);
  connect (timeCheck, SIGNAL (clicked (bool)), this, SLOT (slotTimeCheckClicked (bool)));
  attrBoxLayout->addWidget (tbox);


  int32_t cnt = 0;
  QString attrString;

  if ((cnt = gsfAttributesSet (pfm_global->gsf_attribute_num)))
    {
      attrString = QString ("GSF (%1)").arg (cnt);
      gsf = new QPushButton (attrString, this);
    }
  else
    {
      gsf = new QPushButton ("GSF", this);
    }
  gsf->setToolTip (tr ("Select depth attributes for GSF files (* indicates some attributes set)"));
  gsf->setWhatsThis (gsfText);
  connect (gsf, SIGNAL (clicked ()), this, SLOT (slotGSFClicked ()));
  attrBoxLayout->addWidget (gsf);

  gsfPalette = gsf->palette ();
  widgetBackgroundColor = gsfPalette.color (QPalette::Normal, QPalette::Button);
  if (gsfAttributesSet (pfm_global->gsf_attribute_num))
    {
      gsfPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      gsfPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      gsf->setPalette (gsfPalette);
    }


  if ((cnt = hofAttributesSet (pfm_global->hof_attribute_num)))
    {
      attrString = QString ("HOF (%1)").arg (cnt);
      hof = new QPushButton (attrString, this);
    }
  else
    {
      hof = new QPushButton ("HOF", this);
    }
  hof->setToolTip (tr ("Select depth attributes for HOF files (red background indicates some attributes set)"));
  hof->setWhatsThis (hofText);
  connect (hof, SIGNAL (clicked ()), this, SLOT (slotHOFClicked ()));
  attrBoxLayout->addWidget (hof);

  hofPalette = hof->palette ();
  if (hofAttributesSet (pfm_global->hof_attribute_num))
    {
      hofPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      hofPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      hof->setPalette (hofPalette);
    }


  if ((cnt = tofAttributesSet (pfm_global->tof_attribute_num)))
    {
      attrString = QString ("TOF (%1)").arg (cnt);
      tof = new QPushButton (attrString, this);
    }
  else
    {
      tof = new QPushButton ("TOF", this);
    }
  tof->setToolTip (tr ("Select depth attributes for TOF files (red background indicates some attributes set)"));
  tof->setWhatsThis (tofText);
  connect (tof, SIGNAL (clicked ()), this, SLOT (slotTOFClicked ()));
  attrBoxLayout->addWidget (tof);

  tofPalette = tof->palette ();
  if (tofAttributesSet (pfm_global->tof_attribute_num))
    {
      tofPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      tofPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      tof->setPalette (tofPalette);
    }


  if ((cnt = czmilAttributesSet (pfm_global->czmil_attribute_num)))
    {
      attrString = QString ("CZMIL (%1)").arg (cnt);
      czmil = new QPushButton (attrString, this);
    }
  else
    {
      czmil = new QPushButton ("CZMIL", this);
    }
  czmil->setToolTip (tr ("Select depth attributes for CZMIL files (red background indicates some attributes set)"));
  czmil->setWhatsThis (czmilText);
  connect (czmil, SIGNAL (clicked ()), this, SLOT (slotCZMILClicked ()));
  attrBoxLayout->addWidget (czmil);

  czmilPalette = czmil->palette ();
  if (czmilAttributesSet (pfm_global->czmil_attribute_num))
    {
      czmilPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      czmilPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      czmil->setPalette (czmilPalette);
    }

  if ((cnt = lasAttributesSet (pfm_global->las_attribute_num)))
    {
      attrString = QString ("LAS (%1)").arg (cnt);
      las = new QPushButton (attrString, this);
    }
  else
    {
      las = new QPushButton ("LAS", this);
    }
  las->setToolTip (tr ("Select depth attributes for LAS files (red background indicates some attributes set)"));
  las->setWhatsThis (lasText);
  connect (las, SIGNAL (clicked ()), this, SLOT (slotLASClicked ()));
  attrBoxLayout->addWidget (las);

  lasPalette = las->palette ();
  if (lasAttributesSet (pfm_global->las_attribute_num))
    {
      lasPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      lasPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      las->setPalette (lasPalette);
    }

  if ((cnt = bagAttributesSet (pfm_global->bag_attribute_num)))
    {
      attrString = QString ("BAG (%1)").arg (cnt);
      bag = new QPushButton (attrString, this);
    }
  else
    {
      bag = new QPushButton ("BAG", this);
    }
  bag->setToolTip (tr ("Select depth attributes for BAG files (red background indicates some attributes set)"));
  bag->setWhatsThis (bagText);
  connect (bag, SIGNAL (clicked ()), this, SLOT (slotBAGClicked ()));
  attrBoxLayout->addWidget (bag);

  bagPalette = bag->palette ();
  if (bagAttributesSet (pfm_global->bag_attribute_num))
    {
      bagPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      bagPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      bag->setPalette (bagPalette);
    }


  vbox->addWidget (attrBox);


  QGroupBox *hvBox = new QGroupBox (tr ("Horizontal and vertical errors"), this);
  QHBoxLayout *hvBoxLayout = new QHBoxLayout;
  hvBox->setLayout (hvBoxLayout);
  hvBoxLayout->setSpacing (10);


  QGroupBox *horizontalBox = new QGroupBox (tr ("Default horizontal error"), this);
  QHBoxLayout *horizontalBoxLayout = new QHBoxLayout;
  horizontalBox->setLayout (horizontalBoxLayout);
  horizontalBoxLayout->setSpacing (10);

  horizontal = new QDoubleSpinBox (this);
  horizontal->setDecimals (2);
  horizontal->setRange (0.0, 100.0);
  horizontal->setSingleStep (5.0);
  horizontal->setValue (pfm_global->horizontal_error);
  horizontal->setWrapping (true);
  horizontal->setToolTip (tr ("Set the default horizontal error value"));
  horizontal->setWhatsThis (horizontalText);
  horizontalBoxLayout->addWidget (horizontal);


  hvBoxLayout->addWidget (horizontalBox);


  QGroupBox *verticalBox = new QGroupBox (tr ("Default vertical error"), this);
  QHBoxLayout *verticalBoxLayout = new QHBoxLayout;
  verticalBox->setLayout (verticalBoxLayout);
  verticalBoxLayout->setSpacing (10);

  vertical = new QDoubleSpinBox (this);
  vertical->setDecimals (2);
  vertical->setRange (0.0, 100.0);
  vertical->setSingleStep (5.0);
  vertical->setValue (pfm_global->vertical_error);
  vertical->setWrapping (true);
  vertical->setToolTip (tr ("Set the default vertical error value"));
  vertical->setWhatsThis (verticalText);
  verticalBoxLayout->addWidget (vertical);


  hvBoxLayout->addWidget (verticalBox);


  vbox->addWidget (hvBox);

  registerField ("hofOld", hofOld);
  registerField ("hofNull", hofNull);
  registerField ("subPath", subPath);
  registerField ("invLand", invLand);
  registerField ("invSec", invSec);
  registerField ("refData", refData);
  registerField ("expCheck", expCheck);
  registerField ("tofRef", tofRef);
  registerField ("wsClass", wsClass);
  registerField ("landClass", landClass);
  registerField ("waterClass", waterClass);
  registerField ("ref_ws", ref_ws);
  registerField ("HFfilt", HFfilt);
  registerField ("czmilChan1", czmilChan[CZMIL_SHALLOW_CHANNEL_1]);
  registerField ("czmilChan2", czmilChan[CZMIL_SHALLOW_CHANNEL_2]);
  registerField ("czmilChan3", czmilChan[CZMIL_SHALLOW_CHANNEL_3]);
  registerField ("czmilChan4", czmilChan[CZMIL_SHALLOW_CHANNEL_4]);
  registerField ("czmilChan5", czmilChan[CZMIL_SHALLOW_CHANNEL_5]);
  registerField ("czmilChan6", czmilChan[CZMIL_SHALLOW_CHANNEL_6]);
  registerField ("czmilChan7", czmilChan[CZMIL_SHALLOW_CHANNEL_7]);
  registerField ("czmilIRChan", czmilChan[CZMIL_IR_CHANNEL]);
  registerField ("czmilDeepChan", czmilChan[CZMIL_DEEP_CHANNEL]);
  registerField ("gsfNominal", gsfNominal);
  registerField ("srtmBest", srtmBest);
  registerField ("srtmOne", srtmOne);
  registerField ("srtmThree", srtmThree);
  registerField ("srtmThirty", srtmThirty);
  registerField ("srtmRef", srtmRef);
  registerField ("srtm2Ex", srtm2Ex);
  registerField ("cubeAttributes", cubeAttributes);

  registerField ("horizontal", horizontal, "value", "valueChanged");
  registerField ("vertical", vertical, "value", "valueChanged");
}



bool 
globalPage::validatePage ()
{
  if ((!wsClass->isChecked ()) && (!landClass->isChecked ()) && (!waterClass->isChecked ()))
    {
      QMessageBox::critical (this, "pfmLoad", tr ("You must select one or more of <b>Load water surface data</b>, <b>Load land data</b>, or "
                                                  "<b>Load water data</b> in the CZMIL tab to continue!"));

      return (false);
    }

  bool data = false;

  for (int32_t i = 0 ; i < 9 ; i++)
    {
      if (czmilChan[i]->isChecked ())
        {
          data = true;
          break;
        }
    }

  if (!data)
    {          
      QMessageBox::critical (this, "pfmLoad",
                             tr ("You must select one or more of the <b>Load channel data</b> boxes in the CZMIL tab to continue!"));
    }

  if (HFfilt->isChecked ())
    {
      QMessageBox msgBox (this);
      msgBox.setIcon (QMessageBox::Question);
      msgBox.setInformativeText (tr ("You have requested to load CZMIL data that has been invalidated by a HydroFusion filter. "
                                     "You should only do this if you are testing a new version of HydroFusion and "
                                     "expect that there might be filtered returns that need to be restored.<br><br>"
                                     "Do you reallly want to do this?"));
      msgBox.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
      msgBox.setDefaultButton (QMessageBox::No);
      int32_t ret = msgBox.exec ();

      if (ret == QMessageBox::No) HFfilt->setChecked (false);
    }

  return (data);
}



void
globalPage::slotHelp ()
{
  QWhatsThis::enterWhatsThisMode ();
}



void
globalPage::slotHofOldClicked (int state)
{
  if (state)
    {
      invSec->setEnabled (false);

      QMessageBox::warning (this, tr ("pfmLoad GCS Compatible Mode"),
                            tr ("Do not use GCS compatible mode unless you are going to reprocess the data in the PFM using Optech's GCS program!"));
    }
  else
    {
      invSec->setEnabled (true);
    }
}



void
globalPage::slotRefWsClicked (int state)
{
  //  If we turned on "Reference water surface data", turn on "Load water surface".

  if (state)
    {
      flags->czmil_water_surface = NVTrue;
      flags->ref_water_surface = NVTrue;

      wsClass->setChecked (true);
    }
  else
    {
      QMessageBox::warning (this, "pfmLoad",
                            tr ("Since you are turning off the CZMIL Reference Water surface flag you may want to turn off loading CZMIL Water surface data."));
    }
}



void
globalPage::slotTimeCheckClicked (bool state)
{
  //  If we unset the time attribute check box we need to adjust all of the already selected attributes.

  if (!state)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->time_attribute_num;
      pfm_global->time_attribute_num = 0;


      //  Move all lower attributes up one and decrement the counter.

      move_and_decrement (selected);
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          timeCheck->setChecked (false);

          QMessageBox::warning (this, tr ("pfmLoad Time Attribute"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->time_attribute_num = pfm_global->attribute_count;
        }
    }
}



void
globalPage::slotGSFClicked ()
{
  if (global_dialog->gsfD) global_dialog->gsfD->close ();

  global_dialog->gsfD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->gsfD->setWindowTitle (tr ("pfmLoad GSF Attributes"));

  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->gsfD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  gsfGrp = new QButtonGroup (global_dialog->gsfD);
  gsfGrp->setExclusive (false);
  connect (gsfGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotGSFAttrClicked (int)));

  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    {
      gsf_attr[i] = new QCheckBox (QString (pfm_global->gsf_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->gsf_attribute[i].name);
      gsf_attr[i]->setToolTip (string);
      gsfGrp->addButton (gsf_attr[i], i);

      if (pfm_global->gsf_attribute_num[i]) gsf_attr[i]->setChecked (true);

      if (pfm_global->appending) gsf_attr[i]->setEnabled (false);

      vbox->addWidget (gsf_attr[i]);
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->gsfD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the GSF attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearGSF ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->gsfD);
  closeButton->setToolTip (tr ("Close the GSF attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseGSF ()));
  actions->addWidget (closeButton);

  global_dialog->gsfD->show ();
}



void 
globalPage::slotGSFAttrClicked (int index)
{
  //  If we unset a GSF attribute check box we need to adjust all of the already selected attributes.

  if (gsf_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->gsf_attribute_num[index];

      for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
        {
          if (pfm_global->gsf_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->gsf_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (gsfGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotGSFAttrClicked (int)));
          gsf_attr[index]->setChecked (false);
          connect (gsfGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotGSFAttrClicked (int)));

          QMessageBox::warning (global_dialog->gsfD, tr ("pfmLoad GSF Attributes"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->gsf_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = gsfAttributesSet (pfm_global->gsf_attribute_num)))
    {
      gsfPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      gsfPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      gsf->setPalette (gsfPalette);
      QString attrString = QString ("GSF (%1)").arg (cnt);
      gsf->setText (attrString);
    }
  else
    {
      gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      gsf->setPalette (gsfPalette);
      gsf->setText ("GSF");
    }
}



void 
globalPage::slotClearGSF ()
{
  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    {
      if (pfm_global->gsf_attribute_num[i])
        {
          int32_t selected = pfm_global->gsf_attribute_num[i];

          pfm_global->gsf_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (gsfGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotGSFAttrClicked (int)));
              gsf_attr[i]->setChecked (false);
              connect (gsfGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotGSFAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  gsf->setPalette (gsfPalette);
  gsf->setText ("GSF");
}



void 
globalPage::slotCloseGSF ()
{
  global_dialog->gsfD->close ();
}



void
globalPage::slotHOFClicked ()
{
  if (global_dialog->hofD) global_dialog->hofD->close ();

  global_dialog->hofD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->hofD->setWindowTitle (tr ("pfmLoad HOF Attributes"));


  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->hofD);
  vbox->setMargin (5);
  vbox->setSpacing (5);

  QHBoxLayout *hbox = new QHBoxLayout;
  hbox->setMargin (5);
  hbox->setSpacing (5);
  QVBoxLayout *vboxLLayout = new QVBoxLayout;
  QVBoxLayout *vboxRLayout = new QVBoxLayout;
  hbox->addLayout (vboxLLayout);
  hbox->addLayout (vboxRLayout);


  vbox->addLayout (hbox);


  hofGrp = new QButtonGroup (global_dialog->hofD);
  hofGrp->setExclusive (false);
  connect (hofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotHOFAttrClicked (int)));

  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    {
      hof_attr[i] = new QCheckBox (QString (pfm_global->hof_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->hof_attribute[i].name);
      hof_attr[i]->setToolTip (string);
      hofGrp->addButton (hof_attr[i], i);

      if (pfm_global->hof_attribute_num[i]) hof_attr[i]->setChecked (true);

      if (pfm_global->appending) hof_attr[i]->setEnabled (false);

      if (i < HOF_ATTRIBUTES / 2)
        {
          vboxLLayout->addWidget (hof_attr[i]);
        }
      else
        {
          vboxRLayout->addWidget (hof_attr[i]);
        }
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->hofD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the HOF attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearHOF ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->hofD);
  closeButton->setToolTip (tr ("Close the HOF attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseHOF ()));
  actions->addWidget (closeButton);


  global_dialog->hofD->show ();
}



void 
globalPage::slotHOFAttrClicked (int index)
{
  //  If we unset a HOF attribute check box we need to adjust all of the already selected attributes.

  if (hof_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->hof_attribute_num[index];

      for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
        {
          if (pfm_global->hof_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->hof_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (hofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotHOFAttrClicked (int)));
          hof_attr[index]->setChecked (false);
          connect (hofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotHOFAttrClicked (int)));

          QMessageBox::warning (global_dialog->hofD, tr ("pfmLoad HOF Attributes"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->hof_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = hofAttributesSet (pfm_global->hof_attribute_num)))
    {
      hofPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      hofPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      hof->setPalette (hofPalette);
      QString attrString = QString ("HOF (%1)").arg (cnt);
      hof->setText (attrString);
    }
  else
    {
      hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      hof->setPalette (hofPalette);
      hof->setText ("HOF");
    }
}



void 
globalPage::slotClearHOF ()
{
  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    {
      if (pfm_global->hof_attribute_num[i])
        {
          int32_t selected = pfm_global->hof_attribute_num[i];

          pfm_global->hof_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (hofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotHOFAttrClicked (int)));
              hof_attr[i]->setChecked (false);
              connect (hofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotHOFAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  hof->setPalette (hofPalette);
  hof->setText ("HOF");
}



void 
globalPage::slotCloseHOF ()
{
  global_dialog->hofD->close ();
}



void
globalPage::slotTOFClicked ()
{
  if (global_dialog->tofD) global_dialog->tofD->close ();

  global_dialog->tofD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->tofD->setWindowTitle (tr ("pfmLoad TOF Attributes"));

  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->tofD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  tofGrp = new QButtonGroup (global_dialog->tofD);
  tofGrp->setExclusive (false);
  connect (tofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotTOFAttrClicked (int)));

  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    {
      tof_attr[i] = new QCheckBox (QString (pfm_global->tof_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->tof_attribute[i].name);
      tof_attr[i]->setToolTip (string);
      tofGrp->addButton (tof_attr[i], i);

      if (pfm_global->tof_attribute_num[i]) tof_attr[i]->setChecked (true);

      if (pfm_global->appending) tof_attr[i]->setEnabled (false);

      vbox->addWidget (tof_attr[i]);
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->tofD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the TOF attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearTOF ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->tofD);
  closeButton->setToolTip (tr ("Close the TOF attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseTOF ()));
  actions->addWidget (closeButton);


  global_dialog->tofD->show ();
}



void 
globalPage::slotTOFAttrClicked (int index)
{
  //  If we unset a TOF attribute check box we need to adjust all of the already selected attributes.

  if (tof_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->tof_attribute_num[index];

      for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
        {
          if (pfm_global->tof_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->tof_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (tofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotTOFAttrClicked (int)));
          tof_attr[index]->setChecked (false);
          connect (tofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotTOFAttrClicked (int)));

          QMessageBox::warning (global_dialog->tofD, tr ("pfmLoad TOF Attributes"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->tof_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = tofAttributesSet (pfm_global->tof_attribute_num)))
    {
      tofPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      tofPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      tof->setPalette (tofPalette);
      QString attrString = QString ("TOF (%1)").arg (cnt);
      tof->setText (attrString);
    }
  else
    {
      tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      tof->setPalette (tofPalette);
      tof->setText ("TOF");
    }
}



void 
globalPage::slotClearTOF ()
{
  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    {
      if (pfm_global->tof_attribute_num[i])
        {
          int32_t selected = pfm_global->tof_attribute_num[i];

          pfm_global->tof_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (tofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotTOFAttrClicked (int)));
              tof_attr[i]->setChecked (false);
              connect (tofGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotTOFAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  tof->setPalette (tofPalette);
  tof->setText ("TOF");
}



void 
globalPage::slotCloseTOF ()
{
  global_dialog->tofD->close ();
}



void
globalPage::slotCZMILClicked ()
{
  if (global_dialog->czmilD) global_dialog->czmilD->close ();

  global_dialog->czmilD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->czmilD->setWindowTitle (tr ("pfmLoad CZMIL Attributes"));

  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->czmilD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  czmilGrp = new QButtonGroup (global_dialog->czmilD);
  czmilGrp->setExclusive (false);
  connect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCZMILAttrClicked (int)));

  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    {
      czmil_attr[i] = new QCheckBox (QString (pfm_global->czmil_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->czmil_attribute[i].name);
      czmil_attr[i]->setToolTip (string);
      czmil_attr[i]->setWhatsThis (pfm_global->czmil_attribute_description[i]);
      czmilGrp->addButton (czmil_attr[i], i);

      if (i > CZMIL_CSF)
        {
          czmilPalette.setColor (QPalette::Normal, QPalette::Base, Qt::red);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Base, Qt::red);
          czmil_attr[i]->setPalette (czmilPalette);
        }

      if (pfm_global->czmil_attribute_num[i]) czmil_attr[i]->setChecked (true);

      if (pfm_global->appending) czmil_attr[i]->setEnabled (false);

      vbox->addWidget (czmil_attr[i]);
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *bHelp = new QPushButton (this);
  bHelp->setIcon (QIcon (":/icons/contextHelp.png"));
  bHelp->setToolTip (tr ("Enter What's This mode for help"));
  connect (bHelp, SIGNAL (clicked ()), this, SLOT (slotHelp ()));
  actions->addWidget (bHelp);

  actions->addStretch (10);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->czmilD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the CZMIL attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearCZMIL ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->czmilD);
  closeButton->setToolTip (tr ("Close the CZMIL attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseCZMIL ()));
  actions->addWidget (closeButton);


  global_dialog->czmilD->show ();
}



void 
globalPage::slotCZMILAttrClicked (int index)
{
  //  If we unset a CZMIL attribute check box we need to adjust all of the already selected attributes.

  if (czmil_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->czmil_attribute_num[index];

      for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
        {
          if (pfm_global->czmil_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->czmil_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCZMILAttrClicked (int)));
          czmil_attr[index]->setChecked (false);
          connect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCZMILAttrClicked (int)));

          QMessageBox::warning (global_dialog->czmilD, tr ("pfmLoad CZMIL Attributes"),
                                tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->czmil_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = czmilAttributesSet (pfm_global->czmil_attribute_num)))
    {
      czmilPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      czmilPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      czmil->setPalette (czmilPalette);
      QString attrString = QString ("CZMIL (%1)").arg (cnt);
      czmil->setText (attrString);
    }
  else
    {
      czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      czmil->setPalette (czmilPalette);
      czmil->setText ("CZMIL");
    }
}



void 
globalPage::slotClearCZMIL ()
{
  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    {
      if (pfm_global->czmil_attribute_num[i])
        {
          int32_t selected = pfm_global->czmil_attribute_num[i];

          pfm_global->czmil_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCZMILAttrClicked (int)));
              czmil_attr[i]->setChecked (false);
              connect (czmilGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotCZMILAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  czmil->setPalette (czmilPalette);
  czmil->setText ("CZMIL");
}



void 
globalPage::slotCloseCZMIL ()
{
  global_dialog->czmilD->close ();
}



void
globalPage::slotLASClicked ()
{
  if (global_dialog->lasD) global_dialog->lasD->close ();

  global_dialog->lasD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->lasD->setWindowTitle (tr ("pfmLoad LAS Attributes"));

  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->lasD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  lasGrp = new QButtonGroup (global_dialog->lasD);
  lasGrp->setExclusive (false);
  connect (lasGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotLASAttrClicked (int)));

  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    {
      las_attr[i] = new QCheckBox (QString (pfm_global->las_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->las_attribute[i].name);
      las_attr[i]->setToolTip (string);
      lasGrp->addButton (las_attr[i], i);

      if (i > 8)
        {
          lasPalette.setColor (QPalette::Normal, QPalette::Base, Qt::red);
          lasPalette.setColor (QPalette::Inactive, QPalette::Base, Qt::red);
          las_attr[i]->setPalette (lasPalette);
        }

      if (pfm_global->las_attribute_num[i]) las_attr[i]->setChecked (true);

      if (pfm_global->appending) las_attr[i]->setEnabled (false);

      vbox->addWidget (las_attr[i]);
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->lasD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the LAS attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearLAS ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->lasD);
  closeButton->setToolTip (tr ("Close the LAS attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseLAS ()));
  actions->addWidget (closeButton);


  global_dialog->lasD->show ();
}



void 
globalPage::slotLASAttrClicked (int index)
{
  //  If we unset a LAS attribute check box we need to adjust all of the already selected attributes.

  if (las_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->las_attribute_num[index];

      for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
        {
          if (pfm_global->las_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->las_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (lasGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotLASAttrClicked (int)));
          las_attr[index]->setChecked (false);
          connect (lasGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotLASAttrClicked (int)));

          QMessageBox::warning (global_dialog->lasD, tr ("pfmLoad LAS Attributes"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->las_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = lasAttributesSet (pfm_global->las_attribute_num)))
    {
      lasPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      lasPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      las->setPalette (lasPalette);
      QString attrString = QString ("LAS (%1)").arg (cnt);
      las->setText (attrString);
    }
  else
    {
      lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      las->setPalette (lasPalette);
      las->setText ("LAS");
    }
}



void 
globalPage::slotClearLAS ()
{
  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    {
      if (pfm_global->las_attribute_num[i])
        {
          int32_t selected = pfm_global->las_attribute_num[i];

          pfm_global->las_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (lasGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotLASAttrClicked (int)));
              las_attr[i]->setChecked (false);
              connect (lasGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotLASAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  las->setPalette (lasPalette);
  las->setText ("LAS");
}



void 
globalPage::slotCloseLAS ()
{
  global_dialog->lasD->close ();
}



void
globalPage::slotBAGClicked ()
{
  if (global_dialog->bagD) global_dialog->bagD->close ();

  global_dialog->bagD = new QDialog (parent, (Qt::WindowFlags) Qt::WA_DeleteOnClose);
  global_dialog->bagD->setWindowTitle (tr ("pfmLoad BAG Attributes"));

  QVBoxLayout *vbox = new QVBoxLayout (global_dialog->bagD);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  bagGrp = new QButtonGroup (global_dialog->bagD);
  bagGrp->setExclusive (false);
  connect (bagGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotBAGAttrClicked (int)));

  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    {
      bag_attr[i] = new QCheckBox (QString (pfm_global->bag_attribute[i].name));
      QString string = tr ("Toggle %1 attribute").arg (pfm_global->bag_attribute[i].name);
      bag_attr[i]->setToolTip (string);
      bagGrp->addButton (bag_attr[i], i);

      if (pfm_global->bag_attribute_num[i]) bag_attr[i]->setChecked (true);

      if (pfm_global->appending) bag_attr[i]->setEnabled (false);

      vbox->addWidget (bag_attr[i]);
    }
  

  QHBoxLayout *actions = new QHBoxLayout (0);
  vbox->addLayout (actions);

  QPushButton *clearButton = new QPushButton (tr ("Clear"), global_dialog->bagD);
  clearButton->setToolTip (tr ("Clear all selected attributes in the BAG attribute dialog"));
  connect (clearButton, SIGNAL (clicked ()), this, SLOT (slotClearBAG ()));
  actions->addWidget (clearButton);

  if (pfm_global->appending) clearButton->setEnabled (false);

  QPushButton *closeButton = new QPushButton (tr ("Close"), global_dialog->bagD);
  closeButton->setToolTip (tr ("Close the BAG attribute dialog"));
  connect (closeButton, SIGNAL (clicked ()), this, SLOT (slotCloseBAG ()));
  actions->addWidget (closeButton);


  global_dialog->bagD->show ();
}



void 
globalPage::slotBAGAttrClicked (int index)
{
  //  If we unset a BAG attribute check box we need to adjust all of the already selected attributes.

  if (bag_attr[index]->checkState () != Qt::Checked)
    {
      //  Find out which attribute number it was set to and zero it out.

      int16_t selected = pfm_global->bag_attribute_num[index];

      for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
        {
          if (pfm_global->bag_attribute_num[i])
            {
              if (i == index)
                {
                  pfm_global->bag_attribute_num[i] = 0;


                  //  Move all lower attributes up one and decrement the counter.

                  move_and_decrement (selected);
                  break;
                }
            }
        }
    }
  else
    {
      //  Make sure we don't exceed our allowable number of depth attributes.

      if (pfm_global->attribute_count >= NUM_ATTR)
        {
          disconnect (bagGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotBAGAttrClicked (int)));
          bag_attr[index]->setChecked (false);
          connect (bagGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotBAGAttrClicked (int)));

          QMessageBox::warning (global_dialog->bagD, tr ("pfmLoad BAG Attributes"), tr ("Only %1 index attributes are allowed").arg (NUM_ATTR));
        }
      else
        {
          //  Increment the attribute count and set the attribute number for this attribute.

          pfm_global->attribute_count++;
          pfm_global->bag_attribute_num[index] = pfm_global->attribute_count;
        }
    }


  //  Set the number of attributes set (if any) and set the proper color for the button.

  int32_t cnt = 0;
  if ((cnt = bagAttributesSet (pfm_global->bag_attribute_num)))
    {
      bagPalette.setColor (QPalette::Normal, QPalette::Button, Qt::red);
      bagPalette.setColor (QPalette::Inactive, QPalette::Button, Qt::red);
      bag->setPalette (bagPalette);
      QString attrString = QString ("BAG (%1)").arg (cnt);
      bag->setText (attrString);
    }
  else
    {
      bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
      bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
      bag->setPalette (bagPalette);
      bag->setText ("BAG");
    }
}



void 
globalPage::slotClearBAG ()
{
  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    {
      if (pfm_global->bag_attribute_num[i])
        {
          int32_t selected = pfm_global->bag_attribute_num[i];

          pfm_global->bag_attribute_num[i] = 0;

          move_and_decrement (selected);


          //  Now clear the check box, unless we're initializing...

          if (!initializing)
            {
              disconnect (bagGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotBAGAttrClicked (int)));
              bag_attr[i]->setChecked (false);
              connect (bagGrp, SIGNAL (buttonClicked (int)), this, SLOT (slotBAGAttrClicked (int)));
            }
        }
    }


  //  Clear the number of attributes set and set the proper color for the button.

  bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
  bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
  bag->setPalette (bagPalette);
  bag->setText ("BAG");
}



void 
globalPage::slotCloseBAG ()
{
  global_dialog->bagD->close ();
}



void
globalPage::slotOptClicked (int id)
{
  switch (id)
    {
    case 0:
      if (srtmBest->isChecked ())
        {
          if (srtm2 && !flags->srtme) 
            QMessageBox::information (this, tr ("pfmLoad SRTM2 data"),
                                      tr ("WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n\n"
                                          "If your PFM file includes any areas outside of the United States it will be limited "
                                          "distribution due to the inclusion of NGA SRTM2 topography data.  If you do not want to "
                                          "include the SRTM2 data please select 'Exclude SRTM2 data' from the 'Optional flag settings'.\n\n"
                                          "WARNING WARNING WARNING WARNING WARNING WARNING WARNING"));

          flags->srtmb = NVTrue;

          flags->srtm1 = NVFalse;
          flags->srtm3 = NVFalse;
          flags->srtm30 = NVFalse;
          srtmOne->setChecked (false);
          srtmThree->setChecked (false);
          srtmThirty->setChecked (false);
        }
      else
        {
          flags->srtmb = NVFalse;
        }
      break;

    case 1:
      if (srtmOne->isChecked ())
        {
          if (srtm2 && !flags->srtme) 
            QMessageBox::information (this, tr ("pfmLoad SRTM2 data"),
                                      tr ("WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n\n"
                                          "If your PFM file includes any areas outside of the United States it will be limited "
                                          "distribution due to the inclusion of NGA SRTM2 topography data.  If you do not want to "
                                          "include the SRTM2 data please select 'Exclude SRTM2 data' from the 'Optional flag settings'.\n\n"
                                          "WARNING WARNING WARNING WARNING WARNING WARNING WARNING"));

          flags->srtm1 = NVTrue;

          flags->srtmb = NVFalse;
          flags->srtm3 = NVFalse;
          flags->srtm30 = NVFalse;
          srtmBest->setChecked (false);
          srtmThree->setChecked (false);
          srtmThirty->setChecked (false);
        }
      else
        {
          flags->srtm1 = NVFalse;
        }
      break;

    case 2:
      if (srtmThree->isChecked ())
        {
          flags->srtm3 = NVTrue;

          flags->srtmb = NVFalse;
          flags->srtm1 = NVFalse;
          flags->srtm30 = NVFalse;
          srtmBest->setChecked (false);
          srtmOne->setChecked (false);
          srtmThirty->setChecked (false);
        }
      else
        {
          flags->srtm3 = NVFalse;
        }
      break;

    case 3:
      if (srtmThirty->isChecked ())
        {
          flags->srtm30 = NVTrue;

          flags->srtmb = NVFalse;
          flags->srtm1 = NVFalse;
          flags->srtm3 = NVFalse;
          srtmBest->setChecked (false);
          srtmOne->setChecked (false);
          srtmThree->setChecked (false);
        }
      else
        {
          flags->srtm30 = NVFalse;
        }
      break;

    case 4:
      if (srtm2Ex->isChecked ())
        {
          flags->srtme = NVTrue;
        }
      else
        {
          flags->srtme = NVFalse;
          if (srtmBest->isChecked () || srtmOne->isChecked ())
            {
              QMessageBox::information (this, tr ("pfmLoad SRTM2 data"),
                                        tr ("WARNING WARNING WARNING WARNING WARNING WARNING WARNING\n\n"
                                            "If your PFM file includes any areas outside of the United States it will be limited "
                                            "distribution due to the inclusion of NGA SRTM2 topography data.  If you do not want to "
                                            "include the SRTM2 data please select 'Exclude SRTM2 data' from the 'Optional flag settings'.\n\n"
                                            "WARNING WARNING WARNING WARNING WARNING WARNING WARNING"));
            }
        }
      break;
    }
}



void 
globalPage::setFields (PFM_GLOBAL *pfm_glb, FLAGS *fl)
{
  setField ("hofOld", fl->old);
  setField ("hofNull", fl->hof);
  setField ("subPath", fl->sub);
  setField ("invLand", fl->lnd);
  setField ("invSec", fl->sec);
  setField ("refData", fl->ref);
  setField ("expCheck", fl->expand);
  setField ("tofRef", fl->tof);
  setField ("ref_ws", fl->ref_water_surface);
  setField ("wsClass", fl->czmil_water_surface);
  setField ("landClass", fl->czmil_land);
  setField ("waterClass", fl->czmil_water);
  setField ("gsfNominal", fl->nom);
  setField ("srtmBest", fl->srtmb);
  setField ("srtmOne", fl->srtm1);
  setField ("srtmThree", fl->srtm3);
  setField ("srtmThirty", fl->srtm30);
  setField ("srtmRef", fl->srtmr);
  setField ("cubeAttributes", fl->attr);
  setField ("horizontal", pfm_glb->horizontal_error);
  setField ("vertical", pfm_glb->vertical_error);
  setField ("czmilChan1", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_1]);
  setField ("czmilChan2", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_2]);
  setField ("czmilChan3", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_3]);
  setField ("czmilChan4", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_4]);
  setField ("czmilChan5", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_5]);
  setField ("czmilChan6", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_6]);
  setField ("czmilChan7", fl->czmil_chan[CZMIL_SHALLOW_CHANNEL_7]);
  setField ("czmilIRChan", fl->czmil_chan[CZMIL_IR_CHANNEL]);
  setField ("czmilDeepChan", fl->czmil_chan[CZMIL_DEEP_CHANNEL]);

  if (fl->old)
    {
      invSec->setEnabled (false);
    }
  else
    {
      invSec->setEnabled (true);
    }


  //  Set initializing flag

  initializing = NVTrue;


  //  Turn off the attribute flags when appending.

  if (pfm_glb->appending)
    {
      timeCheck->setEnabled (false);
    }
  else
    {
      switch (pfm_glb->data_type)
        {
        case L_GSF:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, true);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, false);

          gsf->setEnabled (true);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_CZMIL:
          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          globalTab->setTabEnabled (1, true);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, false);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (true);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_HOF:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, true);
          globalTab->setTabEnabled (4, false);

          gsf->setEnabled (false);
          hof->setEnabled (true);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_TOF:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (true);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_LAS:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (true);
          bag->setEnabled (false);
          break;

        case L_YXZ:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_LLZ:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_DTED:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_CHRTR:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          bagPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          bagPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          bag->setPalette (bagPalette);
          bag->setText ("BAG");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (false);
          break;

        case L_BAG:
          czmilPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          czmilPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          czmil->setPalette (czmilPalette);
          czmil->setText ("CZMIL");

          hofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          hofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          hof->setPalette (hofPalette);
          hof->setText ("HOF");

          tofPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          tofPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          tof->setPalette (tofPalette);
          tof->setText ("TOF");

          lasPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          lasPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          las->setPalette (lasPalette);
          las->setText ("LAS");

          gsfPalette.setColor (QPalette::Normal, QPalette::Button, widgetBackgroundColor);
          gsfPalette.setColor (QPalette::Inactive, QPalette::Button, widgetBackgroundColor);
          gsf->setPalette (gsfPalette);
          gsf->setText ("GSF");

          globalTab->setTabEnabled (1, false);
          globalTab->setTabEnabled (2, false);
          globalTab->setTabEnabled (3, false);
          globalTab->setTabEnabled (4, true);

          gsf->setEnabled (false);
          hof->setEnabled (false);
          tof->setEnabled (false);
          czmil->setEnabled (false);
          las->setEnabled (false);
          bag->setEnabled (true);
          break;
        }
    }

  initializing = NVFalse;
}



void 
globalPage::move_and_decrement (int32_t selected)
{
  //  Move all selected attributes that were lower than the selected slot up by one slot.

  if (pfm_global->time_attribute_num && pfm_global->time_attribute_num > selected) pfm_global->time_attribute_num--;

  switch (pfm_global->data_type)
    {
    case L_GSF:
      for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
        {
          if (pfm_global->gsf_attribute_num[i] && pfm_global->gsf_attribute_num[i] > selected) pfm_global->gsf_attribute_num[i]--;
        }
      break;


    case L_HOF:
      for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
        {
          if (pfm_global->hof_attribute_num[i] && pfm_global->hof_attribute_num[i] > selected) pfm_global->hof_attribute_num[i]--;
        }
      break;

    case L_TOF:
      for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
        {
          if (pfm_global->tof_attribute_num[i] && pfm_global->tof_attribute_num[i] > selected) pfm_global->tof_attribute_num[i]--;
        }
      break;

    case L_CZMIL:
      for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
        {
          if (pfm_global->czmil_attribute_num[i] && pfm_global->czmil_attribute_num[i] > selected) pfm_global->czmil_attribute_num[i]--;
        }
      break;

    case L_LAS:
      for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
        {
          if (pfm_global->las_attribute_num[i] && pfm_global->las_attribute_num[i] > selected) pfm_global->las_attribute_num[i]--;
        }
      break;

    case L_BAG:
      for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
        {
          if (pfm_global->bag_attribute_num[i] && pfm_global->bag_attribute_num[i] > selected) pfm_global->bag_attribute_num[i]--;
        }
      break;
    }


  //  Decrement the attribute count.

  pfm_global->attribute_count--;
}
