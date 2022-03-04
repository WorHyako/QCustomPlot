add_library(qcustomplot STATIC
        qcustomplot.cpp)

target_link_libraries(qcustomplot PUBLIC
        Qt5::Core
        Qt5::Widgets
        Qt5::PrintSupport)

target_compile_definitions(qcustomplot PRIVATE QCUSTOMPLOT_COMPILE_LIBRARY)
