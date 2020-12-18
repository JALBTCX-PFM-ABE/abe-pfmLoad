
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



#include "inputPage.hpp"
#include "inputPageHelp.hpp"

inputPage::inputPage (QWidget *parent, QString *inFilt, QTextEdit **infiles, PFM_GLOBAL *pfmg):
  QWizardPage (parent)
{
  pfm_global = pfmg;
  inputFilter = inFilt;


  setTitle (tr ("Input Data Files"));

  setPixmap (QWizard::WatermarkPixmap, QPixmap(":/icons/pfmLoadWatermark.png"));


  QVBoxLayout *vbox = new QVBoxLayout (this);
  vbox->setMargin (5);
  vbox->setSpacing (5);


  QHBoxLayout *browseBox = new QHBoxLayout;
  browseBox->setSpacing (5);


  QGroupBox *fileBox = new QGroupBox (tr ("Files"), this);
  QHBoxLayout *fileBoxLayout = new QHBoxLayout;
  fileBox->setLayout (fileBoxLayout);
  fileBoxLayout->setSpacing (10);


  inputBrowse = new QPushButton (tr ("Browse"), this);
  inputBrowse->setToolTip (tr ("Add input files to the list"));
  inputBrowse->setWhatsThis (inputBrowseText);
  connect (inputBrowse, SIGNAL (clicked ()), this, SLOT (slotInputBrowseClicked ()));
  fileBoxLayout->addWidget (inputBrowse);
  browseBox->addWidget (fileBox, 1);


  QGroupBox *dirBox = new QGroupBox (tr ("Directories"), this);
  QHBoxLayout *dirBoxLayout = new QHBoxLayout;
  dirBox->setLayout (dirBoxLayout);
  dirBoxLayout->setSpacing (10);


  QGroupBox *maskBox = new QGroupBox (tr ("Directory file mask"), this);
  QHBoxLayout *maskBoxLayout = new QHBoxLayout;
  maskBox->setLayout (maskBoxLayout);
  maskBoxLayout->setSpacing (10);

  fileMask = new QComboBox (this);
  fileMask->setToolTip (tr ("Set the file mask for the directory Browse button"));
  fileMask->setWhatsThis (fileMaskText);
  fileMask->setEditable (true);
  fileMask->addItem ("GSF (*.d[0-9][0-9] *.gsf)");
  fileMask->addItem ("CZMIL (*.cpf)");
  fileMask->addItem ("HOF (*.hof)");
  fileMask->addItem ("TOF (*.tof)");
  fileMask->addItem ("LAS (*.las *.laz *.LAS *.LAZ)");
  fileMask->addItem ("YXZ (*.yxz *.txt)");
  fileMask->addItem ("LLZ (*.llz)");
  fileMask->addItem ("DTED (*.dt1 *.dt2)");
  fileMask->addItem ("CHRTR (*.fin *.ch2)");
  fileMask->addItem ("BAG (*.bag)");
  fileMask->addItem ("LIST (*.lst)");
  maskBoxLayout->addWidget (fileMask);

  if (*inputFilter == "CZMIL (*.cpf)")
    {
      fileMask->setCurrentIndex (1);
    }
  else if (*inputFilter == "HOF (*.hof)")
    {
      fileMask->setCurrentIndex (2);
    }
  else if (*inputFilter == "TOF (*.tof)")
    {
      fileMask->setCurrentIndex (3);
    }
  else if (*inputFilter == "LAS (*.las *.laz *.LAS *.LAZ)")
    {
      fileMask->setCurrentIndex (4);
    }
  else if (*inputFilter == "YXZ (*.yxz *.txt)")
    {
      fileMask->setCurrentIndex (5);
    }
  else if (*inputFilter == "LLZ (*.llz)")
    {
      fileMask->setCurrentIndex (6);
    }
  else if (*inputFilter == "DTED (*.dt1 *.dt2)")
    {
      fileMask->setCurrentIndex (7);
    }
  else if (*inputFilter == "CHRTR (*.fin *.ch2)")
    {
      fileMask->setCurrentIndex (8);
    }
  else if (*inputFilter == "BAG (*.bag)")
    {
      fileMask->setCurrentIndex (9);
    }
  else if (*inputFilter == "LIST (*.lst)")
    {
      fileMask->setCurrentIndex (10);
    }
  else
    {
      fileMask->setCurrentIndex (0);
    }
  connect (fileMask, SIGNAL (currentIndexChanged (const QString &)), this, SLOT (slotFileMaskTextChanged (const QString &)));
  dirBoxLayout->addWidget (maskBox);


  dirBrowse = new QPushButton (tr ("Browse"), this);
  dirBrowse->setToolTip (tr ("Add input directory contents to the list"));
  dirBrowse->setWhatsThis (dirBrowseText);
  connect (dirBrowse, SIGNAL (clicked ()), this, SLOT (slotDirBrowseClicked ()));
  dirBoxLayout->addWidget (dirBrowse);


  browseBox->addWidget (dirBox, 1);


  QGroupBox *pfmBox = new QGroupBox (tr ("PFM files"), this);
  QHBoxLayout *pfmBoxLayout = new QHBoxLayout;
  pfmBox->setLayout (pfmBoxLayout);
  pfmBoxLayout->setSpacing (10);


  pfmBrowse = new QPushButton (tr ("Browse"), this);
  pfmBrowse->setToolTip (tr ("Add input files from a preexisting PFM structure to the list"));
  pfmBrowse->setWhatsThis (pfmBrowseText);
  connect (pfmBrowse, SIGNAL (clicked ()), this, SLOT (slotPfmBrowseClicked ()));
  pfmBoxLayout->addWidget (pfmBrowse);
  browseBox->addWidget (pfmBox, 1);


  vbox->addLayout (browseBox);


  QGroupBox *inputBox = new QGroupBox (tr ("Input file list"), this);
  QHBoxLayout *inputBoxLayout = new QHBoxLayout;
  inputBox->setLayout (inputBoxLayout);
  inputBoxLayout->setSpacing (10);


  inputFiles = new QTextEdit (this);
  inputFiles->setWhatsThis (inputFilesText);
  inputFiles->setLineWrapMode (QTextEdit::NoWrap);
  inputBoxLayout->addWidget (inputFiles);


  vbox->addWidget (inputBox, 10);


  *infiles = inputFiles;
}



