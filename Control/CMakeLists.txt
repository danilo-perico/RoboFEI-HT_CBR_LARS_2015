include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
  ${RoboFEI-HT_SOURCE_DIR}/Dynamixel/include
)

link_directories(
  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
  ${RoboFEI-HT_BINARY_DIR}/Dynamixel}
)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

add_executable(control src/control.cpp)
add_dependencies(control dynamixel dxl_hal blackboard)

target_link_libraries(control
dynamixel  
dxl_hal 
blackboard
 )

install(TARGETS control DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS control DESTINATION /usr/local/bin)


