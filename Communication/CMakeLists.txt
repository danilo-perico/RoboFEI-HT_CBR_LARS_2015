find_package(OpenCV REQUIRED)

include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
  ${OpenCV_INCLUDE_DIRS}
)

link_directories(
  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

add_executable(communication src/communication.cc)
add_dependencies(communication blackboard)

target_link_libraries(communication
  blackboard
  ${OpenCV_LIBRARIES}
)

#install(TARGETS communication DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
install(TARGETS communication DESTINATION /usr/local/bin)

