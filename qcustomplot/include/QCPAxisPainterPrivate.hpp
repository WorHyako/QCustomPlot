#ifndef QCUSTOMPLOT_QCPAXISPAINTERPRIVATE_HPP
#define QCUSTOMPLOT_QCPAXISPAINTERPRIVATE_HPP

#include "QCPAxis.hpp"
#include "QCPLineEnding.hpp"
#include "QCustomPlot.hpp"
#include "QCPPainter.hpp"

namespace QCP {

    class QCPAxisPainterPrivate {
    public:
        explicit QCPAxisPainterPrivate(QCustomPlot *parentPlot);

        virtual ~QCPAxisPainterPrivate();

        virtual void draw(QCPPainter *painter);

        virtual int size();

        void clearCache();

        QRect axisSelectionBox() const { return mAxisSelectionBox; }

        QRect tickLabelsSelectionBox() const { return mTickLabelsSelectionBox; }

        QRect labelSelectionBox() const { return mLabelSelectionBox; }

        QCPAxis::AxisType type;
        QPen basePen;
        QCPLineEnding lowerEnding, upperEnding; // directly accessed by QCPAxis setters/getters
        int labelPadding; // directly accessed by QCPAxis setters/getters
        QFont labelFont;
        QColor labelColor;
        QString label;
        int tickLabelPadding; // directly accessed by QCPAxis setters/getters
        double tickLabelRotation; // directly accessed by QCPAxis setters/getters
        QCPAxis::LabelSide tickLabelSide; // directly accessed by QCPAxis setters/getters
        bool substituteExponent;
        bool numberMultiplyCross; // directly accessed by QCPAxis setters/getters
        int tickLengthIn, tickLengthOut, subTickLengthIn, subTickLengthOut; // directly accessed by QCPAxis setters/getters
        QPen tickPen, subTickPen;
        QFont tickLabelFont;
        QColor tickLabelColor;
        QRect axisRect, viewportRect;
        int offset; // directly accessed by QCPAxis setters/getters
        bool abbreviateDecimalPowers;
        bool reversedEndings;

        QVector<double> subTickPositions;
        QVector<double> tickPositions;
        QVector<QString> tickLabels;

    protected:
        struct CachedLabel {
            QPointF offset;
            QPixmap pixmap;
        };
        struct TickLabelData {
            QString basePart, expPart, suffixPart;
            QRect baseBounds, expBounds, suffixBounds, totalBounds, rotatedTotalBounds;
            QFont baseFont, expFont;
        };
        QCustomPlot *mParentPlot;
        QByteArray mLabelParameterHash; // to determine whether mLabelCache needs to be cleared due to changed parameters
        QCache<QString, CachedLabel> mLabelCache;
        QRect mAxisSelectionBox, mTickLabelsSelectionBox, mLabelSelectionBox;

        virtual QByteArray generateLabelParameterHash() const;

        virtual void placeTickLabel(QCPPainter *painter, double position, int distanceToAxis, const QString &text,
                                    QSize *tickLabelsSize);

        virtual void drawTickLabel(QCPPainter *painter, double x, double y, const TickLabelData &labelData) const;

        virtual TickLabelData getTickLabelData(const QFont &font, const QString &text) const;

        virtual QPointF getTickLabelDrawOffset(const TickLabelData &labelData) const;

        virtual void getMaxTickLabelSize(const QFont &font, const QString &text, QSize *tickLabelsSize) const;
    };
}
#endif
