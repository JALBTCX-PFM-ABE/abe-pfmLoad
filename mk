#!/bin/bash

if [ ! $PFM_ABE_DEV ]; then

    export PFM_ABE_DEV=${1:-"/usr/local"}

fi

export PFM_BIN=$PFM_ABE_DEV/bin
export PFM_LIB=$PFM_ABE_DEV/lib
export PFM_INCLUDE=$PFM_ABE_DEV/include


CHECK_QT=`echo $QTDIR | grep "qt-3"`
if [ $CHECK_QT ] || [ !$QTDIR ]; then
    QTDIST=`ls ../../FOSS_libraries/qt-*.tar.gz | cut -d- -f5 | cut -dt -f1 | cut -d. --complement -f4`
    QT_TOP=Trolltech/Qt-$QTDIST
    QTDIR=$PFM_ABE_DEV/$QT_TOP
fi


#  Check for major version >= 5 so that we can add the "widgets" field to QT

QT_MAJOR_VERSION=`echo $QTDIR | sed -e 's/^.*Qt-//' | cut -d. -f1`
if [ $QT_MAJOR_VERSION -ge 5 ];then
    WIDGETS="widgets"
else
    WIDGETS=""
fi


SYS=`uname -s`


if [ $SYS = "Linux" ]; then
    DEFS=NVLinux
    LIBRARIES="-L $PFM_LIB -lgsf -lllz -lCZMIL -lCHARTS -lchrtr2 -lbag -lbeecrypt -lhdf5 -lBinaryFeatureData -lnvutility -lpfm -llas -lgdal -lxml2 -lpoppler -lproj -lGLU"
    export LD_LIBRARY_PATH=$PFM_LIB:$QTDIR/lib:$LD_LIBRARY_PATH
else
    DEFS="WIN32 NVWIN3X UINT32_C INT32_C"
    LIBRARIES="-L $PFM_LIB -lgsf -lllz -lCZMIL -lCHARTS -lchrtr2 -lbag -lbeecrypt -lhdf5 -lBinaryFeatureData -lnvutility -lpfm -llas -lgdal -lxml2 -lpoppler -lproj -lwsock32 -liconv"
    export QMAKESPEC=win32-g++
    EXCEPTIONS=exceptions
fi


# This is the only way I can keep lasdefinitions.hpp from barfing warnings all over my builds.

LASLIB_BS="-fno-strict-aliasing"


# As of gcc 6 --enable-default-pie has been built in to the gcc compiler.
# We need to turn it off.

GVERSION=`gcc -dumpversion | cut -f 1 -d.`
MFLAGS=""
if [ $GVERSION -gt 5 ]; then
    MFLAGS=-no-pie
fi


#  Get the name from the directory name

NAME=`basename $PWD`


# Building the Makefile using qmake and adding extra includes, defines, and libs


rm -f qrc_icons.cpp $NAME.pro Makefile

$QTDIR/bin/qmake -project -o $NAME.tmp
cat >$NAME.pro <<EOF
RC_FILE = $NAME.rc
RESOURCES = icons.qrc
contains(QT_CONFIG, opengl): QT += opengl
QT += $WIDGETS
INCLUDEPATH += $PFM_INCLUDE
LIBS += $LIBRARIES
DEFINES += $DEFS
DEFINES += $CZMIL_DEF
CONFIG += $EXCEPTIONS
CONFIG += console
QMAKE_CXXFLAGS += $LASLIB_BS
QMAKE_LFLAGS += $MFLAGS
EOF

cat $NAME.tmp >>$NAME.pro
rm $NAME.tmp


$QTDIR/bin/qmake -o Makefile



if [ $SYS = "Linux" ]; then
    make
    if [ $? != 0 ];then
        exit -1
    fi
    chmod 755 $NAME
    mv $NAME $PFM_BIN
    mkdir $PFM_BIN/pfmIcons 2>/dev/null
    cp icons/$NAME.png $PFM_BIN/pfmIcons


    #  Translation, if the file exists.

    TRANS=$NAME"_xx.ts"
    if [ -f $TRANS ]; then
        lupdate -no-obsolete $NAME.pro
    fi

else
    if [ ! $WINMAKE ]; then
        WINMAKE=release
    fi
    make $WINMAKE
    if [ $? != 0 ];then
        exit -1
    fi
    chmod 755 $WINMAKE/$NAME.exe
    cp $WINMAKE/$NAME.exe $PFM_BIN
    rm $WINMAKE/$NAME.exe
    cp icons/$NAME.ico $PFM_BIN
fi


# Get rid of the Makefile so there is no confusion.  It will be generated again the next time we build.

rm Makefile
