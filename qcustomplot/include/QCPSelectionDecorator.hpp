#ifndef QCUSTOMPLOT_QCPSELECTIONDECORATOR_HPP
#define QCUSTOMPLOT_QCPSELECTIONDECORATOR_HPP

#include "defs.hpp"
#include "QCPScatterStyle.hpp"
#include "QCPDataSelection.hpp"
#include "QCPAbstractPlottable.hpp"

#include <QtCore>

namespace QCP {

    class QCP_LIB_DECL QCPSelectionDecorator {
    Q_GADGET
    public:
        Q_DISABLE_COPY(QCPSelectionDecorator)

        QCPSelectionDecorator();

        virtual ~QCPSelectionDecorator();

        QPen pen() const { return mPen; }

        QBrush brush() const { return mBrush; }

        QCPScatterStyle scatterStyle() const { return mScatterStyle; }

        QCPScatterStyle::ScatterProperties usedScatterProperties() const { return mUsedScatterProperties; }

        void setPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setScatterStyle(const QCPScatterStyle &scatterStyle,
                             QCPScatterStyle::ScatterProperties usedProperties = QCPScatterStyle::spPen);

        void setUsedScatterProperties(const QCPScatterStyle::ScatterProperties &properties);

        void applyPen(QCPPainter *painter) const;

        void applyBrush(QCPPainter *painter) const;

        QCPScatterStyle getFinalScatterStyle(const QCPScatterStyle &unselectedStyle) const;

        virtual void copyFrom(const QCPSelectionDecorator *other);

        virtual void drawDecoration(QCPPainter *painter, QCPDataSelection selection);

    protected:
        QPen mPen;
        QBrush mBrush;
        QCPScatterStyle mScatterStyle;
        QCPScatterStyle::ScatterProperties mUsedScatterProperties;
        QCPAbstractPlottable *mPlottable;

        virtual bool registerWithPlottable(QCPAbstractPlottable *plottable);
    };
}
Q_DECLARE_METATYPE(QCPSelectionDecorator*)

#endif
