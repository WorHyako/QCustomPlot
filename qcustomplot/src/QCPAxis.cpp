#include "include/QCPAxis.hpp"

using namespace QCP;

/*! \class QCPAxis
  \brief Manages a single axis inside a QCustomPlot.

  Usually doesn't need to be instantiated externally. Access %QCustomPlot's default four axes via
  QCustomPlot::xAxis (bottom), QCustomPlot::yAxis (left), QCustomPlot::xAxis2 (top) and
  QCustomPlot::yAxis2 (right).

  Axes are always part of an axis rect, see QCPAxisRect.
  \image html AxisNamesOverview.png
  <center>Naming convention of axis parts</center>
  \n

  \image html AxisRectSpacingOverview.png
  <center>Overview of the spacings and paddings that define the geometry of an axis. The dashed gray line
  on the left represents the QCustomPlot widget border.</center>

  Each axis holds an instance of QCPAxisTicker which is used to generate the tick coordinates and
  tick labels. You can access the currently installed \ref ticker or set a new one (possibly one of
  the specialized subclasses, or your own subclass) via \ref setTicker. For details, see the
  documentation of QCPAxisTicker.
*/

/*! \fn Qt::Orientation QCPAxis::orientation() const

  Returns the orientation of this axis. The axis orientation (horizontal or vertical) is deduced
  from the axis type (left, top, right or bottom).

  \see orientation(AxisType type), pixelOrientation
*/

/*! \fn QCPGrid *QCPAxis::grid() const

  Returns the \ref QCPGrid instance belonging to this axis. Access it to set details about the way the
  grid is displayed.
*/

/*! \fn static Qt::Orientation QCPAxis::orientation(AxisType type)

  Returns the orientation of the specified axis type

  \see orientation(), pixelOrientation
*/

/*! \fn int QCPAxis::pixelOrientation() const

  Returns which direction points towards higher coordinate values/keys, in pixel space.

  This method returns either 1 or -1. If it returns 1, then going in the positive direction along
  the orientation of the axis in pixels corresponds to going from lower to higher axis coordinates.
  On the other hand, if this method returns -1, going to smaller pixel values corresponds to going
  from lower to higher axis coordinates.

  For example, this is useful to easily shift axis coordinates by a certain amount given in pixels,
  without having to care about reversed or vertically aligned axes:

  \code
  double newKey = keyAxis->pixelToCoord(keyAxis->coordToPixel(oldKey)+10*keyAxis->pixelOrientation());
  \endcode

  \a newKey will then contain a key that is ten pixels towards higher keys, starting from \a oldKey.
*/

/*! \fn QSharedPointer<QCPAxisTicker> QCPAxis::ticker() const

  Returns a modifiable shared pointer to the currently installed axis ticker. The axis ticker is
  responsible for generating the tick positions and tick labels of this axis. You can access the
  \ref QCPAxisTicker with this method and modify basic properties such as the approximate tick count
  (\ref QCPAxisTicker::setTickCount).

  You can gain more control over the axis ticks by setting a different \ref QCPAxisTicker subclass, see
  the documentation there. A new axis ticker can be set with \ref setTicker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may share the same axis
  ticker simply by passing the same shared pointer to multiple axes.

  \see setTicker
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange)

  This signal is emitted when the range of this axis has changed. You can connect it to the \ref
  setRange slot of another axis to communicate the new range to the other axis, in order for it to
  be synchronized.

  You may also manipulate/correct the range with \ref setRange in a slot connected to this signal.
  This is useful if for example a maximum range span shall not be exceeded, or if the lower/upper
  range shouldn't go beyond certain values (see \ref QCPRange::bounded). For example, the following
  slot would limit the x axis to ranges between 0 and 10:
  \code
  customPlot->xAxis->setRange(newRange.bounded(0, 10))
  \endcode
*/

/*! \fn void QCPAxis::rangeChanged(const QCPRange &newRange, const QCPRange &oldRange)
  \overload

  Additionally to the new range, this signal also provides the previous range held by the axis as
  \a oldRange.
*/

/*! \fn void QCPAxis::scaleTypeChanged(QCPAxis::ScaleType scaleType);

  This signal is emitted when the scale type changes, by calls to \ref setScaleType
*/

/*! \fn void QCPAxis::selectionChanged(QCPAxis::SelectableParts selection)

  This signal is emitted when the selection state of this axis has changed, either by user interaction
  or by a direct call to \ref setSelectedParts.
*/

/*! \fn void QCPAxis::selectableChanged(const QCPAxis::SelectableParts &parts);

  This signal is emitted when the selectability changes, by calls to \ref setSelectableParts
*/

/*!
  Constructs an Axis instance of Type \a type for the axis rect \a parent.

  Usually it isn't necessary to instantiate axes directly, because you can let QCustomPlot create
  them for you with \ref QCPAxisRect::addAxis. If you want to use own QCPAxis-subclasses however,
  create them manually and then inject them also via \ref QCPAxisRect::addAxis.
*/
    QCPAxis::QCPAxis(QCPAxisRect *parent, AxisType type) :
            QCPLayerable(parent->parentPlot(), QString(), parent),
// axis base:
            mAxisType(type),
            mAxisRect(parent),
            mPadding(5),
            mOrientation(orientation(type)),
            mSelectableParts(spAxis | spTickLabels | spAxisLabel),
            mSelectedParts(spNone),
            mBasePen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            mSelectedBasePen(QPen(Qt::blue, 2)),
// axis label:
            mLabel(),
            mLabelFont(mParentPlot->font()),
            mSelectedLabelFont(QFont(mLabelFont.family(), mLabelFont.pointSize(), QFont::Bold)),
            mLabelColor(Qt::black),
            mSelectedLabelColor(Qt::blue),
// tick labels:
            mTickLabels(true),
            mTickLabelFont(mParentPlot->font()),
            mSelectedTickLabelFont(QFont(mTickLabelFont.family(), mTickLabelFont.pointSize(), QFont::Bold)),
            mTickLabelColor(Qt::black),
            mSelectedTickLabelColor(Qt::blue),
            mNumberPrecision(6),
            mNumberFormatChar('g'),
            mNumberBeautifulPowers(true),
// ticks and subticks:
            mTicks(true),
            mSubTicks(true),
            mTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            mSelectedTickPen(QPen(Qt::blue, 2)),
            mSubTickPen(QPen(Qt::black, 0, Qt::SolidLine, Qt::SquareCap)),
            mSelectedSubTickPen(QPen(Qt::blue, 2)),
// scale and range:
            mRange(0, 5),
            mRangeReversed(false),
            mScaleType(stLinear),
// internal members:
            mGrid(new QCPGrid(this)),
            mAxisPainter(new QCPAxisPainterPrivate(parent->parentPlot())),
            mTicker(new QCPAxisTicker),
            mCachedMarginValid(false),
            mCachedMargin(0),
            mDragging(false)
    {
        setParent(parent);
        mGrid->setVisible(false);
        setAntialiased(false);
        setLayer(mParentPlot->currentLayer()); // it's actually on that layer already, but we want it in front of the grid, so we place it on there again

        if (type == atTop)
        {
            setTickLabelPadding(3);
            setLabelPadding(6);
        } else if (type == atRight)
        {
            setTickLabelPadding(7);
            setLabelPadding(12);
        } else if (type == atBottom)
        {
            setTickLabelPadding(3);
            setLabelPadding(3);
        } else if (type == atLeft)
        {
            setTickLabelPadding(5);
            setLabelPadding(10);
        }
    }

    QCPAxis::~QCPAxis()
    {
        delete mAxisPainter;
        delete mGrid; // delete grid here instead of via parent ~QObject for better defined deletion order
    }

