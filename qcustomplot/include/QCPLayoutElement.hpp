#ifndef QCUSTOMPLOT_QCPLAYOUTELEMENT_HPP
#define QCUSTOMPLOT_QCPLAYOUTELEMENT_HPP

#include "defs.hpp"
#include "enums.hpp"
#include "QCPLayerable.hpp"
#include "QCPMarginGroup.hpp"
#include "QCPLayout.hpp"

#include <QHash>
#include <QRect>
#include <QMargins>
#include <QList>
#include <QPointF>
#include <QVariant>

namespace QCP {

    class QCP_LIB_DECL QCPLayoutElement : public QCPLayerable {
    Q_OBJECT
        Q_PROPERTY(QCPLayout *layout READ layout)
        Q_PROPERTY(QRect rect READ rect)
        Q_PROPERTY(QRect outerRect READ outerRect WRITE setOuterRect)
        Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
        Q_PROPERTY(QMargins minimumMargins READ minimumMargins WRITE setMinimumMargins)
        Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
        Q_PROPERTY(QSize maximumSize READ maximumSize WRITE setMaximumSize)
        Q_PROPERTY(SizeConstraintRect sizeConstraintRect READ sizeConstraintRect WRITE setSizeConstraintRect)
    public:
        /*!
        Defines the phases of the update process, that happens just before a replot. At each phase,
        \ref update is called with the according UpdatePhase value.
      */
        enum UpdatePhase {
            upPreparation, ///< Phase used for any type of preparation that needs to be done before margin calculation and layout
            upMargins,    ///< Phase in which the margins are calculated and set
            upLayout     ///< Final phase in which the layout system places the rects of the elements
        };

        Q_ENUM(UpdatePhase)

        /*!
        Defines to which rect of a layout element the size constraints that can be set via \ref
        setMinimumSize and \ref setMaximumSize apply. The outer rect (\ref outerRect) includes the
        margins (e.g. in the case of a QCPAxisRect the axis labels), whereas the inner rect (\ref rect)
        does not.

        \see setSizeConstraintRect
      */
        enum SizeConstraintRect {
            scrInnerRect, ///< Minimum/Maximum size constraints apply to inner rect
            scrOuterRect ///< Minimum/Maximum size constraints apply to outer rect, thus include layout element margins
        };

        Q_ENUM(SizeConstraintRect)

        Q_DISABLE_COPY(QCPLayoutElement)

        explicit QCPLayoutElement(QCustomPlot *parentPlot = nullptr);

        ~QCPLayoutElement() override;

        QCPLayout *layout() const { return mParentLayout; }

        QRect rect() const { return mRect; }

        QRect outerRect() const { return mOuterRect; }

        QMargins margins() const { return mMargins; }

        QMargins minimumMargins() const { return mMinimumMargins; }

        MarginSides autoMargins() const { return mAutoMargins; }

        QSize minimumSize() const { return mMinimumSize; }

        QSize maximumSize() const { return mMaximumSize; }

        SizeConstraintRect sizeConstraintRect() const { return mSizeConstraintRect; }

        QCPMarginGroup *marginGroup(MarginSide side) const { return mMarginGroups.value(side, nullptr); }

        QHash<MarginSide, QCPMarginGroup *> marginGroups() const { return mMarginGroups; }

        void setOuterRect(const QRect &rect);

        void setMargins(const QMargins &margins);

        void setMinimumMargins(const QMargins &margins);

        void setAutoMargins(MarginSides sides);

        void setMinimumSize(const QSize &size);

        void setMinimumSize(int width, int height);

        void setMaximumSize(const QSize &size);

        void setMaximumSize(int width, int height);

        void setSizeConstraintRect(SizeConstraintRect constraintRect);

        void setMarginGroup(MarginSides sides, QCPMarginGroup *group);

        virtual void update(UpdatePhase phase);

        virtual QSize minimumOuterSizeHint() const;

        virtual QSize maximumOuterSizeHint() const;

        virtual QList<QCPLayoutElement *> elements(bool recursive) const;

        double selectTest(const QPointF &pos, bool onlySelectable, QVariant *details = nullptr) const override;

    protected:
        QCPLayout *mParentLayout;
        QSize mMinimumSize, mMaximumSize;
        SizeConstraintRect mSizeConstraintRect;
        QRect mRect, mOuterRect;
        QMargins mMargins, mMinimumMargins;
        MarginSides mAutoMargins;
        QHash<MarginSide, QCPMarginGroup *> mMarginGroups;

        virtual int calculateAutoMargin(MarginSide side);

        virtual void layoutChanged();

        void applyDefaultAntialiasingHint(QCPPainter *painter) const override { Q_UNUSED(painter) }

        void draw(QCPPainter *painter) override { Q_UNUSED(painter) }

        void parentPlotInitialized(QCustomPlot *parentPlot) override;
    };
}
Q_DECLARE_METATYPE(QCP::QCPLayoutElement::UpdatePhase)

#endif
