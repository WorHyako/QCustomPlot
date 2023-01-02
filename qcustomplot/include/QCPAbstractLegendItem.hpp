#ifndef QCUSTOMPLOT_QCPABSTRACTLEGENDITEM_HPP
#define QCUSTOMPLOT_QCPABSTRACTLEGENDITEM_HPP

#include "defs.hpp"
#include "enums.hpp"
#include "QCPLayoutElement.hpp"
#include "QCPLegend.hpp"
#include "QCPPainter.hpp"

#include <QFont>
#include <QColor>
#include <QMouseEvent>
#include <QPointF>
#include <QVariant>

namespace QCP {

    class QCP_LIB_DECL QCPAbstractLegendItem : public QCPLayoutElement {
    Q_OBJECT
        /// \cond INCLUDE_QPROPERTIES
        Q_PROPERTY(QCPLegend *parentLegend READ parentLegend)
        Q_PROPERTY(QFont font READ font WRITE setFont)
        Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
        Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
        Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
        Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectionChanged)
        Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectableChanged)
        /// \endcond
    public:
        Q_DISABLE_COPY(QCPAbstractLegendItem)

        explicit QCPAbstractLegendItem(QCPLegend *parent);

        QCPLegend *parentLegend() const { return mParentLegend; }

        QFont font() const { return mFont; }

        QColor textColor() const { return mTextColor; }

        QFont selectedFont() const { return mSelectedFont; }

        QColor selectedTextColor() const { return mSelectedTextColor; }

        bool selectable() const { return mSelectable; }

        bool selected() const { return mSelected; }

        void setFont(const QFont &font);

        void setTextColor(const QColor &color);

        void setSelectedFont(const QFont &font);

        void setSelectedTextColor(const QColor &color);

        Q_SLOT void setSelectable(bool selectable);

        Q_SLOT void setSelected(bool selected);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

    signals:

        void selectionChanged(bool selected);

        void selectableChanged(bool selectable);

    protected:
        QCPLegend *mParentLegend;
        QFont mFont;
        QColor mTextColor;
        QFont mSelectedFont;
        QColor mSelectedTextColor;
        bool mSelectable, mSelected;

        Interaction selectionCategory() const override;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        QRect clipRect() const override;

        void draw(QCPPainter *painter) override = 0;

        void selectEvent(QMouseEvent *event, bool additive, const QVariant &details,
                         bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;
    };
}
#endif
