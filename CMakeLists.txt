cmake_minimum_required( VERSION 2.8 )
project( cortex-ocean )

set( CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/config/cmake )

find_package( PkgConfig REQUIRED )
pkg_check_modules( Cortex REQUIRED cortex )

include_directories(
  ${Cortex_INCLUDE_DIRS}
  )

link_directories(
  ${Cortex_LIBRARY_DIRS}
  )

add_library( OceanOp
  SHARED
  ${CMAKE_SOURCE_DIR}/src/OceanOp.cpp
  )

add_library( CortexOceanBinding
  SHARED
  ${CMAKE_SOURCE_DIR}/src/CortexOceanBinding.cpp
  )

set_target_properties( CortexOceanBinding
  PROPERTIES
  OUTPUT_NAME "_CortexOcean"
  PREFIX ""
  SUFFIX ".so"
  
  )

target_link_libraries( CortexOceanBinding
  ${Cortex_LIBRARIES}
  OceanOp
  )
