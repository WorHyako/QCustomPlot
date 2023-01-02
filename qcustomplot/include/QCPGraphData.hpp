#ifndef QCUSTOMPLOT_QCPGRAPHDATA_HPP
#define QCUSTOMPLOT_QCPGRAPHDATA_HPP

#include "defs.hpp"
#include "QCPRange.hpp"
#include "QCPDataContainer.hpp"

#include <QtCore>

namespace QCP {

    class QCP_LIB_DECL QCPGraphData {
    public:
        QCPGraphData();

        QCPGraphData(double key, double value);

        inline double sortKey() const { return key; }

        inline static QCPGraphData fromSortKey(double sortKey) { return QCPGraphData(sortKey, 0); }

        inline static bool sortKeyIsMainKey() { return true; }

        inline double mainKey() const { return key; }

        inline double mainValue() const { return value; }

        inline QCPRange valueRange() const { return QCPRange(value, value); }

        double key, value;
    };

/*! \typedef QCPGraphDataContainer

  Container for storing \ref QCPGraphData points. The data is stored sorted by \a key.

  This template instantiation is the container in which QCPGraph holds its data. For details about
  the generic container, see the documentation of the class template \ref QCPDataContainer.

  \see QCPGraphData, QCPGraph::setData
*/
    typedef QCPDataContainer<QCPGraphData> QCPGraphDataContainer;
}
Q_DECLARE_TYPEINFO(QCP::QCPGraphData, Q_PRIMITIVE_TYPE);

#endif
