#ifndef QCUSTOMPLOT_QCPAXIS_HPP
#define QCUSTOMPLOT_QCPAXIS_HPP

#include "defs.hpp"
#include "QCPLayerable.hpp"
#include "QCPAxisTicker.hpp"
#include "QCPGrid.hpp"
#include "QCPLineEnding.hpp"
#include "QCPAxisPainterPrivate.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPAxis : public QCPLayerable {
    Q_OBJECT
        /// \cond INCLUDE_QPROPERTIES
        Q_PROPERTY(AxisType axisType READ axisType)
        Q_PROPERTY(QCPAxisRect * axisRect READ axisRect)
        Q_PROPERTY(ScaleType scaleType READ scaleType WRITE setScaleType NOTIFY scaleTypeChanged)
        Q_PROPERTY(QCPRange range READ range WRITE setRange NOTIFY rangeChanged)
        Q_PROPERTY(bool rangeReversed READ rangeReversed WRITE setRangeReversed)
        Q_PROPERTY(QSharedPointer<QCPAxisTicker> ticker READ ticker WRITE setTicker)
        Q_PROPERTY(bool ticks READ ticks WRITE setTicks)
        Q_PROPERTY(bool tickLabels READ tickLabels WRITE setTickLabels)
        Q_PROPERTY(int tickLabelPadding READ tickLabelPadding WRITE setTickLabelPadding)
        Q_PROPERTY(QFont tickLabelFont READ tickLabelFont WRITE setTickLabelFont)
        Q_PROPERTY(QColor tickLabelColor READ tickLabelColor WRITE setTickLabelColor)
        Q_PROPERTY(double tickLabelRotation READ tickLabelRotation WRITE setTickLabelRotation)
        Q_PROPERTY(LabelSide tickLabelSide READ tickLabelSide WRITE setTickLabelSide)
        Q_PROPERTY(QString numberFormat READ numberFormat WRITE setNumberFormat)
        Q_PROPERTY(int numberPrecision READ numberPrecision WRITE setNumberPrecision)
        Q_PROPERTY(QVector<double> tickVector READ tickVector)
        Q_PROPERTY(QVector<QString> tickVectorLabels READ tickVectorLabels)
        Q_PROPERTY(int tickLengthIn READ tickLengthIn WRITE setTickLengthIn)
        Q_PROPERTY(int tickLengthOut READ tickLengthOut WRITE setTickLengthOut)
        Q_PROPERTY(bool subTicks READ subTicks WRITE setSubTicks)
        Q_PROPERTY(int subTickLengthIn READ subTickLengthIn WRITE setSubTickLengthIn)
        Q_PROPERTY(int subTickLengthOut READ subTickLengthOut WRITE setSubTickLengthOut)
        Q_PROPERTY(QPen basePen READ basePen WRITE setBasePen)
        Q_PROPERTY(QPen tickPen READ tickPen WRITE setTickPen)
        Q_PROPERTY(QPen subTickPen READ subTickPen WRITE setSubTickPen)
        Q_PROPERTY(QFont labelFont READ labelFont WRITE setLabelFont)
        Q_PROPERTY(QColor labelColor READ labelColor WRITE setLabelColor)
        Q_PROPERTY(QString label READ label WRITE setLabel)
        Q_PROPERTY(int labelPadding READ labelPadding WRITE setLabelPadding)
        Q_PROPERTY(int padding READ padding WRITE setPadding)
        Q_PROPERTY(int offset READ offset WRITE setOffset)
        Q_PROPERTY(SelectableParts selectedParts READ selectedParts WRITE setSelectedParts NOTIFY selectionChanged)
        Q_PROPERTY(
                SelectableParts selectableParts READ selectableParts WRITE setSelectableParts NOTIFY selectableChanged)
        Q_PROPERTY(QFont selectedTickLabelFont READ selectedTickLabelFont WRITE setSelectedTickLabelFont)
        Q_PROPERTY(QFont selectedLabelFont READ selectedLabelFont WRITE setSelectedLabelFont)
        Q_PROPERTY(QColor selectedTickLabelColor READ selectedTickLabelColor WRITE setSelectedTickLabelColor)
        Q_PROPERTY(QColor selectedLabelColor READ selectedLabelColor WRITE setSelectedLabelColor)
        Q_PROPERTY(QPen selectedBasePen READ selectedBasePen WRITE setSelectedBasePen)
        Q_PROPERTY(QPen selectedTickPen READ selectedTickPen WRITE setSelectedTickPen)
        Q_PROPERTY(QPen selectedSubTickPen READ selectedSubTickPen WRITE setSelectedSubTickPen)
        Q_PROPERTY(QCPLineEnding lowerEnding READ lowerEnding WRITE setLowerEnding)
        Q_PROPERTY(QCPLineEnding upperEnding READ upperEnding WRITE setUpperEnding)
        Q_PROPERTY(QCPGrid *grid READ grid)
        /// \endcond
    public:
        /*!
        Defines at which side of the axis rect the axis will appear. This also affects how the tick
        marks are drawn, on which side the labels are placed etc.
      */
        enum AxisType {
            atLeft = 0x01,  ///< <tt>0x01</tt> Axis is vertical and on the left side of the axis rect
            atRight = 0x02, ///< <tt>0x02</tt> Axis is vertical and on the right side of the axis rect
            atTop = 0x04, ///< <tt>0x04</tt> Axis is horizontal and on the top side of the axis rect
            atBottom = 0x08, ///< <tt>0x08</tt> Axis is horizontal and on the bottom side of the axis rect
        };

        Q_ENUM(AxisType)

        Q_FLAGS(AxisTypes)
        Q_DECLARE_FLAGS(AxisTypes, AxisType)
        /*!
        Defines on which side of the axis the tick labels (numbers) shall appear.

        \see setTickLabelSide
      */
        enum LabelSide {
            lsInside    ///< Tick labels will be displayed inside the axis rect and clipped to the inner axis rect
            , lsOutside  ///< Tick labels will be displayed outside the axis rect
        };

        Q_ENUM(LabelSide)

        /*!
        Defines the scale of an axis.
        \see setScaleType
      */
        enum ScaleType {
            stLinear,      ///< Linear scaling
            stLogarithmic ///< Logarithmic scaling with correspondingly transformed axis coordinates (possibly also \ref setTicker to a \ref QCPAxisTickerLog instance).
        };

        Q_ENUM(ScaleType)

        /*!
        Defines the selectable parts of an axis.
        \see setSelectableParts, setSelectedParts
      */
        enum SelectablePart {
            spNone = 0,   ///< None of the selectable parts
            spAxis = 0x001, ///< The axis backbone and tick marks
            spTickLabels = 0x002, ///< Tick labels (numbers) of this axis (as a whole, not individually)
            spAxisLabel = 0x004  ///< The axis label
        };

        Q_ENUM(SelectablePart)

        Q_FLAGS(SelectableParts)
        Q_DECLARE_FLAGS(SelectableParts, SelectablePart)

        Q_DISABLE_COPY(QCPAxis)

        QCPAxis(QCPAxisRect *parent, AxisType type);

        ~QCPAxis() override;

        // getters:
        AxisType axisType() const { return mAxisType; }

        QCPAxisRect *axisRect() const { return mAxisRect; }

        ScaleType scaleType() const { return mScaleType; }

        const QCPRange range() const { return mRange; }

        bool rangeReversed() const { return mRangeReversed; }

        QSharedPointer<QCPAxisTicker> ticker() const { return mTicker; }

        bool ticks() const { return mTicks; }

        bool tickLabels() const { return mTickLabels; }

        int tickLabelPadding() const;

        QFont tickLabelFont() const { return mTickLabelFont; }

        QColor tickLabelColor() const { return mTickLabelColor; }

        double tickLabelRotation() const;

        LabelSide tickLabelSide() const;

        QString numberFormat() const;

        int numberPrecision() const { return mNumberPrecision; }

        QVector<double> tickVector() const { return mTickVector; }

        QVector<QString> tickVectorLabels() const { return mTickVectorLabels; }

        int tickLengthIn() const;

        int tickLengthOut() const;

        bool subTicks() const { return mSubTicks; }

        int subTickLengthIn() const;

        int subTickLengthOut() const;

        QPen basePen() const { return mBasePen; }

        QPen tickPen() const { return mTickPen; }

        QPen subTickPen() const { return mSubTickPen; }

        QFont labelFont() const { return mLabelFont; }

        QColor labelColor() const { return mLabelColor; }

        QString label() const { return mLabel; }

        int labelPadding() const;

        int padding() const { return mPadding; }

        int offset() const;

        SelectableParts selectedParts() const { return mSelectedParts; }

        SelectableParts selectableParts() const { return mSelectableParts; }

        QFont selectedTickLabelFont() const { return mSelectedTickLabelFont; }

        QFont selectedLabelFont() const { return mSelectedLabelFont; }

        QColor selectedTickLabelColor() const { return mSelectedTickLabelColor; }

        QColor selectedLabelColor() const { return mSelectedLabelColor; }

        QPen selectedBasePen() const { return mSelectedBasePen; }

        QPen selectedTickPen() const { return mSelectedTickPen; }

        QPen selectedSubTickPen() const { return mSelectedSubTickPen; }

        QCPLineEnding lowerEnding() const;

        QCPLineEnding upperEnding() const;

        QCPGrid *grid() const { return mGrid; }

        Q_SLOT void setScaleType(QCPAxis::ScaleType type);

        Q_SLOT void setRange(const QCPRange &range);

        void setRange(double lower, double upper);

        void setRange(double position, double size, Qt::AlignmentFlag alignment);

        void setRangeLower(double lower);

        void setRangeUpper(double upper);

        void setRangeReversed(bool reversed);

        void setTicker(QSharedPointer<QCPAxisTicker> ticker);

        void setTicks(bool show);

        void setTickLabels(bool show);

        void setTickLabelPadding(int padding);

        void setTickLabelFont(const QFont &font);

        void setTickLabelColor(const QColor &color);

        void setTickLabelRotation(double degrees);

        void setTickLabelSide(LabelSide side);

        void setNumberFormat(const QString &formatCode);

        void setNumberPrecision(int precision);

        void setTickLength(int inside, int outside = 0);

        void setTickLengthIn(int inside);

        void setTickLengthOut(int outside);

        void setSubTicks(bool show);

        void setSubTickLength(int inside, int outside = 0);

        void setSubTickLengthIn(int inside);

        void setSubTickLengthOut(int outside);

        void setBasePen(const QPen &pen);

        void setTickPen(const QPen &pen);

        void setSubTickPen(const QPen &pen);

        void setLabelFont(const QFont &font);

        void setLabelColor(const QColor &color);

        void setLabel(const QString &str);

        void setLabelPadding(int padding);

        void setPadding(int padding);

        void setOffset(int offset);

        void setSelectedTickLabelFont(const QFont &font);

        void setSelectedLabelFont(const QFont &font);

        void setSelectedTickLabelColor(const QColor &color);

        void setSelectedLabelColor(const QColor &color);

        void setSelectedBasePen(const QPen &pen);

        void setSelectedTickPen(const QPen &pen);

        void setSelectedSubTickPen(const QPen &pen);

        Q_SLOT void setSelectableParts(const QCPAxis::SelectableParts &selectableParts);

        Q_SLOT void setSelectedParts(const QCPAxis::SelectableParts &selectedParts);

        void setLowerEnding(const QCPLineEnding &ending);

        void setUpperEnding(const QCPLineEnding &ending);

        virtual double
        selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const Q_DECL_OVERRIDE;

        Qt::Orientation orientation() const { return mOrientation; }

        int pixelOrientation() const { return rangeReversed() != (orientation() == Qt::Vertical) ? -1 : 1; }

        void moveRange(double diff);

        void scaleRange(double factor);

        void scaleRange(double factor, double center);

        void setScaleRatio(const QCPAxis *otherAxis, double ratio = 1.0);

        void rescale(bool onlyVisiblePlottables = false);

        double pixelToCoord(double value) const;

        double coordToPixel(double value) const;

        SelectablePart getPartAt(const QPointF &pos) const;

        QList<QCPAbstractPlottable *> plottables() const;

        QList<QCPGraph *> graphs() const;

        QList<QCPAbstractItem *> items() const;

        static AxisType marginSideToAxisType(MarginSide side);

        static Qt::Orientation orientation(AxisType type) {
            return type == atBottom || type == atTop ? Qt::Horizontal : Qt::Vertical;
        }

        static AxisType opposite(AxisType type);

    signals:

        void rangeChanged(const QCPRange &newRange);

        void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);

        void scaleTypeChanged(QCPAxis::ScaleType scaleType);

        void selectionChanged(const QCPAxis::SelectableParts &parts);

        void selectableChanged(const QCPAxis::SelectableParts &parts);

    protected:
        // axis base:
        AxisType mAxisType;
        QCPAxisRect *mAxisRect;
        //int mOffset; // in QCPAxisPainter
        int mPadding;
        Qt::Orientation mOrientation;
        SelectableParts mSelectableParts, mSelectedParts;
        QPen mBasePen, mSelectedBasePen;
        //QCPLineEnding mLowerEnding, mUpperEnding; // in QCPAxisPainter
        // axis label:
        //int mLabelPadding; // in QCPAxisPainter
        QString mLabel;
        QFont mLabelFont, mSelectedLabelFont;
        QColor mLabelColor, mSelectedLabelColor;
        // tick labels:
        //int mTickLabelPadding; // in QCPAxisPainter
        bool mTickLabels;
        //double mTickLabelRotation; // in QCPAxisPainter
        QFont mTickLabelFont, mSelectedTickLabelFont;
        QColor mTickLabelColor, mSelectedTickLabelColor;
        int mNumberPrecision;
        QLatin1Char mNumberFormatChar;
        bool mNumberBeautifulPowers;
        //bool mNumberMultiplyCross; // QCPAxisPainter
        // ticks and subticks:
        bool mTicks;
        bool mSubTicks;
        //int mTickLengthIn, mTickLengthOut, mSubTickLengthIn, mSubTickLengthOut; // QCPAxisPainter
        QPen mTickPen, mSelectedTickPen;
        QPen mSubTickPen, mSelectedSubTickPen;
        // scale and range:
        QCPRange mRange;
        bool mRangeReversed;
        ScaleType mScaleType;

        QCPGrid *mGrid;
        QCPAxisPainterPrivate *mAxisPainter;
        QSharedPointer<QCPAxisTicker> mTicker;
        QVector<double> mTickVector;
        QVector<QString> mTickVectorLabels;
        QVector<double> mSubTickVector;
        bool mCachedMarginValid;
        int mCachedMargin;
        bool mDragging;
        QCPRange mDragStartRange;
        AntialiasedElements mAADragBackup, mNotAADragBackup;

        virtual int calculateMargin();

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        QCP::Interaction selectionCategory() const override;

        void
        selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;

        void mousePressEvent(QMouseEvent *event, const QVariant &details) override;

        void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) override;

        void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos) override;

        void wheelEvent(QWheelEvent *event) override;

        void setupTickVectors();

        QPen getBasePen() const;

        QPen getTickPen() const;

        QPen getSubTickPen() const;

        QFont getTickLabelFont() const;

        QFont getLabelFont() const;

        QColor getTickLabelColor() const;

        QColor getLabelColor() const;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::SelectableParts)

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPAxis::AxisTypes)

}
Q_DECLARE_METATYPE(QCP::QCPAxis::AxisType)

Q_DECLARE_METATYPE(QCP::QCPAxis::LabelSide)

Q_DECLARE_METATYPE(QCP::QCPAxis::ScaleType)

Q_DECLARE_METATYPE(QCP::QCPAxis::SelectablePart)

#endif