void 
inputPage::slotInputBrowseClicked ()
{
  QFileDialog fd (this, tr ("pfmLoad Input files"));
  fd.setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (&fd, pfm_global->input_dir);


  QStringList filters;
  filters << "GSF (*.d[0-9][0-9] *.gsf)"
          << "CZMIL (*.cpf)"
          << "HOF (*.hof)"
          << "TOF (*.tof)"
          << "LAS (*.las *.laz *.LAS *.LAZ)"
          << "YXZ (*.yxz *.txt)"
          << "LLZ (*.llz)"
          << "DTED (*.dt1 *.dt2)"
          << "CHRTR (*.fin *.ch2)"
          << "BAG (*.bag)"
          << "LIST (*.lst)"
          << "All files (*)";

  fd.setNameFilters (filters);
  fd.setFileMode (QFileDialog::ExistingFiles);
  fd.selectNameFilter (*inputFilter);


  QStringList files;


  if (fd.exec () == QDialog::Accepted) 
    {
      files = fd.selectedFiles ();

      pfm_global->input_dir = fd.directory ().absolutePath ();


      int32_t ltype = 0;

      *inputFilter = fd.selectedNameFilter ();

      if (*inputFilter == "GSF (*.d[0-9][0-9] *.gsf)")
        {
          ltype = 1;
        }
      else if (*inputFilter == "CZMIL (*.cpf)")
        {
          ltype = 2;
        }
      else if (*inputFilter == "HOF (*.hof)")
        {
          ltype = 3;
        }
      else if (*inputFilter == "TOF (*.tof)")
        {
          ltype = 4;
        }
      else if (*inputFilter == "LAS (*.las *.laz *.LAS *.LAZ)")
        {
          ltype = 5;
        }
      else if (*inputFilter == "YXZ (*.yxz *.txt)")
        {
          ltype = 6;
        }
      else if (*inputFilter == "LLZ (*.llz)")
        {
          ltype = 7;
        }
      else if (*inputFilter == "DTED (*.dt1 *.dt2)")
        {
          ltype = 8;
        }
      else if (*inputFilter == "CHRTR (*.fin *.ch2)")
        {
          ltype = 9;
        }
      else if (*inputFilter == "BAG (*.bag)")
        {
          ltype = 10;
        }
      else if (*inputFilter == "LIST (*.lst)")
        {
          ltype = 11;
        }


      //  Only allow one data type (even if it's LIST).

      if (!pfm_global->data_type)
        {
          pfm_global->data_type = ltype;
        }
      else
        {
          if (pfm_global->data_type != ltype)
            {
              QMessageBox::warning (this, tr ("pfmLoad"),
                                    tr ("This program now only allows a single data type when building a PFM.  If you wish to compare "
                                        "data of different types, build multiple PFMs and open them together in pfmView."));
              return;
            }
        }


      //  Note the offset to get back to zero base...

      fileMask->setCurrentIndex (ltype - 1);


      for (int32_t i = 0 ; i < files.size () ; i++)
        {
          inputFiles->append (files.at (i));
        }
    }
}



