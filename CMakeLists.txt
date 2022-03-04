cmake_minimum_required(VERSION 3.5)

project(qcustomplot)

include(GNUInstallDirs)

# Find Qt packages
find_package(Qt5 COMPONENTS Core Widgets PrintSupport REQUIRED)

add_subdirectory(qcustomplot)
