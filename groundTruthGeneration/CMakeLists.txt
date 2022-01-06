project(diabetesLearning)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)
add_executable(main ${SRC_LIST})
find_package( OpenCV REQUIRED )
add_definitions("-std=c++17")
target_link_libraries( main ${OpenCV_LIBS} )

