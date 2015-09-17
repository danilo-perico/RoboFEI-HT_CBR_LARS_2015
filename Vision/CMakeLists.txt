find_package(OpenCV REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)

#inclui o caminho para os headers (*.h) usando a variável de ambiente (e.g.: ${RoboFEI-HT_SOURCE_DIR})  
include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Control/Framework/include
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
#		${Boost_LIBRARIES}
)

#adiciona o link para a biblioteca criada para o blackboard

set(LIB_DARWIN ${RoboFEI-HT_SOURCE_DIR}/Control/Linux/lib/darwin.a)

target_link_libraries(vision 
  dynamixel 
  dxl_hal
  blackboard
  ${OpenCV_LIBRARIES}
  ${Boost_LIBRARIES}
  ${LIB_DARWIN}
)
#indica caminho para instalação 
install(TARGETS vision DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS communication DESTINATION /usr/local/bin)


