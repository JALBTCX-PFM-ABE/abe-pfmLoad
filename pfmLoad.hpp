
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



#ifndef PFMLOAD_H
#define PFMLOAD_H

#include "pfmLoadDef.hpp"
#include "startPage.hpp"
#include "pfmPage.hpp"
#include "globalPage.hpp"
#include "inputPage.hpp"
#include "runPage.hpp"
#include "version.hpp"

void envin (PFM_GLOBAL *pfm_global, FLAGS *flags, PFM_DEFINITION *ref_def, int32_t *window_x, int32_t *window_width,
            int32_t *window_y, int32_t *window_height, QString *inputFilter);
void envout (PFM_GLOBAL *pfm_global, FLAGS *flags, PFM_DEFINITION *ref_def, int32_t *window_x, int32_t *window_width,
             int32_t *window_y, int32_t *window_height, QString *inputFilter);

class pfmLoad : public QWizard
{
  Q_OBJECT


public:

  pfmLoad (int *argc = 0, char **argv = 0, QWidget *parent = 0, QString parm_file = "");
  ~pfmLoad ();


signals:


protected:

  void initializePage (int id);
  void cleanupPage (int id);
  void error_and_summary ();


  int32_t          pfm_file_count, input_file_count, window_x, window_y, window_width, window_height;

  uint8_t          cube_available, upr_file, csf_attr;

  char             error_file[512], cube_name[50], loader_name[50], tmp_file[1024];

  PFM_DEFINITION   pfm_def[MAX_LOAD_FILES], ref_def;

  PFM_GLOBAL       pfm_global;

  GLOBAL_DIALOG    global_dialog;

  globalPage       *globalPg;

  FILE_DEFINITION  *input_file_def;

  FLAGS            flags;

  RUN_PROGRESS     progress;

  char             fname[512];

  QTextEdit        *inputFiles;

  QStringList      input_files;

  QString          inputFilter, parameter_file;

  QProcess         *loadProc;

  QListWidget      *checkList;


protected slots:

  void reject ();
  void slotHelpClicked ();
  void slotCustomButtonClicked (int id);
  void slotParameterFilterSelected (const QString &filter);

  void slotLoadReadyReadStandardError ();
  void slotLoadError (QProcess::ProcessError error);
  void slotLoadDone (int exitCode, QProcess::ExitStatus exitStatus);

};

#endif
