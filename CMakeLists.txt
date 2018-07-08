cmake_minimum_required(VERSION 3.10.0)
project(wst-mbs)
add_compile_options(-std=c++11)
include_directories(/opt/boost/include)
link_directories(/opt/boost/lib)

add_executable(wst-mbs main.cc
    servers/httpsession.cc
    servers/httpserver.cc)

target_link_libraries(wst-mbs boost_system pthread)