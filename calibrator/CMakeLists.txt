include_directories(SYSTEM ${ALIROOT_INCLUDES})
message(${ALIROOT_INCLUDES})
set(LIBS ${LIBS} ${ALIROOT_LIBRARIES})

include_directories(.)
add_library( libCalibrator Henrik2010.cxx CorrFcn.cxx calib.C )
