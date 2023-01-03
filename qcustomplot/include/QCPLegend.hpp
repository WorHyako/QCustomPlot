#ifndef QCUSTOMPLOT_QCPLEGEND_HPP
#define QCUSTOMPLOT_QCPLEGEND_HPP

#include "QCPLayoutGrid.hpp"
#include "QCPPlottableLegendItem.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPLegend : public QCPLayoutGrid {
    Q_OBJECT
        Q_PROPERTY(QPen borderPen READ borderPen WRITE setBorderPen)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
        Q_PROPERTY(QFont font READ font WRITE setFont)
        Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
        Q_PROPERTY(QSize iconSize READ iconSize WRITE setIconSize)
        Q_PROPERTY(int iconTextPadding READ iconTextPadding WRITE setIconTextPadding)
        Q_PROPERTY(QPen iconBorderPen READ iconBorderPen WRITE setIconBorderPen)
        Q_PROPERTY(
                SelectableParts selectableParts READ selectableParts WRITE setSelectableParts NOTIFY selectionChanged)
        Q_PROPERTY(SelectableParts selectedParts READ selectedParts WRITE setSelectedParts NOTIFY selectableChanged)
        Q_PROPERTY(QPen selectedBorderPen READ selectedBorderPen WRITE setSelectedBorderPen)
        Q_PROPERTY(QPen selectedIconBorderPen READ selectedIconBorderPen WRITE setSelectedIconBorderPen)
        Q_PROPERTY(QBrush selectedBrush READ selectedBrush WRITE setSelectedBrush)
        Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
        Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
    public:
        /*!
        Defines the selectable parts of a legend

        \see setSelectedParts, setSelectableParts
      */
        enum SelectablePart {
            spNone = 0x000,///< <tt>0x000</tt> None
            spLegendBox = 0x001, ///< <tt>0x001</tt> The legend box (frame)
            spItems = 0x002 ///< <tt>0x002</tt> Legend items individually (see \ref selectedItems)
        };

        Q_ENUM(SelectablePart)

        Q_FLAGS(SelectableParts)
        Q_DECLARE_FLAGS(SelectableParts, SelectablePart)

        Q_DISABLE_COPY(QCPLegend)

        explicit QCPLegend();

        ~QCPLegend() override;

        QPen borderPen() const { return mBorderPen; }

        QBrush brush() const { return mBrush; }

        QFont font() const { return mFont; }

        QColor textColor() const { return mTextColor; }

        QSize iconSize() const { return mIconSize; }

        int iconTextPadding() const { return mIconTextPadding; }

        QPen iconBorderPen() const { return mIconBorderPen; }

        SelectableParts selectableParts() const { return mSelectableParts; }

        SelectableParts selectedParts() const;

        QPen selectedBorderPen() const { return mSelectedBorderPen; }

        QPen selectedIconBorderPen() const { return mSelectedIconBorderPen; }

        QBrush selectedBrush() const { return mSelectedBrush; }

        QFont selectedFont() const { return mSelectedFont; }

        QColor selectedTextColor() const { return mSelectedTextColor; }

        void setBorderPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setFont(const QFont &font);

        void setTextColor(const QColor &color);

        void setIconSize(const QSize &size);

        void setIconSize(int width, int height);

        void setIconTextPadding(int padding);

        void setIconBorderPen(const QPen &pen);

        Q_SLOT void setSelectableParts(const SelectableParts &selectableParts);

        Q_SLOT void setSelectedParts(const SelectableParts &selectedParts);

        void setSelectedBorderPen(const QPen &pen);

        void setSelectedIconBorderPen(const QPen &pen);

        void setSelectedBrush(const QBrush &brush);

        void setSelectedFont(const QFont &font);

        void setSelectedTextColor(const QColor &color);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        QCPAbstractLegendItem *item(int index) const;

        QCPPlottableLegendItem *itemWithPlottable(const QCPAbstractPlottable *plottable) const;

        int itemCount() const;

        bool hasItem(QCPAbstractLegendItem *item) const;

        bool hasItemWithPlottable(const QCPAbstractPlottable *plottable) const;

        bool addItem(QCPAbstractLegendItem *item);

        bool removeItem(int index);

        bool removeItem(QCPAbstractLegendItem *item);

        void clearItems();

        QList<QCPAbstractLegendItem *> selectedItems() const;

    signals:

        void selectionChanged(QCPLegend::SelectableParts parts);

        void selectableChanged(QCPLegend::SelectableParts parts);

    protected:
        QPen mBorderPen, mIconBorderPen;
        QBrush mBrush;
        QFont mFont;
        QColor mTextColor;
        QSize mIconSize;
        int mIconTextPadding;
        SelectableParts mSelectedParts, mSelectableParts;
        QPen mSelectedBorderPen, mSelectedIconBorderPen;
        QBrush mSelectedBrush;
        QFont mSelectedFont;
        QColor mSelectedTextColor;

        void parentPlotInitialized(QCustomPlot *parentPlot) override;

        Interaction selectionCategory() const override;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        void
        selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;

        QPen getBorderPen() const;

        QBrush getBrush() const;
    };

    Q_DECLARE_OPERATORS_FOR_FLAGS(QCPLegend::SelectableParts)
}
Q_DECLARE_METATYPE(QCP::QCPLegend::SelectablePart)

#endif
