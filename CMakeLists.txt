cmake_minimum_required(VERSION 3.5)

project(qcustomplot)

include(GNUInstallDirs)

# Find Qt packages
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

# Qt settings
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(qcustomplot)

# Make installable

install(TARGETS qcustomplot
        EXPORT qcustomplotConfig
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES qcustomplot/qcustomplot.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

export(TARGETS qcustomplot
        FILE "${CMAKE_CURRENT_BINARY_DIR}/qcustomplotConfig.cmake")

install(EXPORT qcustomplotConfig
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/qcustomplot/cmake")
