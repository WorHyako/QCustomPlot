#ifndef QCUSTOMPLOT_QCPMARGINGROUP_HPP
#define QCUSTOMPLOT_QCPMARGINGROUP_HPP

#include "defs.hpp"
#include "QCustomPlot.hpp"
#include "QCPLayoutElement.hpp"

#include <QObject>

namespace QCP {

    class QCP_LIB_DECL QCPMarginGroup : public QObject {
    Q_OBJECT
    public:
        Q_DISABLE_COPY(QCPMarginGroup)

        explicit QCPMarginGroup(QCustomPlot *parentPlot);

        ~QCPMarginGroup() override;

        QList<QCPLayoutElement *> elements(QCP::MarginSide side) const { return mChildren.value(side); }

        bool isEmpty() const;

        void clear();

    protected:
        QCustomPlot *mParentPlot;
        QHash<QCP::MarginSide, QList<QCPLayoutElement *> > mChildren;

        virtual int commonMargin(QCP::MarginSide side) const;

        void addChild(QCP::MarginSide side, QCPLayoutElement *element);

        void removeChild(QCP::MarginSide side, QCPLayoutElement *element);
    };
}

#endif
