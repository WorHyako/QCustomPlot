#ifndef QCUSTOMPLOT_QCPFINANCIALDATA_H
#define QCUSTOMPLOT_QCPFINANCIALDATA_H

namespace QCP {

    class QCP_LIB_DECL QCPFinancialData
            {
                    public:
                    QCPFinancialData();
                    QCPFinancialData(double key, double open, double high, double low, double close);

                    inline double sortKey() const { return key; }
                    inline static QCPFinancialData fromSortKey(double sortKey) { return QCPFinancialData(sortKey, 0, 0, 0, 0); }
                    inline static bool sortKeyIsMainKey() { return true; }

                    inline double mainKey() const { return key; }
                    inline double mainValue() const { return open; }

                    inline QCPRange valueRange() const { return QCPRange(low, high); } // open and close must lie between low and high, so we don't need to check them

                    double key, open, high, low, close;
            };
    Q_DECLARE_TYPEINFO(QCPFinancialData, Q_PRIMITIVE_TYPE);


/*! \typedef QCPFinancialDataContainer

  Container for storing \ref QCPFinancialData points. The data is stored sorted by \a key.

  This template instantiation is the container in which QCPFinancial holds its data. For details
  about the generic container, see the documentation of the class template \ref QCPDataContainer.

  \see QCPFinancialData, QCPFinancial::setData
*/
    typedef QCPDataContainer<QCPFinancialData> QCPFinancialDataContainer;

} // QCP

#endif //QCUSTOMPLOT_QCPFINANCIALDATA_H
