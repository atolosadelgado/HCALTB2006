#----------------------------------------------------------------------------
# Setup the project
cmake_minimum_required(VERSION 3.16...3.27)
project(G01)
set(CMAKE_BUILD_TYPE Release)
#----------------------------------------------------------------------------
# Find Geant4 package, activating all available Vis drivers by default
# You can set WITH_GEANT4_VIS to OFF via the command line or ccmake/cmake-gui
# to build a batch mode only executable
#
option(WITH_GEANT4_VIS "Build example with Geant4 Vis drivers" ON)
if(WITH_GEANT4_VIS)
  find_package(Geant4 REQUIRED gdml vis_all)
else()
  find_package(Geant4 REQUIRED gdml)
endif()

#----------------------------------------------------------------------------
# Setup Geant4 include directories and compile definitions
#
include(${Geant4_USE_FILE})

#----------------------------------------------------------------------------
# Locate sources and headers for this project
#
include_directories(${PROJECT_SOURCE_DIR}/include
                    ${Geant4_INCLUDE_DIR}
                    )

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/YourActionInitialization.cc
                  ${PROJECT_SOURCE_DIR}/src/YourRunAction.cc
                  ${PROJECT_SOURCE_DIR}/src/YourEventAction.cc
                  ${PROJECT_SOURCE_DIR}/src/YourSteppingAction.cc
                  ${PROJECT_SOURCE_DIR}/src/YourDetectorConstructor.cc

                  )
file(GLOB headers   ${PROJECT_SOURCE_DIR}/include/YourDetectorConstructor.hh
                    ${PROJECT_SOURCE_DIR}/include/YourActionInitialization.hh
                    ${PROJECT_SOURCE_DIR}/include/YourRunAction.hh
                    ${PROJECT_SOURCE_DIR}/include/YourEventAction.hh
                    ${PROJECT_SOURCE_DIR}/include/YourSteppingAction.hh
                    ${PROJECT_SOURCE_DIR}/include/YourPrimaryGenerator.hh
                    )

#----------------------------------------------------------------------------
# include ROOT dependency for histograming
#
# Find ROOT package
find_package(ROOT REQUIRED COMPONENTS Hist)
# Include ROOT headers
include_directories(${ROOT_INCLUDE_DIRS})
# Enable ROOT-specific flags (e.g. RTTI, C++17 if needed)
include(${ROOT_USE_FILE})

#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(gdml_sim gdml_sim.cc ${sources} ${headers})
target_link_libraries(gdml_sim ${Geant4_LIBRARIES}  ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gdml_sim DESTINATION bin)

enable_testing()

add_test(
    NAME RunGdmlSimMatScan
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            ${CMAKE_SOURCE_DIR}/RunD121_HGCal.gdml
            original_cuts
            matscan
            FTFP_BERT
            ${CMAKE_SOURCE_DIR}/gun_matscan.mac
)

add_test(
    NAME RunGdmlSimOriginalCuts
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            ${CMAKE_SOURCE_DIR}/RunD121_HGCal.gdml
            original_cuts
            profile
            FTFP_BERT
            ${CMAKE_SOURCE_DIR}/gun.mac
)

add_test(
    NAME RunGdmlSimNewCuts
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            ${CMAKE_SOURCE_DIR}/RunD121_HGCal.gdml
            new_cuts
            profile
            FTFP_BERT
            ${CMAKE_SOURCE_DIR}/gun.mac
)
set_tests_properties(RunGdmlSimNewCuts PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
