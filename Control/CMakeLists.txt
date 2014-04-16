include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
)

link_directories(
  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

add_executable(control src/control.cpp)
add_dependencies(control blackboard)

target_link_libraries(control
  blackboard
 )

install(TARGETS control DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS control DESTINATION /usr/local/bin)


