cmake_minimum_required(VERSION 3.29)
project(db)

set(CMAKE_CXX_STANDARD 20)

add_executable(db main.cpp
        modules/storage/table.h
        modules/language/lexer.h
        modules/language/lexer.cpp
        modules/language/parser.h
        modules/language/parser.cpp
        modules/storage/table.cpp
        modules/storage/database.h
        modules/storage/database.cpp
        modules/language/utils.h
        modules/language/utils.cpp
)
