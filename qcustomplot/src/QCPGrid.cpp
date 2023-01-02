#include "include/QCPGrid.hpp"

using namespace QCP;
/*! \class QCPGrid
  \brief Responsible for drawing the grid of a QCPAxis.

  This class is tightly bound to QCPAxis. Every axis owns a grid instance and uses it to draw the
  grid lines, sub grid lines and zero-line. You can interact with the grid of an axis via \ref
  QCPAxis::grid. Normally, you don't need to create an instance of QCPGrid yourself.

  The axis and grid drawing was split into two classes to allow them to be placed on different
  layers (both QCPAxis and QCPGrid inherit from QCPLayerable). Thus it is possible to have the grid
  in the background and the axes in the foreground, and any plottables/items in between. This
  described situation is the default setup, see the QCPLayer documentation.
*/

/*!
  Creates a QCPGrid instance and sets default values.

  You shouldn't instantiate grids on their own, since every QCPAxis brings its own QCPGrid.
*/
    QCPGrid::QCPGrid(QCPAxis *parentAxis) :
            QCPLayerable(parentAxis->parentPlot(), QString(), parentAxis),
            mSubGridVisible{},
            mAntialiasedSubGrid{},
            mAntialiasedZeroLine{},
            mParentAxis(parentAxis)
    {
// warning: this is called in QCPAxis constructor, so parentAxis members should not be accessed/called
        setParent(parentAxis);
        setPen(QPen(QColor(200,200,200), 0, Qt::DotLine));
        setSubGridPen(QPen(QColor(220,220,220), 0, Qt::DotLine));
        setZeroLinePen(QPen(QColor(200,200,200), 0, Qt::SolidLine));
        setSubGridVisible(false);
        setAntialiased(false);
        setAntialiasedSubGrid(false);
        setAntialiasedZeroLine(false);
    }

/*!
  Sets whether grid lines at sub tick marks are drawn.

  \see setSubGridPen
*/
    void QCPGrid::setSubGridVisible(bool visible)
    {
        mSubGridVisible = visible;
    }

/*!
  Sets whether sub grid lines are drawn antialiased.
*/
    void QCPGrid::setAntialiasedSubGrid(bool enabled)
    {
        mAntialiasedSubGrid = enabled;
    }

/*!
  Sets whether zero lines are drawn antialiased.
*/
    void QCPGrid::setAntialiasedZeroLine(bool enabled)
    {
        mAntialiasedZeroLine = enabled;
    }

/*!
  Sets the pen with which (major) grid lines are drawn.
*/
    void QCPGrid::setPen(const QPen &pen)
    {
        mPen = pen;
    }

/*!
  Sets the pen with which sub grid lines are drawn.
*/
    void QCPGrid::setSubGridPen(const QPen &pen)
    {
        mSubGridPen = pen;
    }

/*!
  Sets the pen with which zero lines are drawn.

  Zero lines are lines at value coordinate 0 which may be drawn with a different pen than other grid
  lines. To disable zero lines and just draw normal grid lines at zero, set \a pen to Qt::NoPen.
*/
    void QCPGrid::setZeroLinePen(const QPen &pen)
    {
        mZeroLinePen = pen;
    }

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing the major grid lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.

  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.

  \see setAntialiased
*/
    void QCPGrid::applyDefaultAntialiasingHint(QCPPainter *painter) const
    {
        applyAntialiasingHint(painter, mAntialiased, QCP::aeGrid);
    }

/*! \internal

  Draws grid lines and sub grid lines at the positions of (sub) ticks of the parent axis, spanning
  over the complete axis rect. Also draws the zero line, if appropriate (\ref setZeroLinePen).
*/
    void QCPGrid::draw(QCPPainter *painter)
    {
        if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

        if (mParentAxis->subTicks() && mSubGridVisible)
            drawSubGridLines(painter);
        drawGridLines(painter);
    }

/*! \internal

  Draws the main grid lines and possibly a zero line with the specified painter.

  This is a helper function called by \ref draw.
*/
    void QCPGrid::drawGridLines(QCPPainter *painter) const
    {
        if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

        const int tickCount = mParentAxis->mTickVector.size();
        double t; // helper variable, result of coordinate-to-pixel transforms
        if (mParentAxis->orientation() == Qt::Horizontal)
        {
// draw zeroline:
            int zeroLineIndex = -1;
            if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
            {
                applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
                painter->setPen(mZeroLinePen);
                double epsilon = mParentAxis->range().size()*1E-6; // for comparing double to zero
                for (int i=0; i<tickCount; ++i)
                {
                    if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
                    {
                        zeroLineIndex = i;
                        t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
                        painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
                        break;
                    }
                }
            }
// draw grid lines:
            applyDefaultAntialiasingHint(painter);
            painter->setPen(mPen);
            for (int i=0; i<tickCount; ++i)
            {
                if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
                t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // x
                painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
            }
        } else
        {
// draw zeroline:
            int zeroLineIndex = -1;
            if (mZeroLinePen.style() != Qt::NoPen && mParentAxis->mRange.lower < 0 && mParentAxis->mRange.upper > 0)
            {
                applyAntialiasingHint(painter, mAntialiasedZeroLine, QCP::aeZeroLine);
                painter->setPen(mZeroLinePen);
                double epsilon = mParentAxis->mRange.size()*1E-6; // for comparing double to zero
                for (int i=0; i<tickCount; ++i)
                {
                    if (qAbs(mParentAxis->mTickVector.at(i)) < epsilon)
                    {
                        zeroLineIndex = i;
                        t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
                        painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
                        break;
                    }
                }
            }
// draw grid lines:
            applyDefaultAntialiasingHint(painter);
            painter->setPen(mPen);
            for (int i=0; i<tickCount; ++i)
            {
                if (i == zeroLineIndex) continue; // don't draw a gridline on top of the zeroline
                t = mParentAxis->coordToPixel(mParentAxis->mTickVector.at(i)); // y
                painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
            }
        }
    }

/*! \internal

  Draws the sub grid lines with the specified painter.

  This is a helper function called by \ref draw.
*/
    void QCPGrid::drawSubGridLines(QCPPainter *painter) const
    {
        if (!mParentAxis) { qDebug() << Q_FUNC_INFO << "invalid parent axis"; return; }

        applyAntialiasingHint(painter, mAntialiasedSubGrid, QCP::aeSubGrid);
        double t; // helper variable, result of coordinate-to-pixel transforms
        painter->setPen(mSubGridPen);
        if (mParentAxis->orientation() == Qt::Horizontal)
        {
            foreach (double tickCoord, mParentAxis->mSubTickVector)
            {
                t = mParentAxis->coordToPixel(tickCoord); // x
                painter->drawLine(QLineF(t, mParentAxis->mAxisRect->bottom(), t, mParentAxis->mAxisRect->top()));
            }
        } else
        {
            foreach (double tickCoord, mParentAxis->mSubTickVector)
            {
                t = mParentAxis->coordToPixel(tickCoord);
                painter->drawLine(QLineF(mParentAxis->mAxisRect->left(), t, mParentAxis->mAxisRect->right(), t));
            }
        }
    }