#ifndef QCUSTOMPLOT_QCPTEXTELEMENT_HPP
#define QCUSTOMPLOT_QCPTEXTELEMENT_HPP

#include "QCPLayoutElement.hpp"

namespace QCP {

    class QCP_LIB_DECL QCPTextElement : public QCPLayoutElement {
    Q_OBJECT
        Q_PROPERTY(QString text READ text WRITE setText)
        Q_PROPERTY(QFont font READ font WRITE setFont)
        Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
        Q_PROPERTY(QFont selectedFont READ selectedFont WRITE setSelectedFont)
        Q_PROPERTY(QColor selectedTextColor READ selectedTextColor WRITE setSelectedTextColor)
        Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
        Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
    public:
        Q_DISABLE_COPY(QCPTextElement)

        explicit QCPTextElement(QCustomPlot *parentPlot);

        QCPTextElement(QCustomPlot *parentPlot, const QString &text);

        QCPTextElement(QCustomPlot *parentPlot, const QString &text, double pointSize);

        QCPTextElement(QCustomPlot *parentPlot, const QString &text, const QString &fontFamily, double pointSize);

        QCPTextElement(QCustomPlot *parentPlot, const QString &text, const QFont &font);

        QString text() const { return mText; }

        int textFlags() const { return mTextFlags; }

        QFont font() const { return mFont; }

        QColor textColor() const { return mTextColor; }

        QFont selectedFont() const { return mSelectedFont; }

        QColor selectedTextColor() const { return mSelectedTextColor; }

        bool selectable() const { return mSelectable; }

        bool selected() const { return mSelected; }

        void setText(const QString &text);

        void setTextFlags(int flags);

        void setFont(const QFont &font);

        void setTextColor(const QColor &color);

        void setSelectedFont(const QFont &font);

        void setSelectedTextColor(const QColor &color);

        Q_SLOT void setSelectable(bool selectable);

        Q_SLOT void setSelected(bool selected);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

        void mousePressEvent(QMouseEvent *event, const QVariant &details) override;

        void mouseReleaseEvent(QMouseEvent *event, const QPointF &startPos) override;

        void mouseDoubleClickEvent(QMouseEvent *event, const QVariant &details) override;

    signals:

        void selectionChanged(bool selected);

        void selectableChanged(bool selectable);

        void clicked(QMouseEvent *event);

        void doubleClicked(QMouseEvent *event);

    protected:
        QString mText;
        int mTextFlags;
        QFont mFont;
        QColor mTextColor;
        QFont mSelectedFont;
        QColor mSelectedTextColor;
        QRect mTextBoundingRect;
        bool mSelectable, mSelected;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override;

        QSize minimumOuterSizeHint() const override;

        QSize maximumOuterSizeHint() const override;

        void
        selectEvent(QMouseEvent *event, bool additive, const QVariant &details, bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;

        QFont mainFont() const;

        QColor mainTextColor() const;
    };
}
#endif
