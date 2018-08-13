cmake_minimum_required(VERSION 3.1.0)

project(wst-mbs)

add_compile_options(-std=c++11)

include_directories(
    "${PROJECT_SOURCE_DIR}"
    "${PROJECT_SOURCE_DIR}/wst-ext/boost/include"
    "${PROJECT_SOURCE_DIR}/wst-ext/jsoncpp/include"
    "${PROJECT_SOURCE_DIR}/wst-ext/agora/v2.2.2/include"
)

link_directories(  
    "${PROJECT_SOURCE_DIR}/wst-ext/boost/lib"
    "${PROJECT_SOURCE_DIR}/wst-ext/jsoncpp/lib"
    "${PROJECT_SOURCE_DIR}/wst-ext/agora/v2.2.2/libs"
)

add_executable(wst-mbs main.cc
    handler/handler.cc
    handler/agorarecord.cc
    handler/manager.cc
    servers/httputils.cc
    servers/httplistener.cc
    servers/httpsession.cc
    servers/httpserver.cc
)

target_link_libraries(wst-mbs boost_system jsoncpp pthread recorder)