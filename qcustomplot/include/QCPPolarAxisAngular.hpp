#ifndef QCUSTOMPLOT_QCPPOLARAXISANGULAR_HPP
#define QCUSTOMPLOT_QCPPOLARAXISANGULAR_HPP

#include "QCPLayoutElement.hpp"
#include "QCPPolarGrid.hpp"
#include "QCPLabelPainterPrivate.hpp"
#include "QCPPolarGraph.hpp"
#include "QCPPolarAxisRadial.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPPolarAxisAngular : public QCPLayoutElement {
    Q_OBJECT
    public:
        /*!
        Defines the selectable parts of an axis.
        \see setSelectableParts, setSelectedParts
      */
        enum SelectablePart {
            spNone = 0,    ///< None of the selectable parts
            spAxis = 0x001,  ///< The axis backbone and tick marks
            spTickLabels = 0x002, ///< Tick labels (numbers) of this axis (as a whole, not individually)
            spAxisLabel = 0x004  ///< The axis label
        };

        Q_ENUM(SelectablePart)

        Q_FLAGS(SelectableParts)
        Q_DECLARE_FLAGS(SelectableParts, SelectablePart)

        /*!
        TODO
      */
        enum LabelMode {
            lmUpright,  ///<
            lmRotated ///<
        };

        Q_ENUM(LabelMode)

        Q_DISABLE_COPY(QCPPolarAxisAngular)

        explicit QCPPolarAxisAngular(QCustomPlot *parentPlot);

        virtual ~QCPPolarAxisAngular();

        QPixmap background() const { return mBackgroundPixmap; }

        QBrush backgroundBrush() const { return mBackgroundBrush; }

        bool backgroundScaled() const { return mBackgroundScaled; }

        Qt::AspectRatioMode backgroundScaledMode() const { return mBackgroundScaledMode; }

        bool rangeDrag() const { return mRangeDrag; }

        bool rangeZoom() const { return mRangeZoom; }

        double rangeZoomFactor() const { return mRangeZoomFactor; }

        const QCPRange range() const { return mRange; }

        bool rangeReversed() const { return mRangeReversed; }

        double angle() const { return mAngle; }

        QSharedPointer<QCPAxisTicker> ticker() const { return mTicker; }

        bool ticks() const { return mTicks; }

        bool tickLabels() const { return mTickLabels; }

        int tickLabelPadding() const { return mLabelPainter.padding(); }

        QFont tickLabelFont() const { return mTickLabelFont; }

        QColor tickLabelColor() const { return mTickLabelColor; }

        double tickLabelRotation() const { return mLabelPainter.rotation(); }

        LabelMode tickLabelMode() const;

        QString numberFormat() const;

        int numberPrecision() const { return mNumberPrecision; }

        QVector<double> tickVector() const { return mTickVector; }

        QVector<QString> tickVectorLabels() const { return mTickVectorLabels; }

        int tickLengthIn() const { return mTickLengthIn; }

        int tickLengthOut() const { return mTickLengthOut; }

        bool subTicks() const { return mSubTicks; }

        int subTickLengthIn() const { return mSubTickLengthIn; }

        int subTickLengthOut() const { return mSubTickLengthOut; }

        QPen basePen() const { return mBasePen; }

        QPen tickPen() const { return mTickPen; }

        QPen subTickPen() const { return mSubTickPen; }

        QFont labelFont() const { return mLabelFont; }

        QColor labelColor() const { return mLabelColor; }

        QString label() const { return mLabel; }

        int labelPadding() const { return mLabelPadding; }

        SelectableParts selectedParts() const { return mSelectedParts; }

        SelectableParts selectableParts() const { return mSelectableParts; }

        QFont selectedTickLabelFont() const { return mSelectedTickLabelFont; }

        QFont selectedLabelFont() const { return mSelectedLabelFont; }

        QColor selectedTickLabelColor() const { return mSelectedTickLabelColor; }

        QColor selectedLabelColor() const { return mSelectedLabelColor; }

        QPen selectedBasePen() const { return mSelectedBasePen; }

        QPen selectedTickPen() const { return mSelectedTickPen; }

        QPen selectedSubTickPen() const { return mSelectedSubTickPen; }

        QCPPolarGrid *grid() const { return mGrid; }

        void setBackground(const QPixmap &pm);

        void setBackground(const QPixmap &pm, bool scaled, Qt::AspectRatioMode mode = Qt::KeepAspectRatioByExpanding);

        void setBackground(const QBrush &brush);

        void setBackgroundScaled(bool scaled);

        void setBackgroundScaledMode(Qt::AspectRatioMode mode);

        void setRangeDrag(bool enabled);

        void setRangeZoom(bool enabled);

        void setRangeZoomFactor(double factor);

        Q_SLOT void setRange(const QCPRange &range);

        void setRange(double lower, double upper);

        void setRange(double position, double size, Qt::AlignmentFlag alignment);

        void setRangeLower(double lower);

        void setRangeUpper(double upper);

        void setRangeReversed(bool reversed);

        void setAngle(double degrees);

        void setTicker(QSharedPointer<QCPAxisTicker> ticker);

        void setTicks(bool show);

        void setTickLabels(bool show);

        void setTickLabelPadding(int padding);

        void setTickLabelFont(const QFont &font);

        void setTickLabelColor(const QColor &color);

        void setTickLabelRotation(double degrees);

        void setTickLabelMode(LabelMode mode);

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

        void setLabelPosition(Qt::AlignmentFlag position);

        void setSelectedTickLabelFont(const QFont &font);

        void setSelectedLabelFont(const QFont &font);

        void setSelectedTickLabelColor(const QColor &color);

        void setSelectedLabelColor(const QColor &color);

        void setSelectedBasePen(const QPen &pen);

        void setSelectedTickPen(const QPen &pen);

        void setSelectedSubTickPen(const QPen &pen);

        Q_SLOT void setSelectableParts(const QCPPolarAxisAngular::SelectableParts &selectableParts);

        Q_SLOT void setSelectedParts(const QCPPolarAxisAngular::SelectableParts &selectedParts);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = 0) const override;

        void update(UpdatePhase phase) override;

        QList<QCPLayoutElement *> elements(bool recursive) const override;

        bool removeGraph(QCPPolarGraph *graph);

        int radialAxisCount() const;

        QCPPolarAxisRadial *radialAxis(int index = 0) const;

        QList<QCPPolarAxisRadial *> radialAxes() const;

        QCPPolarAxisRadial *addRadialAxis(QCPPolarAxisRadial *axis = 0);

        bool removeRadialAxis(QCPPolarAxisRadial *axis);

        QCPLayoutInset *insetLayout() const { return mInsetLayout; }

        QRegion exactClipRegion() const;

        void moveRange(double diff);

        void scaleRange(double factor);

        void scaleRange(double factor, double center);

        void rescale(bool onlyVisiblePlottables = false);

        double coordToAngleRad(double coord) const {
            return mAngleRad + (coord - mRange.lower) / mRange.size() *
                               (mRangeReversed ? -2.0 * M_PI : 2.0 * M_PI);
        } // mention in doc that return doesn't wrap
        double angleRadToCoord(double angleRad) const {
            return mRange.lower + (angleRad - mAngleRad) /
                                  (mRangeReversed ? -2.0 * M_PI : 2.0 *
                                                                  M_PI) *
                                  mRange.size();
        }

        void pixelToCoord(QPointF pixelPos, double &angleCoord, double &radiusCoord) const;

        QPointF coordToPixel(double angleCoord, double radiusCoord) const;

        SelectablePart getPartAt(const QPointF &pos) const;

        int left() const { return mRect.left(); }

        int right() const { return mRect.right(); }

        int top() const { return mRect.top(); }

        int bottom() const { return mRect.bottom(); }

        int width() const { return mRect.width(); }

        int height() const { return mRect.height(); }

        QSize size() const { return mRect.size(); }

        QPoint topLeft() const { return mRect.topLeft(); }

        QPoint topRight() const { return mRect.topRight(); }

        QPoint bottomLeft() const { return mRect.bottomLeft(); }

        QPoint bottomRight() const { return mRect.bottomRight(); }

        QPointF center() const { return mCenter; }

        double radius() const { return mRadius; }

    signals:

        void rangeChanged(const QCPRange &newRange);

        void rangeChanged(const QCPRange &newRange, const QCPRange &oldRange);

        void selectionChanged(const QCPPolarAxisAngular::SelectableParts &parts);

        void selectableChanged(const QCPPolarAxisAngular::SelectableParts &parts);

    protected:
        QBrush mBackgroundBrush;
        QPixmap mBackgroundPixmap;
        QPixmap mScaledBackgroundPixmap;
        bool mBackgroundScaled;
        Qt::AspectRatioMode mBackgroundScaledMode;
        QCPLayoutInset *mInsetLayout;
        bool mRangeDrag;
        bool mRangeZoom;
        double mRangeZoomFactor;

        // axis base:
        double mAngle, mAngleRad;
        SelectableParts mSelectableParts, mSelectedParts;
        QPen mBasePen, mSelectedBasePen;
        // axis label:
        int mLabelPadding;
        QString mLabel;
        QFont mLabelFont, mSelectedLabelFont;
        QColor mLabelColor, mSelectedLabelColor;
        // tick labels:
        //int mTickLabelPadding; in label painter
        bool mTickLabels;
        //double mTickLabelRotation; in label painter
        QFont mTickLabelFont, mSelectedTickLabelFont;
        QColor mTickLabelColor, mSelectedTickLabelColor;
        int mNumberPrecision;
        QLatin1Char mNumberFormatChar;
        bool mNumberBeautifulPowers;
        bool mNumberMultiplyCross;
        // ticks and subticks:
        bool mTicks;
        bool mSubTicks;
        int mTickLengthIn, mTickLengthOut, mSubTickLengthIn, mSubTickLengthOut;
        QPen mTickPen, mSelectedTickPen;
        QPen mSubTickPen, mSelectedSubTickPen;
        // scale and range:
        QCPRange mRange;
        bool mRangeReversed;

        QPointF mCenter;
        double mRadius;
        QList<QCPPolarAxisRadial *> mRadialAxes;
        QCPPolarGrid *mGrid;
        QList<QCPPolarGraph *> mGraphs;
        QSharedPointer<QCPAxisTicker> mTicker;
        QVector<double> mTickVector;
        QVector<QString> mTickVectorLabels;
        QVector<QPointF> mTickVectorCosSin;
        QVector<double> mSubTickVector;
        QVector<QPointF> mSubTickVectorCosSin;
        bool mDragging;
        QCPRange mDragAngularStart;
        QList<QCPRange> mDragRadialStart;
        QCP::AntialiasedElements mAADragBackup, mNotAADragBackup;
        QCPLabelPainterPrivate mLabelPainter;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        QCP::Interaction selectionCategory() const override;

        void mousePressEvent(QMouseEvent *event, const QVariant &details) override;

        void mouseMoveEvent(QMouseEvent *event, const QPointF &startPos) override;

        void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos) override;

        void wheelEvent(QWheelEvent *event) override;

        bool registerPolarGraph(QCPPolarGraph *graph);

        void drawBackground(QCPPainter *painter, const QPointF &center, double radius);

        void setupTickVectors();

        QPen getBasePen() const;

        QPen getTickPen() const;

        QPen getSubTickPen() const;

        QFont getTickLabelFont() const;

        QFont getLabelFont() const;

        QColor getTickLabelColor() const;

        QColor getLabelColor() const;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPolarAxisAngular::SelectableParts)
}

Q_DECLARE_METATYPE(QCP::QCPPolarAxisAngular::SelectablePart)

#endif