/* No documentation as it is a property getter */
    int QCPAxis::tickLabelPadding() const
    {
        return mAxisPainter->tickLabelPadding;
    }

/* No documentation as it is a property getter */
    double QCPAxis::tickLabelRotation() const
    {
        return mAxisPainter->tickLabelRotation;
    }

/* No documentation as it is a property getter */
    QCPAxis::LabelSide QCPAxis::tickLabelSide() const
    {
        return mAxisPainter->tickLabelSide;
    }

/* No documentation as it is a property getter */
    QString QCPAxis::numberFormat() const
    {
        QString result;
        result.append(mNumberFormatChar);
        if (mNumberBeautifulPowers)
        {
            result.append(QLatin1Char('b'));
            if (mAxisPainter->numberMultiplyCross)
                result.append(QLatin1Char('c'));
        }
        return result;
    }

/* No documentation as it is a property getter */
    int QCPAxis::tickLengthIn() const
    {
        return mAxisPainter->tickLengthIn;
    }

/* No documentation as it is a property getter */
    int QCPAxis::tickLengthOut() const
    {
        return mAxisPainter->tickLengthOut;
    }

/* No documentation as it is a property getter */
    int QCPAxis::subTickLengthIn() const
    {
        return mAxisPainter->subTickLengthIn;
    }

/* No documentation as it is a property getter */
    int QCPAxis::subTickLengthOut() const
    {
        return mAxisPainter->subTickLengthOut;
    }

/* No documentation as it is a property getter */
    int QCPAxis::labelPadding() const
    {
        return mAxisPainter->labelPadding;
    }

/* No documentation as it is a property getter */
    int QCPAxis::offset() const
    {
        return mAxisPainter->offset;
    }

/* No documentation as it is a property getter */
    QCPLineEnding QCPAxis::lowerEnding() const
    {
        return mAxisPainter->lowerEnding;
    }

/* No documentation as it is a property getter */
    QCPLineEnding QCPAxis::upperEnding() const
    {
        return mAxisPainter->upperEnding;
    }

/*!
  Sets whether the axis uses a linear scale or a logarithmic scale.

  Note that this method controls the coordinate transformation. For logarithmic scales, you will
  likely also want to use a logarithmic tick spacing and labeling, which can be achieved by setting
  the axis ticker to an instance of \ref QCPAxisTickerLog :

  \snippet documentation/doc-code-snippets/mainwindow.cpp qcpaxisticker-log-creation

  See the documentation of \ref QCPAxisTickerLog about the details of logarithmic axis tick
  creation.

  \ref setNumberPrecision
*/
    void QCPAxis::setScaleType(QCPAxis::ScaleType type)
    {
        if (mScaleType != type)
        {
            mScaleType = type;
            if (mScaleType == stLogarithmic)
                setRange(mRange.sanitizedForLogScale());
            mCachedMarginValid = false;
            emit scaleTypeChanged(mScaleType);
        }
    }

/*!
  Sets the range of the axis.

  This slot may be connected with the \ref rangeChanged signal of another axis so this axis
  is always synchronized with the other axis range, when it changes.

  To invert the direction of an axis, use \ref setRangeReversed.
*/
    void QCPAxis::setRange(const QCPRange &range)
    {
        if (range.lower == mRange.lower && range.upper == mRange.upper)
            return;

        if (!QCPRange::validRange(range)) return;
        QCPRange oldRange = mRange;
        if (mScaleType == stLogarithmic)
        {
            mRange = range.sanitizedForLogScale();
        } else
        {
            mRange = range.sanitizedForLinScale();
        }
        emit rangeChanged(mRange);
        emit rangeChanged(mRange, oldRange);
    }

/*!
  Sets whether the user can (de-)select the parts in \a selectable by clicking on the QCustomPlot surface.
  (When \ref QCustomPlot::setInteractions contains iSelectAxes.)

  However, even when \a selectable is set to a value not allowing the selection of a specific part,
  it is still possible to set the selection of this part manually, by calling \ref setSelectedParts
  directly.

  \see SelectablePart, setSelectedParts
*/
    void QCPAxis::setSelectableParts(const SelectableParts &selectable)
    {
        if (mSelectableParts != selectable)
        {
            mSelectableParts = selectable;
            emit selectableChanged(mSelectableParts);
        }
    }

/*!
  Sets the selected state of the respective axis parts described by \ref SelectablePart. When a part
  is selected, it uses a different pen/font.

  The entire selection mechanism for axes is handled automatically when \ref
  QCustomPlot::setInteractions contains iSelectAxes. You only need to call this function when you
  wish to change the selection state manually.

  This function can change the selection state of a part, independent of the \ref setSelectableParts setting.

  emits the \ref selectionChanged signal when \a selected is different from the previous selection state.

  \see SelectablePart, setSelectableParts, selectTest, setSelectedBasePen, setSelectedTickPen, setSelectedSubTickPen,
  setSelectedTickLabelFont, setSelectedLabelFont, setSelectedTickLabelColor, setSelectedLabelColor
*/
    void QCPAxis::setSelectedParts(const SelectableParts &selected)
    {
        if (mSelectedParts != selected)
        {
            mSelectedParts = selected;
            emit selectionChanged(mSelectedParts);
        }
    }

/*!
  \overload

  Sets the lower and upper bound of the axis range.

  To invert the direction of an axis, use \ref setRangeReversed.

  There is also a slot to set a range, see \ref setRange(const QCPRange &range).
*/
    void QCPAxis::setRange(double lower, double upper)
    {
        if (lower == mRange.lower && upper == mRange.upper)
            return;

        if (!QCPRange::validRange(lower, upper)) return;
        QCPRange oldRange = mRange;
        mRange.lower = lower;
        mRange.upper = upper;
        if (mScaleType == stLogarithmic)
        {
            mRange = mRange.sanitizedForLogScale();
        } else
        {
            mRange = mRange.sanitizedForLinScale();
        }
        emit rangeChanged(mRange);
        emit rangeChanged(mRange, oldRange);
    }

/*!
  \overload

  Sets the range of the axis.

  The \a position coordinate indicates together with the \a alignment parameter, where the new
  range will be positioned. \a size defines the size of the new axis range. \a alignment may be
  Qt::AlignLeft, Qt::AlignRight or Qt::AlignCenter. This will cause the left border, right border,
  or center of the range to be aligned with \a position. Any other values of \a alignment will
  default to Qt::AlignCenter.
*/
    void QCPAxis::setRange(double position, double size, Qt::AlignmentFlag alignment)
    {
        if (alignment == Qt::AlignLeft)
            setRange(position, position+size);
        else if (alignment == Qt::AlignRight)
            setRange(position-size, position);
        else // alignment == Qt::AlignCenter
            setRange(position-size/2.0, position+size/2.0);
    }

