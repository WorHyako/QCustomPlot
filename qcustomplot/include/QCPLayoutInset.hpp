#ifndef QCUSTOMPLOT_QCPLAYOUTINSET_HPP
#define QCUSTOMPLOT_QCPLAYOUTINSET_HPP

#include "QCPLayout.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPLayoutInset : public QCPLayout {
    Q_OBJECT
    public:
/*!
Defines how the placement and sizing is handled for a certain element in a QCPLayoutInset.
*/
        enum InsetPlacement {
            ipFree            ///< The element may be positioned/sized arbitrarily, see \ref setInsetRect
            , ipBorderAligned  ///< The element is aligned to one of the layout sides, see \ref setInsetAlignment
        };

        Q_ENUM(InsetPlacement)

        Q_DISABLE_COPY(QCPLayoutInset)

        explicit QCPLayoutInset();

        ~QCPLayoutInset() override;

        InsetPlacement insetPlacement(int index) const;

        Qt::Alignment insetAlignment(int index) const;

        QRectF insetRect(int index) const;

        void setInsetPlacement(int index, InsetPlacement placement);

        void setInsetAlignment(int index, Qt::Alignment alignment);

        void setInsetRect(int index, const QRectF &rect);

        void updateLayout() override;

        int elementCount() const override;

        QCPLayoutElement *elementAt(int index) const override;

        QCPLayoutElement *takeAt(int index) override;

        bool take(QCPLayoutElement *element) override;

        void simplify() override {}

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        void addElement(QCPLayoutElement *element, Qt::Alignment alignment);

        void addElement(QCPLayoutElement *element, const QRectF &rect);

    protected:
        QList<QCPLayoutElement *> mElements;
        QList<InsetPlacement> mInsetPlacement;
        QList<Qt::Alignment> mInsetAlignment;
        QList<QRectF> mInsetRect;
    };
}
Q_DECLARE_METATYPE(QCP::QCPLayoutInset::InsetPlacement)

#endif
