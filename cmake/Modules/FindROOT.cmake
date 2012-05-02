# -*- mode: cmake -*-
#
# - Finds ROOT instalation
# This module sets up ROOT information
# We suppose root-config to be in the PATH. Otherwise we stop.
#
# Taken from AliRoot at some point, modified by
#    Henrik Qvigstad <henrik.qvigstad@cern.ch>
#
# If ROOT is found, the following is defined:
#  ROOT_FOUND - true if ROOT is found
#  ROOT_INCLUDES - The ROOT include directories (you may use the alias ROOT_INCLUDE_DIRS)
#  ROOT_LIBS - the ROOT library statements (you may use the alias ROOT_LIBRARIES)
#  ( ROOT_DEFINITION - not functional! At this time, this find macro overrides F77, CC, & CXX ! )


Find_program(ROOT_CONFIG root-config)

If (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")
  Set(ROOT_FOUND FALSE)
  If(ROOT_FIND_REQUIRED)
    message(FATAL_ERROR "Install Root and make sure it (root-config) is in PATH")
  Else(ROOT_FIND_REQUIRED)
    Message(WARNING "Install Root and make sure it (root-config) is in PATH")
  Endif(ROOT_FIND_REQUIRED)
  
Else (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")  
  
  Set(ROOT_FOUND TRUE)

  Execute_process(
    COMMAND root-config --prefix 
    OUTPUT_VARIABLE ROOTSYS 
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  Execute_process(
    COMMAND root-config --arch
    OUTPUT_VARIABLE ALICE_TARGET
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  Execute_process(
    COMMAND root-config --f77 
    OUTPUT_VARIABLE ROOT_f77 
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  If(APPLE)
    Execute_process(
      COMMAND which ${ROOT_f77}
      OUTPUT_VARIABLE ROOT_f77path
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ENV{F77} ${ROOT_f77path})
  Else(APPLE)
    Set(ENV{F77} ${ROOT_f77})
  Endif(APPLE)
  message(STATUS "findROOT.cmake: set \$ENV{F77} as $ENV{F77}")

  Execute_process(
    COMMAND root-config --cc
    OUTPUT_VARIABLE ROOT_cc 
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  If(APPLE)
    Execute_process(
      COMMAND which ${ROOT_cc}
      OUTPUT_VARIABLE ROOT_ccpath
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ENV{CC} ${ROOT_ccpath})
  Else(APPLE)
    Set(ENV{CC} ${ROOT_cc})
  Endif(APPLE)
  message(STATUS "findROOT.cmake: set \$ENV{CC} as $ENV{CC}")

  Execute_process(
    COMMAND root-config --cxx
    OUTPUT_VARIABLE ROOT_cxx
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  If(APPLE)
    Execute_process(
      COMMAND which ${ROOT_cxx}
      OUTPUT_VARIABLE ROOT_cxxpath
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ENV{CXX} ${ROOT_cxxpath})
  Else(APPLE)
    Set(ENV{CXX} ${ROOT_cxx})
  Endif(APPLE)
  message(STATUS "FindROOT.cmake: set \$ENV{CXX} as $ENV{CXX}")

  Execute_process(
    COMMAND root-config --version 
    OUTPUT_VARIABLE ROOT_VERSION
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  Execute_process(
    COMMAND root-config --incdir
    OUTPUT_VARIABLE ROOT_INCLUDE_DIRS
    OUTPUT_STRIP_TRAILING_WHITESPACE)
  Set(ROOT_INCLUDES ${ROOT_INCLUDE_DIRS})

  Execute_process(
    COMMAND root-config --glibs
    OUTPUT_VARIABLE ROOT_LIBRARIES
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  Find_program(ROOTCINT rootcint)
  If(NOT ROOTCINT)
    Message(WARNING "Found ROOT but not rootcint, your ROOT installation  may be corrupted")
  EndIf(NOT ROOTCINT)

  Set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lThread -lMinuit -lHtml -lVMC -lEG -lGeom -lTreePlayer -lXMLIO -lProof)
  Set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lProofPlayer -lMLP -lSpectrum -lEve -lRGL -lGed -lXMLParser -lPhysics)
  #set(ROOT_LIBRARIES ${ROOT_LIBRARIES} -lEGPythia6) # causes erros
  Set(ROOT_LIBS ${ROOT_LIBRARIES})
  Set(ROOT_LIBRARY_DIR ${ROOTSYS}/lib)
  

  # Make variables changeble to the advanced user
  #Mark_as_advanced(ROOT_LIBRARY_DIR ROOT_INCLUDE_DIR ROOT_DEFINITIONS)

  Set(LD_LIBRARY_PATH ${LD_LIBRARY_PATH} ${ROOT_LIBRARY_DIR})

  Message(STATUS "findROOT.cmake: Found Root ${ROOT_VERSION} in ${ROOTSYS}/bin/root")



 # static check of version  
# we need at least version 5.00/00
#   If (NOT ROOT_MIN_VERSION)
#     Set(ROOT_MIN_VERSION "5.00/00")
#   Endif (NOT ROOT_MIN_VERSION)
#    
#   # now parse the parts of the user given version string into variables
#   String(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+" "\\1" req_root_major_vers "${ROOT_MIN_VERSION}")
#   String(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" req_root_minor_vers "${ROOT_MIN_VERSION}")
#   String(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+)" "\\1" req_root_patch_vers "${ROOT_MIN_VERSION}")
#    
#   # and now the version string given by qmake
#   String(REGEX REPLACE "^([0-9]+)\\.[0-9][0-9]+\\/[0-9][0-9]+.*" "\\1" found_root_major_vers "${ROOT_VERSION}")
#   String(REGEX REPLACE "^[0-9]+\\.([0-9][0-9])+\\/[0-9][0-9]+.*" "\\1" found_root_minor_vers "${ROOT_VERSION}")
#   String(REGEX REPLACE "^[0-9]+\\.[0-9][0-9]+\\/([0-9][0-9]+).*" "\\1" found_root_patch_vers "${ROOT_VERSION}")
# 
#   If (found_root_major_vers LESS 5)
#     Message(FATAL_ERROR "Invalid ROOT version \"${ROOT_VERSION}\", at least major version 4 is required, e.g. \"5.00/00\"")
#   Endif(found_root_major_vers LESS 5)
# 
#   # compute an overall version number which can be compared at once
#   Math(EXPR req_vers "${req_root_major_vers}*10000 + ${req_root_minor_vers}*100 + ${req_root_patch_vers}")
#   Math(EXPR found_vers "${found_root_major_vers}*10000 + ${found_root_minor_vers}*100 + ${found_root_patch_vers}")
#    
#   If (found_vers LESS req_vers)
#     Set(ROOT_FOUND FALSE)
#     Set(ROOT_INSTALLED_VERSION_TOO_OLD TRUE)
#   Else (found_vers LESS req_vers)
#     Set(ROOT_FOUND TRUE)
#   Endif (found_vers LESS req_vers)
# 
Endif (${ROOT_CONFIG} MATCHES "ROOT_CONFIG-NOTFOUND")  


#####################################################################################
# Defunct: ???

# Macro(ROOT_GENERATE_DICTIONARY INFILES LINKDEF_FILE OUTFILE INCLUDE_DIRS_IN)
#  
#   Set(_special_settings "${ARGV4}")
#   Set(INCLUDE_DIRS)
#   Set(infiles_nopath)
# 
#   Foreach (_current_FILE ${INCLUDE_DIRS_IN})
#     Set(INCLUDE_DIRS ${INCLUDE_DIRS} -I${_current_FILE})   
#   Endforeach (_current_FILE ${INCLUDE_DIRS_IN})
#  
#   String(REGEX REPLACE "^(.*)\\.(.*)$" "\\1.h" bla "${OUTFILE}")
#   Set(OUTFILES ${OUTFILE} ${bla})
# 
#   Foreach (_current_FILE ${INFILES})
#     Get_filename_component(name_wo_path ${_current_FILE} NAME)
#     Set(infiles_nopath ${infiles_nopath} ${name_wo_path})   
#   Endforeach (_current_FILE ${INFILES})
# 
#   Get_property(_defs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY COMPILE_DEFINITIONS)
#   Set(_ddefs)
#   Foreach (_def ${_defs})
#     Set(_ddefs "${_ddefs} -D${_def}")
#   Endforeach (_def ${_defs})
#   Separate_arguments(_ddefs)
# 
#   Add_custom_command(OUTPUT ${OUTFILES}
#      COMMAND DYLD_LIBRARY_PATH=$ENV{DYLD_LIBRARY_PATH}:${ROOT_LIBRARY_DIR} ${ROOTCINT}
#      ARGS -f ${OUTFILE} -c -DHAVE_CONFIG_H ${_ddefs} ${_special_settings} ${INCLUDE_DIRS} ${infiles_nopath} ${LINKDEF_FILE} 
#      DEPENDS ${INFILES} ${LINKDEF_FILE})
# 
# Endmacro(ROOT_GENERATE_DICTIONARY)
