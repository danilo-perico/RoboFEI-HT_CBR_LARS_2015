#inclui o caminho para os headers (*.h) usando a variável de ambiente (e.g.: ${RoboFEI-HT_SOURCE_DIR})  
include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
)

#link_directories(
#  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
#)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})

#cria executável
add_executable(decision src/decision.cpp)

#adiciona as dependências = e.g. decison depende do blackboard
add_dependencies(decision blackboard)

#adiciona o link para a biblioteca criada blackboard
target_link_libraries(decision
  blackboard
  ${OpenCV_LIBRARIES}
)
#indica caminho para instalação 
install(TARGETS decision DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS communication DESTINATION /usr/local/bin)


