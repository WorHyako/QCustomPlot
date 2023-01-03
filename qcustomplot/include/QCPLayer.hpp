#ifndef QCUSTOMPLOT_QCPLAYER_HPP
#define QCUSTOMPLOT_QCPLAYER_HPP

#include "defs.hpp"
#include "QCustomPlot.hpp"

#include <QObject>
#include <QString>
#include <QList>

namespace QCP {

    class QCP_LIB_DECL QCPLayer : public QObject {
    Q_OBJECT
        Q_PROPERTY(QCustomPlot *parentPlot READ parentPlot)
        Q_PROPERTY(QString name READ name)
        Q_PROPERTY(int index READ index)
        Q_PROPERTY(QList<QCPLayerable *> children READ children)
        Q_PROPERTY(bool visible READ visible WRITE setVisible)
        Q_PROPERTY(LayerMode mode READ mode WRITE setMode)
    public:

        /*!
        Defines the different rendering modes of a layer. Depending on the mode, certain layers can be
        replotted individually, without the need to replot (possibly complex) layerables on other
        layers.

        \see setMode
      */
        enum LayerMode {
            lmLogical,   ///< Layer is used only for rendering order, and shares paint buffer with all other adjacent logical layers.
            lmBuffered ///< Layer has its own paint buffer and may be replotted individually (see \ref replot).
        };

        Q_ENUM(LayerMode)

        Q_DISABLE_COPY(QCPLayer)

        QCPLayer(QCustomPlot *parentPlot, const QString &layerName);

        virtual ~QCPLayer();

        QCustomPlot *parentPlot() const { return mParentPlot; }

        QString name() const { return mName; }

        int index() const { return mIndex; }

        QList<QCPLayerable *> children() const { return mChildren; }

        bool visible() const { return mVisible; }

        LayerMode mode() const { return mMode; }

        void setVisible(bool visible);

        void setMode(LayerMode mode);

        void replot();

    protected:
        QCustomPlot *mParentPlot;
        QString mName;
        int mIndex;
        QList<QCPLayerable *> mChildren;
        bool mVisible;
        LayerMode mMode;

        QWeakPointer<QCPAbstractPaintBuffer> mPaintBuffer;

        void draw(QCPPainter *painter);

        void drawToPaintBuffer();

        void addChild(QCPLayerable *layerable, bool prepend);

        void removeChild(QCPLayerable *layerable);
    };
}
Q_DECLARE_METATYPE(QCP::QCPLayer::LayerMode)

#endif
