#ifndef QCUSTOMPLOT_QCPITEMBRACKET_HPP
#define QCUSTOMPLOT_QCPITEMBRACKET_HPP

#include "QCPAbstractItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPItemBracket : public QCPAbstractItem {
    Q_OBJECT
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QPen selectedPen READ selectedPen WRITE setSelectedPen)
        Q_PROPERTY(double length READ length WRITE setLength)
        Q_PROPERTY(BracketStyle style READ style WRITE setStyle)
    public:
        /*!
        Defines the various visual shapes of the bracket item. The appearance can be further modified
        by \ref setLength and \ref setPen.

        \see setStyle
      */
        enum BracketStyle {
            bsSquare, ///< A brace with angled edges
            bsRound, ///< A brace with round edges
            bsCurly,///< A curly brace
            bsCalligraphic, ///< A curly brace with varying stroke width giving a calligraphic impression
        };

        Q_ENUM(BracketStyle)

        explicit QCPItemBracket(QCustomPlot *parentPlot);

        ~QCPItemBracket() override;

        QPen pen() const { return mPen; }

        QPen selectedPen() const { return mSelectedPen; }

        double length() const { return mLength; }

        BracketStyle style() const { return mStyle; }

        void setPen(const QPen &pen);

        void setSelectedPen(const QPen &pen);

        void setLength(double length);

        void setStyle(BracketStyle style);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPItemPosition *const left;
        QCPItemPosition *const right;
        QCPItemAnchor *const center;

    protected:
        enum AnchorIndex {
            aiCenter
        };
        QPen mPen, mSelectedPen;
        double mLength;
        BracketStyle mStyle;

        void draw(QCPPainter *painter) override;

        QPointF anchorPixelPosition(int anchorId) const override;

        QPen mainPen() const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPItemBracket::BracketStyle)

#endif
