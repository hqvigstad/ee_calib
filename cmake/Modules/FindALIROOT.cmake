# -*- mode: cmake -*-
# author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
#
# This is a cmake module for AliROOT
#
# usage: 
# 	 find_library(ALIROOT [COMPONENTS/REQUIRED ... ])
#
# components
#     PHOS, MUON, FMD, PMD, HMPID, T0, ZDZ, ACORDE, VZERO, EMCAL, TPC, ITS, TRD, TOF, HLT, MFT
#
# If ALIROOT is found, the following is defined:
#  ALIROOT_FOUND - true if AliROOT is found
#  ALIROOT_INCLUDES - The AliROOT include directories (you may use the alias ALIROOT_INCLUDE_DIRS)
#  ALIROOT_LIBS - the AliROOT library statements (you may use the alias ALIROOT_LIBRARIES)
#  ( ALIROOT_DEFINITION - not functional!  )
#
# I make no claims to the usebility of this module.
# Permission is hereby granted to use and distribute this module under the terms of the GNU LGPL



# find root-config
Find_program(ROOT_CONFIG root-config )

if (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")
  if(ALIROOT_FIND_REQUIRED)
    message(FATAL_ERROR "Install Root and make sure it (root-config) is in PATH")
  elseif(ALIROOT_FIND_REQUIRED)
    message(WARNING "Install Root and make sure it (root-config) is in PATH")
  endif(ROOT_FIND_REQUIRED)
endif (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")


# determin target
execute_process(
    COMMAND root-config --arch
    OUTPUT_VARIABLE ALICE_TARGET
    OUTPUT_STRIP_TRAILING_WHITESPACE)
# libs should be found in
message(STATUS "FindALIROOT.cmake: searching for AliROOT in $ENV{ALICE_ROOT}/lib/tgt_${ALICE_TARGET}")
set(ALIROOT_LIB_DIR $ENV{ALICE_ROOT}/lib/tgt_${ALICE_TARGET})


# check to see if libESD.so is built
find_library(ALIROOT_FOUND ESD ${ALIROOT_LIB_DIR})
message(STATUS ${ALIROOT_FOUND})
if(NOT ALIROOT_FOUND)
  message(STATUS "AliROOT not found")
  if(ALIROOT_FIND_REQUIRED)
    message(FATAL_ERROR "AliROOT Required!")
  endif(ALIROOT_FIND_REQUIRED)
else(NOT ALIROOT_FOUND) # if found
  message(STATUS "libESD found, assuming that complete and installed aliroot build is found in \$ALICE_ROOT")
  set(ALIROOT_FOUND TRUE)
endif(NOT ALIROOT_FOUND)




# Main AliROOT (should maybe be split into sub-components like the detector modules)
set(ALIROOT_INCLUDE "$ENV{ALICE_ROOT}/include/")
set(ALIROOT_LIB "-L${ALIROOT_LIB_DIR} -lmicrocern -llhapdf -lpythia6")
set(ALIROOT_LIB "${ALIROOT_LIB} -lSTEERBase -lESD -lCDB -lRAWDatabase -lRAWDatarec -lAOD -lANALYSIS")
set(ALIROOT_LIB "${ALIROOT_LIB} -lSTEER -lRAWDatasim -lFASTSIM -lEVGEN -lAliPythia6 -lSTAT")
set(ALIROOT_LIB "${ALIROOT_LIB} -lhijing -lTHijing -lSTRUCT")


# *** Components ***

# set ALIROOT_USE_*COMPONENT* variables
foreach( component ${ALIROOT_FIND_COMPONENTS} )
  set( ALIROOT_USE_${component} TRUE )
endforeach( component )


# PHOS
if(ALIROOT_USE_PHOS)
  find_file(ALIROOT_PHOS_FOUND NAMES AliPHOSReconstructor.h  PATHS $ENV{ALICE_ROOT}/PHOS/)
  if(ALIROOT_PHOS_FOUND)
    # includes
    set(ALIROOT_PHOS_INCLUDE $ENV{ALICE_ROOT}/PHOS/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_PHOS_INCLUDE})
    # libs
    set(ALIROOT_PHOS_LIB "-L${ALIROOT_LIB_DIR} -lPHOSUtils -lPHOSbase -lPHOSsim -lPHOSrec")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_PHOS_LIB})
  elseif(ALIROOT_PHOS_FOUND)
    message(STATUS "ALIROOT_PHOS not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliPHOSReconstructor.h in $ENV{ALICE_ROOT}/PHOS/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_PHOS_FOUND)
endif(ALIROOT_USE_PHOS)


# MUON
if(ALIROOT_USE_MUON)
  find_file(ALIROOT_MUON_FOUND NAMES AliMUONReconstructor.h  PATHS $ENV{ALICE_ROOT}/MUON/)
  if(ALIROOT_MUON_FOUND)
    # includes
    set(ALIROOT_MUON_INCLUDE $ENV{ALICE_ROOT}/MUON/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_MUON_INCLUDE})
    # libs
    set(ALIROOT_MUON_LIB "-L${ALIROOT_LIB_DIR} -lMUONmapping -lMUONgeometry -lMUONcalib -lMUONraw -lMUONtrigger -lMUONbase -lMUONsim -lMUONrec -lMUONevaluation")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_MUON_LIB})
  elseif(ALIROOT_MUON_FOUND)
    message(STATUS "ALIROOT_MUON not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliMUONReconstructor.h in $ENV{ALICE_ROOT}/MUON/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_MUON_FOUND)
endif(ALIROOT_USE_MUON)


# FMD
if(ALIROOT_USE_FMD)
  find_file(ALIROOT_FMD_FOUND NAMES AliFMDReconstructor.h  PATHS $ENV{ALICE_ROOT}/FMD/)
  if(ALIROOT_FMD_FOUND)
    # includes
    set(ALIROOT_FMD_INCLUDE $ENV{ALICE_ROOT}/FMD/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_FMD_INCLUDE})
    # libs
    set(ALIROOT_FMD_LIB "-L${ALIROOT_LIB_DIR} -lFMDbase -lFMDsim -lFMDrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_FMD_LIB})
  elseif(ALIROOT_FMD_FOUND)
    message(STATUS "ALIROOT_FMD not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliFMDReconstructor.h in $ENV{ALICE_ROOT}/FMD/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_FMD_FOUND)
endif(ALIROOT_USE_FMD)


# PMD
if(ALIROOT_USE_PMD)
  find_file(ALIROOT_PMD_FOUND NAMES AliPMDReconstructor.h  PATHS $ENV{ALICE_ROOT}/PMD/)
  if(ALIROOT_PMD_FOUND)
    # includes
    set(ALIROOT_PMD_INCLUDE $ENV{ALICE_ROOT}/PMD/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_PMD_INCLUDE})
    # libs
    set(ALIROOT_PMD_LIB "-L${ALIROOT_LIB_DIR} -lPMDbase -lPMDsim -lPMDrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_PMD_LIB})
  elseif(ALIROOT_PMD_FOUND)
    message(STATUS "ALIROOT_PMD not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliPMDReconstructor.h in $ENV{ALICE_ROOT}/PMD/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_PMD_FOUND)
endif(ALIROOT_USE_PMD)


# HMPID
if(ALIROOT_USE_HMPID)
  find_file(ALIROOT_HMPID_FOUND NAMES AliHMPIDReconstructor.h  PATHS $ENV{ALICE_ROOT}/HMPID/)
  if(ALIROOT_HMPID_FOUND)
    # includes
    set(ALIROOT_HMPID_INCLUDE $ENV{ALICE_ROOT}/HMPID/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_HMPID_INCLUDE})
    # libs
    set(ALIROOT_HMPID_LIB "-L${ALIROOT_LIB_DIR} -lHMPIDbase -lHMPIDsim -lHMPIDrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_HMPID_LIB})
  elseif(ALIROOT_HMPID_FOUND)
    message(STATUS "ALIROOT_HMPID not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliHMPIDReconstructor.h in $ENV{ALICE_ROOT}/HMPID/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_HMPID_FOUND)
endif(ALIROOT_USE_HMPID)


# T0
if(ALIROOT_USE_T0)
  find_file(ALIROOT_T0_FOUND NAMES AliT0Reconstructor.h  PATHS $ENV{ALICE_ROOT}/T0/)
  if(ALIROOT_T0_FOUND)
    # includes
    set(ALIROOT_T0_INCLUDE $ENV{ALICE_ROOT}/T0/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_T0_INCLUDE})
    # libs
    set(ALIROOT_T0_LIB "-L${ALIROOT_LIB_DIR} -lT0base -lT0sim -lT0rec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_T0_LIB})
  elseif(ALIROOT_T0_FOUND)
    message(STATUS "ALIROOT_T0 not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliT0Reconstructor.h in $ENV{ALICE_ROOT}/T0/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_T0_FOUND)
endif(ALIROOT_USE_T0)


# ZDC
if(ALIROOT_USE_ZDC)
  find_file(ALIROOT_ZDC_FOUND NAMES AliZDCReconstructor.h  PATHS $ENV{ALICE_ROOT}/ZDC/)
  if(ALIROOT_ZDC_FOUND)
    # includes
    set(ALIROOT_ZDC_INCLUDE $ENV{ALICE_ROOT}/ZDC/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_ZDC_INCLUDE})
    # libs
    set(ALIROOT_ZDC_LIB "-L${ALIROOT_LIB_DIR} -lZDCbase -lZDCsim -lZDCrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_ZDC_LIB})
  elseif(ALIROOT_ZDC_FOUND)
    message(STATUS "ALIROOT_ZDC not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliZDCReconstructor.h in $ENV{ALICE_ROOT}/ZDC/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_ZDC_FOUND)
endif(ALIROOT_USE_ZDC)


# ACORDE
if(ALIROOT_USE_ACORDE)
  find_file(ALIROOT_ACORDE_FOUND NAMES AliACORDEReconstructor.h  PATHS $ENV{ALICE_ROOT}/ACORDE/)
  if(ALIROOT_ACORDE_FOUND)
    # includes
    set(ALIROOT_ACORDE_INCLUDE $ENV{ALICE_ROOT}/ACORDE/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_ACORDE_INCLUDE})
    # libs
    set(ALIROOT_ACORDE_LIB "-L${ALIROOT_LIB_DIR} -lACORDEbase -lACORDErec -lACORDEsim ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_ACORDE_LIB})
  elseif(ALIROOT_ACORDE_FOUND)
    message(STATUS "ALIROOT_ACORDE not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliACORDEReconstructor.h in $ENV{ALICE_ROOT}/ACORDE/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_ACORDE_FOUND)
endif(ALIROOT_USE_ACORDE)


# VZERO
if(ALIROOT_USE_VZERO)
  find_file(ALIROOT_VZERO_FOUND NAMES AliVZEROReconstructor.h  PATHS $ENV{ALICE_ROOT}/VZERO/)
  if(ALIROOT_VZERO_FOUND)
    # includes
    set(ALIROOT_VZERO_INCLUDE $ENV{ALICE_ROOT}/VZERO/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_VZERO_INCLUDE})
    # libs
    set(ALIROOT_VZERO_LIB "-L${ALIROOT_LIB_DIR} -lVZERObase -lVZEROrec -lVZEROsim ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_VZERO_LIB})
  elseif(ALIROOT_VZERO_FOUND)
    message(STATUS "ALIROOT_VZERO not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliVZEROReconstructor.h in $ENV{ALICE_ROOT}/VZERO/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_VZERO_FOUND)
endif(ALIROOT_USE_VZERO)


# EMCAL
if(ALIROOT_USE_EMCAL)
  find_file(ALIROOT_EMCAL_FOUND NAMES AliEMCALReconstructor.h  PATHS $ENV{ALICE_ROOT}/EMCAL/)
  if(ALIROOT_EMCAL_FOUND)
    # includes
    set(ALIROOT_EMCAL_INCLUDE $ENV{ALICE_ROOT}/EMCAL/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_EMCAL_INCLUDE})
    # libs
    set(ALIROOT_EMCAL_LIB "-L${ALIROOT_LIB_DIR} -lEMCALraw -lEMCALUtils -lEMCALbase -lEMCALsim -lEMCALrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_EMCAL_LIB})
  elseif(ALIROOT_EMCAL_FOUND)
    message(STATUS "ALIROOT_EMCAL not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliEMCALReconstructor.h in $ENV{ALICE_ROOT}/EMCAL/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_EMCAL_FOUND)
endif(ALIROOT_USE_EMCAL)


# TPC
if(ALIROOT_USE_TPC)
  find_file(ALIROOT_TPC_FOUND NAMES AliTPCReconstructor.h  PATHS $ENV{ALICE_ROOT}/TPC/)
  if(ALIROOT_TPC_FOUND)
    # includes
    set(ALIROOT_TPC_INCLUDE $ENV{ALICE_ROOT}/TPC/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_TPC_INCLUDE})
    # libs
    set(ALIROOT_TPC_LIB "-L${ALIROOT_LIB_DIR} -lTPCbase -lTPCrec -lTPCsim -lTPCfast ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_TPC_LIB})
  elseif(ALIROOT_TPC_FOUND)
    message(STATUS "ALIROOT_TPC not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliTPCReconstructor.h in $ENV{ALICE_ROOT}/TPC/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_TPC_FOUND)
endif(ALIROOT_USE_TPC)


# ITS
if(ALIROOT_USE_ITS)
  find_file(ALIROOT_ITS_FOUND NAMES AliITSReconstructor.h  PATHS $ENV{ALICE_ROOT}/ITS/)
  if(ALIROOT_ITS_FOUND)
    # includes
    set(ALIROOT_ITS_INCLUDE $ENV{ALICE_ROOT}/ITS/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_ITS_INCLUDE})
    # libs
    set(ALIROOT_ITS_LIB "-L${ALIROOT_LIB_DIR} -lITSbase -lITSsim -lITSrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_ITS_LIB})
  elseif(ALIROOT_ITS_FOUND)
    message(STATUS "ALIROOT_ITS not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliITSReconstructor.h in $ENV{ALICE_ROOT}/ITS/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_ITS_FOUND)
endif(ALIROOT_USE_ITS)


# TRD
if(ALIROOT_USE_TRD)
  find_file(ALIROOT_TRD_FOUND NAMES AliTRDReconstructor.h  PATHS $ENV{ALICE_ROOT}/TRD/)
  if(ALIROOT_TRD_FOUND)
    # includes
    set(ALIROOT_TRD_INCLUDE $ENV{ALICE_ROOT}/TRD/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_TRD_INCLUDE})
    # libs
    set(ALIROOT_TRD_LIB "-L${ALIROOT_LIB_DIR} -lTRDbase -lTRDsim -lTRDrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_TRD_LIB})
  elseif(ALIROOT_TRD_FOUND)
    message(STATUS "ALIROOT_TRD not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliTRDReconstructor.h in $ENV{ALICE_ROOT}/TRD/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_TRD_FOUND)
endif(ALIROOT_USE_TRD)


# TOF
if(ALIROOT_USE_TOF)
  find_file(ALIROOT_TOF_FOUND NAMES AliTOFReconstructor.h  PATHS $ENV{ALICE_ROOT}/TOF/)
  if(ALIROOT_TOF_FOUND)
    # includes
    set(ALIROOT_TOF_INCLUDE $ENV{ALICE_ROOT}/TOF/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_TOF_INCLUDE})
    # libs
    set(ALIROOT_TOF_LIB "-L${ALIROOT_LIB_DIR} -lTOFbase -lTOFsim -lTOFrec ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_TOF_LIB})
  elseif(ALIROOT_TOF_FOUND)
    message(STATUS "ALIROOT_TOF not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliTOFReconstructor.h in $ENV{ALICE_ROOT}/TOF/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_TOF_FOUND)
endif(ALIROOT_USE_TOF)


# HLT
if(ALIROOT_USE_HLT)
  find_file(ALIROOT_HLT_FOUND NAMES AliHLTReconstructor.h  PATHS $ENV{ALICE_ROOT}/HLT/)
  if(ALIROOT_HLT_FOUND)
    # includes
    set(ALIROOT_HLT_INCLUDE $ENV{ALICE_ROOT}/HLT/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_HLT_INCLUDE})
    # libs
    set(ALIROOT_HLT_LIB "-L${ALIROOT_LIB_DIR} -lHLTbase -lHLTinterface -lHLTrec -lHLTsim ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_HLT_LIB})
  elseif(ALIROOT_HLT_FOUND)
    message(STATUS "ALIROOT_HLT not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliHLTReconstructor.h in $ENV{ALICE_ROOT}/HLT/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_HLT_FOUND)
endif(ALIROOT_USE_HLT)


# MFT
if(ALIROOT_USE_MFT)
  find_file(ALIROOT_MFT_FOUND NAMES AliMFTReconstructor.h  PATHS $ENV{ALICE_ROOT}/MFT/)
  if(ALIROOT_MFT_FOUND)
    # includes
    set(ALIROOT_MFT_INCLUDE $ENV{ALICE_ROOT}/MFT/)
    set(ALIROOT_INCLUDE ${ALIROOT_INCLUDE} ${ALIROOT_MFT_INCLUDE})
    # libs
    set(ALIROOT_MFT_LIB "-L${ALIROOT_LIB_DIR} -lMFTbase -lMFTrec -lMFTsim ")
    set(ALIROOT_LIB ${ALIROOT_LIB} ${ALIROOT_MFT_LIB})
  elseif(ALIROOT_MFT_FOUND)
    message(STATUS "ALIROOT_MFT not found")
    if(ALIROOT_FIND_REQUIRED)
      message(FATAL_ERROR "could not find AliMFTReconstructor.h in $ENV{ALICE_ROOT}/MFT/")
    endif(ALIROOT_FIND_REQUIRED)
  endif(ALIROOT_MFT_FOUND)
endif(ALIROOT_USE_MFT)



# Final setting up of variables
set(ALIROOT_INCLUDES ${ALIROOT_INCLUDE})
set(ALIROOT_INCLUDE_DIRS ${ALIROOT_INCLUDE})
set(ALIROOT_LIBS ${ALIROOT_LIB})
set(ALIROOT_LIBRARIES ${ALIROOT_LIB})

#message(STATUS "FindALIROOT.cmake: ALIROOT_FOUND: ${ALIROOT_FOUND}")
#message(STATUS "FindALIROOT.cmake: ALIROOT_INCLUDES: ${ALIROOT_INCLUDES}")
#message(STATUS "FindALIROOT.cmake: ALIROOT_LIBS: ${ALIROOT_LIBS}")


