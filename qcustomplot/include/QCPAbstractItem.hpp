#ifndef QCUSTOMPLOT_QCPABSTRACTITEM_HPP
#define QCUSTOMPLOT_QCPABSTRACTITEM_HPP

#include "defs.hpp"
#include "QCPLayerable.hpp"
#include "QCPItemPosition.hpp"
#include "QCPItemAnchor.hpp"
#include "QCPPainter.hpp"
#include "QCPAxisRect.hpp"
#include "QCustomPlot.hpp"

#include <QMouseEvent>
#include <QString>
#include <QList>
#include <QPointF>
#include <QVariant>
#include <QRectF>

namespace QCP {

    class QCP_LIB_DECL QCPAbstractItem
            : public QCPLayerable {
    Q_OBJECT
        /// \cond INCLUDE_QPROPERTIES
        Q_PROPERTY(bool clipToAxisRect READ clipToAxisRect WRITE setClipToAxisRect)
        Q_PROPERTY(QCPAxisRect *clipAxisRect READ clipAxisRect WRITE setClipAxisRect)
        Q_PROPERTY(bool selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
        Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectionChanged)
        /// \endcond
    public:
        Q_DISABLE_COPY(QCPAbstractItem)

        explicit QCPAbstractItem(QCustomPlot *parentPlot);

        ~QCPAbstractItem() override;

        bool clipToAxisRect() const { return mClipToAxisRect; }

        QCPAxisRect *clipAxisRect() const;

        bool selectable() const { return mSelectable; }

        bool selected() const { return mSelected; }

        void setClipToAxisRect(bool clip);

        void setClipAxisRect(QCPAxisRect *rect);

        Q_SLOT void setSelectable(bool selectable);

        Q_SLOT void setSelected(bool selected);

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override = 0;

        QList<QCPItemPosition *> positions() const { return mPositions; }

        QList<QCPItemAnchor *> anchors() const { return mAnchors; }

        QCPItemPosition *position(const QString &name) const;

        QCPItemAnchor *anchor(const QString &name) const;

        bool hasAnchor(const QString &name) const;

    signals:

        void selectionChanged(bool selected);

        void selectableChanged(bool selectable);

    protected:
        bool mClipToAxisRect;
        QPointer<QCPAxisRect> mClipAxisRect;
        QList<QCPItemPosition *> mPositions;
        QList<QCPItemAnchor *> mAnchors;
        bool mSelectable, mSelected;

        QCP::Interaction selectionCategory() const override;

        QRect clipRect() const override;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void draw(QCPPainter *painter) override = 0;

        void selectEvent(QMouseEvent *event, bool additive, const QVariant &details,
                         bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;

        virtual QPointF anchorPixelPosition(int anchorId) const;

        double rectDistance(const QRectF &rect, const QPointF &pos, bool filledRect) const;

        QCPItemPosition *createPosition(const QString &name);

        QCPItemAnchor *createAnchor(const QString &name, int anchorId);
    };
}
#endif
