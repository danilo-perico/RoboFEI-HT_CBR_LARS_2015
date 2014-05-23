
find_package( Boost REQUIRED )

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
  ${RoboFEI-HT_SOURCE_DIR}/IMU/include/um6
  ${SOURCE_DIR}/usr/local/include/serial

)

include_directories(include)
add_library(comms SHARED src/comms.cpp)
install(TARGETS comms DESTINATION ${RoboFEI-HT_BINARY_DIR}/lib)
add_library(registers SHARED src/registers.cpp)
install(TARGETS registers DESTINATION ${RoboFEI-HT_BINARY_DIR}/lib)


#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

add_executable(imu src/imu.cpp)
add_dependencies(imu 
blackboard serial comms registers ${Boost_LIBRARIES})

target_link_libraries(imu 
blackboard serial comms registers rt pthread ${Boost_LIBRARIES})

install(TARGETS imu DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS control DESTINATION /usr/local/bin)



