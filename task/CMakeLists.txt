include_directories(SYSTEM ${ALIROOT_INCLUDES})
message(${ALIROOT_INCLUDES})
set(LIBS ${LIBS} ${ALIROOT_LIBRARIES})

add_library( libTask EqEnCalibTask.cxx )