/*!
  Sets the lower bound of the axis range. The upper bound is not changed.
  \see setRange
*/
    void QCPAxis::setRangeLower(double lower)
    {
        if (mRange.lower == lower)
            return;

        QCPRange oldRange = mRange;
        mRange.lower = lower;
        if (mScaleType == stLogarithmic)
        {
            mRange = mRange.sanitizedForLogScale();
        } else
        {
            mRange = mRange.sanitizedForLinScale();
        }
        emit rangeChanged(mRange);
        emit rangeChanged(mRange, oldRange);
    }

/*!
  Sets the upper bound of the axis range. The lower bound is not changed.
  \see setRange
*/
    void QCPAxis::setRangeUpper(double upper)
    {
        if (mRange.upper == upper)
            return;

        QCPRange oldRange = mRange;
        mRange.upper = upper;
        if (mScaleType == stLogarithmic)
        {
            mRange = mRange.sanitizedForLogScale();
        } else
        {
            mRange = mRange.sanitizedForLinScale();
        }
        emit rangeChanged(mRange);
        emit rangeChanged(mRange, oldRange);
    }

/*!
  Sets whether the axis range (direction) is displayed reversed. Normally, the values on horizontal
  axes increase left to right, on vertical axes bottom to top. When \a reversed is set to true, the
  direction of increasing values is inverted.

  Note that the range and data interface stays the same for reversed axes, e.g. the \a lower part
  of the \ref setRange interface will still reference the mathematically smaller number than the \a
  upper part.
*/
    void QCPAxis::setRangeReversed(bool reversed)
    {
        mRangeReversed = reversed;
    }

/*!
  The axis ticker is responsible for generating the tick positions and tick labels. See the
  documentation of QCPAxisTicker for details on how to work with axis tickers.

  You can change the tick positioning/labeling behaviour of this axis by setting a different
  QCPAxisTicker subclass using this method. If you only wish to modify the currently installed axis
  ticker, access it via \ref ticker.

  Since the ticker is stored in the axis as a shared pointer, multiple axes may share the same axis
  ticker simply by passing the same shared pointer to multiple axes.

  \see ticker
*/
    void QCPAxis::setTicker(QSharedPointer<QCPAxisTicker> ticker)
{
    if (ticker)
    mTicker = ticker;
    else
    qDebug() << Q_FUNC_INFO << "can not set nullptr as axis ticker";
// no need to invalidate margin cache here because produced tick labels are checked for changes in setupTickVector
}

/*!
  Sets whether tick marks are displayed.

  Note that setting \a show to false does not imply that tick labels are invisible, too. To achieve
  that, see \ref setTickLabels.

  \see setSubTicks
*/
void QCPAxis::setTicks(bool show)
{
    if (mTicks != show)
    {
        mTicks = show;
        mCachedMarginValid = false;
    }
}

/*!
  Sets whether tick labels are displayed. Tick labels are the numbers drawn next to tick marks.
*/
void QCPAxis::setTickLabels(bool show)
{
    if (mTickLabels != show)
    {
        mTickLabels = show;
        mCachedMarginValid = false;
        if (!mTickLabels)
            mTickVectorLabels.clear();
    }
}

