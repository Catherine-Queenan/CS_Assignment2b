cmake_minimum_required(VERSION 3.29)
project(CS_Assignment2b)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(ConsoleApp)
include_directories(UploadServer)

add_executable(CS_Assignment2b
    ConsoleApp/Socket.cpp
    ConsoleApp/Socket.h
    UploadServer/HttpServlet.h
    UploadServer/HttpServletRequest.cpp
    UploadServer/HttpServletRequest.h
    UploadServer/HttpServletResponse.cpp
    UploadServer/HttpServletResponse.h
    UploadServer/Socket.cpp
    UploadServer/Socket.h
    UploadServer/UploadServer.cpp
    UploadServer/UploadServer.h
    UploadServer/UploadServerThread.cpp
    UploadServer/UploadServerThread.h
    UploadServer/UploadServlet.cpp
    UploadServer/UploadServlet.h
    .gitignore
    README.md)
