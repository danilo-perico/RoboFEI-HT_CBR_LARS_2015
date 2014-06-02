find_package(OpenCV REQUIRED)

#inclui o caminho para os headers (*.h) usando a variável de ambiente (e.g.: ${RoboFEI-HT_SOURCE_DIR})  
include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
  ${RoboFEI-HT_SOURCE_DIR}/Dynamixel/include
  ${OpenCV_INCLUDE_DIRS}
)

#link_directories(
#  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
#)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

#cria executável
add_executable(vision src/vision.cpp)

#adiciona as dependências = e.g. communication depende do blackboard
add_dependencies(vision dynamixel dxl_hal blackboard
		${OpenCV_LIBRARIES}
)

#adiciona o link para a biblioteca criada para o blackboard
target_link_libraries(vision 
  dynamixel 
  dxl_hal
  blackboard
  ${OpenCV_LIBRARIES}
)
#indica caminho para instalação 
install(TARGETS vision DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS communication DESTINATION /usr/local/bin)


