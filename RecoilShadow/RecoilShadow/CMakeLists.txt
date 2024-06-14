#----------------------------------------------------------------------------

# Setup the project
cmake_minimum_required(VERSION 2.8.12 FATAL_ERROR)
project(RecoilShadow)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

#----------------------------------------------------------------------------

# Find g4, and consider whether or not the visualisation is enabled
option(WITH_GEANT4_UIVIS "Build example with Geant4 UI and Vis drivers" ON)

if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

#----------------------------------------------------------------------------

# Setup Geant4 include directories

include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------

# Find ROOT (required package)

option(WITH_ROOT "Build with support for ROOT" OFF)

if(WITH_ROOT)
  find_package(ROOT REQUIRED)
endif()

#----------------------------------------------------------------------------

# Locate sources and headers for this project

include_directories(${PROJECT_SOURCE_DIR}/include
                    ${PROJECT_SOURCE_DIR}/shared/include
                    ${Geant4_INCLUDE_DIR})

if(WITH_ROOT)
  include_directories(${ROOT_INCLUDE_DIRS})
endif()

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc
                  ${PROJECT_SOURCE_DIR}/shared/src/*.cc)

file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh
                  ${PROJECT_SOURCE_DIR}/shared/include/*.hh)

# set rpath for install executabile
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

#----------------------------------------------------------------------------

# Set up the executable

add_executable(RecoilShadow src/main.cc ${sources} ${headers})

target_link_libraries(RecoilShadow
                    ${Geant4_LIBRARIES}
                    ${ROOT_LIBRARIES})

#----------------------------------------------------------------------------

# Find these macros and copy them into the build folder (required destination)

set(MACROS
  gps_radioactive.mac
  init_vis.mac
  vis.mac
  runMacroBatch.mac
  runMacroVis.mac
  )

foreach(_file ${MACROS})
  configure_file(
    ${PROJECT_SOURCE_DIR}/${_file}
    ${PROJECT_BINARY_DIR}/${_file}
    COPYONLY
    )
endforeach()

#----------------------------------------------------------------------------

# Install the executable to CMAKE_INSTALL_PREFIX/bin

SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/bin")

install(TARGETS RecoilShadow DESTINATION bin)

#----------------------------------------------------------------------------