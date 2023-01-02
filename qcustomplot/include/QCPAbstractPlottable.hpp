#ifndef QCUSTOMPLOT_QCPABSTRACTPLOTTABLE_HPP
#define QCUSTOMPLOT_QCPABSTRACTPLOTTABLE_HPP

#include "defs.hpp"
#include "QCPLayerable.hpp"
#include "QCPAxis.hpp"
#include "QCPDataSelection.hpp"
#include "QCPSelectionDecorator.hpp"
#include "QCPRange.hpp"
#include "QCPPlottableInterface1D.hpp"

#include <QPointer>

namespace QCP {

    class QCP_LIB_DECL QCPAbstractPlottable : public QCPLayerable {
    Q_OBJECT
        /// \cond INCLUDE_QPROPERTIES
        Q_PROPERTY(QString name READ name WRITE setName)
        Q_PROPERTY(bool antialiasedFill READ antialiasedFill WRITE setAntialiasedFill)
        Q_PROPERTY(bool antialiasedScatters READ antialiasedScatters WRITE setAntialiasedScatters)
        Q_PROPERTY(QPen pen READ pen WRITE setPen)
        Q_PROPERTY(QBrush brush READ brush WRITE setBrush)
        Q_PROPERTY(QCPAxis * keyAxis READ keyAxis WRITE setKeyAxis)
        Q_PROPERTY(QCPAxis * valueAxis READ valueAxis WRITE setValueAxis)
        Q_PROPERTY(QCP::SelectionType selectable READ selectable WRITE setSelectable NOTIFY selectableChanged)
        Q_PROPERTY(QCPDataSelection selection READ selection WRITE setSelection NOTIFY selectionChanged)
        Q_PROPERTY(QCPSelectionDecorator * selectionDecorator READ selectionDecorator WRITE setSelectionDecorator)
    public:
        Q_DISABLE_COPY(QCPAbstractPlottable)

        /// \endcond
        QCPAbstractPlottable(QCPAxis *keyAxis, QCPAxis *valueAxis);

        ~QCPAbstractPlottable() override;

        // getters:
        QString name() const { return mName; }

        bool antialiasedFill() const { return mAntialiasedFill; }

        bool antialiasedScatters() const { return mAntialiasedScatters; }

        QPen pen() const { return mPen; }

        QBrush brush() const { return mBrush; }

        QCPAxis *keyAxis() const { return mKeyAxis.data(); }

        QCPAxis *valueAxis() const { return mValueAxis.data(); }

        SelectionType selectable() const { return mSelectable; }

        bool selected() const { return !mSelection.isEmpty(); }

        QCPDataSelection selection() const { return mSelection; }

        QCPSelectionDecorator *selectionDecorator() const { return mSelectionDecorator; }

        void setName(const QString &name);

        void setAntialiasedFill(bool enabled);

        void setAntialiasedScatters(bool enabled);

        void setPen(const QPen &pen);

        void setBrush(const QBrush &brush);

        void setKeyAxis(QCPAxis *axis);

        void setValueAxis(QCPAxis *axis);

        Q_SLOT void setSelectable(QCP::SelectionType selectable);

        Q_SLOT void setSelection(QCPDataSelection selection);

        void setSelectionDecorator(QCPSelectionDecorator *decorator);

        double selectTest(const QPointF &pos, bool onlySelectable,
                          QVariant *details = nullptr) const override = 0; // actually introduced in QCPLayerable as non-pure, but we want to force reimplementation for plottables
        virtual QCPPlottableInterface1D *interface1D() { return nullptr; }

        virtual QCPRange getKeyRange(bool &foundRange, SignDomain inSignDomain = sdBoth) const = 0;

        virtual QCPRange getValueRange(bool &foundRange, SignDomain inSignDomain = sdBoth,
                                       const QCPRange &inKeyRange = QCPRange()) const = 0;

        void coordsToPixels(double key, double value, double &x, double &y) const;

        const QPointF coordsToPixels(double key, double value) const;

        void pixelsToCoords(double x, double y, double &key, double &value) const;

        void pixelsToCoords(const QPointF &pixelPos, double &key, double &value) const;

        void rescaleAxes(bool onlyEnlarge = false) const;

        void rescaleKeyAxis(bool onlyEnlarge = false) const;

        void rescaleValueAxis(bool onlyEnlarge = false, bool inKeyRange = false) const;

        bool addToLegend(QCPLegend *legend);

        bool addToLegend();

        bool removeFromLegend(QCPLegend *legend) const;

        bool removeFromLegend() const;

    signals:

        void selectionChanged(bool selected);

        void selectionChanged(const QCPDataSelection &selection);

        void selectableChanged(QCP::SelectionType selectable);

    protected:
        QString mName;
        bool mAntialiasedFill, mAntialiasedScatters;
        QPen mPen;
        QBrush mBrush;
        QPointer<QCPAxis> mKeyAxis, mValueAxis;
        QCP::SelectionType mSelectable;
        QCPDataSelection mSelection;
        QCPSelectionDecorator *mSelectionDecorator;

        QRect clipRect() const override;

        void draw(QCPPainter *painter) override = 0;

        QCP::Interaction selectionCategory() const override;

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override;

        void selectEvent(QMouseEvent *event, bool additive, const QVariant &details,
                         bool *selectionStateChanged) override;

        void deselectEvent(bool *selectionStateChanged) override;

        virtual void drawLegendIcon(QCPPainter *painter, const QRectF &rect) const = 0;

        void applyFillAntialiasingHint(QCPPainter *painter) const;

        void applyScattersAntialiasingHint(QCPPainter *painter) const;

    };
}
#endif
