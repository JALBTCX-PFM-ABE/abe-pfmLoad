
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



#ifndef GLOBALPAGE_H
#define GLOBALPAGE_H

#include "pfmLoadDef.hpp"


class globalPage:public QWizardPage
{
  Q_OBJECT 


public:

  globalPage (QWidget *prnt = 0, PFM_GLOBAL *pfm_glb = NULL, FLAGS *fl = NULL, GLOBAL_DIALOG *gd = NULL, uint8_t cube_available = NVFalse);

  void setFields (PFM_GLOBAL *pfm_glb, FLAGS *fl);


signals:


protected:

  bool validatePage ();

  FLAGS            *flags;

  PFM_GLOBAL       *pfm_global;

  GLOBAL_DIALOG    *global_dialog;

  QWidget          *parent;

  uint8_t          srtm1, srtm2, srtm3, srtm30, cube_available;

  QDoubleSpinBox   *horizontal, *vertical, *capture, *horiz, *distance, *minContext, *maxContext;

  QSpinBox         *maxFiles, *maxLines, *maxPings, *maxBeams, *queue;

  QCheckBox        *hofOld, *hofNull, *subPath, *invLand, *invSec, *lidAttr, *refData, *expCheck, *tofRef, *wsClass, *ref_ws,
                   *landClass, *waterClass, *HFfilt, *czmilChan[9], *gsfNominal, *srtmBest, *srtmOne, *srtmThree,
                   *srtmThirty, *srtmRef, *srtm2Ex, *timeCheck, *cubeAttributes;

  QPalette         gsfPalette, hofPalette, tofPalette, czmilPalette, lasPalette, bagPalette;

  QColor           widgetBackgroundColor;

  QButtonGroup     *optGrp, *gsfGrp, *hofGrp, *tofGrp, *czmilGrp, *lasGrp, *bagGrp, *czmilLoadGrp;

  QGroupBox        *czmilBox, *hofBox, *tofBox, *gsfBox;

  QTabWidget       *globalTab;

  QCheckBox        *gsf_attr[GSF_ATTRIBUTES], *hof_attr[HOF_ATTRIBUTES], *tof_attr[TOF_ATTRIBUTES], *czmil_attr[CZMIL_ATTRIBUTES],
                   *las_attr[LAS_ATTRIBUTES], *bag_attr[BAG_ATTRIBUTES];

  QPushButton      *gsf, *hof, *tof, *czmil, *las, *bag;

  uint8_t          initializing;


  void move_and_decrement (int32_t selected);


protected slots:

  void slotHelp ();
  void slotHofOldClicked (int state);
  void slotTimeCheckClicked (bool state);
  void slotGSFClicked ();
  void slotGSFAttrClicked (int index);
  void slotClearGSF ();
  void slotCloseGSF ();
  void slotHOFClicked ();
  void slotHOFAttrClicked (int index);
  void slotClearHOF ();
  void slotCloseHOF ();
  void slotTOFClicked ();
  void slotTOFAttrClicked (int index);
  void slotClearTOF ();
  void slotCloseTOF ();
  void slotCZMILClicked ();
  void slotCZMILAttrClicked (int index);
  void slotClearCZMIL ();
  void slotCloseCZMIL ();
  void slotLASClicked ();
  void slotLASAttrClicked (int index);
  void slotClearLAS ();
  void slotCloseLAS ();
  void slotBAGClicked ();
  void slotBAGAttrClicked (int index);
  void slotClearBAG ();
  void slotCloseBAG ();
  void slotOptClicked (int id);
  void slotRefWsClicked (int state);

private:
};

#endif
