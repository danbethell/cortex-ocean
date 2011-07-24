cmake_minimum_required( VERSION 2.8 )
project( cortex-ocean )
set( CORTEX_OCEAN_VERSION "1" )

find_package( PkgConfig REQUIRED )
pkg_check_modules( Cortex REQUIRED cortex )

include_directories(
  ${Cortex_INCLUDE_DIRS}
  )

link_directories(
  ${Cortex_LIBRARY_DIRS}
  )

add_library( CortexOceanBinding
  SHARED
  ${CMAKE_SOURCE_DIR}/src/OceanOp.cpp
  ${CMAKE_SOURCE_DIR}/src/CortexOceanBinding.cpp
  )

set_target_properties( CortexOceanBinding
  PROPERTIES
  OUTPUT_NAME "CortexOcean"
  PREFIX ""
  SUFFIX ".so"
  )

target_link_libraries( CortexOceanBinding
  ${Cortex_LIBRARIES}
  fftw3f
  )
    
set( INSTALL_PATH ${Cortex_PREFIX}/ops/cortexOcean/ocean )
install( TARGETS CortexOceanBinding
    DESTINATION ${INSTALL_PATH}
    )

function(CORTEX_OP_STUB stub_path stub_cmd)    
    install( CODE "file( WRITE ${stub_path} \"import os, sys\nsys.path.append( os.path.dirname( __file__ ) )\n\n${stub_cmd}\n\")" )
endfunction(CORTEX_OP_STUB)
    
cortex_op_stub( 
    "${INSTALL_PATH}/ocean-${CORTEX_OCEAN_VERSION}.py"
    "from CortexOcean import OceanOp as ocean"
    )
