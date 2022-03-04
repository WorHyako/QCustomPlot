add_library(qcustomplot STATIC
        qcustomplot.cpp)

target_link_libraries(qcustomplot PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::PrintSupport)

target_include_directories(qcustomplot PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>)

target_compile_definitions(qcustomplot PRIVATE QCUSTOMPLOT_COMPILE_LIBRARY)
