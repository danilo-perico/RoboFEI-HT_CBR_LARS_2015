#inclui o caminho para os headers (*.h) usando a variável de ambiente (e.g.: ${RoboFEI-HT_SOURCE_DIR})  
include_directories(
  include
  ${RoboFEI-HT_SOURCE_DIR}/Blackboard/include
  ${RoboFEI-HT_SOURCE_DIR}/Planning/include
)


include_directories(include)
add_library(planning_code SHARED src/planning_code.cpp)
install(TARGETS planning_code DESTINATION ${RoboFEI-HT_BINARY_DIR}/lib)


#link_directories(
#  ${RoboFEI-HT_BINARY_DIR}/Blackboard}
#)

#MESSAGE(STATUS "CMAKE_SOURCE_DIR:         " ${RoboFEI-HT_SOURCE_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${RoboFEI-HT_BINARY_DIR})
#MESSAGE(STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR})

#cria executável
add_executable(planning src/main.cpp)

#adiciona as dependências = e.g. planning depende do blackboard
add_dependencies(planning blackboard planning_code)

#adiciona o link para a biblioteca criada blackboard
target_link_libraries(planning
  blackboard planning_code
)
#indica caminho para instalação 
install(TARGETS planning DESTINATION ${RoboFEI-HT_BINARY_DIR}/bin)
#install(TARGETS communication DESTINATION /usr/local/bin)


