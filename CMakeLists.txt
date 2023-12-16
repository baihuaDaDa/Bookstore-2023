cmake_minimum_required(VERSION 3.22)
project(Bookstore_2023)

set(CMAKE_CXX_STANDARD 14)

add_executable(code
        main.cpp
        include/System/system.cpp
        include/System/system.hpp
        include/Memory/Memory.cpp
        include/Memory/Memory.h
        include/Memory/MemoryRiver.h
        include/Book/book.cpp
        include/Book/book.hpp
        include/User/user.cpp
        include/User/user.hpp)
