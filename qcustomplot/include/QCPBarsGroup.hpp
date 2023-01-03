#ifndef QCUSTOMPLOT_QCPBARSGROUP_HPP
#define QCUSTOMPLOT_QCPBARSGROUP_HPP

#include "QCustomPlot.hpp"
#include "QCPBars.hpp"

#include <QObject>

namespace QCP {

    class QCP_LIB_DECL QCPBarsGroup : public QObject {
    Q_OBJECT
        Q_PROPERTY(SpacingType spacingType READ spacingType WRITE setSpacingType)
        Q_PROPERTY(double spacing READ spacing WRITE setSpacing)
    public:
        /*!
        Defines the ways the spacing between bars in the group can be specified. Thus it defines what
        the number passed to \ref setSpacing actually means.

        \see setSpacingType, setSpacing
      */
        enum SpacingType {
            stAbsolute,      ///< Bar spacing is in absolute pixels
            stAxisRectRatio, ///< Bar spacing is given by a fraction of the axis rect size
            stPlotCoords,   ///< Bar spacing is in key coordinates and thus scales with the key axis range
        };

        Q_ENUM(SpacingType)

        Q_DISABLE_COPY(QCPBarsGroup)

        explicit QCPBarsGroup(QCustomPlot *parentPlot);

        virtual ~QCPBarsGroup();

        SpacingType spacingType() const { return mSpacingType; }

        double spacing() const { return mSpacing; }

        void setSpacingType(SpacingType spacingType);

        void setSpacing(double spacing);

        QList<QCPBars *> bars() const { return mBars; }

        QCPBars *bars(int index) const;

        int size() const { return mBars.size(); }

        bool isEmpty() const { return mBars.isEmpty(); }

        void clear();

        bool contains(QCPBars *bars) const { return mBars.contains(bars); }

        void append(QCPBars *bars);

        void insert(int i, QCPBars *bars);

        void remove(QCPBars *bars);

    protected:
        QCustomPlot *mParentPlot;
        SpacingType mSpacingType;
        double mSpacing;
        QList<QCPBars *> mBars;

        void registerBars(QCPBars *bars);

        void unregisterBars(QCPBars *bars);

        double keyPixelOffset(const QCPBars *bars, double keyCoord);

        double getPixelSpacing(const QCPBars *bars, double keyCoord);
    };
}
Q_DECLARE_METATYPE(QCP::QCPBarsGroup::SpacingType)

#endif
