cmake_minimum_required(VERSION 3.26)
project(untitled)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/home/kagg886/Qt/6.6.1/gcc_64")

find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

qt_add_resources(res.qrc)

add_executable(untitled main.cpp
        MainWindow.cpp
        MainWindow.h
        network.cpp
        network.h
        looper.cpp
        looper.h
        Ini.cpp
        Ini.h
        res.qrc
)
target_link_libraries(untitled
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

