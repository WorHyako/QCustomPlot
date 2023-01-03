#ifndef QCUSTOMPLOT_QCPITEMANCHOR_HPP
#define QCUSTOMPLOT_QCPITEMANCHOR_HPP

#include "defs.hpp"
#include "QCustomPlot.hpp"
#include "QCPAbstractItem.hpp"

#include <QtCore>

namespace QCP {

    class QCP_LIB_DECL QCPItemAnchor {
    Q_GADGET
    public:
        Q_DISABLE_COPY(QCPItemAnchor)

        QCPItemAnchor(QCustomPlot *parentPlot, QCPAbstractItem *parentItem, const QString &name, int anchorId = -1);

        virtual ~QCPItemAnchor();

        QString name() const { return mName; }

        virtual QPointF pixelPosition() const;

    protected:
        QString mName;

        QCustomPlot *mParentPlot;
        QCPAbstractItem *mParentItem;
        int mAnchorId;
        QSet<QCPItemPosition *> mChildrenX, mChildrenY;

        virtual QCPItemPosition *toQCPItemPosition() { return nullptr; }

        void addChildX(QCPItemPosition *pos); // called from pos when this anchor is set as parent
        void removeChildX(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
        void addChildY(QCPItemPosition *pos); // called from pos when this anchor is set as parent
        void removeChildY(QCPItemPosition *pos); // called from pos when its parent anchor is reset or pos deleted
    };
}
#endif
