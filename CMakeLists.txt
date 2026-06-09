cmake_minimum_required(VERSION 3.5)
project(pairs_uav_state_estimators)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-Wall -Wextra)

if(COVERAGE)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} --coverage")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

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

find_package(catkin REQUIRED COMPONENTS ${CATKIN_DEPENDENCIES})
find_package(Eigen3 REQUIRED)
set(Eigen_INCLUDE_DIRS ${EIGEN3_INCLUDE_DIRS})
set(Eigen_LIBRARIES ${EIGEN3_LIBRARIES})

generate_dynamic_reconfigure_options(
  cfg/Correction.cfg
  cfg/LateralEstimator.cfg
  cfg/AltitudeEstimator.cfg
  cfg/HeadingEstimator.cfg
  )

set(LIBRARIES
  PairsUavStateEstimators_Common
  PairsUavStateEstimators_GpsGarmin
  PairsUavStateEstimators_GpsBaro
  PairsUavStateEstimators_Rtk
  PairsUavStateEstimators_RtkGarmin
  PairsUavStateEstimators_Passthrough
  PairsUavStateEstimators_GroundTruth
  PairsUavStateEstimators_Dummy
  PairsUavStateEstimators_GarminAgl
  )

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${LIBRARIES}
  CATKIN_DEPENDS ${CATKIN_DEPENDENCIES}
  DEPENDS Eigen
  )

include_directories(include ${CMAKE_SOURCE_DIR} ${catkin_INCLUDE_DIRS} ${Eigen_INCLUDE_DIRS})

add_library(PairsUavStateEstimators_Common
  src/estimators/state/state_generic.cpp
  src/estimators/altitude/alt_generic.cpp
  src/estimators/lateral/lat_generic.cpp
  src/estimators/heading/hdg_generic.cpp
  src/estimators/heading/hdg_passthrough.cpp
  )
add_dependencies(PairsUavStateEstimators_Common
  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_gencfg)
target_link_libraries(PairsUavStateEstimators_Common ${catkin_LIBRARIES} ${Eigen_LIBRARIES})

foreach(est GpsGarmin GpsBaro Rtk RtkGarmin Passthrough GroundTruth Dummy)
  string(TOLOWER ${est} est_lower)
  add_library(PairsUavStateEstimators_${est}
    src/estimators/state/${est_lower}.cpp)
  add_dependencies(PairsUavStateEstimators_${est}
    ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_gencfg)
  target_link_libraries(PairsUavStateEstimators_${est}
    PairsUavStateEstimators_Common ${catkin_LIBRARIES} ${Eigen_LIBRARIES})
endforeach()

add_library(PairsUavStateEstimators_GarminAgl src/estimators/agl/garmin_agl.cpp)
add_dependencies(PairsUavStateEstimators_GarminAgl
  ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS} ${PROJECT_NAME}_gencfg)
target_link_libraries(PairsUavStateEstimators_GarminAgl
  PairsUavStateEstimators_Common ${catkin_LIBRARIES} ${Eigen_LIBRARIES})

if(CATKIN_ENABLE_TESTING AND PAIRS_ENABLE_TESTING)
  message(WARNING "Testing enabled.")
  add_subdirectory(test)
endif()

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

install(FILES estimator_plugins.xml
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  )