void 
inputPage::slotDirBrowseClicked ()
{
  QString title = tr ("pfmLoad Input directories, mask = %1").arg (*inputFilter);
  QFileDialog *fd = new QFileDialog (this, title);
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, pfm_global->input_dir);


  QStringList filters;
  filters << "GSF (*.d[0-9][0-9] *.gsf)"
          << "CZMIL (*.cpf)"
          << "HOF (*.hof)"
          << "TOF (*.tof)"
          << "LAS (*.las *.laz *.LAS *.LAZ)"
          << "YXZ (*.yxz *.txt)"
          << "LLZ (*.llz)"
          << "DTED (*.dt1 *.dt2)"
          << "CHRTR (*.fin *.ch2)"
          << "BAG (*.bag)"
          << "LIST (*.lst)";

  fd->setNameFilters (filters);

  fd->setFileMode (QFileDialog::Directory);

  fd->setNameFilter (*inputFilter);


  //  Make the directory file dialog support multiple directories.

  QListView *fdListView = fd->findChild<QListView*> ("listView");

  if (fdListView)
    {
      fdListView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    }

  QTreeView *fdTreeView = fd->findChild<QTreeView*> ();

  if (fdTreeView)
    {
      fdTreeView->setSelectionMode (QAbstractItemView::ExtendedSelection);
    }


  QString file;
  QStringList files;

  if (fd->exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a directory.

      pfm_global->input_dir = fd->directory ().absolutePath ();

      files = fd->selectedFiles ();


      int32_t ltype = 0;
      QStringList nameFilter;
      QString type;

      if (*inputFilter == "GSF (*.d[0-9][0-9] *.gsf)")
        {
          ltype = 1;
          type = "GSF";
          nameFilter << "*.d[0-9][0-9]" << "*.gsf";
        }
      else if (*inputFilter == "CZMIL (*.cpf)")
        {
          ltype = 2;
          type = "CZMIL";
          nameFilter << "*.cpf";
        }
      else if (*inputFilter == "HOF (*.hof)")
        {
          ltype = 3;
          type = "HOF";
          nameFilter << "*.hof";
        }
      else if (*inputFilter == "TOF (*.tof)")
        {
          ltype = 4;
          type = "TOF";
          nameFilter << "*.tof";
        }
      else if (*inputFilter == "LAS (*.las *.laz *.LAS *.LAZ)")
        {
          ltype = 5;
          type = "LAS";
          nameFilter << "*.las" << "*.laz" << "*.LAS" << "*.LAZ";
        }
      else if (*inputFilter == "YXZ (*.yxz *.txt)")
        { 
          ltype = 6;
          type = "YXZ";
          nameFilter << "*.yxz" << "*.txt";
        }
      else if (*inputFilter == "LLZ (*.llz)")
        {
          ltype = 7;
          type = "LLZ";
          nameFilter << "*.llz";
        }
      else if (*inputFilter == "DTED (*.dt1 *.dt2)")
        {
          ltype = 8;
          type = "DTED";
          nameFilter << "*.dt1" << "*.dt2";
        }
      else if (*inputFilter == "CHRTR (*.fin *.ch2)")
        {
          ltype = 9;
          type = "CHRTR";
          nameFilter << "*.fin" << "*.ch2";
        }
      else if (*inputFilter == "BAG (*.bag)")
        {
          ltype = 10;
          type = "BAG";
          nameFilter << "*.bag";
        }
      else if (*inputFilter == "LIST (*.lst)")
        {
          ltype = 11;
          type = "LIST";
          nameFilter << "*.lst";
        }


      QDir files_dir;
      files_dir.setNameFilters (nameFilter);


      //  Only allow one data type (even if it's LIST).

      if (!pfm_global->data_type)
        {
          pfm_global->data_type = ltype;
        }
      else
        {
          if (pfm_global->data_type != ltype)
            {
              QMessageBox::warning (this, tr ("pfmLoad"),
                                    tr ("This program now only allows a single data type when building a PFM.  If you wish to compare "
                                        "data of different types, build multiple PFMs and open them in pfmView."));
              return;
            }
        }


      for (int32_t i = 0 ; i < files.size () ; i++)
        {
          file = files.at (i);

          if (file.isEmpty ()) file = fd->directory ().absolutePath ();

          QDir dirs;
          dirs.cd (file);

          dirs.setFilter (QDir::Dirs | QDir::Readable);


          //  Get all matching files in this directory.

          files_dir.setFilter (QDir::Files | QDir::Readable);


          //  Add the filter and directory to the directories list

          pfm_global->input_dirs += (type + ":" + file);


          if (files_dir.cd (file))
            {
              QFileInfoList flist = files_dir.entryInfoList ();
              for (int32_t j = 0 ; j < flist.size () ; j++)
                {
                  //  Don't load HOF timing lines by default.  These can still be loaded using the file browser.

                  QString tst = flist.at (j).absoluteFilePath ();

                  if (!inputFilter->contains ("*.hof") || tst.mid (tst.length () - 13, 4) != "_TA_")
                    {
                      inputFiles->append (tst);
                    }
                }
            }


          //  Get all directories in this directory.

          QFileInfoList dlist = dirs.entryInfoList ();
          QStringList dirList;
          for (int32_t j = 0 ; j < dlist.size () ; j++)
            {
              if (dlist.at (j).fileName () != "." && dlist.at (j).fileName () != "..") 
                dirList.append (dlist.at (j).absoluteFilePath ());
            }


          //  Get all subordinate directories.

          for (int32_t j = 0 ; j < dirList.size () ; j++)
            {
              QString dirName = dirList.at (j);

              if (dirs.cd (dirName))
                {
                  QFileInfoList nlist = dirs.entryInfoList ();
                  for (int32_t k = 0 ; k < nlist.size () ; k++)
                    {
                      if (nlist.at (k).fileName () != "." && nlist.at (k).fileName () != "..") 
                        dirList.append (nlist.at (k).absoluteFilePath ());
                    }
                }
            }


          //  Get all matching files in all subordinate directories

          for (int32_t j = 0 ; j < dirList.size () ; j++)
            {
              files_dir.setFilter (QDir::Files | QDir::Readable);
              files_dir.setNameFilters (nameFilter);

              QString dirName = dirList.at (j);

              if (files_dir.cd (dirName))
                {
                  QFileInfoList sub_flist = files_dir.entryInfoList ();

                  for (int32_t k = 0 ; k < sub_flist.size () ; k++)
                    {

                      //  Don't load HOF timing lines by default.  These can still be loaded using the file browser.

                      QString tst = sub_flist.at (k).absoluteFilePath ();

                      if (!inputFilter->contains ("*.hof") || tst.mid (tst.length () - 13, 4) != "_TA_")
                        {
                          inputFiles->append (tst);
                        }
                    }
                }
            }
        }
    }
}



