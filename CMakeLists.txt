cmake_minimum_required(VERSION 3.5)
project(pairs_uav_state_estimators)

set(CATKIN_DEPENDENCIES
  cmake_modules
  dynamic_reconfigure
  geometry_msgs
  message_generation
  message_runtime
  pairs_lib
  pairs_msgs
  pairs_uav_managers
  nav_msgs
  nodelet
  roscpp
  roslint
  sensor_msgs
  std_msgs
  tf2_geometry_msgs
  pairs_uav_testing
  )

find_package(catkin REQUIRED COMPONENTS
  ${CATKIN_DEPENDENCIES}
  )

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall)
add_definitions(-Wextra)

if(COVERAGE)
  message(WARNING "building with --coverage, the performance might be limited")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

# include Eigen3
find_package(Eigen3 REQUIRED)
set(Eigen_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIRS})
set(Eigen_LIBRARIES ${Eigen_LIBRARIES})

set(LIBRARIES
  PairsUavStateEstimators_Common
  PairsUavStateEstimators_GpsGarmin
  PairsUavStateEstimators_GpsBaro
  PairsUavStateEstimators_Rtk
  PairsUavStateEstimators_RtkGarmin
  PairsUavStateEstimators_Passthrough
  PairsUavStateEstimators_F9P
  PairsUavStateEstimators_GroundTruth
  PairsUavStateEstimators_Dummy
  PairsUavStateEstimators_GarminAgl
  )

# clang-tidy linting
# find_program(CLANG_TIDY_COMMAND NAMES clang-tidy)
# set(CMAKE_CXX_CLANG_TIDY "clang-tidy;-header-filter='${CMAKE_SOURCE_DIR}/*'")
# set(CLANG_TIDY_DEFINITIONS "--use-color")

generate_dynamic_reconfigure_options(
  cfg/Correction.cfg
  cfg/LateralEstimator.cfg
  cfg/AltitudeEstimator.cfg
  cfg/HeadingEstimator.cfg
  )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${LIBRARIES}
  CATKIN_DEPENDS ${CATKIN_DEPENDENCIES}
  DEPENDS Eigen
  )

include_directories(
  include
  ${CMAKE_SOURCE_DIR}
  ${catkin_INCLUDE_DIRS}
  ${Eigen_INCLUDE_DIRS}
  ${dynamic_reconfigure_PACKAGE_PATH}/cmake/cfgbuild.cmake
  )

set(ROSLINT_CPP_OPTS "--filter=-whitespace/comments,-whitespace/braces,-legal/copyright,-readability/multiline_comment,-whitespace/blank_line,-whitespace/line_length,-readability/todo,-build/include_what_you_use,-runtime/references,-build/include")
roslint_cpp()

add_library(PairsUavStateEstimators_Common
  src/estimators/state/state_generic.cpp
  src/estimators/altitude/alt_generic.cpp
  src/estimators/lateral/lat_generic.cpp
  src/estimators/heading/hdg_generic.cpp
  src/estimators/heading/hdg_passthrough.cpp
  )

## Specify libraries to link a library or executable target against
target_link_libraries(PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

add_dependencies(PairsUavStateEstimators_Common
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_GpsGarmin
  src/estimators/state/gps_garmin.cpp
  )

## Specify libraries to link a library or executable target against
target_link_libraries(PairsUavStateEstimators_GpsGarmin
  PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_GpsGarmin
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_GpsBaro
  src/estimators/state/gps_baro.cpp
  )

## Specify libraries to link a library or executable target against
target_link_libraries(PairsUavStateEstimators_GpsBaro
  PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_GpsBaro
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_Rtk
  src/estimators/state/rtk.cpp
  )

target_link_libraries(PairsUavStateEstimators_Rtk
  PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_Rtk
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_RtkGarmin
  src/estimators/state/rtk_garmin.cpp
  )

target_link_libraries(PairsUavStateEstimators_RtkGarmin
  PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_RtkGarmin
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_Passthrough
  src/estimators/state/passthrough.cpp
  )

target_link_libraries(PairsUavStateEstimators_Passthrough
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_Passthrough
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_F9P
  src/estimators/state/f9p.cpp
  )

target_link_libraries(PairsUavStateEstimators_F9P
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_F9P
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_GroundTruth
  src/estimators/state/ground_truth.cpp
  )

target_link_libraries(PairsUavStateEstimators_GroundTruth
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_GroundTruth
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_Dummy
  src/estimators/state/dummy.cpp
  )

target_link_libraries(PairsUavStateEstimators_Dummy
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_Dummy
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

add_library(PairsUavStateEstimators_GarminAgl
  src/estimators/agl/garmin_agl.cpp
  )

target_link_libraries(PairsUavStateEstimators_GarminAgl
  PairsUavStateEstimators_Common
  ${catkin_LIBRARIES}
  ${Eigen_LIBRARIES}
  )

## Add configure headers for dynamic reconfigure
add_dependencies(PairsUavStateEstimators_GarminAgl
  ${${PROJECT_NAME}_EXPORTED_TARGETS}
  ${catkin_EXPORTED_TARGETS}
  ${PROJECT_NAME}_gencfg
  )

## --------------------------------------------------------------
## |                           Testing                          |
## --------------------------------------------------------------

if(CATKIN_ENABLE_TESTING AND PAIRS_ENABLE_TESTING)

  message(WARNING "Testing enabled.")

  add_subdirectory(test)

endif()

## --------------------------------------------------------------
## |                           Install                          |
## --------------------------------------------------------------

install(TARGETS ${LIBRARIES}
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
  )

install(DIRECTORY config
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )

install(DIRECTORY include/pairs_uav_state_estimators/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  )

install(DIRECTORY scripts/
  USE_SOURCE_PERMISSIONS
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
  )

install(DIRECTORY scripts
  USE_SOURCE_PERMISSIONS
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )

install(FILES estimator_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )
