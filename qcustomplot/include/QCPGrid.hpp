#ifndef QCUSTOMPLOT_QCPGRID_HPP
#define QCUSTOMPLOT_QCPGRID_HPP

#include "defs.hpp"
#include "QCPLayerable.hpp"
#include "QCPPainter.hpp"

#include <QObject>

namespace QCP {

    class QCP_LIB_DECL QCPGrid : public QCPLayerable {
    Q_OBJECT
        Q_PROPERTY(bool subGridVisible READ subGridVisible WRITE setSubGridVisible)
        Q_PROPERTY(bool antialiasedSubGrid READ antialiasedSubGrid WRITE setAntialiasedSubGrid)
        Q_PROPERTY(bool antialiasedZeroLine READ antialiasedZeroLine WRITE setAntialiasedZeroLine)
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen subGridPen READ subGridPen WRITE setSubGridPen)
        Q_PROPERTY(QPen zeroLinePen READ zeroLinePen WRITE setZeroLinePen)
    public:
        explicit QCPGrid(QCPAxis *parentAxis);

        virtual ~QCPGrid() = default;

        bool subGridVisible() const { return mSubGridVisible; }

        bool antialiasedSubGrid() const { return mAntialiasedSubGrid; }

        bool antialiasedZeroLine() const { return mAntialiasedZeroLine; }

        QPen pen() const { return mPen; }

        QPen subGridPen() const { return mSubGridPen; }

        QPen zeroLinePen() const { return mZeroLinePen; }

        void setSubGridVisible(bool visible);

        void setAntialiasedSubGrid(bool enabled);

        void setAntialiasedZeroLine(bool enabled);

        void setPen(const QPen &pen);

        void setSubGridPen(const QPen &pen);

        void setZeroLinePen(const QPen &pen);

    protected:
        bool mSubGridVisible;
        bool mAntialiasedSubGrid, mAntialiasedZeroLine;
        QPen mPen, mSubGridPen, mZeroLinePen;

        QCPAxis *mParentAxis;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        void drawGridLines(QCPPainter *painter) const;

        void drawSubGridLines(QCPPainter *painter) const;
    };
}
#endif
