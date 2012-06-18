#!/bin/bash

ahome=/alice/cern.ch/user/h/hqvigsta
code=$ahome/code/ee_calib/pi0sim

alien_mkdir -p $code
alien_cp -n file:ee_calib.C       alien:$code
alien_cp -n file:ee_calib.jdl     alien:$code
alien_cp -n file:ee_calib.sh      alien:$ahome/bin
alien_cp -n file:mergeoutput.jdl  alien:$code
alien_cp -n file:validate.sh      alien:$code
alien_cp -n file:../task/EqEnCalibTask.h alien:$code
alien_cp -n file:../task/EqEnCalibTask.cxx alien:$code
