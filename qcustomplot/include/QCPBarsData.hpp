#ifndef QCUSTOMPLOT_QCPBARSDATA_H
#define QCUSTOMPLOT_QCPBARSDATA_H

namespace QCP {

    class QCP_LIB_DECL QCPBarsData
            {
                    public:
                    QCPBarsData();
                    QCPBarsData(double key, double value);

                    inline double sortKey() const { return key; }
                    inline static QCPBarsData fromSortKey(double sortKey) { return QCPBarsData(sortKey, 0); }
                    inline static bool sortKeyIsMainKey() { return true; }

                    inline double mainKey() const { return key; }
                    inline double mainValue() const { return value; }

                    inline QCPRange valueRange() const { return QCPRange(value, value); } // note that bar base value isn't held in each QCPBarsData and thus can't/shouldn't be returned here

                    double key, value;
            };
    Q_DECLARE_TYPEINFO(QCPBarsData, Q_PRIMITIVE_TYPE);


/*! \typedef QCPBarsDataContainer

  Container for storing \ref QCPBarsData points. The data is stored sorted by \a key.

  This template instantiation is the container in which QCPBars holds its data. For details about
  the generic container, see the documentation of the class template \ref QCPDataContainer.

  \see QCPBarsData, QCPBars::setData
*/
    typedef QCPDataContainer<QCPBarsData> QCPBarsDataContainer;


} // QCP

#endif //QCUSTOMPLOT_QCPBARSDATA_H
