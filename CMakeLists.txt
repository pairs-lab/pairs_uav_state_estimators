cmake_minimum_required(VERSION 3.28)
project(pairs_uav_state_estimators)

# set the correct standards
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 1 == use rclcpp::Timer implementation
# 0 == use PAIRS's thread timer implementation
set(USE_ROS_TIMER 0)

if(${USE_ROS_TIMER})
  MESSAGE(WARNING "[pairs_uav_state_estimators]: Compiling with ROS Timers. This can cause high CPU load in runtime.")
endif()

# set the compile options to show code warnings
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra)
endif()

if(ENABLE_COVERAGE)
  message(WARNING "building for coverage, the performance might be limited")

  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage -fprofile-arcs")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs")

  add_definitions("-O1")
  add_definitions("-g")
endif()

set(DEPENDENCIES
  rclcpp
  rclcpp_components
  pairs_msgs
  pairs_lib
  pluginlib
  geometry_msgs
  nav_msgs
  std_msgs
  sensor_msgs
  pairs_uav_managers
  tf2_geometry_msgs
  Eigen3
)

foreach(DEP IN LISTS DEPENDENCIES)
  find_package(${DEP} REQUIRED)
endforeach()

set(LIBRARIES
  PairsUavStateEstimators_Common
  PairsUavStateEstimators_GpsBaro
  PairsUavStateEstimators_GpsGarmin
  PairsUavStateEstimators_GarminAgl
  PairsUavStateEstimators_Passthrough
  PairsUavStateEstimators_F9P
  PairsUavStateEstimators_Rtk
  PairsUavStateEstimators_RtkGarmin
  PairsUavStateEstimators_GroundTruth
  PairsUavStateEstimators_Dummy
)

# Common estimators

add_library(PairsUavStateEstimators_Common SHARED)
add_library(pairs_uav_state_estimators::common ALIAS PairsUavStateEstimators_Common)

target_sources(PairsUavStateEstimators_Common
  PRIVATE
    src/estimators/state/state_generic.cpp
    src/estimators/altitude/alt_generic.cpp
    src/estimators/lateral/lat_generic.cpp
    src/estimators/heading/hdg_generic.cpp
    src/estimators/heading/hdg_passthrough.cpp
  PUBLIC FILE_SET HEADERS BASE_DIRS "include"
  FILES
    # include/pairs_uav_state_estimators
    # include/pairs_uav_state_estimators/estimators
    "include/pairs_uav_state_estimators/estimators/correction.h"
    "include/pairs_uav_state_estimators/estimators/partial_estimator.h"
    # include/pairs_uav_state_estimators/estimators/agl
    "include/pairs_uav_state_estimators/estimators/agl/garmin_agl.h"
    # include/pairs_uav_state_estimators/estimators/altitude
    "include/pairs_uav_state_estimators/estimators/altitude/alt_generic.h"
    "include/pairs_uav_state_estimators/estimators/altitude/altitude_estimator.h"
    # include/pairs_uav_state_estimators/estimators/heading
    "include/pairs_uav_state_estimators/estimators/heading/hdg_generic.h"
    "include/pairs_uav_state_estimators/estimators/heading/hdg_passthrough.h"
    "include/pairs_uav_state_estimators/estimators/heading/heading_estimator.h"
    # include/pairs_uav_state_estimators/estimators/lateral
    "include/pairs_uav_state_estimators/estimators/lateral/lat_generic.h"
    "include/pairs_uav_state_estimators/estimators/lateral/lateral_estimator.h"
    # include/pairs_uav_state_estimators/estimators/state
    "include/pairs_uav_state_estimators/estimators/state/dummy.h"
    "include/pairs_uav_state_estimators/estimators/state/f9p.h"
    "include/pairs_uav_state_estimators/estimators/state/ground_truth.h"
    "include/pairs_uav_state_estimators/estimators/state/passthrough.h"
    "include/pairs_uav_state_estimators/estimators/state/state_generic.h"
    # include/pairs_uav_state_estimators/processors
    "include/pairs_uav_state_estimators/processors/proc_excessive_tilt.h"
    "include/pairs_uav_state_estimators/processors/proc_mag_declination.h"
    "include/pairs_uav_state_estimators/processors/proc_median_filter.h"
    "include/pairs_uav_state_estimators/processors/proc_saturate.h"
    "include/pairs_uav_state_estimators/processors/proc_tf_to_world.h"
    "include/pairs_uav_state_estimators/processors/processor.h"
    # include/pairs_uav_state_estimators/processors/mag_declination
    "include/pairs_uav_state_estimators/processors/mag_declination/geo_mag_declination.h"
    "include/pairs_uav_state_estimators/processors/mag_declination/geo_magnetic_tables.hpp"
)