/*!
  Sets the distance between the axis base line (including any outward ticks) and the tick labels.
  \see setLabelPadding, setPadding
*/
void QCPAxis::setTickLabelPadding(int padding)
{
    if (mAxisPainter->tickLabelPadding != padding)
    {
        mAxisPainter->tickLabelPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the font of the tick labels.

  \see setTickLabels, setTickLabelColor
*/
void QCPAxis::setTickLabelFont(const QFont &font)
{
    if (font != mTickLabelFont)
    {
        mTickLabelFont = font;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the color of the tick labels.

  \see setTickLabels, setTickLabelFont
*/
void QCPAxis::setTickLabelColor(const QColor &color)
{
    mTickLabelColor = color;
}

/*!
  Sets the rotation of the tick labels. If \a degrees is zero, the labels are drawn normally. Else,
  the tick labels are drawn rotated by \a degrees clockwise. The specified angle is bound to values
  from -90 to 90 degrees.

  If \a degrees is exactly -90, 0 or 90, the tick labels are centered on the tick coordinate. For
  other angles, the label is drawn with an offset such that it seems to point toward or away from
  the tick mark.
*/
void QCPAxis::setTickLabelRotation(double degrees)
{
    if (!qFuzzyIsNull(degrees-mAxisPainter->tickLabelRotation))
    {
        mAxisPainter->tickLabelRotation = qBound(-90.0, degrees, 90.0);
        mCachedMarginValid = false;
    }
}

/*!
  Sets whether the tick labels (numbers) shall appear inside or outside the axis rect.

  The usual and default setting is \ref lsOutside. Very compact plots sometimes require tick labels
  to be inside the axis rect, to save space. If \a side is set to \ref lsInside, the tick labels
  appear on the inside are additionally clipped to the axis rect.
*/
void QCPAxis::setTickLabelSide(LabelSide side)
{
    mAxisPainter->tickLabelSide = side;
    mCachedMarginValid = false;
}

/*!
  Sets the number format for the numbers in tick labels. This \a formatCode is an extended version
  of the format code used e.g. by QString::number() and QLocale::toString(). For reference about
  that, see the "Argument Formats" section in the detailed description of the QString class.

  \a formatCode is a string of one, two or three characters.

  <b>The first character</b> is identical to
  the normal format code used by Qt. In short, this means: 'e'/'E' scientific format, 'f' fixed
  format, 'g'/'G' scientific or fixed, whichever is shorter. For the 'e', 'E', and 'f' formats,
  the precision set by \ref setNumberPrecision represents the number of digits after the decimal
  point. For the 'g' and 'G' formats, the precision represents the maximum number of significant
  digits, trailing zeroes are omitted.

  <b>The second and third characters</b> are optional and specific to QCustomPlot:\n
  If the first char was 'e' or 'g', numbers are/might be displayed in the scientific format, e.g.
  "5.5e9", which is ugly in a plot. So when the second char of \a formatCode is set to 'b' (for
  "beautiful"), those exponential numbers are formatted in a more natural way, i.e. "5.5
  [multiplication sign] 10 [superscript] 9". By default, the multiplication sign is a centered dot.
  If instead a cross should be shown (as is usual in the USA), the third char of \a formatCode can
  be set to 'c'. The inserted multiplication signs are the UTF-8 characters 215 (0xD7) for the
  cross and 183 (0xB7) for the dot.

  Examples for \a formatCode:
  \li \c g normal format code behaviour. If number is small, fixed format is used, if number is large,
  normal scientific format is used
  \li \c gb If number is small, fixed format is used, if number is large, scientific format is used with
  beautifully typeset decimal powers and a dot as multiplication sign
  \li \c ebc All numbers are in scientific format with beautifully typeset decimal power and a cross as
  multiplication sign
  \li \c fb illegal format code, since fixed format doesn't support (or need) beautifully typeset decimal
  powers. Format code will be reduced to 'f'.
  \li \c hello illegal format code, since first char is not 'e', 'E', 'f', 'g' or 'G'. Current format
  code will not be changed.
*/
void QCPAxis::setNumberFormat(const QString &formatCode)
{
    if (formatCode.isEmpty())
    {
        qDebug() << Q_FUNC_INFO << "Passed formatCode is empty";
        return;
    }
    mCachedMarginValid = false;

// interpret first char as number format char:
    QString allowedFormatChars(QLatin1String("eEfgG"));
    if (allowedFormatChars.contains(formatCode.at(0)))
    {
        mNumberFormatChar = QLatin1Char(formatCode.at(0).toLatin1());
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (first char not in 'eEfgG'):" << formatCode;
        return;
    }
    if (formatCode.length() < 2)
    {
        mNumberBeautifulPowers = false;
        mAxisPainter->numberMultiplyCross = false;
        return;
    }

// interpret second char as indicator for beautiful decimal powers:
    if (formatCode.at(1) == QLatin1Char('b') && (mNumberFormatChar == QLatin1Char('e') || mNumberFormatChar == QLatin1Char('g')))
    {
        mNumberBeautifulPowers = true;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (second char not 'b' or first char neither 'e' nor 'g'):" << formatCode;
        return;
    }
    if (formatCode.length() < 3)
    {
        mAxisPainter->numberMultiplyCross = false;
        return;
    }

// interpret third char as indicator for dot or cross multiplication symbol:
    if (formatCode.at(2) == QLatin1Char('c'))
    {
        mAxisPainter->numberMultiplyCross = true;
    } else if (formatCode.at(2) == QLatin1Char('d'))
    {
        mAxisPainter->numberMultiplyCross = false;
    } else
    {
        qDebug() << Q_FUNC_INFO << "Invalid number format code (third char neither 'c' nor 'd'):" << formatCode;
        return;
    }
}

/*!
  Sets the precision of the tick label numbers. See QLocale::toString(double i, char f, int prec)
  for details. The effect of precisions are most notably for number Formats starting with 'e', see
  \ref setNumberFormat
*/
void QCPAxis::setNumberPrecision(int precision)
{
    if (mNumberPrecision != precision)
    {
        mNumberPrecision = precision;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the length of the ticks in pixels. \a inside is the length the ticks will reach inside the
  plot and \a outside is the length they will reach outside the plot. If \a outside is greater than
  zero, the tick labels and axis label will increase their distance to the axis accordingly, so
  they won't collide with the ticks.

  \see setSubTickLength, setTickLengthIn, setTickLengthOut
*/
void QCPAxis::setTickLength(int inside, int outside)
{
    setTickLengthIn(inside);
    setTickLengthOut(outside);
}

/*!
  Sets the length of the inward ticks in pixels. \a inside is the length the ticks will reach
  inside the plot.

  \see setTickLengthOut, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthIn(int inside)
{
    if (mAxisPainter->tickLengthIn != inside)
    {
        mAxisPainter->tickLengthIn = inside;
    }
}

/*!
  Sets the length of the outward ticks in pixels. \a outside is the length the ticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels and axis label will
  increase their distance to the axis accordingly, so they won't collide with the ticks.

  \see setTickLengthIn, setTickLength, setSubTickLength
*/
void QCPAxis::setTickLengthOut(int outside)
{
    if (mAxisPainter->tickLengthOut != outside)
    {
        mAxisPainter->tickLengthOut = outside;
        mCachedMarginValid = false; // only outside tick length can change margin
    }
}

/*!
  Sets whether sub tick marks are displayed.

  Sub ticks are only potentially visible if (major) ticks are also visible (see \ref setTicks)

  \see setTicks
*/
void QCPAxis::setSubTicks(bool show)
{
    if (mSubTicks != show)
    {
        mSubTicks = show;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the length of the subticks in pixels. \a inside is the length the subticks will reach inside
  the plot and \a outside is the length they will reach outside the plot. If \a outside is greater
  than zero, the tick labels and axis label will increase their distance to the axis accordingly,
  so they won't collide with the ticks.

  \see setTickLength, setSubTickLengthIn, setSubTickLengthOut
*/
void QCPAxis::setSubTickLength(int inside, int outside)
{
    setSubTickLengthIn(inside);
    setSubTickLengthOut(outside);
}

/*!
  Sets the length of the inward subticks in pixels. \a inside is the length the subticks will reach inside
  the plot.

  \see setSubTickLengthOut, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthIn(int inside)
{
    if (mAxisPainter->subTickLengthIn != inside)
    {
        mAxisPainter->subTickLengthIn = inside;
    }
}

/*!
  Sets the length of the outward subticks in pixels. \a outside is the length the subticks will reach
  outside the plot. If \a outside is greater than zero, the tick labels will increase their
  distance to the axis accordingly, so they won't collide with the ticks.

  \see setSubTickLengthIn, setSubTickLength, setTickLength
*/
void QCPAxis::setSubTickLengthOut(int outside)
{
    if (mAxisPainter->subTickLengthOut != outside)
    {
        mAxisPainter->subTickLengthOut = outside;
        mCachedMarginValid = false; // only outside tick length can change margin
    }
}

/*!
  Sets the pen, the axis base line is drawn with.

  \see setTickPen, setSubTickPen
*/
void QCPAxis::setBasePen(const QPen &pen)
{
    mBasePen = pen;
}

/*!
  Sets the pen, tick marks will be drawn with.

  \see setTickLength, setBasePen
*/
void QCPAxis::setTickPen(const QPen &pen)
{
    mTickPen = pen;
}

/*!
  Sets the pen, subtick marks will be drawn with.

  \see setSubTickCount, setSubTickLength, setBasePen
*/
void QCPAxis::setSubTickPen(const QPen &pen)
{
    mSubTickPen = pen;
}

/*!
  Sets the font of the axis label.

  \see setLabelColor
*/
void QCPAxis::setLabelFont(const QFont &font)
{
    if (mLabelFont != font)
    {
        mLabelFont = font;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the color of the axis label.

  \see setLabelFont
*/
void QCPAxis::setLabelColor(const QColor &color)
{
    mLabelColor = color;
}

/*!
  Sets the text of the axis label that will be shown below/above or next to the axis, depending on
  its orientation. To disable axis labels, pass an empty string as \a str.
*/
void QCPAxis::setLabel(const QString &str)
{
    if (mLabel != str)
    {
        mLabel = str;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the distance between the tick labels and the axis label.

  \see setTickLabelPadding, setPadding
*/
void QCPAxis::setLabelPadding(int padding)
{
    if (mAxisPainter->labelPadding != padding)
    {
        mAxisPainter->labelPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the padding of the axis.

  When \ref QCPAxisRect::setAutoMargins is enabled, the padding is the additional outer most space,
  that is left blank.

  The axis padding has no meaning if \ref QCPAxisRect::setAutoMargins is disabled.

  \see setLabelPadding, setTickLabelPadding
*/
void QCPAxis::setPadding(int padding)
{
    if (mPadding != padding)
    {
        mPadding = padding;
        mCachedMarginValid = false;
    }
}

/*!
  Sets the offset the axis has to its axis rect side.

  If an axis rect side has multiple axes and automatic margin calculation is enabled for that side,
  only the offset of the inner most axis has meaning (even if it is set to be invisible). The
  offset of the other, outer axes is controlled automatically, to place them at appropriate
  positions.
*/
void QCPAxis::setOffset(int offset)
{
    mAxisPainter->offset = offset;
}

/*!
  Sets the font that is used for tick labels when they are selected.

  \see setTickLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelFont(const QFont &font)
{
    if (font != mSelectedTickLabelFont)
    {
        mSelectedTickLabelFont = font;
// don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
    }
}

/*!
  Sets the font that is used for the axis label when it is selected.

  \see setLabelFont, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelFont(const QFont &font)
{
    mSelectedLabelFont = font;
// don't set mCachedMarginValid to false here because margin calculation is always done with non-selected fonts
}

/*!
  Sets the color that is used for tick labels when they are selected.

  \see setTickLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickLabelColor(const QColor &color)
{
    if (color != mSelectedTickLabelColor)
    {
        mSelectedTickLabelColor = color;
    }
}

/*!
  Sets the color that is used for the axis label when it is selected.

  \see setLabelColor, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedLabelColor(const QColor &color)
{
    mSelectedLabelColor = color;
}

/*!
  Sets the pen that is used to draw the axis base line when selected.

  \see setBasePen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedBasePen(const QPen &pen)
{
    mSelectedBasePen = pen;
}

/*!
  Sets the pen that is used to draw the (major) ticks when selected.

  \see setTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedTickPen(const QPen &pen)
{
    mSelectedTickPen = pen;
}

/*!
  Sets the pen that is used to draw the subticks when selected.

  \see setSubTickPen, setSelectableParts, setSelectedParts, QCustomPlot::setInteractions
*/
void QCPAxis::setSelectedSubTickPen(const QPen &pen)
{
    mSelectedSubTickPen = pen;
}

/*!
  Sets the style for the lower axis ending. See the documentation of QCPLineEnding for available
  styles.

  For horizontal axes, this method refers to the left ending, for vertical axes the bottom ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.

  \see setUpperEnding
*/
void QCPAxis::setLowerEnding(const QCPLineEnding &ending)
{
    mAxisPainter->lowerEnding = ending;
}

/*!
  Sets the style for the upper axis ending. See the documentation of QCPLineEnding for available
  styles.

  For horizontal axes, this method refers to the right ending, for vertical axes the top ending.
  Note that this meaning does not change when the axis range is reversed with \ref
  setRangeReversed.

  \see setLowerEnding
*/
void QCPAxis::setUpperEnding(const QCPLineEnding &ending)
{
    mAxisPainter->upperEnding = ending;
}

/*!
  If the scale type (\ref setScaleType) is \ref stLinear, \a diff is added to the lower and upper
  bounds of the range. The range is simply moved by \a diff.

  If the scale type is \ref stLogarithmic, the range bounds are multiplied by \a diff. This
  corresponds to an apparent "linear" move in logarithmic scaling by a distance of log(diff).
*/
void QCPAxis::moveRange(double diff)
{
    QCPRange oldRange = mRange;
    if (mScaleType == stLinear)
    {
        mRange.lower += diff;
        mRange.upper += diff;
    } else // mScaleType == stLogarithmic
    {
        mRange.lower *= diff;
        mRange.upper *= diff;
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis by \a factor around the center of the current axis range. For
  example, if \a factor is 2.0, then the axis range will double its size, and the point at the axis
  range center won't have changed its position in the QCustomPlot widget (i.e. coordinates around
  the center will have moved symmetrically closer).

  If you wish to scale around a different coordinate than the current axis range center, use the
  overload \ref scaleRange(double factor, double center).
*/
void QCPAxis::scaleRange(double factor)
{
    scaleRange(factor, range().center());
}

/*! \overload

  Scales the range of this axis by \a factor around the coordinate \a center. For example, if \a
  factor is 2.0, \a center is 1.0, then the axis range will double its size, and the point at
  coordinate 1.0 won't have changed its position in the QCustomPlot widget (i.e. coordinates
  around 1.0 will have moved symmetrically closer to 1.0).

  \see scaleRange(double factor)
*/
void QCPAxis::scaleRange(double factor, double center)
{
    QCPRange oldRange = mRange;
    if (mScaleType == stLinear)
    {
        QCPRange newRange;
        newRange.lower = (mRange.lower-center)*factor + center;
        newRange.upper = (mRange.upper-center)*factor + center;
        if (QCPRange::validRange(newRange))
            mRange = newRange.sanitizedForLinScale();
    } else // mScaleType == stLogarithmic
    {
        if ((mRange.upper < 0 && center < 0) || (mRange.upper > 0 && center > 0)) // make sure center has same sign as range
        {
            QCPRange newRange;
            newRange.lower = qPow(mRange.lower/center, factor)*center;
            newRange.upper = qPow(mRange.upper/center, factor)*center;
            if (QCPRange::validRange(newRange))
                mRange = newRange.sanitizedForLogScale();
        } else
            qDebug() << Q_FUNC_INFO << "Center of scaling operation doesn't lie in same logarithmic sign domain as range:" << center;
    }
    emit rangeChanged(mRange);
    emit rangeChanged(mRange, oldRange);
}

/*!
  Scales the range of this axis to have a certain scale \a ratio to \a otherAxis. The scaling will
  be done around the center of the current axis range.

  For example, if \a ratio is 1, this axis is the \a yAxis and \a otherAxis is \a xAxis, graphs
  plotted with those axes will appear in a 1:1 aspect ratio, independent of the aspect ratio the
  axis rect has.

  This is an operation that changes the range of this axis once, it doesn't fix the scale ratio
  indefinitely. Note that calling this function in the constructor of the QCustomPlot's parent
  won't have the desired effect, since the widget dimensions aren't defined yet, and a resizeEvent
  will follow.
*/
void QCPAxis::setScaleRatio(const QCPAxis *otherAxis, double ratio)
{
    int otherPixelSize, ownPixelSize;

    if (otherAxis->orientation() == Qt::Horizontal)
        otherPixelSize = otherAxis->axisRect()->width();
    else
        otherPixelSize = otherAxis->axisRect()->height();

    if (orientation() == Qt::Horizontal)
        ownPixelSize = axisRect()->width();
    else
        ownPixelSize = axisRect()->height();

    double newRangeSize = ratio*otherAxis->range().size()*ownPixelSize/double(otherPixelSize);
    setRange(range().center(), newRangeSize, Qt::AlignCenter);
}

/*!
  Changes the axis range such that all plottables associated with this axis are fully visible in
  that dimension.

  \see QCPAbstractPlottable::rescaleAxes, QCustomPlot::rescaleAxes
*/
void QCPAxis::rescale(bool onlyVisiblePlottables)
{
    QCPRange newRange;
    bool haveRange = false;
    foreach (QCPAbstractPlottable *plottable, plottables())
    {
        if (!plottable->realVisibility() && onlyVisiblePlottables)
            continue;
        QCPRange plottableRange;
        bool currentFoundRange;
        QCP::SignDomain signDomain = QCP::sdBoth;
        if (mScaleType == stLogarithmic)
            signDomain = (mRange.upper < 0 ? QCP::sdNegative : QCP::sdPositive);
        if (plottable->keyAxis() == this)
            plottableRange = plottable->getKeyRange(currentFoundRange, signDomain);
        else
            plottableRange = plottable->getValueRange(currentFoundRange, signDomain);
        if (currentFoundRange)
        {
            if (!haveRange)
                newRange = plottableRange;
            else
                newRange.expand(plottableRange);
            haveRange = true;
        }
    }
    if (haveRange)
    {
        if (!QCPRange::validRange(newRange)) // likely due to range being zero (plottable has only constant data in this axis dimension), shift current range to at least center the plottable
        {
            double center = (newRange.lower+newRange.upper)*0.5; // upper and lower should be equal anyway, but just to make sure, incase validRange returned false for other reason
            if (mScaleType == stLinear)
            {
                newRange.lower = center-mRange.size()/2.0;
                newRange.upper = center+mRange.size()/2.0;
            } else // mScaleType == stLogarithmic
            {
                newRange.lower = center/qSqrt(mRange.upper/mRange.lower);
                newRange.upper = center*qSqrt(mRange.upper/mRange.lower);
            }
        }
        setRange(newRange);
    }
}

/*!
  Transforms \a value, in pixel coordinates of the QCustomPlot widget, to axis coordinates.
*/
double QCPAxis::pixelToCoord(double value) const
{
    if (orientation() == Qt::Horizontal)
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (value-mAxisRect->left())/double(mAxisRect->width())*mRange.size()+mRange.lower;
            else
                return -(value-mAxisRect->left())/double(mAxisRect->width())*mRange.size()+mRange.upper;
        } else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
                return qPow(mRange.upper/mRange.lower, (value-mAxisRect->left())/double(mAxisRect->width()))*mRange.lower;
            else
                return qPow(mRange.upper/mRange.lower, (mAxisRect->left()-value)/double(mAxisRect->width()))*mRange.upper;
        }
    } else // orientation() == Qt::Vertical
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (mAxisRect->bottom()-value)/double(mAxisRect->height())*mRange.size()+mRange.lower;
            else
                return -(mAxisRect->bottom()-value)/double(mAxisRect->height())*mRange.size()+mRange.upper;
        } else // mScaleType == stLogarithmic
        {
            if (!mRangeReversed)
                return qPow(mRange.upper/mRange.lower, (mAxisRect->bottom()-value)/double(mAxisRect->height()))*mRange.lower;
            else
                return qPow(mRange.upper/mRange.lower, (value-mAxisRect->bottom())/double(mAxisRect->height()))*mRange.upper;
        }
    }
}

/*!
  Transforms \a value, in coordinates of the axis, to pixel coordinates of the QCustomPlot widget.
*/
double QCPAxis::coordToPixel(double value) const
{
    if (orientation() == Qt::Horizontal)
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return (value-mRange.lower)/mRange.size()*mAxisRect->width()+mAxisRect->left();
            else
                return (mRange.upper-value)/mRange.size()*mAxisRect->width()+mAxisRect->left();
        } else // mScaleType == stLogarithmic
        {
            if (value >= 0.0 && mRange.upper < 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->right()+200 : mAxisRect->left()-200;
            else if (value <= 0.0 && mRange.upper >= 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->left()-200 : mAxisRect->right()+200;
            else
            {
                if (!mRangeReversed)
                    return qLn(value/mRange.lower)/qLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
                else
                    return qLn(mRange.upper/value)/qLn(mRange.upper/mRange.lower)*mAxisRect->width()+mAxisRect->left();
            }
        }
    } else // orientation() == Qt::Vertical
    {
        if (mScaleType == stLinear)
        {
            if (!mRangeReversed)
                return mAxisRect->bottom()-(value-mRange.lower)/mRange.size()*mAxisRect->height();
            else
                return mAxisRect->bottom()-(mRange.upper-value)/mRange.size()*mAxisRect->height();
        } else // mScaleType == stLogarithmic
        {
            if (value >= 0.0 && mRange.upper < 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->top()-200 : mAxisRect->bottom()+200;
            else if (value <= 0.0 && mRange.upper >= 0.0) // invalid value for logarithmic scale, just draw it outside visible range
                return !mRangeReversed ? mAxisRect->bottom()+200 : mAxisRect->top()-200;
            else
            {
                if (!mRangeReversed)
                    return mAxisRect->bottom()-qLn(value/mRange.lower)/qLn(mRange.upper/mRange.lower)*mAxisRect->height();
                else
                    return mAxisRect->bottom()-qLn(mRange.upper/value)/qLn(mRange.upper/mRange.lower)*mAxisRect->height();
            }
        }
    }
}

/*!
  Returns the part of the axis that is hit by \a pos (in pixels). The return value of this function
  is independent of the user-selectable parts defined with \ref setSelectableParts. Further, this
  function does not change the current selection state of the axis.

  If the axis is not visible (\ref setVisible), this function always returns \ref spNone.

  \see setSelectedParts, setSelectableParts, QCustomPlot::setInteractions
*/
QCPAxis::SelectablePart QCPAxis::getPartAt(const QPointF &pos) const
{
    if (!mVisible)
        return spNone;

    if (mAxisPainter->axisSelectionBox().contains(pos.toPoint()))
        return spAxis;
    else if (mAxisPainter->tickLabelsSelectionBox().contains(pos.toPoint()))
        return spTickLabels;
    else if (mAxisPainter->labelSelectionBox().contains(pos.toPoint()))
        return spAxisLabel;
    else
        return spNone;
}

/* inherits documentation from base class */
double QCPAxis::selectTest(const QPointF &pos, bool onlySelectable, QVariant *details) const
{
    if (!mParentPlot) return -1;
    SelectablePart part = getPartAt(pos);
    if ((onlySelectable && !mSelectableParts.testFlag(part)) || part == spNone)
        return -1;

    if (details)
        details->setValue(part);
    return mParentPlot->selectionTolerance()*0.99;
}

/*!
  Returns a list of all the plottables that have this axis as key or value axis.

  If you are only interested in plottables of type QCPGraph, see \ref graphs.

  \see graphs, items
*/
QList<QCPAbstractPlottable*> QCPAxis::plottables() const
{
    QList<QCPAbstractPlottable*> result;
    if (!mParentPlot) return result;

    foreach (QCPAbstractPlottable *plottable, mParentPlot->mPlottables)
    {
        if (plottable->keyAxis() == this || plottable->valueAxis() == this)
            result.append(plottable);
    }
    return result;
}

/*!
  Returns a list of all the graphs that have this axis as key or value axis.

  \see plottables, items
*/
QList<QCPGraph*> QCPAxis::graphs() const
{
    QList<QCPGraph*> result;
    if (!mParentPlot) return result;

    foreach (QCPGraph *graph, mParentPlot->mGraphs)
    {
        if (graph->keyAxis() == this || graph->valueAxis() == this)
            result.append(graph);
    }
    return result;
}

/*!
  Returns a list of all the items that are associated with this axis. An item is considered
  associated with an axis if at least one of its positions uses the axis as key or value axis.

  \see plottables, graphs
*/
QList<QCPAbstractItem*> QCPAxis::items() const
{
    QList<QCPAbstractItem*> result;
    if (!mParentPlot) return result;

    foreach (QCPAbstractItem *item, mParentPlot->mItems)
    {
        foreach (QCPItemPosition *position, item->positions())
        {
            if (position->keyAxis() == this || position->valueAxis() == this)
            {
                result.append(item);
                break;
            }
        }
    }
    return result;
}

/*!
  Transforms a margin side to the logically corresponding axis type. (QCP::msLeft to
  QCPAxis::atLeft, QCP::msRight to QCPAxis::atRight, etc.)
*/
QCPAxis::AxisType QCPAxis::marginSideToAxisType(QCP::MarginSide side)
{
    switch (side)
    {
        case QCP::msLeft: return atLeft;
        case QCP::msRight: return atRight;
        case QCP::msTop: return atTop;
        case QCP::msBottom: return atBottom;
        default: break;
    }
    qDebug() << Q_FUNC_INFO << "Invalid margin side passed:" << static_cast<int>(side);
    return atLeft;
}

/*!
  Returns the axis type that describes the opposite axis of an axis with the specified \a type.
*/
QCPAxis::AxisType QCPAxis::opposite(QCPAxis::AxisType type)
{
    switch (type)
    {
        case atLeft: return atRight;
        case atRight: return atLeft;
        case atBottom: return atTop;
        case atTop: return atBottom;
    }
    qDebug() << Q_FUNC_INFO << "invalid axis type";
    return atLeft;
}

/* inherits documentation from base class */
void QCPAxis::selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged)
{
    Q_UNUSED(event)
    SelectablePart part = details.value<SelectablePart>();
    if (mSelectableParts.testFlag(part))
    {
        SelectableParts selBefore = mSelectedParts;
        setSelectedParts(additive ? mSelectedParts^part : part);
        if (selectionStateChanged)
            *selectionStateChanged = mSelectedParts != selBefore;
    }
}

/* inherits documentation from base class */
void QCPAxis::deselectEvent(bool *selectionStateChanged)
{
    SelectableParts selBefore = mSelectedParts;
    setSelectedParts(mSelectedParts & ~mSelectableParts);
    if (selectionStateChanged)
        *selectionStateChanged = mSelectedParts != selBefore;
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.

  For the axis to accept this event and perform the single axis drag, the parent \ref QCPAxisRect
  must be configured accordingly, i.e. it must allow range dragging in the orientation of this axis
  (\ref QCPAxisRect::setRangeDrag) and this axis must be a draggable axis (\ref
  QCPAxisRect::setRangeDragAxes)

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.
*/
void QCPAxis::mousePressEvent(QMouseEvent *event, const QVariant &details)
{
    Q_UNUSED(details)
    if (!mParentPlot->interactions().testFlag(QCP::iRangeDrag) ||
        !mAxisRect->rangeDrag().testFlag(orientation()) ||
        !mAxisRect->rangeDragAxes(orientation()).contains(this))
    {
        event->ignore();
        return;
    }

    if (event->buttons() & Qt::LeftButton)
    {
        mDragging = true;
// initialize antialiasing backup in case we start dragging:
        if (mParentPlot->noAntialiasingOnDrag())
        {
            mAADragBackup = mParentPlot->antialiasedElements();
            mNotAADragBackup = mParentPlot->notAntialiasedElements();
        }
// Mouse range dragging interaction:
        if (mParentPlot->interactions().testFlag(QCP::iRangeDrag))
            mDragStartRange = mRange;
    }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.

  \see QCPAxis::mousePressEvent
*/
void QCPAxis::mouseMoveEvent(QMouseEvent *event, const QPointF &startPos)
{
    if (mDragging)
    {
        const double startPixel = orientation() == Qt::Horizontal ? startPos.x() : startPos.y();
        const double currentPixel = orientation() == Qt::Horizontal ? event->pos().x() : event->pos().y();
        if (mScaleType == QCPAxis::stLinear)
        {
            const double diff = pixelToCoord(startPixel) - pixelToCoord(currentPixel);
            setRange(mDragStartRange.lower+diff, mDragStartRange.upper+diff);
        } else if (mScaleType == QCPAxis::stLogarithmic)
        {
            const double diff = pixelToCoord(startPixel) / pixelToCoord(currentPixel);
            setRange(mDragStartRange.lower*diff, mDragStartRange.upper*diff);
        }

        if (mParentPlot->noAntialiasingOnDrag())
            mParentPlot->setNotAntialiasedElements(QCP::aeAll);
        mParentPlot->replot(QCustomPlot::rpQueuedReplot);
    }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user drag individual axes
  exclusively, by startig the drag on top of the axis.

  \seebaseclassmethod

  \note The dragging of possibly multiple axes at once by starting the drag anywhere in the axis
  rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::mousePressEvent.

  \see QCPAxis::mousePressEvent
*/
void QCPAxis::mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos)
{
    Q_UNUSED(event)
    Q_UNUSED(startPos)
    mDragging = false;
    if (mParentPlot->noAntialiasingOnDrag())
    {
        mParentPlot->setAntialiasedElements(mAADragBackup);
        mParentPlot->setNotAntialiasedElements(mNotAADragBackup);
    }
}

/*! \internal

  This mouse event reimplementation provides the functionality to let the user zoom individual axes
  exclusively, by performing the wheel event on top of the axis.

  For the axis to accept this event and perform the single axis zoom, the parent \ref QCPAxisRect
  must be configured accordingly, i.e. it must allow range zooming in the orientation of this axis
  (\ref QCPAxisRect::setRangeZoom) and this axis must be a zoomable axis (\ref
  QCPAxisRect::setRangeZoomAxes)

  \seebaseclassmethod

  \note The zooming of possibly multiple axes at once by performing the wheel event anywhere in the
  axis rect is handled by the axis rect's mouse event, e.g. \ref QCPAxisRect::wheelEvent.
*/
void QCPAxis::wheelEvent(QWheelEvent *event)
{
// Mouse range zooming interaction:
    if (!mParentPlot->interactions().testFlag(QCP::iRangeZoom) ||
        !mAxisRect->rangeZoom().testFlag(orientation()) ||
        !mAxisRect->rangeZoomAxes(orientation()).contains(this))
    {
        event->ignore();
        return;
    }

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
    const double delta = event->delta();
#else
    const double delta = event->angleDelta().y();
#endif

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    const QPointF pos = event->pos();
#else
    const QPointF pos = event->position();
#endif

    const double wheelSteps = delta/120.0; // a single step delta is +/-120 usually
    const double factor = qPow(mAxisRect->rangeZoomFactor(orientation()), wheelSteps);
    scaleRange(factor, pixelToCoord(orientation() == Qt::Horizontal ? pos.x() : pos.y()));
    mParentPlot->replot();
}

/*! \internal

  A convenience function to easily set the QPainter::Antialiased hint on the provided \a painter
  before drawing axis lines.

  This is the antialiasing state the painter passed to the \ref draw method is in by default.

  This function takes into account the local setting of the antialiasing flag as well as the
  overrides set with \ref QCustomPlot::setAntialiasedElements and \ref
  QCustomPlot::setNotAntialiasedElements.

  \seebaseclassmethod

  \see setAntialiased
*/
void QCPAxis::applyDefaultAntialiasingHint(QCPPainter *painter) const
{
    applyAntialiasingHint(painter, mAntialiased, QCP::aeAxes);
}

/*! \internal

  Draws the axis with the specified \a painter, using the internal QCPAxisPainterPrivate instance.

  \seebaseclassmethod
*/
void QCPAxis::draw(QCPPainter *painter)
{
    QVector<double> subTickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
    tickPositions.reserve(mTickVector.size());
    tickLabels.reserve(mTickVector.size());
    subTickPositions.reserve(mSubTickVector.size());

    if (mTicks)
    {
        for (int i=0; i<mTickVector.size(); ++i)
        {
            tickPositions.append(coordToPixel(mTickVector.at(i)));
            if (mTickLabels)
                tickLabels.append(mTickVectorLabels.at(i));
        }

        if (mSubTicks)
        {
            const int subTickCount = mSubTickVector.size();
            for (int i=0; i<subTickCount; ++i)
                subTickPositions.append(coordToPixel(mSubTickVector.at(i)));
        }
    }

// transfer all properties of this axis to QCPAxisPainterPrivate which it needs to draw the axis.
// Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
    mAxisPainter->type = mAxisType;
    mAxisPainter->basePen = getBasePen();
    mAxisPainter->labelFont = getLabelFont();
    mAxisPainter->labelColor = getLabelColor();
    mAxisPainter->label = mLabel;
    mAxisPainter->substituteExponent = mNumberBeautifulPowers;
    mAxisPainter->tickPen = getTickPen();
    mAxisPainter->subTickPen = getSubTickPen();
    mAxisPainter->tickLabelFont = getTickLabelFont();
    mAxisPainter->tickLabelColor = getTickLabelColor();
    mAxisPainter->axisRect = mAxisRect->rect();
    mAxisPainter->viewportRect = mParentPlot->viewport();
    mAxisPainter->abbreviateDecimalPowers = mScaleType == stLogarithmic;
    mAxisPainter->reversedEndings = mRangeReversed;
    mAxisPainter->tickPositions = tickPositions;
    mAxisPainter->tickLabels = tickLabels;
    mAxisPainter->subTickPositions = subTickPositions;
    mAxisPainter->draw(painter);
}

/*! \internal

  Prepares the internal tick vector, sub tick vector and tick label vector. This is done by calling
  QCPAxisTicker::generate on the currently installed ticker.

  If a change in the label text/count is detected, the cached axis margin is invalidated to make
  sure the next margin calculation recalculates the label sizes and returns an up-to-date value.
*/
void QCPAxis::setupTickVectors()
{
    if (!mParentPlot) return;
    if ((!mTicks && !mTickLabels && !mGrid->visible()) || mRange.size() <= 0) return;

    QVector<QString> oldLabels = mTickVectorLabels;
    mTicker->generate(mRange, mParentPlot->locale(), mNumberFormatChar, mNumberPrecision, mTickVector, mSubTicks ? &mSubTickVector : nullptr, mTickLabels ? &mTickVectorLabels : nullptr);
    mCachedMarginValid &= mTickVectorLabels == oldLabels; // if labels have changed, margin might have changed, too
}

/*! \internal

  Returns the pen that is used to draw the axis base line. Depending on the selection state, this
  is either mSelectedBasePen or mBasePen.
*/
QPen QCPAxis::getBasePen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedBasePen : mBasePen;
}

/*! \internal

  Returns the pen that is used to draw the (major) ticks. Depending on the selection state, this
  is either mSelectedTickPen or mTickPen.
*/
QPen QCPAxis::getTickPen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedTickPen : mTickPen;
}

/*! \internal

  Returns the pen that is used to draw the subticks. Depending on the selection state, this
  is either mSelectedSubTickPen or mSubTickPen.
*/
QPen QCPAxis::getSubTickPen() const
{
    return mSelectedParts.testFlag(spAxis) ? mSelectedSubTickPen : mSubTickPen;
}

/*! \internal

  Returns the font that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelFont or mTickLabelFont.
*/
QFont QCPAxis::getTickLabelFont() const
{
    return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelFont : mTickLabelFont;
}

/*! \internal

  Returns the font that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelFont or mLabelFont.
*/
QFont QCPAxis::getLabelFont() const
{
    return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelFont : mLabelFont;
}

/*! \internal

  Returns the color that is used to draw the tick labels. Depending on the selection state, this
  is either mSelectedTickLabelColor or mTickLabelColor.
*/
QColor QCPAxis::getTickLabelColor() const
{
    return mSelectedParts.testFlag(spTickLabels) ? mSelectedTickLabelColor : mTickLabelColor;
}

/*! \internal

  Returns the color that is used to draw the axis label. Depending on the selection state, this
  is either mSelectedLabelColor or mLabelColor.
*/
QColor QCPAxis::getLabelColor() const
{
    return mSelectedParts.testFlag(spAxisLabel) ? mSelectedLabelColor : mLabelColor;
}

/*! \internal

  Returns the appropriate outward margin for this axis. It is needed if \ref
  QCPAxisRect::setAutoMargins is set to true on the parent axis rect. An axis with axis type \ref
  atLeft will return an appropriate left margin, \ref atBottom will return an appropriate bottom
  margin and so forth. For the calculation, this function goes through similar steps as \ref draw,
  so changing one function likely requires the modification of the other one as well.

  The margin consists of the outward tick length, tick label padding, tick label size, label
  padding, label size, and padding.

  The margin is cached internally, so repeated calls while leaving the axis range, fonts, etc.
  unchanged are very fast.
*/
int QCPAxis::calculateMargin()
{
    if (!mVisible) // if not visible, directly return 0, don't cache 0 because we can't react to setVisible in QCPAxis
        return 0;

    if (mCachedMarginValid)
        return mCachedMargin;

// run through similar steps as QCPAxis::draw, and calculate margin needed to fit axis and its labels
    int margin = 0;

    QVector<double> tickPositions; // the final coordToPixel transformed vector passed to QCPAxisPainter
    QVector<QString> tickLabels; // the final vector passed to QCPAxisPainter
    tickPositions.reserve(mTickVector.size());
    tickLabels.reserve(mTickVector.size());

    if (mTicks)
    {
        for (int i=0; i<mTickVector.size(); ++i)
        {
            tickPositions.append(coordToPixel(mTickVector.at(i)));
            if (mTickLabels)
                tickLabels.append(mTickVectorLabels.at(i));
        }
    }
// transfer all properties of this axis to QCPAxisPainterPrivate which it needs to calculate the size.
// Note that some axis painter properties are already set by direct feed-through with QCPAxis setters
    mAxisPainter->type = mAxisType;
    mAxisPainter->labelFont = getLabelFont();
    mAxisPainter->label = mLabel;
    mAxisPainter->tickLabelFont = mTickLabelFont;
    mAxisPainter->axisRect = mAxisRect->rect();
    mAxisPainter->viewportRect = mParentPlot->viewport();
    mAxisPainter->tickPositions = tickPositions;
    mAxisPainter->tickLabels = tickLabels;
    margin += mAxisPainter->size();
    margin += mPadding;

    mCachedMargin = margin;
    mCachedMarginValid = true;
    return margin;
}

/* inherits documentation from base class */
Interaction QCPAxis::selectionCategory() const
{
    return iSelectAxes;
}