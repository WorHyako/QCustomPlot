#ifndef QCUSTOMPLOT_QCPSCATTERSTYLE_HPP
#define QCUSTOMPLOT_QCPSCATTERSTYLE_HPP

#include "defs.hpp"
#include "QCPPainter.hpp"

#include <QtCore>
#include <QPainterPath>

namespace QCP {

    class QCP_LIB_DECL QCPScatterStyle {
    Q_GADGET
    public:
        /*!
        Represents the various properties of a scatter style instance. For example, this enum is used
        to specify which properties of \ref QCPSelectionDecorator::setScatterStyle will be used when
        highlighting selected data points.

        Specific scatter properties can be transferred between \ref QCPScatterStyle instances via \ref
        setFromOther.
      */
        enum ScatterProperty {
            spNone = 0x00, ///< <tt>0x00</tt> None
            spPen = 0x01, ///< <tt>0x01</tt> The pen property, see \ref setPen
            spBrush = 0x02, ///< <tt>0x02</tt> The brush property, see \ref setBrush
            spSize = 0x04, ///< <tt>0x04</tt> The size property, see \ref setSize
            spShape = 0x08, ///< <tt>0x08</tt> The shape property, see \ref setShape
            spAll = 0xFF  ///< <tt>0xFF</tt> All properties
        };

        Q_ENUM(ScatterProperty)

        Q_FLAGS(ScatterProperties)
        Q_DECLARE_FLAGS(ScatterProperties, ScatterProperty)

        /*!
        Defines the shape used for scatter points.

        On plottables/items that draw scatters, the sizes of these visualizations (with exception of
        \ref ssDot and \ref ssPixmap) can be controlled with the \ref setSize function. Scatters are
        drawn with the pen and brush specified with \ref setPen and \ref setBrush.
      */
        enum ScatterShape {
            ssNone,     ///< no scatter symbols are drawn (e.g. in QCPGraph, data only represented with lines)
            ssDot, ///< \enumimage{ssDot.png} a single pixel (use \ref ssDisc or \ref ssCircle if you want a round shape with a certain radius)
            ssCross, ///< \enumimage{ssCross.png} a cross
            ssPlus, ///< \enumimage{ssPlus.png} a plus
            ssCircle, ///< \enumimage{ssCircle.png} a circle
            ssDisc, ///< \enumimage{ssDisc.png} a circle which is filled with the pen's color (not the brush as with ssCircle)
            ssSquare, ///< \enumimage{ssSquare.png} a square
            ssDiamond, ///< \enumimage{ssDiamond.png} a diamond
            ssStar, ///< \enumimage{ssStar.png} a star with eight arms, i.e. a combination of cross and plus
            ssTriangle, ///< \enumimage{ssTriangle.png} an equilateral triangle, standing on baseline
            ssTriangleInverted, ///< \enumimage{ssTriangleInverted.png} an equilateral triangle, standing on corner
            ssCrossSquare, ///< \enumimage{ssCrossSquare.png} a square with a cross inside
            ssPlusSquare, ///< \enumimage{ssPlusSquare.png} a square with a plus inside
            ssCrossCircle, ///< \enumimage{ssCrossCircle.png} a circle with a cross inside
            ssPlusCircle, ///< \enumimage{ssPlusCircle.png} a circle with a plus inside
            ssPeace, ///< \enumimage{ssPeace.png} a circle, with one vertical and two downward diagonal lines
            ssPixmap,///< a custom pixmap specified by \ref setPixmap, centered on the data point coordinates
            ssCustom    ///< custom painter operations are performed per scatter (As QPainterPath, see \ref setCustomPath)
        };

        Q_ENUM(ScatterShape)

        QCPScatterStyle();

        QCPScatterStyle(ScatterShape shape, double size = 6);

        QCPScatterStyle(ScatterShape shape, const QColor &color, double size);

        QCPScatterStyle(ScatterShape shape, const QColor &color, const QColor &fill, double size);

        QCPScatterStyle(ScatterShape shape, const QPen &pen, const QBrush &brush, double size);

        QCPScatterStyle(const QPixmap &pixmap);

        QCPScatterStyle(const QPainterPath &customPath, const QPen &pen, const QBrush &brush = Qt::NoBrush,
                        double size = 6);

        double size() const { return mSize; }

        ScatterShape shape() const { return mShape; }

        QPen pen() const { return mPen; }

        QBrush brush() const { return mBrush; }

        QPixmap pixmap() const { return mPixmap; }

        QPainterPath customPath() const { return mCustomPath; }

        void setFromOther(const QCPScatterStyle &other, ScatterProperties properties);

        void setSize(double size);

        void setShape(ScatterShape shape);

        void setPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setPixmap(const QPixmap &pixmap);

        void setCustomPath(const QPainterPath &customPath);

        bool isNone() const { return mShape == ssNone; }

        bool isPenDefined() const { return mPenDefined; }

        void undefinePen();

        void applyTo(QCPPainter *painter, const QPen &defaultPen) const;

        void drawShape(QCPPainter *painter, const QPointF &pos) const;

        void drawShape(QCPPainter *painter, double x, double y) const;

    protected:
        double mSize;
        ScatterShape mShape;
        QPen mPen;
        QBrush mBrush;
        QPixmap mPixmap;
        QPainterPath mCustomPath;

        bool mPenDefined;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPScatterStyle::ScatterProperties)

}
Q_DECLARE_TYPEINFO(QCP::QCPScatterStyle, Q_MOVABLE_TYPE);

Q_DECLARE_METATYPE(QCP::QCPScatterStyle::ScatterProperty)

Q_DECLARE_METATYPE(QCP::QCPScatterStyle::ScatterShape)

#endif
