cmake_minimum_required(VERSION 3.28)	# Проверка версии CMake
set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0" CACHE STRING "Minimum OS X deployment version")

project(SmartCalc VERSION 2.0 LANGUAGES CXX)	# Название проекта

set(CMAKE_CXX_STANDARD 17)	# Стандарт C++17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Widgets REQUIRED)	# Импортировать модули Widgets Core

add_subdirectory(qcustomplot) # здесь добавляется подпапка qcustomplot

set(SOURCE_EXE main.cpp model.cpp view.cpp controller.cpp)	# Установка переменной со списком исходников для исполняемого файла

include_directories(../headers)	# Расположение заголовочных файлов

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

add_dependencies(main qcustomplot)
target_compile_definitions(main PRIVATE QCUSTOMPLOT_USE_LIBRARY)

target_link_libraries(main PRIVATE Qt6::Widgets qcustomplot)


# info: https://runebook.dev/ru/docs/qt/cmake-get-started