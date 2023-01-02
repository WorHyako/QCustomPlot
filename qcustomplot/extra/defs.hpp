#ifndef QCUSTOMPLOT_DEFS_H
#define QCUSTOMPLOT_DEFS_H

#define QCUSTOMPLOT_VERSION_STR "2.1.1"
#define QCUSTOMPLOT_VERSION 0x020101

// decl definitions for shared library compilation/usage:
#if defined(QT_STATIC_BUILD)
#  define QCP_LIB_DECL
#elif defined(QCUSTOMPLOT_COMPILE_LIBRARY)
#  define QCP_LIB_DECL Q_DECL_EXPORT
#elif defined(QCUSTOMPLOT_USE_LIBRARY)
#  define QCP_LIB_DECL Q_DECL_IMPORT
#else
#  define QCP_LIB_DECL
#endif

// define empty macro for Q_DECL_OVERRIDE if it doesn't exist (Qt < 5)
#ifndef Q_DECL_OVERRIDE
#  define Q_DECL_OVERRIDE
#endif

#endif
