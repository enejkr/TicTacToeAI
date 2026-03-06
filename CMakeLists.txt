cmake_minimum_required(VERSION 3.28.3)
project(TicTacToeAI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set Qt6 installation path
set(CMAKE_PREFIX_PATH "C:/Qt/6.10.2/mingw_64")

# Enable Qt's automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(TicTacToeAI main.cpp)
target_link_libraries(TicTacToeAI PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

# Qt test executable
add_executable(QtTest qt_test.cpp)
target_link_libraries(QtTest PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets)

