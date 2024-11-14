cmake_minimum_required(VERSION 3.29)
project(CS_Assignment2b)

set(CMAKE_CXX_STANDARD 20)

include_directories(.)
include_directories(ConsoleApp)
include_directories(UploadServer)

add_executable(CS_Assignment2b
    UploadServer/HttpServlet.h
    UploadServer/HttpServletRequest.h
    UploadServer/HttpServletResponse.cpp
    UploadServer/HttpServletResponse.h
    UploadServer/Socket.cpp
    UploadServer/Socket.h
    UploadServer/UploadServer.cpp
    UploadServer/UploadServerThread.cpp
    UploadServer/UploadServerThread.h
    UploadServer/UploadServlet.cpp
    UploadServer/UploadServlet.h
    .gitignore
    README.md
        ../lab-9---abstract-factory-EugenieVK/Maze_Factory.hpp
        ../lab-9---abstract-factory-EugenieVK/maze.hpp
        ../lab-9---abstract-factory-EugenieVK/room.hpp
        ../lab-9---abstract-factory-EugenieVK/door.hpp
        ../lab-9---abstract-factory-EugenieVK/wall.hpp
        ../lab-9---abstract-factory-EugenieVK/maze_game.cpp
        ../lab-9---abstract-factory-EugenieVK/maze_game.hpp
        ../lab-9---abstract-factory-EugenieVK/room.cpp
        ../lab-9---abstract-factory-EugenieVK/maze.cpp)
