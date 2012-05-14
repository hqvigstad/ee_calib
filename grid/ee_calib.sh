#!/bin/bash
export GRID_TOKEN=OK
echo ===========================
echo $PATH 
echo $ROOTSYS
echo $LD_LIBRARY_PATH
echo $GRID_TOKEN
echo dataset = $1
echo ==========================

ls -l
echo ==================================================================

aliroot -b -q ee_calib.C\(\"$1\"\)

echo ==================================================================
ls -l
echo "########################    Train finished    ###########################"
