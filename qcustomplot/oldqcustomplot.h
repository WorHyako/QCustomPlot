/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2022 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: https://www.qcustomplot.com/                         **
**             Date: 06.11.22                                             **
**          Version: 2.1.1                                                **
****************************************************************************/

#ifndef QCUSTOMPLOT_H
#define QCUSTOMPLOT_H

#include <QtCore/qglobal.h>

// some Qt version/configuration dependent macros to include or exclude certain code paths:
#ifdef QCUSTOMPLOT_USE_OPENGL
#  if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#    define QCP_OPENGL_PBUFFER
#  else
#    define QCP_OPENGL_FBO
#  endif
#  if QT_VERSION >= QT_VERSION_CHECK(5, 3, 0)
#    define QCP_OPENGL_OFFSCREENSURFACE
#  endif
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 4, 0)
#  define QCP_DEVICEPIXELRATIO_SUPPORTED
#  if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
#    define QCP_DEVICEPIXELRATIO_FLOAT
#  endif
#endif

#include <QtCore/QObject>
#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>
#include <QtCore/QTimer>
#include <QtGui/QPainter>
#include <QtGui/QPainterPath>
#include <QtGui/QPaintEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QWheelEvent>
#include <QtGui/QPixmap>
#include <QtCore/QVector>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMultiMap>
#include <QtCore/QFlags>
#include <QtCore/QDebug>
#include <QtCore/QStack>
#include <QtCore/QCache>
#include <QtCore/QMargins>
#include <qmath.h>
#include <limits>
#include <algorithm>
#ifdef QCP_OPENGL_FBO
#  include <QtGui/QOpenGLContext>
#  if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
#    include <QtGui/QOpenGLFramebufferObject>
#  else
#    include <QOpenGLFramebufferObject>
#    include <QOpenGLPaintDevice>
#  endif
#  ifdef QCP_OPENGL_OFFSCREENSURFACE
#    include <QtGui/QOffscreenSurface>
#  else
#    include <QtGui/QWindow>
#  endif
#endif
#ifdef QCP_OPENGL_PBUFFER
#  include <QtOpenGL/QGLPixelBuffer>
#endif
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#  include <qnumeric.h>
#  include <QtGui/QWidget>
#  include <QtGui/QPrinter>
#  include <QtGui/QPrintEngine>
#else
#  include <QtNumeric>
#  include <QtWidgets/QWidget>
#  include <QtPrintSupport/QtPrintSupport>
#endif
#if QT_VERSION >= QT_VERSION_CHECK(4, 8, 0)
#  include <QtCore/QElapsedTimer>
#endif
# if QT_VERSION >= QT_VERSION_CHECK(5, 2, 0)
#  include <QtCore/QTimeZone>
#endif

class QCPPainter;
class QCustomPlot;
class QCPLayerable;
class QCPLayoutElement;
class QCPLayout;
class QCPAxis;
class QCPAxisRect;
class QCPAxisPainterPrivate;
class QCPAbstractPlottable;
class QCPGraph;
class QCPAbstractItem;
class QCPPlottableInterface1D;
class QCPLegend;
class QCPItemPosition;
class QCPLayer;
class QCPAbstractLegendItem;
class QCPSelectionRect;
class QCPColorMap;
class QCPColorScale;
class QCPBars;
class QCPPolarAxisRadial;
class QCPPolarAxisAngular;
class QCPPolarGrid;
class QCPPolarGraph;

/*!
  The QCP Namespace contains general enums, QFlags and functions used throughout the QCustomPlot
  library.

  It provides QMetaObject-based reflection of its enums and flags via \a QCP::staticMetaObject.
*/

// Qt version < 6.2.0: to get metatypes Q_GADGET/Q_ENUMS/Q_FLAGS in namespace we have to make it look like a class during moc-run
#if QT_VERSION >= 0x060200 // don't use QT_VERSION_CHECK here, some moc versions don't understand it
namespace QCP {
    Q_NAMESPACE // this is how to add the staticMetaObject to namespaces in newer Qt versions
#else // Qt version older than 6.2.0
    #  ifndef Q_MOC_RUN
namespace QCP {
#  else // not in moc run
class QCP {
  Q_GADGET
  Q_ENUMS(ExportPen)
  Q_ENUMS(ResolutionUnit)
  Q_ENUMS(SignDomain)
  Q_ENUMS(MarginSide)
  Q_ENUMS(AntialiasedElement)
  Q_ENUMS(PlottingHint)
  Q_ENUMS(Interaction)
  Q_ENUMS(SelectionRectMode)
  Q_ENUMS(SelectionType)

  Q_FLAGS(AntialiasedElements)
  Q_FLAGS(PlottingHints)
  Q_FLAGS(MarginSides)
  Q_FLAGS(Interactions)
public:
#  endif
#endif

// for newer Qt versions we have to declare the enums/flags as metatypes inside the namespace using Q_ENUM_NS/Q_FLAG_NS:
// if you change anything here, don't forget to change it for older Qt versions below, too,
// and at the start of the namespace in the fake moc-run class
#if QT_VERSION >= 0x060200
    Q_ENUM_NS(ExportPen)
    Q_ENUM_NS(ResolutionUnit)
    Q_ENUM_NS(SignDomain)
    Q_ENUM_NS(MarginSide)
    Q_ENUM_NS(AntialiasedElement)
    Q_ENUM_NS(PlottingHint)
    Q_ENUM_NS(Interaction)
    Q_ENUM_NS(SelectionRectMode)
    Q_ENUM_NS(SelectionType)

    Q_FLAG_NS(AntialiasedElements)
    Q_FLAG_NS(PlottingHints)
    Q_FLAG_NS(MarginSides)
    Q_FLAG_NS(Interactions)
#else
    extern const QMetaObject staticMetaObject;
#endif

} // end of namespace QCP

Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::AntialiasedElements)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::PlottingHints)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::MarginSides)
Q_DECLARE_OPERATORS_FOR_FLAGS(QCP::Interactions)

// for older Qt versions we have to declare the enums/flags as metatypes outside the namespace using Q_DECLARE_METATYPE:
// if you change anything here, don't forget to change it for newer Qt versions above, too,
// and at the start of the namespace in the fake moc-run class
#if QT_VERSION < QT_VERSION_CHECK(6, 2, 0)
Q_DECLARE_METATYPE(QCP::ExportPen)
Q_DECLARE_METATYPE(QCP::ResolutionUnit)
Q_DECLARE_METATYPE(QCP::SignDomain)
Q_DECLARE_METATYPE(QCP::MarginSide)
Q_DECLARE_METATYPE(QCP::AntialiasedElement)
Q_DECLARE_METATYPE(QCP::PlottingHint)
Q_DECLARE_METATYPE(QCP::Interaction)
Q_DECLARE_METATYPE(QCP::SelectionRectMode)
Q_DECLARE_METATYPE(QCP::SelectionType)
#endif

/* end of 'src/polar/polargraph.h' */


#endif // QCUSTOMPLOT_H

