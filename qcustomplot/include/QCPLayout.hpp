#ifndef QCUSTOMPLOT_QCPLAYOUT_HPP
#define QCUSTOMPLOT_QCPLAYOUT_HPP

#include "defs.hpp"
#include "QCPLayoutElement.hpp"

#include <QObject>
#include <QVector>
#include <QSize>

namespace QCP {

    class QCP_LIB_DECL QCPLayout : public QCPLayoutElement {
    Q_OBJECT
    public:
        Q_DISABLE_COPY(QCPLayout)

        QCPLayout() = default;

        void update(UpdatePhase phase) override;

        QList<QCPLayoutElement *> elements(bool recursive) const override;

        virtual int elementCount() const = 0;

        virtual QCPLayoutElement *elementAt(int index) const = 0;

        virtual QCPLayoutElement *takeAt(int index) = 0;

        virtual bool take(QCPLayoutElement *element) = 0;

        virtual void simplify();

        bool removeAt(int index);

        bool remove(QCPLayoutElement *element);

        void clear();

    protected:
        virtual void updateLayout();

        void sizeConstraintsChanged() const;

        void adoptElement(QCPLayoutElement *el);

        void releaseElement(QCPLayoutElement *el);

        QVector<int> getSectionSizes(QVector<int> maxSizes, QVector<int> minSizes, QVector<double> stretchFactors,
                                     int totalSize) const;

        static QSize getFinalMinimumOuterSize(const QCPLayoutElement *el);

        static QSize getFinalMaximumOuterSize(const QCPLayoutElement *el);
    };
}
#endif