void 
inputPage::slotPfmBrowseClicked ()
{
  QStringList         files, filters;


  QFileDialog *fd = new QFileDialog (this, tr ("pfmLoad Input PFM Structure"));
  fd->setViewMode (QFileDialog::List);


  //  Always add the current working directory and the last used directory to the sidebar URLs in case we're running from the command line.
  //  This function is in the nvutility library.

  setSidebarUrls (fd, pfm_global->input_dir);


  filters << QString ("PFM (*.pfm)");

  fd->setNameFilters (filters);
  fd->setFileMode (QFileDialog::ExistingFile);
  fd->selectNameFilter (QString ("PFM (*.pfm)"));

  if (fd->exec () == QDialog::Accepted)
    {
      //  Save the directory that we were in when we selected a file.

      pfm_global->input_dir = fd->directory ().absolutePath ();


      files = fd->selectedFiles ();

      QString file = files.at (0);


      if (!file.isEmpty())
        {
          int32_t temp_handle;
          PFM_OPEN_ARGS open_args;

          strcpy (open_args.list_path, file.toLatin1 ());

          open_args.checkpoint = 0;

          if ((temp_handle = open_existing_pfm_file (&open_args)) < 0)
            {
              QMessageBox::warning (this, tr ("pfmLoad Input PFM Structure"),
                                    tr ("The file \n%1\n is not a PFM structure or there was an error reading the file.  The error message returned was:\n%2\n").arg 
                                    (QDir::toNativeSeparators (QString (open_args.list_path))).arg (pfm_error_str (pfm_error)));
            }
          else
            {
              int16_t i = 0, type;
              char rfile[1024];
              while (!read_list_file (temp_handle, i, rfile, &type))
                {
                  QString name = QString (rfile);


                  //  Default type is GSF...

                  int32_t ltype = 1;

                  if (name.endsWith (".cpf"))
                    {
                      ltype = 2;
                    }
                  else if (name.endsWith (".hof"))
                    {
                      ltype = 3;
                    }
                  else if (name.endsWith (".tof"))
                    {
                      ltype = 4;
                    }
                  else if (name.endsWith (".las") || name.endsWith (".laz") || name.endsWith (".LAS") || name.endsWith (".LAZ"))
                    {
                      ltype = 5;
                    }
                  else if (name.endsWith (".yxz") || name.endsWith (".txt"))
                    {
                      ltype = 6;
                    }
                  else if (name.endsWith (".llz"))
                    {
                      ltype = 7;
                    }
                  else if (name.endsWith (".dt1") || name.endsWith (".dt2"))
                    {
                      ltype = 8;
                    }
                  else if (name.endsWith (".fin") || name.endsWith (".ch2"))
                    {
                      ltype = 9;
                    }
                  else if (name.endsWith (".bag"))
                    {
                      ltype = 10;
                    }


                  //  Only allow one data type.

                  if (!pfm_global->data_type)
                    {
                      pfm_global->data_type = ltype;
                    }
                  else
                    {
                      if (pfm_global->data_type != ltype)
                        {
                          QMessageBox::warning (this, tr ("pfmLoad"),
                                                tr ("This program now only allows a single data type when building a PFM.  If you "
                                                    "wish to compare data of different types, build multiple PFMs and open them in pfmView."));
                          return;
                        }
                    }

                  inputFiles->append (QString (rfile));
                  i++;
                }

              close_pfm_file (temp_handle);
            }
        }
    }
}


void 
inputPage::slotFileMaskTextChanged (const QString &text)
{
  *inputFilter = text;
}
