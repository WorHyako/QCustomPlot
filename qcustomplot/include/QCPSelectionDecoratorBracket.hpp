#ifndef QCUSTOMPLOT_QCPSELECTIONDECORATORBRACKET_HPP
#define QCUSTOMPLOT_QCPSELECTIONDECORATORBRACKET_HPP

#include "QCPSelectionDecorator.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPSelectionDecoratorBracket : public QCPSelectionDecorator {
    Q_GADGET
    public:

        /*!
        Defines which shape is drawn at the boundaries of selected data ranges.

        Some of the bracket styles further allow specifying a height and/or width, see \ref
        setBracketHeight and \ref setBracketWidth.
      */
        enum BracketStyle {
            bsSquareBracket ///< A square bracket is drawn.
            ,
            bsHalfEllipse   ///< A half ellipse is drawn. The size of the ellipse is given by the bracket width/height properties.
            ,
            bsEllipse       ///< An ellipse is drawn. The size of the ellipse is given by the bracket width/height properties.
            ,
            bsPlus         ///< A plus is drawn.
            ,
            bsUserStyle    ///< Start custom bracket styles at this index when subclassing and reimplementing \ref drawBracket.
        };

        Q_ENUM(BracketStyle)

        QCPSelectionDecoratorBracket();

        ~QCPSelectionDecoratorBracket() override;

        QPen bracketPen() const { return mBracketPen; }

        QBrush bracketBrush() const { return mBracketBrush; }

        int bracketWidth() const { return mBracketWidth; }

        int bracketHeight() const { return mBracketHeight; }

        BracketStyle bracketStyle() const { return mBracketStyle; }

        bool tangentToData() const { return mTangentToData; }

        int tangentAverage() const { return mTangentAverage; }

        void setBracketPen(const QPen &pen);

        void setBracketBrush(const QBrush &brush);

        void setBracketWidth(int width);

        void setBracketHeight(int height);

        void setBracketStyle(BracketStyle style);

        void setTangentToData(bool enabled);

        void setTangentAverage(int pointCount);

        virtual void drawBracket(QCPPainter *painter, int direction) const;

        void drawDecoration(QCPPainter *painter, QCPDataSelection selection) override;

    protected:
        QPen mBracketPen;
        QBrush mBracketBrush;
        int mBracketWidth;
        int mBracketHeight;
        BracketStyle mBracketStyle;
        bool mTangentToData;
        int mTangentAverage;

        double getTangentAngle(const QCPPlottableInterface1D *interface1d, int dataIndex, int direction) const;

        QPointF getPixelCoordinates(const QCPPlottableInterface1D *interface1d, int dataIndex) const;
    };
}
Q_DECLARE_METATYPE(QCP::QCPSelectionDecoratorBracket::BracketStyle)

#endif
