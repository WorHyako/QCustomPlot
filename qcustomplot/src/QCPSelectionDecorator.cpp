#include "include/QCPSelectionDecorator.hpp"

namespace QCP {
/*! \class QCPSelectionDecorator
  \brief Controls how a plottable's data selection is drawn

  Each \ref QCPAbstractPlottable instance has one \ref QCPSelectionDecorator (accessible via \ref
  QCPAbstractPlottable::selectionDecorator) and uses it when drawing selected segments of its data.

  The selection decorator controls both pen (\ref setPen) and brush (\ref setBrush), as well as the
  scatter style (\ref setScatterStyle) if the plottable draws scatters. Since a \ref
  QCPScatterStyle is itself composed of different properties such as color shape and size, the
  decorator allows specifying exactly which of those properties shall be used for the selected data
  point, via \ref setUsedScatterProperties.

  A \ref QCPSelectionDecorator subclass instance can be passed to a plottable via \ref
  QCPAbstractPlottable::setSelectionDecorator, allowing greater customizability of the appearance
  of selected segments.

  Use \ref copyFrom to easily transfer the settings of one decorator to another one. This is
  especially useful since plottables take ownership of the passed selection decorator, and thus the
  same decorator instance can not be passed to multiple plottables.

  Selection decorators can also themselves perform drawing operations by reimplementing \ref
  drawDecoration, which is called by the plottable's draw method. The base class \ref
  QCPSelectionDecorator does not make use of this however. For example, \ref
  QCPSelectionDecoratorBracket draws brackets around selected data segments.
*/

/*!
  Creates a new QCPSelectionDecorator instance with default values
*/
    QCPSelectionDecorator::QCPSelectionDecorator() :
            mPen(QColor(80, 80, 255), 2.5),
            mBrush(Qt::NoBrush),
            mUsedScatterProperties(QCPScatterStyle::spNone),
            mPlottable(nullptr)
    {
    }

    QCPSelectionDecorator::~QCPSelectionDecorator()
    {
    }

/*!
  Sets the pen that will be used by the parent plottable to draw selected data segments.
*/
    void QCPSelectionDecorator::setPen(const QPen &pen)
    {
        mPen = pen;
    }

/*!
  Sets the brush that will be used by the parent plottable to draw selected data segments.
*/
    void QCPSelectionDecorator::setBrush(const QBrush &brush)
    {
        mBrush = brush;
    }

/*!
  Sets the scatter style that will be used by the parent plottable to draw scatters in selected
  data segments.

  \a usedProperties specifies which parts of the passed \a scatterStyle will be used by the
  plottable. The used properties can also be changed via \ref setUsedScatterProperties.
*/
    void QCPSelectionDecorator::setScatterStyle(const QCPScatterStyle &scatterStyle, QCPScatterStyle::ScatterProperties usedProperties)
    {
        mScatterStyle = scatterStyle;
        setUsedScatterProperties(usedProperties);
    }

/*!
  Use this method to define which properties of the scatter style (set via \ref setScatterStyle)
  will be used for selected data segments. All properties of the scatter style that are not
  specified in \a properties will remain as specified in the plottable's original scatter style.

  \see QCPScatterStyle::ScatterProperty
*/
    void QCPSelectionDecorator::setUsedScatterProperties(const QCPScatterStyle::ScatterProperties &properties)
    {
        mUsedScatterProperties = properties;
    }

/*!
  Sets the pen of \a painter to the pen of this selection decorator.

  \see applyBrush, getFinalScatterStyle
*/
    void QCPSelectionDecorator::applyPen(QCPPainter *painter) const
    {
        painter->setPen(mPen);
    }

/*!
  Sets the brush of \a painter to the brush of this selection decorator.

  \see applyPen, getFinalScatterStyle
*/
    void QCPSelectionDecorator::applyBrush(QCPPainter *painter) const
    {
        painter->setBrush(mBrush);
    }

/*!
  Returns the scatter style that the parent plottable shall use for selected scatter points. The
  plottable's original (unselected) scatter style must be passed as \a unselectedStyle. Depending
  on the setting of \ref setUsedScatterProperties, the returned scatter style is a mixture of this
  selecion decorator's scatter style (\ref setScatterStyle), and \a unselectedStyle.

  \see applyPen, applyBrush, setScatterStyle
*/
    QCPScatterStyle QCPSelectionDecorator::getFinalScatterStyle(const QCPScatterStyle &unselectedStyle) const
    {
        QCPScatterStyle result(unselectedStyle);
        result.setFromOther(mScatterStyle, mUsedScatterProperties);

// if style shall inherit pen from plottable (has no own pen defined), give it the selected
// plottable pen explicitly, so it doesn't use the unselected plottable pen when used in the
// plottable:
        if (!result.isPenDefined())
            result.setPen(mPen);

        return result;
    }

/*!
  Copies all properties (e.g. color, fill, scatter style) of the \a other selection decorator to
  this selection decorator.
*/
    void QCPSelectionDecorator::copyFrom(const QCPSelectionDecorator *other)
    {
        setPen(other->pen());
        setBrush(other->brush());
        setScatterStyle(other->scatterStyle(), other->usedScatterProperties());
    }

/*!
  This method is called by all plottables' draw methods to allow custom selection decorations to be
  drawn. Use the passed \a painter to perform the drawing operations. \a selection carries the data
  selection for which the decoration shall be drawn.

  The default base class implementation of \ref QCPSelectionDecorator has no special decoration, so
  this method does nothing.
*/
    void QCPSelectionDecorator::drawDecoration(QCPPainter *painter, QCPDataSelection selection)
    {
        Q_UNUSED(painter)
        Q_UNUSED(selection)
    }

/*! \internal

  This method is called as soon as a selection decorator is associated with a plottable, by a call
  to \ref QCPAbstractPlottable::setSelectionDecorator. This way the selection decorator can obtain a pointer to the plottable that uses it (e.g. to access
  data points via the \ref QCPAbstractPlottable::interface1D interface).

  If the selection decorator was already added to a different plottable before, this method aborts
  the registration and returns false.
*/
    bool QCPSelectionDecorator::registerWithPlottable(QCPAbstractPlottable *plottable)
    {
        if (!mPlottable)
        {
            mPlottable = plottable;
            return true;
        } else
        {
            qDebug() << Q_FUNC_INFO << "This selection decorator is already registered with plottable:" << reinterpret_cast<quintptr>(mPlottable);
            return false;
        }
    }


} // QCP