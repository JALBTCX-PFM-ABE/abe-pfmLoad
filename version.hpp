
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




/*  C/Motif pfm_load replaced by C++/Qt pfmLoad on 04/27/05.  All comments for pfm_load
    have been left in for historical (hysterical ???) purposes.  In addition, the pfm_loader
    program has been combined with pfmLoad so the comments for pfm_loader have been included
    here.  JCD  */


#ifndef VERSION

#define     VERSION     "PFM Software - pfmLoad V8.13 - 08/07/19"

#endif

/*!< <pre>


******************* pfm_load comments *********************


    Version 1.0
    Jan C. Depner
    10/13/99


    Version 2.0
    Jan C. Depner
    02/15/00

    Went to version 2.0 of the PFM library.


    Version 3.0
    Jan C. Depner
    04/19/00

    Completely rewritten.  Looks much better doesn't it?


    Version 3.01
    Jan C. Depner
    04/27/00

    Force the .pfm extension on file creation.


    Version 3.02
    Jan C. Depner
    07/17/00

    Allowed ASCII area files.


    Version 3.03
    Jan C. Depner
    08/20/00

    Added area filter parameters.  Changed SHOALS .out file input section.


    Version 3.04
    Jan C. Depner
    02/21/01

    Passing scale to open_pfm_file as a pointer.


    Version 4.0
    Jan C. Depner
    07/19/01
 
    4.0 PFM library changes.


    Version 4.1
    Jan C. Depner
    09/30/01
 
    Fixed bug in file_cbs.c that was causing the script to be incorrect.


    Version 4.11
    Jan C. Depner
    10/23/01
 
    Changed the run script file name to the first list file name with a
    suffix of .load_run.  Changed size of definitoin page (taller).


    Version 4.12
    Jan C. Depner
    10/30/01
 
    Increased allowable standard deviation filter range to 6.0 from 4.0.


    Version 4.2
    Jan C. Depner
    06/06/02
 
    Removed the swath filter stuff.


    Version 4.3
    Jan C. Depner
    06/15/03
 
    Added ability to use "reference masking".


    Version 4.4
    Jan C. Depner
    07/18/03
 
    Removed experimental "reference masking" and replaced with PFM_REFERENCE
    flagging.  Went to 32 bits for validity fields.


    Version 4.5
    Jan C. Depner
    02/24/04
 
    Changed target file mask to *.xml to support new XML format target files.


    Version 4.6
    Jan C. Depner
    10/11/04
 
    Added [HOF Load Null Flag] and [Invert Substitute Paths Flag] options.
    Added ability to get all files under a directory that match the Filter
    mask in the input multiple file selection box (uses a shelled "find"
    command).


    Version 4.7
    Jan C. Depner
    10/22/04
 
    Added [Invalidate HOF Land Flag] option.


    Version 4.71
    Jan C. Depner
    11/12/04
 
    Bug fix in file_cbs.c for new FC3 compiler.


    Version 4.8
    Jan C. Depner
    12/15/04
 
    Handle the new (PFM 4.5) way that the library creates PFM files - a directory
    with hard-wired filenames.


    Version 4.9
    Jan C. Depner
    01/19/05
 
    Button for loading IVS standard attributes with HOF/TOF data.


    Version 4.91
    Jan C. Depner
    02/25/05

    Switched to open_existing_pfm_file from open_pfm_file.


    Version 4.92
    Jan C. Depner
    03/04/05

    Fix return from open_existing_pfm_file.



******************* pfm_loader comments *********************



#ifndef VERSION

#define     VERSION     "PFM_Software - pfm_loader V6.52 - 03/23/05"

#endif


    Version 1.0
    Jan C. Depner
    05/28/99


    Version 2.0
    Jan C. Depner
    02/15/00

    Went to version 2.0 of the PFM library.


    Version 2.1
    Jan C. Depner
    03/29/00

    Uses version 2.1 of the pfm library.


    Version 3.0
    Jan C. Depner
    04/20/00

    Changed the input to come from stdin and uses [key] = syntax.


    Version 3.1
    Jan C. Depner
    07/06/00

    Loads SHOALS xy2 (11 column) format and merge format.


    Version 3.11
    Jan C. Depner
    07/17/00

    Now able to use standard ASCII area files in addition to ISS-60 .ARE files.


    Version 3.12
    Jan C. Depner
    08/18/00

    Added area filter parameters to inputs.  Added function to clear edits
    around known targets.


    Version 3.2
    Jan C. Depner
    08/27/00

    Finally, the damn thing will handle multiple PFM files!


    Version 3.3
    Jan C. Depner
    09/04/00

    Fixed bugs in the swath filter.  It wouldn't unload the last window if
    it wasn't full.  Also, it wouldn't flag the swath filtered data as 
    PFM_MODIFIED.


    Version 3.4
    Jan C. Depner
    09/27/00

    Fixed the memory leak in the swath filter.  Removed redundant 
    recompute_bin_values from main.c.


    Version 4.0
    Jan C. Depner
    09/29/00

    Replaced along-row running average and STD with nine cell average.  See
    PFM_Filter.c for explanation of algorithm.


    Version 4.1
    Jan C. Depner
    10/12/00

    Modified slightly to work with the more efficient pfm_lib V2.6.


    Version 4.2
    Jan C. Depner
    12/12/00

    Made PFM_Filter buffer up three rows of bin and depth data so that we 
    could compute the average filtered depth and standard deviation on the fly.
    This allowed us to remove the recompute bin loop in the main program when
    filtering data.


    Version 4.21
    Jan C. Depner
    02/21/01

    Trying to catch all of the little bugs in the code.  Check for a handle
    if gsfOpen fails.  This is needed in case the index file is bad or can't
    be opened.  Also, fixed the looping in the scroll bars.


    Version 4.22
    Jan C. Depner
    02/23/01

    Added an error log file.


    Version 4.23
    Jan C. Depner
    02/28/01

    Fixed problem trying to open nonexistent GSF file.


    Version 4.24
    Jan C. Depner
    05/03/01

    Remove the GSF index files before reading them in.  This gets rid of
    any corrupted GSF index files and then regenerates them on open.  Don't
    know what was causing the corrupted GSF index files.  Apparently some
    other process.


    Version 5.0
    Jan C. Depner
    07/19/01
 
    4.0 PFM library changes.


    Version 5.02
    Jan C. Depner
    10/31/01
 
    Changed the name of the errors file to something that makes more sense.
    Made SASS specific records use "nominal" depth since there is no "true"
    depth for SASS.


    Version 5.03
    Jan C. Depner
    11/23/01
 
    No longer errors out on pre-check.  Outputs an error message and goes
    on with load of other files.


    Version 5.04
    Jan C. Depner
    12/03/01
 
    For some unknown reason some GSF files may have all records with beam
    flags except one.  This was bombing the load.  Now I check the beam_flags
    array prior to looking at the data.


    Version 5.05
    Jan C. Depner / Jeff Parker
    05/01/01
 
    Added Jeff's changes to fix projected data problems.


    Version 5.1
    Jan C. Depner
    06/06/02
 
    Removed the swath filter stuff.  PFM 4.1 changes (attributes/classes).


    Version 5.11
    Jeff Parker (SAIC)
    07/12/02
 
    Added correction to get_area_mbr.c to handle RECTANGLE and ELLIPSE
    area files.


    Version 5.12
    Jan C. Depner
    07/23/02
 
    Added correction for filename array.


    Version 5.2
    Jan C. Depner
    11/18/02
 
    Added DEMO data type.


    Version 5.21
    Jan C. Depner
    12/12/02
 
    Close and reopen the PFM files between reading data and recomputing.


    Version 5.3
    Jan C. Depner
    06/15/03
 
    Added UNISIPS depth data type.  Added ability to reference mask the input
    data.


    Version 5.4
    Jan C. Depner
    07/18/03
 
    Removed experimental reference masking and replaced with support for
    PFM_REFERENCE flagging.  Went to 32 bits for validity fields.


    Version 5.5
    Jan C. Depner
    08/03/03
 
    Added support for SHOALS/CHARTS HOF format.


    Version 5.6
    Jan C. Depner
    08/06/03
 
    Added support for SHOALS/CHARTS TOF format.


    Version 5.7
    Jan C. Depner
    08/24/03
 
    Fixed problems with loading TOF values set to -998.0.  Also fixed insidious
    elevation limit problem.


    Version 5.8
    Jan C. Depner
    10/23/03
 
    Decreased storage for filenames.  Changed variable name in load_file.c
    to be more clear.


    Version 5.9
    Jan C. Depner
    12/22/03
 
    Added support for generic YXZ files, signed degrees decimal, space 
    delimited.


    Version 6.0
    Jan C. Depner
    02/24/04
 
    Added support for the new XML target file format.  Removed support for
    the old HYDRO .tgt format.


    Version 6.1
    Jan C. Depner
    04/12/04
 
    Sorting data by row/col index prior to loading to decrease the load time
    and disk thrashing.  Also fixed real stupid thing in clear_targets - don't
    ask, you don't have a need to know ;-)


    Version 6.2
    Jan C. Depner
    09/01/04
 
    Saving -998.0 hof depths as null depths with PFM_USER_04 set to match IVS
    load.


    Version 6.3
    Jan C. Depner
    10/11/04
 
    Made saving -998.0 hof depths as null depths optional [HOF Load Null Flag].
    Added support for inverting Windoze substitute paths 
    [Invert Substitute Paths Flag].


    Version 6.4
    Jan C. Depner
    10/29/04
 
    Shifted PFM numbering of HOF and TOF to start at 1 so that we would be compatible
    with IVS.  Made the HOF and TOF precheck read the header of the files to look at
    the file type string.


    Version 6.41
    Jan C. Depner
    11/17/04
 
    Minor bug fix in hof loader.


    Version 6.42
    Jan C. Depner
    12/10/04
 
    Turn on dynamic_reload to be compatible with IVS pfm_direct and Optech GCS.


    Version 6.43
    Jan C. Depner
    01/14/05
 
    Added support for IVS ASCII xyz format (PFM_ASCXYZ_DATA).


    Version 6.5
    Jan C. Depner
    01/19/05
 
    Now loading IVS standard attributes for LIDAR data.


    Version 6.51
    Jan C. Depner
    03/04/05
 
    Fix return from open_pfm_file.


    Version 6.52
    Jan C. Depner
    03/23/05
 
    Don't set HOF LIDAR nulls to PFM_INVALID or PFM_MODIFIED so GCS can
    deal with them properly if we want to reprocess them.



******************* pfmLoad comments *********************



    Version 1.0
    Jan C. Depner
    04/27/05

    First version of C++/Qt pfmLoad.


    Version 1.01
    Jan C. Depner
    05/23/05

    Added TOF first return reference flag and modified the PFM Browse functionality 
    a bit.


    Version 1.02
    Jan C. Depner
    06/06/05

    Fix bug reading in parameters file - no setField call after input files are being read.
    Changed HOF PFM_USER flags to Second Depth Present, Shoreline Depth Swapped, Land, and 
    NBR.


    Version 1.03
    Jan C. Depner
    06/26/05

    Now checks for SHOALS 3000TH data types in HOF and TOF file checks.  Flags with Bare 
    Earth! instead of NBR!


    Version 1.04
    Jan C. Depner
    07/21/05

    Added Deep Filter Only option since deep flyers have become a problem with the new SHOALS3000THE LIDAR data.


    Version 1.05
    Jan C. Depner
    08/15/05

    Removed support for merge and demo formats.


    Version 1.06
    Jan C. Depner
    10/26/05

    Now uses the PFM 4.6 handle file or list file instead of the PFM directory name.


    Version 1.07
    Jan C. Depner
    11/08/05

    Added load GSF nominal depth flag.


    Version 1.08
    Jan C. Depner
    12/30/05

    Fixed hypack loader bug.


    Version 1.1
    Jan C. Depner
    04/10/06

    Replaced QVBox, QHBox, QVGroupBox, QHGroupBox with QVBoxLayout, QHBoxLayout, QGroupBox to prepare for
    Qt 4.  Changed help button to WhatsThis icon.


    Version 1.11
    Jan C. Depner
    05/10/06

    Minor bug fix for rebuild PFM - checks to see if X and Y bin sizes in degrees are identical.  If so, it
    populates the "minutes" bin size instead of "meters".


    Version 1.12
    Jan C. Depner
    05/18/06

    Changed appearance of file input page.


    Version 1.13
    Jan C. Depner
    05/31/06

    Now opens files to check and see if they're UNISIPS since ISS-60 doesn't use the proper file extension.


    Version 1.2
    Jan C. Depner
    06/30/06

    Added ability to shell areaCheck in order to make or modify the area file.


    Version 1.21
    Jan C. Depner
    08/03/06

    Removed the percent checks in load_file.cpp.


    Version 1.3
    Jan C. Depner
    08/04/06

    Append summary data to end of PFM handle file as comments.


    Version 1.31
    Jan C. Depner
    08/07/06

    Changed max depth from 10000 to 12000... DOH!  Changed GSF file mask to include *.gsf.  Load true depth if
    nominal depth is requested but not available.


    Version 1.32
    Jan C. Depner
    08/25/06

    Doesn't error out on add_depth_record out of area.


    Version 1.33
    Jan C. Depner
    08/31/06

    Added support for NAVO LLZ data format.


    Version 1.34
    Jan C. Depner
    10/06/06

    Now loading ALL tof values even if first and last are identical.


    Version 1.35
    Jan C. Depner
    01/05/07

    Flag auto shallow processed data in HOF files (confidence = 74).


    Version 1.36
    Jan C. Depner
    01/17/07

    If the difference between the first and last TOF returns is less than 5 cm we will not load the first 
    return and will give it the last return validity on unload.


    Version 1.4
    Jan C. Depner
    02/05/07

    Added ability to load Shuttle Radar Topography Mission (SRTM) data if available.  Loads as PFM_NAVO_ASCII_DATA.


    Version 1.41
    Jan C. Depner
    02/09/07

    Oops!  I can't count!  DOH!!!


    Version 1.42
    Jan C. Depner
    02/13/07

    Fix one lat grid cell offset in read of SRTM topo data.


    Version 1.43
    Jan C. Depner
    02/19/07

    Fix point count when reading only SRTM data.


    Version 1.5
    Jan C. Depner
    03/23/07

    Fixed bug in clear_targets.  Sets the SRTM flags instead of trying to load the fake SRTM files.


    Version 1.51
    Jan C. Depner
    03/25/07

    Wasn't picking up the global options.


    Version 1.52
    Jan C. Depner
    08/24/07

    Switched from setGeometry to resize and move.  See Qt4 X11 window geometry documentation.


    Version 1.53
    Jan C. Depner
    09/17/07

    Replaced compute_index with compute_index_ptr.


    Version 1.54
    Jan C. Depner
    10/01/07

    Fixed some small GUI bugs.


    Version 1.55
    Jan C. Depner
    10/31/07

    Fixed "off by one" error in longitude in SRTM read code.  Happy Halloween!


    Version 1.56
    Jan C. Depner
    11/06/07

    Got rid of extra data reads in SRTM read code.


    Version 2.00
    Jan C. Depner
    12/03/07

    Switched to cached_io PFM calls.  Which I've probably screwed up royally ;-)
    Added fflush calls after prints to stderr since flush is not automatic in Windows.
    Added SRTM2 (limdis) data input support.
    Added DTED data input support and removed SHOALS .out and .xy2 support.


    Version 2.01
    Jan C. Depner
    12/21/07

    Fixed PFM file Browse dialog so that you could define a new name in a new directory.


    Version 2.02
    Jan C. Depner
    02/07/08

    Allow definition of an area by extracting it from a pre-existing PFM structure.


    Version 2.03
    Jan C. Depner
    03/10/08

    Came up with reasonable value for max horizontal error (I guess ;-)  Also, fixed the directory browse function
    so that you don't have to navigate around if you want the current working directory.


    Version 2.04
    Jan C. Depner
    03/14/08

    Disable the deep filter, std, and target radius when apply filter is unselected.  Load horizontal and vertical
    errors from GSF.


    Version 2.05
    Jan C. Depner
    03/28/08

    Added NSEW button to area definition section of pfmPage (Paul Marin request).


    Version 2.06
    Jan C. Depner
    04/02/08

    Fixed the CUBE parameters stuff (added the two new SAIC parameters).


    Version 2.07
    Jan C. Depner
    04/07/08

    Replaced single .h files from utility library with include of nvutility.h


    Version 2.08
    Jan C. Depner
    04/08/08

    Corrected the directory browse location problem.


    Version 3.00
    Jan C. Depner
    04/21/08

    Added NAVO PFM CUBE processing options.


    Version 3.01
    Jan C. Depner
    04/23/08

    Added NAVO PFM CUBE settings options in the Global Parameters page.  Removed Along Track Filters and Expert Only stuff.


    Version 3.02
    Jan C. Depner
    05/14/08

    Make sure that the default H/V errors aren't 0.0 if the CUBE attributes are to be loaded.  Also, make sure we can read
    input files prior to checking for type.


    Version 3.03
    Jan C. Depner
    05/27/08

    Switched to Abbreviated Depth Confidence as the phony beam number for HOF data.  Also check it for being
    out of bounds.


    Version 3.04
    Jan C. Depner
    06/02/08

    Computing HOF horizontal and vertical errors in the CHARTS library instead of setting to the default values.


    Version 3.05
    Jan C. Depner
    06/05/08

    Check for comments (#) in .pfm_cfg file when doing "invert paths";


    Version 3.06
    Jan C. Depner
    10/21/08

    Allow .afs area files.


    Version 3.07
    Jan C. Depner
    11/24/08

    Corrected loading of PFM_USER flags for HOF data.


    Version 3.08
    Jan C. Depner
    12/08/08

    Added ability to load CHRTR data (with no unload capability at present).


    Version 3.09
    Jan C. Depner
    03/02/09

    Added ability to load WLF data.


    Version 4.00
    Jan C. Depner
    03/23/09

    Added selectable attributes by data type for GSF, HOF, TOF, and WLF.


    Version 4.01
    Jan C. Depner
    04/09/09

    Added the HOF user flags back in... DOH!


    Version 4.02
    Jan C. Depner
    04/13/09

    Fixed bug that caused checkboxes to uncheck if you set them prior to setting attributes.


    Version 4.03
    Jan C. Depner
    04/14/09

    Fixed bug that caused TOF to not uncheck (cut and paste error).  Switched to Binary Feature Data (BFD)
    from NAVO standard XML target files.


    Version 4.04
    Jan C. Depner
    04/27/09

    Make sure we send just the handle name to navo_pfm_cube instead of the ctl file name.


    Version 4.05
    Jan C. Depner
    05/07/09

    Fix invert paths bug introduced when fixing last bug ;-)


    Version 4.06
    Jan C. Depner
    05/21/09

    Set all QFileDialogs to use List mode instead of Detail mode.


    Version 4.07
    Jan C. Depner
    06/12/09

    Fix the dateline crossing problem.


    Version 4.10
    Jan C. Depner
    06/15/09

    Replaced type of PFM_SHOALS_1K_DATA with PFM_CHARTS_HOF_DATA for Optech HOF files.


    Version 4.11
    Jan C. Depner
    06/16/09

    Cleaned up the GUI for the run page.


    Version 4.12
    Jan C. Depner
    06/24/09

    Changed the BFD names to avoid collision with GNU Binary File Descriptor library.


    Version 4.13
    Jan C. Depner
    07/29/09

    Changed %lf in printf statements to %f.  The MinGW folks broke this even though it works on every
    compiler known to man (including theirs up to the last version).  No compiler warnings or errors
    are issued.  Many thanks to the MinGW folks for fixing what wasn't broken.


    Version 4.14
    Jan C. Depner
    07/31/09

    Added the ability to load HOF data in either the new PFM_CHARTS_HOF_DATA form or the old, GCS compatible, 
    PFM_SHOALS_1K_DATA form.


    Version 4.15
    Jan C. Depner
    08/03/09

    Wasn't invalidating secondary HOF records when "Invalidate HOF Secondary Values" was selected.  Fixed
    bug that caused error message to keep re-appearing if the feature file was gone on a rebuild.


    Version 4.16
    Jan C. Depner
    09/24/09

    Change the window title to the first PFM name when the user presses the Run button.


    Version 4.17
    Jan C. Depner
    10/22/09

    Allow loading of HOF data with ABDC = 0 if we're loading NULL HOF points.


    Version 4.18
    Jan C. Depner
    12/17/09

    Fixed one beam multibeam GSF load error.


    Version 4.19
    Jan C. Depner
    12/29/09

    Stopped the filter from filtering points when there is not enough surrounding valid data.


    Version 4.20
    Jan C. Depner
    01/07/10

    Fixed an SRTM load bug.  Also fixed another single-beam GSF load bug.


    Version 4.21
    Jan C. Depner
    01/11/10

    Fixed SRTM load bug at equator.


    Version 4.22
    Jan C. Depner
    01/15/10

    Yet another one-beam multibeam loading bug.


    Version 4.23
    Jan C. Depner
    02/05/10

    Line names now contain start date and time of line (GSF, HOF, TOF, WLF).


    Version 4.24
    Jan C. Depner
    02/26/10

    Forgot to set PFM_MODIFIED when using "invalidate" options.


    Version 4.25
    Jan C. Depner
    03/04/10

    Added multiple directory selection to the directory browse option in inputPage.cpp.


    Version 4.26
    Jan C. Depner
    03/11/10

    Added ability to store date/time in POSIX minutes (minutes from 01/01/1970) as an attribute for
    GSF, HOF, TOF, and WLF data.


    Version 4.27
    Jan C. Depner
    03/17/10

    Added memset to 0 of all of the OPEN_ARGS structures.  This hasn't been an issue but we've probably just been lucky.
    Happy Saint Patrick's Day!


    Version 4.28
    Jan C. Depner
    04/15/10

    Added ability to load time from LLZ as an attribute.
    Happy Tax Day!


    Version 4.29
    Jan C. Depner
    04/30/10

    Fixed posfix and fixpos calls to use numeric constants instead of strings for type.


    Version 4.30
    Jan C. Depner
    07/06/10

    Removed incipient projected PFM support.  I was never going to finish it anyway ;-)


    Version 4.31
    Jan C. Depner
    08/30/10

    Changed HOF loader to check the absolute value of the abdc so that points that were originally above
    70 but marked as invalid by negating (i.e. -93) will be marked as manually invalid instead of filter invalid.


    Version 4.32
    Jan C. Depner
    09/21/10

    Added changes to support CZMIL test data.


    Version 4.33
    Jan C. Depner
    09/22/10

    Outputs the polygon points to the .prm file instead of the area file name.  This way we can lose of change the
    area file without effecting the .prm file.  This makes more sense anyway since we were depending on an external
    file in what was essentially a script.  We still support the old .prm files with the [Area File] field.


    Version 4.34
    Jan C. Depner
    10/20/10

    Time as a depth attribute is now set across all input files that have time.  Speeded the whole thing up quite a bit
    by making the progress bars only update at 5% intervals.


    Version 4.35
    Jan C. Depner
    11/15/10

    No longer removes GSF index files and regenerates them.  Corrupted GSF index files used to be a problem but
    this has gone away in recent years.  If you do have a problem you can always manually remove the index files.
    Added support for ingesting BAG data.


    Version 4.36
    Jan C. Depner
    12/13/10

    Fixed bug when setting HOF standard user flags.  I forgot that abdc of -70 is land just as abdc of 70 is.


    Version 4.37
    Jan C. Depner
    01/06/11

    Correct problem with the way I was instantiating the main widget.  This caused an intermittent error on Windows7/XP.


    Version 4.40
    Jan C. Depner
    01/17/11

    Added support for CHRTR2 data as we phase out the old CHRTR format.  Still using PFM_CHRTR_DATA flag.  Check the 
    file name to determine the actual type (for now).  Also, removed rebuild and parameter files from start page.
    These options are seldom used and you can essentailly do a rebuild using a pre-existing PFM just by changing the
    name.  The parameter file input is available by adding the parameter file to the command line.  This makes the
    interface much cleaner.


    Version 4.50
    Jan C. Depner
    02/08/11

    Added support for AHAB Hawkeye attributes.


    Version 4.51
    Jan C. Depner
    02/16/11

    Now converts path separators to native separators (\ on Windows) before writing the file name to the list file.


    Version 4.52
    Jan C. Depner
    02/23/11

    Shortened HAWKEYE attribute names because they were blowing out the PFM attribute name fields.


    Version 4.53
    Jan C. Depner
    02/25/11

    Now loads both PFM_HAWKEYE_HYDRO_DATA and PFM_HAWKEYE_TOPO_DATA.


    Version 4.54
    Jan C. Depner
    03/03/11

    Added "Clear" button to attribute dialogs.


    Version 4.55
    Jan C. Depner
    03/08/11

    Added warning when trying to use GCS compatible mode.


    Version 4.56
    Jan C. Depner
    03/15/11

    Fixed relative path bug in pfmPage.cpp.


    Version 4.60
    Jan C. Depner
    04/14/11

    Added ability to load from file list (*.lst) files.


    Version 4.61
    Jan C. Depner
    04/25/11

    Removed the "shoreline depth swapped" HOF lidar user flag and combined it with the "shallow water processed" HOF
    lidar user flag.  Replaced the "shoreline depth swapped" user flag with an "APD" user flag.  Since "shoreline depth swapped"
    (the old shallow water processing method) is never used with "shallow water processed" data processing this should
    cause no problems.


    Version 4.62
    Jan C. Depner
    06/16/11

    Removed HMPS_SPARE_1 flag check since we don't do swath filtering anymore and it's been removed from hmpsflag.h.


    Version 4.63
    Jan C. Depner
    06/27/11

    Save all directories used by the QFileDialogs.  Add current working directory to the sidebar for all QFileDialogs.


    Version 4.70
    Jan C. Depner
    07/21/11

    Added support for DRAFT CZMIL data.


    Version 4.71
    Jan C. Depner
    07/22/11

    Using setSidebarUrls function from nvutility to make sure that current working directory (.) and
    last used directory are in the sidebar URL list of QFileDialogs.


    Version 4.72
    Jan C. Depner
    08/15/11

    Added in all of the currently available CZMIL attributes.  Many more to come.


    Version 4.73
    Jan C. Depner
    09/21/11

    Added ability to create an "update" parameter file (.upr) for use by pfmLoader.
    Replaced bin_inside calls with bin_inside_ptr calls.
    Added U.S. Government diclaimer comments to all files.


    Version 4.74
    Jan C. Depner
    10/06/11

    Fixed bug in TOF loader.  I was under the impression that if the last return was bad (-998.0)
    then the first return must be bad as well.  This is not the case.


    Version 4.75
    Jan C. Depner
    10/28/11

    Added ability to set the amount of cache memory used for each PFM being built.


    Version 4.76
    Jan C. Depner
    11/30/11

    Converted .xpm icons to .png icons.


    Version 4.80
    Jan C. Depner
    01/10/12

    Finally got to check the north of 64N loading with geographic bins.  Turns out that we didn't
    need it (and it didn't work properly anyway).  So, I've commented out that code.



    Version 4.81
    Jan C. Depner
    01/18/12

    Make readParameterFile.cpp check for pre-existing PFMs and set the flag so the input files
    won't be reloaded.  Fixed envin and envout.  They weren't being passed the variables.  DOH!


    Version 4.82
    Jan C. Depner
    01/30/12


    Version 4.83
    Jan C. Depner
    01/30/12

    - Do a better job of handling the Cancel button (get rid of the error file).


    Version 4.84
    Jan C. Depner
    02/21/12

    Now translates between PFM_DESIGNATED_SOUNDING flag and GSF HMPS_SELECTED_DESIGNATED flag.


    Version 4.85
    Jan C. Depner
    03/09/12

    Reads UTM BAG files and converts to lat/lon... sort of.  I'm not sure if this is absolutely correct
    but it looks right.


    Version 4.86
    Jan C. Depner
    05/17/12

    Fixed bug in GSF_PFM_Processing where I assumed (you know what they say about that ;-) that there
    would NEVER be more than 1024 beams.  DOH!


    Version 4.87
    Jan C. Depner
    06/13/12

    Removed temporary CZMIL stuff while finalizing the CZMIL API.


    Version 4.88
    Jan C. Depner
    07/14/12

    - Added support for the preliminary CZMIL CPF format data.
    - Changed all arguments to attribute setting routines to pointers instead of structures.  DOH!


    Version 4.89
    Jan C. Depner
    08/08/12

    Doesn't try to reset PFM_CHECKED and/or PFM_VERIFIED flags during recompute if it' creating a
    new PFM.  This may or may not be the source of segfaults when creating very large, new PFMs.


    Version 4.90
    Jan C. Depner
    08/15/12

    Force flush of the error file after each fprintf to it.


    Version 4.91
    Jan C. Depner
    09/27/12

    Added interest point, interest point rank, channel, and return number as CZMIL attributes.


    Version 4.92
    Jan C. Depner
    10/23/12

    - Added T0 interest point, Kd, and laser energy as CZMIL attributes.
    - Only 8 shopping days left to retirement!


    Version 4.93
    Jan C. Depner
    11/01/12

    - CZMIL points with an ip_rank of 0 (water surface for water shots) will not be loaded.
    - Only 1 shopping day left to retirement!


    Version 4.94
    Jan C. Depner (PFM Software)
    06/03/13

    - Option to load CZMIL null values added.


    Version 4.95
    Jan C. Depner (PFM Software)
    06/05/13

    - Changed "Load CZMIL Water Surface" option to "Invalidate CZMIL Water Surface".
    - Added "Load CZMIL Channel Data" options.


    Version 4.96
    Jan C. Depner (PFM Software)
    06/18/13

    - Added "Invalidate CZMIL Channel Data" options.


    Version 4.97
    Jan C. Depner (PFM Software)
    06/21/13

    - Now creates an initial parameter file (.ipf) in the .pfm.data folder for newly created PFMs.  This can be
      used on subsequent appends if needed.  Mostly it's there for the reload of reprocessed CZMIL data using
      pfm_czmil_reload.


    Version 4.98
    Jan C. Depner (PFM Software)
    06/24/13

    - Added "CZMIL Detection probability" and "CZMIL Validity reason" to the possible CZMIL attributes.


    Version 4.99
    Jan C. Depner (PFM Software)
    07/17/13

    - Changed the "Invalidate CZMIL Water Surface" option to "Load CZMIL Water Surface" option.  We really
      have no good reason for loading water surface data.  We never loaded it for CHARTS and I doubt if
      we'll need it for CZMIL.  Another reason for not loading it is that the unload program will take
      forever to finish if you invalidate the water surface.


    Version 5.00
    Jan C. Depner (PFM Software)
    07/23/13

    - Speeded up the program by almost a factor of 2 just by moving the processEvents calls inside the percent
      checks in load_file.cpp & pfmLoad.cpp.  DOH!
    - Cleaned up a couple of -Wunused-but-set-variable warnings in load_file.cpp.


    Version 5.01
    Jan C. Depner (PFM Software)
    12/09/13

    Switched to using .ini file in $HOME (Linux) or $USERPROFILE (Windows) in the ABE.config directory.  Now
    the applications qsettings will not end up in unknown places like ~/.config/navo.navy.mil/blah_blah_blah on
    Linux or, in the registry (shudder) on Windows.


    Version 5.02
    Jan C. Depner (PFM Software)
    12/16/13

    Added CZMIL standard user flag options.


    Version 5.03
    Jan C. Depner (PFM Software)
    12/19/13

    Changes to deal with CZMIL pre 2.0 reflectance values being set to .99 for NULL.


    Version 5.04
    Jan C. Depner (PFM Software)
    01/06/14

    Fixed an obvious screwup in inputPage.cpp where I was re-using the loop counters.
    Scoping in C++ saved my bacon but it was just too damn confusing.


    Version 5.05
    Jan C. Depner (PFM Software)
    01/08/14

    Moved the CZMIL standard user flag setting code so that invalid points will also be flagged.


    Version 5.06
    Jan C. Depner (PFM Software)
    01/19/14

    Switched to CZMIL_READONLY_SEQUENTIAL for CPF files in order to, hopefully, speed up reads.


    Version 5.07
    Jan C. Depner (PFM Software)
    01/20/14

    Now using processing_mode instead of classification after changes to czmil API to handle
    processing mode being stored in classification prior to libCZMIL 2.0.  Added processing_mode
    to CZMIL attributes.


    Version 5.08
    Jan C. Depner (PFM Software)
    02/17/14

    Disabled Clear buttons, POSIX time attribute buttons, and CZMIL/HOF standard user flag button when appending.


    Version 5.09
    Jan C. Depner (PFM Software)
    02/26/14

    Cleaned up "Set but not used" variables that show up using the 4.8.2 version of gcc.


    Version 5.10
    Jan C. Depner (PFM Software)
    03/03/14

    Replaced HMPS flags with NV_GSF flags.


    Version 5.11
    Stacy Johnson (NAVO), Jan Depner (PFM Software)
    03/13/14

    Since chrtr2 was moved to grid registration we no longer need the half node shifts
    Since bag is grid registration and not a pixel, remove half nodes
    Added support for Bag 1.5.2
    Added option to expand rectangular MBRs by one-half bin for expected output to grid-based formats (BAG, CHRTR2).


    Version 5.12
    Jan C. Depner (PFM Software)
    03/17/14

    Removed WLF support.  Top o' the mornin' to ye!


    Version 5.13
    Jan C. Depner (PFM Software)
    05/07/14

    - Fixed string literal problems in format strings.
    - Check error return from fgets call.


    Version 5.14
    Jan C. Depner (PFM Software)
    05/27/14

    - Removed UNISIPS support.


    Version 5.15
    Jan C. Depner (PFM Software)
    05/27/14

    Now supports PFMv6 instead of PFMv5.


    Version 5.16
    Jan C. Depner (PFM Software)
    06/19/14

    - Removed PFMWDB support.  No one was using it.  It seemed like a good idea but I guess not.


    Version 5.17
    Jan C. Depner (PFM Software)
    07/01/14

    - Replaced all of the old, borrowed icons with new, public domain icons.  Mostly from the Tango set
      but a few from flavour-extended and 32pxmania.


    Version 5.18
    Jan C. Depner (PFM Software)
    07/05/14

    - Had to change the argument order in pj_init_plus for the UTM projection.  Newer versions of 
      proj4 appear to be very sensitive to this.


    Version 5.20
    Jan C. Depner (PFM Software)
    07/12/14

    - Added support for LAS files.  Only geographic or projected UTM using NAD83 zones 3-23 or WGS86
      zones 1-60 north or south.


    Version 5.21
    Jan C. Depner (PFM Software)
    07/17/14

    - No longer uses liblas.  Now uses libslas (my own API for LAS).


    Version 5.22
    Jan C. Depner (PFM Software)
    07/23/14

    - Switched from using the old NV_INT64 and NV_U_INT32 type definitions to the C99 standard stdint.h and
      inttypes.h sized data types (e.g. int64_t and uint32_t).


    Version 5.23
    Jan C. Depner (PFM Software)
    07/29/14

    - Fixed errors discovered by cppcheck.


    Version 5.24
    Jan C. Depner (PFM Software)
    09/04/14

    Support for new CZMIL v2 fields.


    Version 5.25
    Jan C. Depner (PFM Software)
    10/21/14

    More CZMIL v2 changes (specifically, removal of validity_reason).


    Version 5.26
    Jan C. Depner (PFM Software)
    10/24/14

    Added "CZMIL Average Depth" and "CZMIL Null" to CZMIL standard user flags.


    Version 5.30
    Jan C. Depner (PFM Software)
    11/12/14

    Now allows the user to select 5 CZMIL user flags in any order (boy was that complicated).


    Version 5.31
    Jan C. Depner (PFM Software)
    11/24/14

    Made the loader ignore CZMIL_REPROCESSING_BUFFER points.


    Version 5.32
    Jan C. Depner (PFM Software)
    01/06/15

    Added code to deal with new leap second to be added on June 30th, 2015 at 23:59:60.


    Version 5.33
    Jan C. Depner (PFM Software)
    01/27/15

    Outputs message if error encountered reading CZMIL files.


    Version 5.34
    Jan C. Depner (PFM Software)
    02/13/15

    - To give better feedback to shelling programs in the case of errors I've added the program name to all
      output to stderr.


    Version 6.00
    Jan C. Depner (PFM Software)
    03/12/15

    - Replaced my libslas with rapidlasso GmbH LASlib and LASzip.
    - Made preliminary file check always happen.


    Version 6.01
    Jan C. Depner (PFM Software)
    03/14/15

    - Since AHAB Hawkeye has switched to LAS format I have removed support for the old Hawkeye I binary format.


    Version 6.02
    Jan C. Depner (PFM Software)
    03/19/15

    - For LAS files, some people neglect to put in the GeographicTypeGeoKey just assuming that it's WGS_84.
      If we don't find the key we'll do the same.


    Version 6.03
    Jan C. Depner (PFM Software)
    03/30/15

    - Now handles uppercase LAS and LAZ file extensions for Windows afflicted files that have been transferred
      to operating systems that UnDeRsTaNd ThE DiFfErEnCe between upper and lowercase characters.


    Version 6.04
    Jan C. Depner (PFM Software)
    03/31/15

    - Added ability to use ESRI Polygon, PolygonZ, PolygonM, PolyLine, PolyLineZ, or PolyLineM geographic shape
      files as area files.
    - Fixed name filter for GSF files so that it wouldn't find files that don't end in .dNN where NN is numeric.
      This prevents us from seeing, for example, ESRI shape .dbf files in the GSF name list.


    Version 6.05
    Jan C. Depner (PFM Software)
    04/07/15

    - Fixed CZMIL flag settings.  I was breaking out of the loop for things other than NULL.  There can be multiple
      flags per point.  They are not mutually exclusive.


    Version 6.06
    Jan C. Depner (PFM Software)
    04/08/15

    - Correctly handles LAS v1.4 data.


    Version 6.07
    Jan C. Depner (PFM Software)
    04/13/15

    - Don't display CUBE options on global options page if navo_pfm_cube (proprietary) is not available on system.


    Version 6.08
    Jan C. Depner (PFM Software)
    04/14/15

    - Changed point counters to 64 bit integers.


    Version 6.09
    Jan C. Depner (PFM Software)
    04/25/15

    - Changed min and max depth spin boxes so that they would not wrap.  Also put in a check to make sure that max is
      not set less than min and vice versa.


    Version 6.10
    Jan C. Depner (PFM Software)
    05/07/15

    - Changed the point counters in the filter summery to 64 bit integers.


    Version 6.11
    Jan C. Depner (PFM Software)
    05/12/15

    - Removed options to invalidate CZMIL channels.  Invalidating an entire channel caused unload times to 
      be absolutely ridiculous.  If you need to actually kill entire channels do it in the CPF file with cpfFilter.


    Version 6.20
    Jan C. Depner (PFM Software)
    05/19/15

    - Added CSF attributes to the possible CZMIL attributes.  Using any of them will cause the load time to increase by
      approximately 3%.


    Version 6.21
    Jan C. Depner (PFM Software)
    05/21/15

    - Fixed problem with trying to detect similar attribute names in readParameterFile.cpp without the brackets on each end.


    Version 7.01
    Jan C. Depner (PFM Software)
    06/03/15

    - Fixed the display to the list window on Windows.  Microsoft, in their infinite wisdom, uses CR/LF for every line
      of text they output (unlike every other OS).  I guess they thought of the computer as if it were a typewriter.
    - Set CUBE attribute placeholder default to "off".


    Version 7.02
    Jan C. Depner (PFM Software)
    06/21/15

    - Fixed progress bar update problem on Windows.  A CR/LF problem again.  GRRRRR!


    Version 7.03
    Jan C. Depner (PFM Software)
    07/22/15

    - Moved attributes.cpp and attributes.hpp to the utility library since they're used in multiple programs.
      Now I only have to modify them in one place.


    Version 7.04
    Jan C. Depner (PFM Software)
    11/06/15

    - Modified scale factors for CZMIL interest points to match change to CZMIL API (from 10.0 to 100.0).


    Version 7.05
    Jan C. Depner (PFM Software)
    02/14/16

    - Added CZMIL CWF interest point amplitude as an attribute.
    - Happy Valentine's day!


    Version 7.10
    Jan C. Depner (PFM Software)
    04/04/16

    - Fixed self-inflicted bug that caused attributes to not be loaded when appending to existing PFM.  Mea culpa.


    Version 7.11
    Jan C. Depner (PFM Software)
    05/02/16

    - Added code to handle pfmLoader being closed by user when asked for Well-known Text (WKT) for a LAS file.


    Version 7.12
    Jan C. Depner (PFM Software)
    07/12/16

    - Added 3 new CZMIL attributes that were added in version 3.0 of the CZMIL API.  These are CZMIL D_index,
      CZMIL D_index_cube, and CZMIL User data.
    - Removed experimental CWF interest point amplitude attribute.  D_index will work better and it comes
      right out of the CPF file so there isn't a huge amount of overhead.


    Version 7.13
    Jan C. Depner (PFM Software)
    07/21/16

    - Added ability to load CZMIL water surface (ip_rank = 0) data as reference data.


    Version 7.14
    Jan C. Depner (PFM Software)
    07/23/16

    - Removed call to PFM API call "set_cache_max_size".  No longer needed since pfmLoader actually allocates and uses the 
      PFM cache memory.


    Version 7.15
    Jan C. Depner (PFM Software)
    08/05/16

    - Added Help button and help text for attributes in the CZMIL attribute dialog.  May add the same to others later.


    Version 7.16
    Jan C. Depner (PFM Software)
    08/27/16

    - Now uses the same font as all other ABE GUI apps.  Font can only be changed in pfmView Preferences.


    Version 7.17
    Jan C. Depner (PFM Software)
    10/15/16

    - Moved the "Load CUBE attributes" button to the global options from the GSF options (in hopes that I 
      might be able to release a binary of the CUBE process).
    - Removed the "Run CUBE" option.  This just added confusion and code kruft for something that can be
      easily done after the fact by running pfmCube.


    Version 7.18
    Jan C. Depner (PFM Software)
    12/21/16

    - Changed name of cube executable to use the new "cube_pfm" program.


    Version 7.20
    Jan C. Depner (PFM Software)
    04/18/17

    - Added changes to allow for using ip_rank to denote water or land processed returns in CZMIL_OPTECH_CLASS_HYBRID processed waveforms.
    - Limited CZMIL flags to land, water, hybrid, null, and water surface.
    - Happy tax day!!!


    Version 7.21
    Jan C. Depner (PFM Software)
    09/21/17

    - A bunch of changes to support doing translations in the future.  There is a generic
      pfmLoad_xx.ts file that can be run through Qt's "linguist" to translate to another language.


    Version 7.30
    Jan C. Depner (PFM Software)
    06/05/18

    - Changed a bunch of options, specifically:
      - Load land/water/shallow water based on classification ONLY (not processing mode)
      - Remove load of null CZMIL data
      - Remove flag of null CZMIL data
      - Remove flag of water surface data
      - Add "Flag shallow water data"
      - Add "Flag reprocessed data"


    Version 7.31
    Jan C. Depner (PFM Software)
    07/17/18

    - Added LAS 1.4 attributes to the LAS attributes


    Version 8.00
    Jan C. Depner (PFM Software)
    05/17/19

    - Now supports PFMv7.


    Version 8.01
    Jan C. Depner (PFM Software)
    06/29/19

    - Added [CZMIL HydroFusion Filter Invalid Load Flag].


    Version 8.10
    Jan C. Depner (PFM Software)
    07/03/19

    - Now allows only one data type when loading.  Since ABE can now load multiple PFMs (in pfmView) we no longer need
      the confusion caused by loading multiple data types in the same PFM.  We usually only used this for data
      comaprisons and now we can do that using multiple PFMs.  I should have done this many moons ago.
    - Removed support for HYPACK raw and IVS XYZ.  Nobody is using them (AFAIK) and they probably don't work
      anymore.
    - Removed HOF standard user flags and CZMIL user flags.  These will be set by default in pfmLoader since we're
      limiting PFMs to a single data type.


    Version 8.11
    Jan C. Depner (PFM Software)
    07/21/19

    - Added CZMIL Urban noise flags attribute.


    Version 8.12
    Jan C. Depner (PFM Software)
    07/23/19

    - Now that we're only loading one data type we can save the attribute settings for each
      data type separately.  That way, if you switch from one data type to another (e.g. LAS
      to CZMIL) your preferred attribute settings will show up.  Previously, they were cleared
      when switching between data types.


    Version 8.13
    Jan C. Depner (PFM Software)
    08/07/19

    - Now that get_area_mbr supports shape files we don't need to handle it differently from the other
      area file types.

</pre>*/