target_link_libraries(PairsUavStateEstimators_Common
  PUBLIC
    rclcpp::rclcpp
    pairs_lib::pairs_lib
    pluginlib::pluginlib
    pairs_uav_managers::pairs_uav_managers
    Eigen3::Eigen
    ${pairs_msgs_TARGETS}
    ${geometry_msgs_TARGETS}
    ${nav_msgs_TARGETS}
    ${std_msgs_TARGETS}
    ${sensor_msgs_TARGETS}
    ${tf2_geometry_msgs_TARGETS}
)

target_compile_definitions(PairsUavStateEstimators_Common PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# GPS Baro

add_library(PairsUavStateEstimators_GpsBaro SHARED
  src/estimators/state/gps_baro.cpp
)

target_link_libraries(PairsUavStateEstimators_GpsBaro
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_GpsBaro PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# GPS Garmin

add_library(PairsUavStateEstimators_GpsGarmin SHARED
  src/estimators/state/gps_garmin.cpp
)

target_link_libraries(PairsUavStateEstimators_GpsGarmin
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_GpsGarmin PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Garmin AGL

add_library(PairsUavStateEstimators_GarminAgl SHARED
  src/estimators/agl/garmin_agl.cpp
)

target_link_libraries(PairsUavStateEstimators_GarminAgl
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_GarminAgl PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Passthrough

add_library(PairsUavStateEstimators_Passthrough SHARED
  src/estimators/state/passthrough.cpp
)

target_link_libraries(PairsUavStateEstimators_Passthrough
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_Passthrough PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# F9P

add_library(PairsUavStateEstimators_F9P SHARED
  src/estimators/state/f9p.cpp
)

target_link_libraries(PairsUavStateEstimators_F9P
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_F9P PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Rtk

add_library(PairsUavStateEstimators_Rtk SHARED
  src/estimators/state/rtk.cpp
)

target_link_libraries(PairsUavStateEstimators_Rtk
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_Rtk PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Rtk Garmin

add_library(PairsUavStateEstimators_RtkGarmin SHARED
  src/estimators/state/rtk_garmin.cpp
)

target_link_libraries(PairsUavStateEstimators_RtkGarmin
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_RtkGarmin PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Ground truth

add_library(PairsUavStateEstimators_GroundTruth SHARED
  src/estimators/state/ground_truth.cpp
)

target_link_libraries(PairsUavStateEstimators_GroundTruth
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_GroundTruth PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Dummy

add_library(PairsUavStateEstimators_Dummy SHARED
  src/estimators/state/dummy.cpp
)

target_link_libraries(PairsUavStateEstimators_Dummy
  PUBLIC
    pairs_uav_state_estimators::common
)

target_compile_definitions(PairsUavStateEstimators_Dummy PRIVATE USE_ROS_TIMER=${USE_ROS_TIMER})

# Interface

add_library(PairsUavStateEstimators_PairsUavStateEstimators INTERFACE)
add_library(pairs_uav_state_estimators::pairs_uav_state_estimators ALIAS
  PairsUavStateEstimators_PairsUavStateEstimators
)
set_target_properties(PairsUavStateEstimators_PairsUavStateEstimators
  PROPERTIES
    EXPORT_NAME pairs_uav_state_estimators
    OUTPUT_NAME pairs_uav_state_estimators
)

target_link_libraries(PairsUavStateEstimators_PairsUavStateEstimators
  INTERFACE
    ${LIBRARIES}
)

## --------------------------------------------------------------
## |                       export plugins                       |
## --------------------------------------------------------------

# <package of the base class>, <relative path to the xml>
pluginlib_export_plugin_description_file(pairs_uav_managers plugins.xml)

## --------------------------------------------------------------
## |                           testing                          |
## --------------------------------------------------------------

if(ENABLE_TESTS)

  add_subdirectory(test)

  if(NOT DEFINED ENV{IN_NIX_SHELL})
    find_package(ament_cmake_clang_format REQUIRED)
    ament_clang_format(CONFIG_FILE "${PROJECT_SOURCE_DIR}/.clang-format")
    find_package(ament_cmake_xmllint REQUIRED)
    ament_xmllint()
  endif()

endif()

## --------------------------------------------------------------
## |                           install                          |
## --------------------------------------------------------------

install(
  TARGETS
    PairsUavStateEstimators_PairsUavStateEstimators
    ${LIBRARIES}
  EXPORT export_${PROJECT_NAME}
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin
  FILE_SET HEADERS DESTINATION "include/${PROJECT_NAME}"
)

install(
  DIRECTORY config
  DESTINATION share/${PROJECT_NAME}
)

install(
  DIRECTORY scripts
  USE_SOURCE_PERMISSIONS
  DESTINATION lib/${PROJECT_NAME}
)

ament_export_targets(
  export_${PROJECT_NAME} HAS_LIBRARY_TARGET
)

ament_export_include_directories(
  "include/${PROJECT_NAME}"
)

ament_export_libraries(
  ${LIBRARIES}
)

ament_export_dependencies(
  ${DEPENDENCIES}
)

ament_package()
