
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



#ifndef __PFMLOADDEF_H__
#define __PFMLOADDEF_H__

#include <stdio.h>
#include <stdlib.h>
#include <cerrno>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include <QtCore>
#include <QtGui>
#if QT_VERSION >= 0x050000
#include <QtWidgets>
#endif

#include "nvutility.h"
#include "nvutility.hpp"

#include "pfm.h"
#include "pfm_extras.h"

#include "read_srtm_topo.h"
#include "dted.h"
#include "chrtr.h"
#include "chrtr2.h"
#include "binaryFeatureData.h"
#include "shapefil.h"

#include <proj_api.h>


#include "attributes.hpp"


/*  LLZ header files.  */

#include "llz.h"


#define MAX_LOAD_FILES        30


//  These data types are only local and are one off from the data type combobox button indexes.
//  They have no relation to the actual PFM data types and are just used locally to make sure
//  we only load one data type per PFM.

#define L_GSF          1
#define L_CZMIL        2
#define L_HOF          3
#define L_TOF          4
#define L_LAS          5
#define L_YXZ          6
#define L_LLZ          7
#define L_DTED         8
#define L_CHRTR        9
#define L_BAG          10
#define L_LIST         11


typedef struct
{
  QString             name;
  float               max_depth;
  float               min_depth;
  float               precision;
  float               mbin_size;
  float               gbin_size;
  uint8_t             apply_area_filter;
  uint8_t             deep_filter_only;
  PFM_OPEN_ARGS       open_args;
  QString             area;
  int32_t             polygon_count;
  NV_F64_COORD2       polygon[2000];
  QString             mosaic;
  QString             feature;
  float               cellstd;
  float               radius;
  int32_t             index;
  int32_t             hnd;
  int8_t              *add_map;
  uint8_t             existing;
  uint8_t             dateline;
} PFM_DEFINITION;


typedef struct
{
  int32_t            max_files;
  int32_t            max_lines;
  int32_t            max_pings;
  int32_t            max_beams;
  double             horizontal_error;
  double             vertical_error;
  uint8_t            appending;             //  Set if we are appending to one or more pre-existing files
  int16_t            attribute_count;
  ATTR_BOUNDS        time_attribute;
  int16_t            time_attribute_num;
  ATTR_BOUNDS        gsf_attribute[GSF_ATTRIBUTES];
  int16_t            gsf_attribute_num[GSF_ATTRIBUTES];
  ATTR_BOUNDS        hof_attribute[HOF_ATTRIBUTES];
  int16_t            hof_attribute_num[HOF_ATTRIBUTES];
  ATTR_BOUNDS        tof_attribute[TOF_ATTRIBUTES];
  int16_t            tof_attribute_num[TOF_ATTRIBUTES];
  ATTR_BOUNDS        czmil_attribute[CZMIL_ATTRIBUTES];
  QString            czmil_attribute_description[CZMIL_ATTRIBUTES];
  int16_t            czmil_attribute_num[CZMIL_ATTRIBUTES];
  ATTR_BOUNDS        las_attribute[LAS_ATTRIBUTES];
  int16_t            las_attribute_num[LAS_ATTRIBUTES];
  ATTR_BOUNDS        bag_attribute[BAG_ATTRIBUTES];
  int16_t            bag_attribute_num[BAG_ATTRIBUTES];
  QString            input_dir;
  QString            output_dir;
  QString            area_dir;
  QString            mosaic_dir;
  QString            feature_dir;
  QStringList        input_dirs;         //  List of all directories and filters input via the directory browse method of inputPage
  int32_t            cache_mem;
  char               progname[256];      //  This is the program name.  It will be used in all output to stderr so that shelling
                                         //  programs (like abe) will know what program printed out the error message.
  QFont              font;               //  Font used for all ABE GUI applications
  int32_t            data_type;          //  Now that I only allow one data type when building a PFM (since we can now open multiple
                                         //  PFMs in ABE) we only allow one data type in pfmLoad.  This is it.
} PFM_GLOBAL;


typedef struct
{
  QString             name;
  uint8_t             status;
  int32_t             type;
} FILE_DEFINITION;


typedef struct
{
  uint8_t             hof;                   //  Load HOF NULL values
  uint8_t             sub;                   //  Invert paths from Linux to Windoze
  uint8_t             lnd;                   //  Invalidate HOF land data
  uint8_t             ref;                   //  Load data as reference
  uint8_t             tof;                   //  Load TOF first returns as reference
  uint8_t             nom;                   //  Load GSF nominal depth if available
  uint8_t             sec;                   //  Invalidate all HOF secondary returns
  uint8_t             old;                   //  Load HOF LIDAR data in the old PFM_SHOALS_1K_Data form
  uint8_t             srtmb;                 //  Load best resolution SRTM data
  uint8_t             srtm1;                 //  Load 1 second resolution SRTM data (US only)
  uint8_t             srtm2;                 //  Load 1 second resolution SRTM data (world - DOD restricted)
  uint8_t             srtm3;                 //  Load 3 second resolution SRTM data
  uint8_t             srtm30;                //  Load 30 second resolution SRTM data
  uint8_t             srtmr;                 //  Load SRTM as reference
  uint8_t             srtme;                 //  Exclude SRTM2 data (1 second world)
  uint8_t             attr;                  //  Load CUBE attributes
  uint8_t             expand;                /*  Set if we want to expand the MBR by one-half bin size in all directions (useful for pixel
                                                 based grid output).  Note that this flag NEVER gets written to a parameter file since the 
                                                 expansion is done in this program.  pfmLoader just uses the adjusted bounds.  */
  uint8_t             czmil_land;            //  Load CZMIL data classified as land (any classification from 0 to 39)
  uint8_t             czmil_water;           //  Load CZMIL data classified as water (classification 40 and 43 through 45)
  uint8_t             czmil_water_surface;   //  Load CZMIL data classified as water surface (classification 41 and 42)
  uint8_t             ref_water_surface;     //  Load water surface data as reference data
  uint8_t             czmil_chan[9];         //  Load flags for CZMIL channels
  uint8_t             HF_filt;               //  Load HydroFusion filter invalidated data.
} FLAGS;


typedef struct PFM_Load_Parameters 
{
  FLAGS               flags;
  PFM_GLOBAL          pfm_global;
} PFM_LOAD_PARAMETERS;


typedef struct
{
  QDialog             *gsfD;
  QDialog             *hofD;
  QDialog             *tofD;
  QDialog             *czmilD;
  QDialog             *lasD;
  QDialog             *bagD;
} GLOBAL_DIALOG;


typedef struct
{
  QGroupBox           *fbox;
  QProgressBar        *fbar;
} RUN_PROGRESS;



#endif
