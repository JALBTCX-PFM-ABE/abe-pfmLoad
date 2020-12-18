
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



QString runText = 
  pfmLoad::tr ("Pressing this button will start the external pfmLoader process to create or update the PFM structure(s).");

QString saveText = 
  pfmLoad::tr ("Pressing this button will allow you to save the parameters that you have input to this wizard to "
               "a parameter file.  You may enter a new filename or select a preexisting file.  If you enter a new "
               "filename you do not have to add the file extension as one will be added for you.<br><br>"
               "There are two types of parameter files.  The normal parameter file (.prm) which can be used on the "
               "command line for pfmLoad or pfmLoader, or an <b>update</b> parameter file (.upr) that can only be "
               "used as a command line argument to pfmLoader.  The normal parameter file (.prm) may be used in a "
               "subsequent run of pfmLoad/pfmLoader, as a template for another area, or to rebuild the same area "
               "with a different name (and, optionally, modified parameters).<br><br>"
               "The <b>update</b> parameter file (.upr) can only be created if you have used the "
               "<b>Directories->Browse</b> button on the <b>Input Data Files</b> page.  If you want to save the "
               "parameters to a .upr file you must select the .upr filter before saving the file.  A .upr parameters "
               "file is normally used in the field when you want to update previously created PFM files with newly "
               "collected data.  The scenario would go something like this;  you run pfmLoad on the first day of the "
               "survey and define all of your PFM areas, your PFM options, and the directories that you want to "
               "search for new data.  When you get to the run page you save the parameters to a .upr file.  You can "
               "then either press the <b>Load</b> button to load the first batch of data into the PFM(s) or you can "
               "<b>Cancel</b> and run pfmLoader with the .upr file name on the command line.  On subsequent days all "
               "you have to do is run pfmLoader with the .upr file name on the command line in order to add all "
               "newly collected data to the defined PFM(s).");

QString saveStatus = 
  pfmLoad::tr ("Pressing this button will allow you to save the information in the <b>Process status</b> box.  This is "
               "handy if you have encountered an error and want to save the information related to that error.  You may "
               "enter a new filename or select a preexisting file.  If you enter a new filename you do not have to add "
               "the .txt file extension as one will be added for you.");
