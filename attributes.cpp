
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



#include "attributes.hpp"



/*********************************************  IMPORTANT NOTES  **************************************************\

    Don't make the descriptions longer than 29 characters or you'll blow out the PFM attribute name length.

    To add to these you must change the ???_ATTRIBUTES definitions in attributes.hpp

    Because I always forget this - NEVER USE SLASHES (/) IN ANY OF THESE STRINGS!  If you 
    read the documentation for QSettings you'll understand why.

\*********************************************  IMPORTANT NOTES  **************************************************/



//  Time attribute

void setTimeAttributes (ATTR_BOUNDS *time_attribute)
{
  memset (time_attribute, 0, sizeof (ATTR_BOUNDS));

  strcpy (time_attribute->name, "Time (POSIX minutes)");
  time_attribute->scale = 1.0;
  time_attribute->min = (int32_t) ABE_NULL_TIME;
  time_attribute->max = (int32_t) -ABE_NULL_TIME;
}



//  GSF attributes

void setGSFAttributes (ATTR_BOUNDS *gsf_attribute)
{
  memset (gsf_attribute, 0, GSF_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (gsf_attribute[0].name, "GSF Heading");
  gsf_attribute[0].scale = 100.0;
  gsf_attribute[0].min = 0.0;
  gsf_attribute[0].max = 360.0;
  strcpy (gsf_attribute[1].name, "GSF Pitch");
  gsf_attribute[1].scale = 100.0;
  gsf_attribute[1].min = -30.0;
  gsf_attribute[1].max = 30.0;
  strcpy (gsf_attribute[2].name,"GSF Roll");
  gsf_attribute[2].scale = 100.0;
  gsf_attribute[2].min = -50.0;
  gsf_attribute[2].max = 50.0;
  strcpy (gsf_attribute[3].name,"GSF Heave");
  gsf_attribute[3].scale = 100.0;
  gsf_attribute[3].min = -25.0;
  gsf_attribute[3].max = 25.0;
  strcpy (gsf_attribute[4].name,"GSF Course");
  gsf_attribute[4].scale = 100.0;
  gsf_attribute[4].min = 0.0;
  gsf_attribute[4].max = 360.0;
  strcpy (gsf_attribute[5].name,"GSF Speed");
  gsf_attribute[5].scale = 100.0;
  gsf_attribute[5].min = 0.0;
  gsf_attribute[5].max = 30.0;
  strcpy (gsf_attribute[6].name,"GSF Height above ellipsoid");
  gsf_attribute[6].scale = 1000.0;
  gsf_attribute[6].min = -400.0;
  gsf_attribute[6].max = 400.0;
  strcpy (gsf_attribute[7].name,"GSF Ellipsoid datum distance");
  gsf_attribute[7].scale = 1000.0;
  gsf_attribute[7].min = -400.0;
  gsf_attribute[7].max = 400.0;
  strcpy (gsf_attribute[8].name,"GSF Tide correction");
  gsf_attribute[8].scale = 1000.0;
  gsf_attribute[8].min = -20.0;
  gsf_attribute[8].max = 20.0;
  strcpy (gsf_attribute[9].name,"GSF Tide type");
  gsf_attribute[9].scale = 1.0;
  gsf_attribute[9].min = 0;
  gsf_attribute[9].max = 4;
  strcpy (gsf_attribute[10].name,"GSF Across track distance");
  gsf_attribute[10].scale = 100.0;
  gsf_attribute[10].min = -50000.0;
  gsf_attribute[10].max = 50000.0;
  strcpy (gsf_attribute[11].name,"GSF Along track distance");
  gsf_attribute[11].scale = 100.0;
  gsf_attribute[11].min = -20000.0;
  gsf_attribute[11].max = 20000.0;
  strcpy (gsf_attribute[12].name,"GSF Travel time");
  gsf_attribute[12].scale = 1000.0;
  gsf_attribute[12].min = 0.0;
  gsf_attribute[12].max = 20.0;
  strcpy (gsf_attribute[13].name,"GSF Beam angle");
  gsf_attribute[13].scale = 100.0;
  gsf_attribute[13].min = -180.0;
  gsf_attribute[13].max = 180.0;
  strcpy (gsf_attribute[14].name,"GSF Beam angle forward");
  gsf_attribute[14].scale = 100.0;
  gsf_attribute[14].min = -180.0;
  gsf_attribute[14].max = 180.0;
  strcpy (gsf_attribute[15].name,"GSF Mean calibrated amp.");
  gsf_attribute[15].scale = 100.0;
  gsf_attribute[15].min = -200.0;
  gsf_attribute[15].max = 200.0;
  strcpy (gsf_attribute[16].name,"GSF Mean relative amp.");
  gsf_attribute[16].scale = 100.0;
  gsf_attribute[16].min = -200.0;
  gsf_attribute[16].max = 200.0;
  strcpy (gsf_attribute[17].name,"GSF Echo width");
  gsf_attribute[17].scale = 100.0;
  gsf_attribute[17].min = -200.0;
  gsf_attribute[17].max = 200.0;
  strcpy (gsf_attribute[18].name,"GSF Simrad Quality factor");
  gsf_attribute[18].scale = 1.0;
  gsf_attribute[18].min = 0;
  gsf_attribute[18].max = 255;
  strcpy (gsf_attribute[19].name,"GSF Signal to noise");
  gsf_attribute[19].scale = 100.0;
  gsf_attribute[19].min = 0.0;
  gsf_attribute[19].max = 100.0;
  strcpy (gsf_attribute[20].name,"GSF Receive heave");
  gsf_attribute[20].scale = 100.0;
  gsf_attribute[20].min = -100.0;
  gsf_attribute[20].max = 100.0;
  strcpy (gsf_attribute[21].name,"GSF Depth error");
  gsf_attribute[21].scale = 100.0;
  gsf_attribute[21].min = -1000.0;
  gsf_attribute[21].max = 1000.0;
  strcpy (gsf_attribute[22].name,"GSF Across track error");
  gsf_attribute[22].scale = 100.0;
  gsf_attribute[22].min = -5000.0;
  gsf_attribute[22].max = 5000.0;
  strcpy (gsf_attribute[23].name,"GSF Along track error");
  gsf_attribute[23].scale = 100.0;
  gsf_attribute[23].min = -5000.0;
  gsf_attribute[23].max = 5000.0;
  strcpy (gsf_attribute[24].name,"GSF Reson quality flag");
  gsf_attribute[24].scale = 1.0;
  gsf_attribute[24].min = 0;
  gsf_attribute[24].max = 255;
  strcpy (gsf_attribute[25].name,"GSF Beam flag");
  gsf_attribute[25].scale = 1.0;
  gsf_attribute[25].min = 0;
  gsf_attribute[25].max = 255;
  strcpy (gsf_attribute[26].name,"GSF Beam angle forward");
  gsf_attribute[26].scale = 100.0;
  gsf_attribute[26].min = -90.0;
  gsf_attribute[26].max = 90.0;
  strcpy (gsf_attribute[27].name,"GSF Transit sector");
  gsf_attribute[27].scale = 1.0;
  gsf_attribute[27].min = 0;
  gsf_attribute[27].max = 65535;
  strcpy (gsf_attribute[28].name,"GSF Detection info");
  gsf_attribute[28].scale = 1.0;
  gsf_attribute[28].min = 0;
  gsf_attribute[28].max = 65535;
  strcpy (gsf_attribute[29].name,"GSF Incident beam adj.");
  gsf_attribute[29].scale = 100.0;
  gsf_attribute[29].min = -90.0;
  gsf_attribute[29].max = 90.0;
  strcpy (gsf_attribute[30].name,"GSF System cleaning");
  gsf_attribute[30].scale = 1.0;
  gsf_attribute[30].min = 0;
  gsf_attribute[30].max = 65535;
  strcpy (gsf_attribute[31].name,"GSF Doppler correction");
  gsf_attribute[31].scale = 100.0;
  gsf_attribute[31].min = -5000.0;
  gsf_attribute[31].max = 5000.0;
}



  //  HOF attributes

void setHOFAttributes (ATTR_BOUNDS *hof_attribute)
{
  memset (hof_attribute, 0, HOF_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (hof_attribute[0].name, "HOF Haps version");
  hof_attribute[0].scale = 1.0;
  hof_attribute[0].min = 0.0;
  hof_attribute[0].max = 10.0;
  strcpy (hof_attribute[1].name, "HOF Position confidence");
  hof_attribute[1].scale = 1.0;
  hof_attribute[1].min = -100.0;
  hof_attribute[1].max = 100.0;
  strcpy (hof_attribute[2].name, "HOF Status");
  hof_attribute[2].scale = 1.0;
  hof_attribute[2].min = 0.0;
  hof_attribute[2].max = 126.0;
  strcpy (hof_attribute[3].name, "HOF Suggested DKS");
  hof_attribute[3].scale = 1.0;
  hof_attribute[3].min = 0.0;
  hof_attribute[3].max = 126.0;
  strcpy (hof_attribute[4].name, "HOF Suspect status");
  hof_attribute[4].scale = 1.0;
  hof_attribute[4].min = 0.0;
  hof_attribute[4].max = 126.0;
  strcpy (hof_attribute[5].name, "HOF Tide status");
  hof_attribute[5].scale = 1.0;
  hof_attribute[5].min = 0.0;
  hof_attribute[5].max = 4.0;
  strcpy (hof_attribute[6].name, "HOF Abbreviated depth conf.");
  hof_attribute[6].scale = 1.0;
  hof_attribute[6].min = -100.0;
  hof_attribute[6].max = 100.0;
  strcpy (hof_attribute[7].name, "HOF Abbreviated sec dep conf.");
  hof_attribute[7].scale = 1.0;
  hof_attribute[7].min = -100.0;
  hof_attribute[7].max = 100.0;
  strcpy (hof_attribute[8].name, "HOF Data type");
  hof_attribute[8].scale = 1.0;
  hof_attribute[8].min = 0.0;
  hof_attribute[8].max = 1.0;
  strcpy (hof_attribute[9].name, "HOF Land mode");
  hof_attribute[9].scale = 1.0;
  hof_attribute[9].min = 0.0;
  hof_attribute[9].max = 1.0;
  strcpy (hof_attribute[10].name, "HOF Classification status");
  hof_attribute[10].scale = 1.0;
  hof_attribute[10].min = 0.0;
  hof_attribute[10].max = 4096.0;
  strcpy (hof_attribute[11].name, "HOF Wave height");
  hof_attribute[11].scale = 100.0;
  hof_attribute[11].min = -25.0;
  hof_attribute[11].max = 25.0;
  strcpy (hof_attribute[12].name, "HOF Elevation");
  hof_attribute[12].scale = 1000.0;
  hof_attribute[12].min = -1000.0;
  hof_attribute[12].max = 1000.0;
  strcpy (hof_attribute[13].name, "HOF Topo value");
  hof_attribute[13].scale = 1000.0;
  hof_attribute[13].min = -1000.0;
  hof_attribute[13].max = 1000.0;
  strcpy (hof_attribute[14].name, "HOF Altitude");
  hof_attribute[14].scale = 100.0;
  hof_attribute[14].min = 0.0;
  hof_attribute[14].max = 1000.0;
  strcpy (hof_attribute[15].name, "HOF KGPS topo value");
  hof_attribute[15].scale = 100.0;
  hof_attribute[15].min = -1000.0;
  hof_attribute[15].max = 1000.0;
  strcpy (hof_attribute[16].name, "HOF KGPS datum value");
  hof_attribute[16].scale = 100.0;
  hof_attribute[16].min = -1000.0;
  hof_attribute[16].max = 1000.0;
  strcpy (hof_attribute[17].name, "HOF KGPS water level");
  hof_attribute[17].scale = 100.0;
  hof_attribute[17].min = -1000.0;
  hof_attribute[17].max = 1000.0;
  strcpy (hof_attribute[18].name, "HOF K value (TBD)");
  hof_attribute[18].scale = 1.0;
  hof_attribute[18].min =0.0;
  hof_attribute[18].max = 126.0;
  strcpy (hof_attribute[19].name, "HOF Intensity (TBD)");
  hof_attribute[19].scale = 1.0;
  hof_attribute[19].min = 0.0;
  hof_attribute[19].max = 1000.0;
  strcpy (hof_attribute[20].name, "HOF Bottom confidence");
  hof_attribute[20].scale = 1.0;
  hof_attribute[20].min = 0.0;
  hof_attribute[20].max = 126.0;
  strcpy (hof_attribute[21].name, "HOF Secondary depth conf.");
  hof_attribute[21].scale = 1.0;
  hof_attribute[21].min = 0.0;
  hof_attribute[21].max = 126.0;
  strcpy (hof_attribute[22].name, "HOF Nadir angle");
  hof_attribute[22].scale = 100.0;
  hof_attribute[22].min = -45.0;
  hof_attribute[22].max = 45.0;
  strcpy (hof_attribute[23].name, "HOF Scanner azimuth");
  hof_attribute[23].scale = 100.0;
  hof_attribute[23].min = -180.0;
  hof_attribute[23].max = 180.0;
  strcpy (hof_attribute[24].name, "HOF Surface APD FOM");
  hof_attribute[24].scale = 1000.0;
  hof_attribute[24].min = 0.0;
  hof_attribute[24].max = 1000.0;
  strcpy (hof_attribute[25].name, "HOF Surface IR FOM");
  hof_attribute[25].scale = 1000.0;
  hof_attribute[25].min = 0.0;
  hof_attribute[25].max = 1000.0;
  strcpy (hof_attribute[26].name, "HOF Surface Raman FOM");
  hof_attribute[26].scale = 1000.0;
  hof_attribute[26].min = 0.0;
  hof_attribute[26].max = 1000.0;
  strcpy (hof_attribute[27].name, "HOF Coded depth conf.");
  hof_attribute[27].scale = 1.0;
  hof_attribute[27].min = 0.0;
  hof_attribute[27].max = 1000000.0;
  strcpy (hof_attribute[28].name, "HOF Coded sec. depth conf.");
  hof_attribute[28].scale = 1.0;
  hof_attribute[28].min = 0.0;
  hof_attribute[28].max = 1000000.0;
  strcpy (hof_attribute[29].name, "HOF Warnings (1)");
  hof_attribute[29].scale = 1.0;
  hof_attribute[29].min = 0.0;
  hof_attribute[29].max = 32766.0;
  strcpy (hof_attribute[30].name, "HOF Warnings (2)");
  hof_attribute[30].scale = 1.0;
  hof_attribute[30].min = 0.0;
  hof_attribute[30].max = 32766.0;
  strcpy (hof_attribute[31].name, "HOF Warnings (3)");
  hof_attribute[31].scale = 1.0;
  hof_attribute[31].min = 0.0;
  hof_attribute[31].max = 32766.0;
  strcpy (hof_attribute[32].name, "HOF First calculated bfom*10");
  hof_attribute[32].scale = 1.0;
  hof_attribute[32].min = 0.0;
  hof_attribute[32].max = 1022.0;
  strcpy (hof_attribute[33].name, "HOF Second calculated bfom*10");
  hof_attribute[33].scale = 1.0;
  hof_attribute[33].min = 0.0;
  hof_attribute[33].max = 1022.0;
  strcpy (hof_attribute[34].name, "HOF First bottom run req.");
  hof_attribute[34].scale = 1.0;
  hof_attribute[34].min = 0.0;
  hof_attribute[34].max = 15.0;
  strcpy (hof_attribute[35].name, "HOF Second bottom run req.");
  hof_attribute[35].scale = 1.0;
  hof_attribute[35].min = 0.0;
  hof_attribute[35].max = 15.0;
  strcpy (hof_attribute[36].name, "HOF Primary return bin");
  hof_attribute[36].scale = 1.0;
  hof_attribute[36].min = 0.0;
  hof_attribute[36].max = 510.0;
  strcpy (hof_attribute[37].name, "HOF Secondary return bin");
  hof_attribute[37].scale = 1.0;
  hof_attribute[37].min = 0.0;
  hof_attribute[37].max = 510.0;
  strcpy (hof_attribute[38].name, "HOF PMT bottom bin");
  hof_attribute[38].scale = 1.0;
  hof_attribute[38].min = 0.0;
  hof_attribute[38].max = 510.0;
  strcpy (hof_attribute[39].name, "HOF PMT secondary bottom bin");
  hof_attribute[39].scale = 1.0;
  hof_attribute[39].min = 0.0;
  hof_attribute[39].max = 510.0;
  strcpy (hof_attribute[40].name, "HOF APD bottom bin");
  hof_attribute[40].scale = 1.0;
  hof_attribute[40].min = 0.0;
  hof_attribute[40].max = 510.0;
  strcpy (hof_attribute[41].name, "HOF APD secondary bottom bin");
  hof_attribute[41].scale = 1.0;
  hof_attribute[41].min = 0.0;
  hof_attribute[41].max = 510.0;
  strcpy (hof_attribute[42].name, "HOF Bottom channel");
  hof_attribute[42].scale = 1.0;
  hof_attribute[42].min = 0.0;
  hof_attribute[42].max = 2.0;
  strcpy (hof_attribute[43].name, "HOF Secondary bottom channel");
  hof_attribute[43].scale = 1.0;
  hof_attribute[43].min = 0.0;
  hof_attribute[43].max = 2.0;
  strcpy (hof_attribute[44].name, "HOF Surface bin from APD");
  hof_attribute[44].scale = 1.0;
  hof_attribute[44].min = 0.0;
  hof_attribute[44].max = 510.0;
  strcpy (hof_attribute[45].name, "HOF Surface bin from IR");
  hof_attribute[45].scale = 1.0;
  hof_attribute[45].min = 0.0;
  hof_attribute[45].max = 510.0;
  strcpy (hof_attribute[46].name, "HOF Surface bin from Raman");
  hof_attribute[46].scale = 1.0;
  hof_attribute[46].min = 0.0;
  hof_attribute[46].max = 510.0;
  strcpy (hof_attribute[47].name, "HOF Surface channel use");
  hof_attribute[47].scale = 1.0;
  hof_attribute[47].min = 0.0;
  hof_attribute[47].max = 2.0;
}


 
//  TOF attributes

void setTOFAttributes (ATTR_BOUNDS *tof_attribute)
{
  memset (tof_attribute, 0, TOF_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (tof_attribute[0].name, "TOF Classification status");
  tof_attribute[0].scale = 1.0;
  tof_attribute[0].min = 0;
  tof_attribute[0].max = 4094;
  strcpy (tof_attribute[1].name, "TOF Altitude");
  tof_attribute[1].scale = 100.0;
  tof_attribute[1].max = 0.0;
  tof_attribute[1].min = 1000.0;
  strcpy (tof_attribute[2].name, "TOF First return intensity");
  tof_attribute[2].scale = 1.0;
  tof_attribute[2].min = 0;
  tof_attribute[2].max = 126;
  strcpy (tof_attribute[3].name, "TOF Last return intensity");
  tof_attribute[3].scale = 1.0;
  tof_attribute[3].min = 0;
  tof_attribute[3].max = 126;
  strcpy (tof_attribute[4].name, "TOF First return confidence");
  tof_attribute[4].scale = 1.0;
  tof_attribute[4].min = 0;
  tof_attribute[4].max = 126;
  strcpy (tof_attribute[5].name, "TOF Last return confidence");
  tof_attribute[5].scale = 1.0;
  tof_attribute[5].min = 0;
  tof_attribute[5].max = 126;
  strcpy (tof_attribute[6].name, "TOF Nadir angle");
  tof_attribute[6].scale = 100.0;
  tof_attribute[6].min = -45.0;
  tof_attribute[6].max = 45.0;
  strcpy (tof_attribute[7].name, "TOF Scanner azimuth");
  tof_attribute[7].scale = 100.0;
  tof_attribute[7].min = -180.0;
  tof_attribute[7].max = 180.0;
  strcpy (tof_attribute[8].name, "TOF Position confidence");
  tof_attribute[8].scale = 1.0;
  tof_attribute[8].min = 0;
  tof_attribute[8].max = 126;
}



//  CZMIL attributes.
//
//  IMPORTANT NOTE - This version of setCZMILAttributes is different from all of the other versions (in pfmLoader, pfmEdit3D, pfmView)
//  because it has the descriptions of the attributes.  These are used as help text in the globalPage.cpp file for the CZMIL
//  attribute dialog.
//
//  IMPORTANT WARNING - Even though it would make more sense to put czmil_attribute_description into the ATTR_BOUNDS
//  structure, don't do it!  For some inexplicable, Windows type reason, this makes pfmLoad segfault on Windows.

void setCZMILAttributes (ATTR_BOUNDS *czmil_attribute, QString *czmil_attribute_description)
{
  memset (czmil_attribute, 0, CZMIL_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (czmil_attribute[0].name, "CZMIL Reflectance");
  czmil_attribute_description[0] = "Reflectance of the sea floor or the intensity of the return for land shots.";
  czmil_attribute[0].scale = 10000.0;
  czmil_attribute[0].min = 0.0;
  czmil_attribute[0].max = 1.0;
  strcpy (czmil_attribute[1].name, "CZMIL Classification");
  czmil_attribute_description[1] = "LiDAR classification value.  These values are based on the ASPRS LAS 1.4 spec and the 2013 ASPRS paper - \"LAS Domain Profile Description: Topo-Bathy Lidar\", July 17, 2013.";
  czmil_attribute[1].scale = 1.0;
  czmil_attribute[1].min = 0;
  czmil_attribute[1].max = 254;
  strcpy (czmil_attribute[2].name, "CZMIL Off nadir angle");
  czmil_attribute_description[2] = "Angle of the laser away from nadir.";
  czmil_attribute[2].scale = 10000.0;
  czmil_attribute[2].min = -105.0;
  czmil_attribute[2].max = 105.0;
  strcpy (czmil_attribute[3].name, "CZMIL Water level");
  czmil_attribute_description[3] = "Water level elevation above the ellipsoid.";
  czmil_attribute[3].scale = 1000.0;
  czmil_attribute[3].min = -1000.0;
  czmil_attribute[3].max = 1000.0;
  strcpy (czmil_attribute[4].name, "CZMIL Local datum offset");
  czmil_attribute_description[4] = "Offset to a local vertical datum that may be defined in the CPF file header.";
  czmil_attribute[4].scale = 1000.0;
  czmil_attribute[4].min = -1000.0;
  czmil_attribute[4].max = 1000.0;
  strcpy (czmil_attribute[5].name, "CZMIL Interest point");
  czmil_attribute_description[5] = "Location along the X axis of the laser waveform at which depth/position is computed.";
  czmil_attribute[5].scale = 100.0;
  czmil_attribute[5].min = 0.0;
  czmil_attribute[5].max = (float) CZMIL_MAX_PACKETS * 64.0;
  strcpy (czmil_attribute[6].name, "CZMIL Interest point rank");
  czmil_attribute_description[6] = "Non-water surface flag.  This will be 0 for the water surface return of a water processed waveform.";
  czmil_attribute[6].scale = 1.0;
  czmil_attribute[6].min = 0;
  czmil_attribute[6].max = CZMIL_MAX_RETURNS;
  strcpy (czmil_attribute[7].name, "CZMIL Channel number");
  czmil_attribute_description[7] = "Channel number, in this order, central shallow channel, 6 surrounding shallow channels, IR channel, and deep channel.";
  czmil_attribute[7].scale = 1.0;
  czmil_attribute[7].min = 0;
  czmil_attribute[7].max = 9;
  strcpy (czmil_attribute[8].name, "CZMIL Return number");
  czmil_attribute_description[8] = "Number of the return.";
  czmil_attribute[8].scale = 1.0;
  czmil_attribute[8].min = 0;
  czmil_attribute[8].max = CZMIL_MAX_RETURNS;
  strcpy (czmil_attribute[9].name, "CZMIL Kd");
  czmil_attribute_description[9] = "Kd value.";
  czmil_attribute[9].scale = 10000.0;
  czmil_attribute[9].min = 0.0;
  czmil_attribute[9].max = 1.0;
  strcpy (czmil_attribute[10].name, "CZMIL Laser energy");
  czmil_attribute_description[10] = "Laser energy derived from T0 in mJ.";
  czmil_attribute[10].scale = 10000.0;
  czmil_attribute[10].min = 0.0;
  czmil_attribute[10].max = 6.5535;
  strcpy (czmil_attribute[11].name, "CZMIL T0 interest point");
  czmil_attribute_description[11] = "Location along the X axis of the T0 waveform at which elevation/position is computed.";
  czmil_attribute[11].scale = 100.0;
  czmil_attribute[11].min = 0.0;
  czmil_attribute[11].max = 64.0;
  strcpy (czmil_attribute[12].name, "CZMIL Detection probability");
  czmil_attribute_description[12] = "Probability of detection.";
  czmil_attribute[12].scale = 10000.0;
  czmil_attribute[12].min = 0.0;
  czmil_attribute[12].max = 1.0;
  strcpy (czmil_attribute[13].name, "CZMIL Processing mode");
  czmil_attribute_description[13] = "Optech waveform processing mode for each channel (e.g. land, water, shallow water, etc.";
  czmil_attribute[13].scale = 1.0;
  czmil_attribute[13].min = 0;
  czmil_attribute[13].max = 63;
  strcpy (czmil_attribute[14].name, "CZMIL Filter reason");
  czmil_attribute_description[14] = "Reason a point has been marked CZMIL_FILTER_INVALID.";
  czmil_attribute[14].scale = 1.0;
  czmil_attribute[14].min = 0;
  czmil_attribute[14].max = 63;
  strcpy (czmil_attribute[15].name, "CZMIL D_index");
  czmil_attribute_description[15] = "Amplitude of the interest point normalized for noise.  In other words, on a completely noise free signal, if the amplitude of the interest point was 623, d_index would be 623.  When the noise is taken into account d_index will (always) be less than the actual amplitude of the interest point.";
  czmil_attribute[15].scale = 1.0;
  czmil_attribute[15].min = 0.0;
  czmil_attribute[15].max = 1023.0;
  strcpy (czmil_attribute[16].name, "CZMIL D_index_cube");
  czmil_attribute_description[16] = "Prediction of the normalized signal amplitude from a cube on the seafloor (deep channel)";
  czmil_attribute[16].scale = 1.0;
  czmil_attribute[16].min = 0.0;
  czmil_attribute[16].max = 1023.0;
  strcpy (czmil_attribute[17].name, "CZMIL User data");
  czmil_attribute_description[17] = "User data, defined by the application and explained in the [USER DATA DESCRIPTION] field of the CPF file header.";
  czmil_attribute[17].scale = 1.0;
  czmil_attribute[17].min = 0;
  czmil_attribute[17].max = 255;
  strcpy (czmil_attribute[18].name, "CZMIL Urban noise flags");
  czmil_attribute_description[18] = "Urban noise filter flags, based on number of valid returns per channel and per shot.";
  czmil_attribute[18].scale = 1.0;
  czmil_attribute[18].min = 0;
  czmil_attribute[18].max = 255;
  strcpy (czmil_attribute[19].name, "CZMIL CSF Scan angle");
  czmil_attribute_description[19] = QString ("Scan angle of the laser with respect to the aircraft heading.<br><br><b>WARNING: Selecting this check box will cause the CSF file to be read as well as the CPF file.  This will add approximately 3% to your load time.  If you don't need this CSF attribute for filtering based on the attribute value, do not select it!</b>");
  czmil_attribute[19].scale = 100.0;
  czmil_attribute[19].min = 0.0;
  czmil_attribute[19].max = 360.0;
  strcpy (czmil_attribute[20].name, "CZMIL CSF Altitude");
  czmil_attribute_description[20] = QString ("Altitude of the aircraft with respect to the ellipsoid.<br><br><b>WARNING: Selecting this check box will cause the CSF file to be read as well as the CPF file.  This will add approximately 3% to your load time.  If you don't need this CSF attribute for filtering based on the attribute value, do not select it!</b>");
  czmil_attribute[20].scale = 1000.0;
  czmil_attribute[20].min = -1000.0;
  czmil_attribute[20].max = 1000.0;
  strcpy (czmil_attribute[21].name, "CZMIL CSF Roll");
  czmil_attribute_description[21] = QString ("Roll angle of aircraft.<br><br><b>WARNING: Selecting this check box will cause the CSF file to be read as well as the CPF file.  This will add approximately 3% to your load time.  If you don't need this CSF attribute for filtering based on the attribute value, do not select it!</b>");
  czmil_attribute[21].scale = 100.0;
  czmil_attribute[21].min = -100.0;
  czmil_attribute[21].max = 100.0;
  strcpy (czmil_attribute[22].name, "CZMIL CSF Pitch");
  czmil_attribute_description[22] = QString ("Pitch angle of aircraft.<br><br><b>WARNING: Selecting this check box will cause the CSF file to be read as well as the CPF file.  This will add approximately 3% to your load time.  If you don't need this CSF attribute for filtering based on the attribute value, do not select it!</b>");
  czmil_attribute[22].scale = 100.0;
  czmil_attribute[22].min = -100.0;
  czmil_attribute[22].max = 100.0;
  strcpy (czmil_attribute[23].name, "CZMIL CSF Heading");
  czmil_attribute_description[23] = QString ("Aircraft heading.<br><br><b>WARNING: Selecting this check box will cause the CSF file to be read as well as the CPF file.  This will add approximately 3% to your load time.  If you don't need this CSF attribute for filtering based on the attribute value, do not select it!</b>");
  czmil_attribute[23].scale = 100.0;
  czmil_attribute[23].min = 0.0;
  czmil_attribute[23].max = 360.0;
}



//  LAS attributes

void setLASAttributes (ATTR_BOUNDS *las_attribute)
{
  memset (las_attribute, 0, LAS_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (las_attribute[0].name, "LAS Intensity");
  las_attribute[0].scale = 1.0;
  las_attribute[0].min = 0;
  las_attribute[0].max = 65535;
  strcpy (las_attribute[1].name, "LAS Classification");
  las_attribute[1].scale = 1.0;
  las_attribute[1].min = 0;
  las_attribute[1].max = 255;
  strcpy (las_attribute[2].name, "LAS Scan angle rank");
  las_attribute[2].scale = 1.0;
  las_attribute[2].min = -90;
  las_attribute[2].max = 90;
  strcpy (las_attribute[3].name, "LAS Number of returns");
  las_attribute[3].scale = 1.0;
  las_attribute[3].min = 0;
  las_attribute[3].max = 5;
  strcpy (las_attribute[4].name, "LAS Return number");
  las_attribute[4].scale = 1.0;
  las_attribute[4].min = 0;
  las_attribute[4].max = 5;
  strcpy (las_attribute[5].name, "LAS Scan direction flag");
  las_attribute[5].scale = 1.0;
  las_attribute[5].min = 0;
  las_attribute[5].max = 1;
  strcpy (las_attribute[6].name, "LAS Edge of flightline flag");
  las_attribute[6].scale = 1.0;
  las_attribute[6].min = 0;
  las_attribute[6].max = 1;
  strcpy (las_attribute[7].name, "LAS User data");
  las_attribute[7].scale = 1.0;
  las_attribute[7].min = 0;
  las_attribute[7].max = 255;
  strcpy (las_attribute[8].name, "LAS Point source ID");
  las_attribute[8].scale = 1.0;
  las_attribute[8].min = 0;
  las_attribute[8].max = 65535;
  strcpy (las_attribute[9].name, "LAS Scanner channel");
  las_attribute[9].scale = 1.0;
  las_attribute[9].min = 0;
  las_attribute[9].max = 3;
  strcpy (las_attribute[10].name, "LAS RGB red");
  las_attribute[10].scale = 1.0;
  las_attribute[10].min = 0;
  las_attribute[10].max = 65535;
  strcpy (las_attribute[11].name, "LAS RGB green");
  las_attribute[11].scale = 1.0;
  las_attribute[11].min = 0;
  las_attribute[11].max = 65535;
  strcpy (las_attribute[12].name, "LAS RGB blue");
  las_attribute[12].scale = 1.0;
  las_attribute[12].min = 0;
  las_attribute[12].max = 65535;
  strcpy (las_attribute[13].name, "LAS NIR");
  las_attribute[13].scale = 1.0;
  las_attribute[13].min = 0;
  las_attribute[13].max = 65535;
  strcpy (las_attribute[14].name, "LAS Wave packet index");
  las_attribute[14].scale = 1.0;
  las_attribute[14].min = 0;
  las_attribute[14].max = 255;
  strcpy (las_attribute[15].name, "LAS Wave return location");
  las_attribute[15].scale = 10.0;
  las_attribute[15].min = 0;
  las_attribute[15].max = 214748364;
}



//  BAG attributes

void setBAGAttributes (ATTR_BOUNDS *bag_attribute)
{
  memset (bag_attribute, 0, BAG_ATTRIBUTES * sizeof (ATTR_BOUNDS));

  strcpy (bag_attribute[0].name, "BAG Uncertainty");
  bag_attribute[0].scale = 1000.0;
  bag_attribute[0].min = 0.0;
  bag_attribute[0].max = 1001.0;
}



void countTimeAttributes (int16_t time_attribute_num, int16_t *attribute_count)
{
  if (time_attribute_num) (*attribute_count)++;
}


void countGSFAttributes (int16_t *gsf_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    {
      if (gsf_attribute_num[i]) (*attribute_count)++;
    }
}


void countHOFAttributes (int16_t *hof_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    {
      if (hof_attribute_num[i]) (*attribute_count)++;
    }
}


void countTOFAttributes (int16_t *tof_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    {
      if (tof_attribute_num[i]) (*attribute_count)++;
    }
}


void countCZMILAttributes (int16_t *czmil_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    {
      if (czmil_attribute_num[i]) (*attribute_count)++;
    }
}


void countLASAttributes (int16_t *las_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    {
      if (las_attribute_num[i]) (*attribute_count)++;
    }
}


void countBAGAttributes (int16_t *bag_attribute_num, int16_t *attribute_count)
{
  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    {
      if (bag_attribute_num[i]) (*attribute_count)++;
    }
}


int32_t gsfAttributesSet (int16_t *gsf_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < GSF_ATTRIBUTES ; i++)
    {
      if (gsf_attribute_num[i]) count++;
    }

  return (count);
}


int32_t hofAttributesSet (int16_t *hof_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < HOF_ATTRIBUTES ; i++)
    {
      if (hof_attribute_num[i]) count++;
    }

  return (count);
}


int32_t tofAttributesSet (int16_t *tof_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < TOF_ATTRIBUTES ; i++)
    {
      if (tof_attribute_num[i]) count++;
    }

  return (count);
}


int32_t czmilAttributesSet (int16_t *czmil_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < CZMIL_ATTRIBUTES ; i++)
    {
      if (czmil_attribute_num[i]) count++;
    }

  return (count);
}


int32_t lasAttributesSet (int16_t *las_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < LAS_ATTRIBUTES ; i++)
    {
      if (las_attribute_num[i]) count++;
    }

  return (count);
}


int32_t bagAttributesSet (int16_t *bag_attribute_num)
{
  int32_t count = 0;

  for (int32_t i = 0 ; i < BAG_ATTRIBUTES ; i++)
    {
      if (bag_attribute_num[i]) count++;
    }

  return (count);
}
