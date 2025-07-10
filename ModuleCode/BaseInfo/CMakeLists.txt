cmake_minimum_required(VERSION 3.16)

project(BaseInfo LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Xml)

add_library(BaseInfo SHARED
  BaseInfo_global.h
  BaseInfo.cpp
  BaseInfo.h
  BaseInfo.ui
  ModuleTemplate.h
  res.qrc

)

target_link_libraries(BaseInfo PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Xml
)

target_compile_definitions(BaseInfo PRIVATE BASEINFO_LIBRARY)
