#ifndef QCUSTOMPLOT_QCPPAINTER_HPP
#define QCUSTOMPLOT_QCPPAINTER_HPP

#include "defs.hpp"

#include <QPainter>
#include <QPaintDevice>
#include <QStack>

namespace QCP {

    class QCP_LIB_DECL QCPPainter : public QPainter {
    Q_GADGET
    public:
        /*!
        Defines special modes the painter can operate in. They disable or enable certain subsets of features/fixes/workarounds,
        depending on whether they are wanted on the respective output device.
      */
        enum PainterMode {
            pmDefault = 0x00,   ///< <tt>0x00</tt> Default mode for painting on screen devices
            pmVectorized = 0x01,   ///< <tt>0x01</tt> Mode for vectorized painting (e.g. PDF export). For example, this prevents some antialiasing fixes.
            pmNoCaching = 0x02, ///< <tt>0x02</tt> Mode for all sorts of exports (e.g. PNG, PDF,...). For example, this prevents using cached pixmap labels
            pmNonCosmetic = 0x04  ///< <tt>0x04</tt> Turns pen widths 0 to 1, i.e. disables cosmetic pens. (A cosmetic pen is always drawn with width 1 pixel in the vector image/pdf viewer, independent of zoom.)
        };
        Q_ENUM(PainterMode)
        Q_FLAGS(PainterModes)
        Q_DECLARE_FLAGS(PainterModes, PainterMode)

        QCPPainter();

        explicit QCPPainter(QPaintDevice *device);

        bool antialiasing() const { return testRenderHint(QPainter::Antialiasing); }

        PainterModes modes() const { return mModes; }

        void setAntialiasing(bool enabled);

        void setMode(PainterMode mode, bool enabled = true);

        void setModes(PainterModes modes);

        bool begin(QPaintDevice *device);

        void setPen(const QPen &pen);

        void setPen(const QColor &color);

        void setPen(Qt::PenStyle penStyle);

        void drawLine(const QLineF &line);

        void drawLine(const QPointF &p1, const QPointF &p2) { drawLine(QLineF(p1, p2)); }

        void save();

        void restore();

        void makeNonCosmetic();

    protected:
        PainterModes mModes;
        bool mIsAntialiasing;

        QStack<bool> mAntialiasingStack;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPPainter::PainterModes)
}
Q_DECLARE_METATYPE(QCP::QCPPainter::PainterMode)

#endif
