cmake_minimum_required(VERSION 3.5)

project(my_project)

set(CMAKE_CXX_STANDARD 11)

# 添加可执行文件
add_executable(main main.cpp db.cpp)

# 添加头文件搜索路径
include_directories(/usr/include/jsoncpp)

# 添加链接库
find_package(OpenCV REQUIRED)
target_link_libraries(main ${OpenCV_LIBS} curl crypto jsoncpp sqlite3)