#!/bin/sh

#*********************************************************************
# Licensed Materials - Property of IBM                               *
#                                                                    *
# SAMPLE                                                             *
#                                                                    *
# (c) Copyright IBM Corp. 2017 All Rights Reserved                   *
#                                                                    *
# US Government Users Restricted Rights - Use, duplication or        *
# disclosure restricted by GSA ADP Schedule Contract with IBM Corp   *
#*********************************************************************

export OLD_CXX_L6SYSLIB=$_CXX_L6SYSLIB
export OLD_STEPLIB=$STEPLIB

echo "Running 64-bit EXCI connector sample build script"

echo ""
echo "CICS HLQ      = [$MY_CICS_HLQ]"
echo "_CXX_L6SYSLIB = [$_CXX_L6SYSLIB]"
echo "STEPLIB       = [$STEPLIB]"

echo ""
echo "Setting _CXX_L6SYSLIB and STEPLIB to required values"

export _CXX_L6SYSLIB=$MY_CICS_HLQ.SDFHEXCI:CEE.SCEEBND2:CBC.SCCNOBJ:SYS1.CSSLIB
export STEPLIB=$MY_CICS_HLQ.SDFHLOAD:$STEPLIB

echo ""
echo "_CXX_L6SYSLIB = [$_CXX_L6SYSLIB]"
echo "STEPLIB       = [$STEPLIB]"
echo ""
echo "Getting EXCI header file DFHXCPLH"
cp -v "//'$MY_CICS_HLQ.SDFHC370(DFHXCPLH)'" inc/dfhxcplh.h
echo ""
echo "Running MakeFile"
echo ""
echo "-----------------------------------------------------------------------"
echo ""

make full

echo ""
echo "-----------------------------------------------------------------------"
echo ""
echo "Returning _CXX_L6SYSLIB and STEPLIB to original values"

export _CXX_L6SYSLIB=$OLD_CXX_L6SYSLIB
export STEPLIB=$OLD_STEPLIB

echo ""
echo "_CXX_L6SYSLIB = [$_CXX_L6SYSLIB]"
echo "STEPLIB       = [$STEPLIB]"
