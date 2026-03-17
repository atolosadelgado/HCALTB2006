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

file(GLOB sources   ${PROJECT_SOURCE_DIR}/src/YourDetectorConstructor.cc
                    ${PROJECT_SOURCE_DIR}/src/PhysicsList.cc
                    ${PROJECT_SOURCE_DIR}/src/CMSHadronPhysicsFTFP_BERT.cc
                    ${PROJECT_SOURCE_DIR}/src/YourActionInitialization.cc
                    ${PROJECT_SOURCE_DIR}/src/YourPrimaryGenerator.cc
                    ${PROJECT_SOURCE_DIR}/src/YourRunAction.cc
                    ${PROJECT_SOURCE_DIR}/src/YourEventAction.cc
                    ${PROJECT_SOURCE_DIR}/src/SteppingAction.cc
                    ${PROJECT_SOURCE_DIR}/src/YourInputArgsParser.cc
                    ${PROJECT_SOURCE_DIR}/src/ECalSD.cc
                    ${PROJECT_SOURCE_DIR}/src/HCalSD.cc
                    ${PROJECT_SOURCE_DIR}/src/LongitudinalProfileBuilder.cc
                    ${PROJECT_SOURCE_DIR}/src/TrackingAction.cc
                    )

file(GLOB headers   ${PROJECT_SOURCE_DIR}/include/YourDetectorConstructor.hh
                    ${PROJECT_SOURCE_DIR}/include/PhysicsList.hh
                    ${PROJECT_SOURCE_DIR}/include/CMSHadronPhysicsFTFP_BERT.hh
                    ${PROJECT_SOURCE_DIR}/include/YourActionInitialization.hh
                    ${PROJECT_SOURCE_DIR}/include/YourPrimaryGenerator.hh
                    ${PROJECT_SOURCE_DIR}/include/YourRunAction.hh
                    ${PROJECT_SOURCE_DIR}/include/YourEventAction.hh
                    ${PROJECT_SOURCE_DIR}/include/SteppingAction.hh
                    ${PROJECT_SOURCE_DIR}/include/YourInputArgsParser.hh
                    ${PROJECT_SOURCE_DIR}/include/ECalSD.h
                    ${PROJECT_SOURCE_DIR}/include/HCalSD.h
                    ${PROJECT_SOURCE_DIR}/include/SDutils.hh
                    ${PROJECT_SOURCE_DIR}/include/WelfordAccumulator.hh
                    ${PROJECT_SOURCE_DIR}/include/LongitudinalProfileBuilder.hh
                    ${PROJECT_SOURCE_DIR}/include/TrackingAction.hh
                    )

#----------------------------------------------------------------------------
# include ROOT dependency for histograming
#
# Find ROOT package
find_package(ROOT)
if(ROOT_FOUND)
    add_definitions(-DHAVE_ROOT)
    # # Include ROOT headers
    include_directories(${ROOT_INCLUDE_DIRS})
    # Enable ROOT-specific flags (e.g. RTTI, C++17 if needed)
    include(${ROOT_USE_FILE})
endif()


#----------------------------------------------------------------------------
# Add the executable, and link it to the Geant4 libraries

add_executable(gdml_sim gdml_sim.cc ${sources} ${headers})
target_link_libraries(gdml_sim ${Geant4_LIBRARIES}  ${ROOT_LIBRARIES})


#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gdml_sim DESTINATION bin)

enable_testing()
#----------------------
# Test using alias within the macro file

SET( test_name "test_RunGPS_templateMac_Seq" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl FTFP_BERT
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname pi-
            -penergy 50
            -nevents 5
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error" TIMEOUT 600)

#----------------------------------------------------------------------------
# Install the executable to 'bin' directory under CMAKE_INSTALL_PREFIX
#
install(TARGETS gdml_sim DESTINATION bin)

enable_testing()
#----------------------
# Test material scan when replacing ECAL materials with air
#SET( test_name "test_RunGPS_matscan_airECAL" )

#add_test(
    #NAME ${test_name}
    #COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            #-g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            #-pl FTFP_BERT
            #-m ${CMAKE_SOURCE_DIR}/g4matscan.mac
            #-airECAL
#)

#set_tests_properties(${test_name} PROPERTIES
    #WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
#)
#SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error" TIMEOUT 600)

#----------------------
# Run ECAL calibration

SET( test_name "run_ECAL_calibration_CMS_MT" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl CMS
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname e-
            -penergy 50
            -nevents 20
            -nthreads 2
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")

#----------------------
# Run HCAL calibration

SET( test_name "run_HCAL_calibration_CMS_MT" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl CMS
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname e-
            -penergy 50
            -nevents 20
            -nthreads 2
            -airECAL
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")

#----------------------
# Run 50 GeV kaon with CMS physics (MT runmanager)

SET( test_name "run_50GeV_kaon_CMS_MT" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl FTFP_BERT
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname kaon-
            -penergy 50
            -nevents 20
            -nthreads 2
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")

#----------------------
# Run 50 GeV pion with CMS physics (MT runmanager)

SET( test_name "run_50GeV_antiproton_CMS_MT" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl CMS
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname anti_proton
            -penergy 50
            -nevents 20
            -nthreads 2
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")


#----------------------
# Run 50 GeV pion with CMS physics (sequential runmanager)

SET( test_name "run_50GeV_pion_CMS_Seq" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl CMS
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname pi-
            -penergy 50
            -nevents 20
            -nthreads 1
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")


#----------------------
# Run 50 GeV pion with CMS physics (MT runmanager)

SET( test_name "runMT_50GeV_pion_CMS_MT" )
add_test(
    NAME ${test_name}
    COMMAND ${CMAKE_BINARY_DIR}/gdml_sim
            -g ${CMAKE_SOURCE_DIR}/data/geometry/geometryTB2006H2.gdml
            -pl CMS
            -m ${CMAKE_SOURCE_DIR}/gps_singlerun.mac
            -pname pi-
            -penergy 50
            -nevents 20
            -nthreads 2
)

set_tests_properties(${test_name} PROPERTIES
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
SET_TESTS_PROPERTIES( ${test_name} PROPERTIES FAIL_REGULAR_EXPRESSION  " Exception; EXCEPTION;ERROR;Error")
