#include "include/QCPCurveData.hpp"

namespace QCP {
/*! \class QCPCurveData
  \brief Holds the data of one single data point for QCPCurve.

  The stored data is:
  \li \a t: the free ordering parameter of this curve point, like in the mathematical vector <em>(x(t), y(t))</em>. (This is the \a sortKey)
  \li \a key: coordinate on the key axis of this curve point (this is the \a mainKey)
  \li \a value: coordinate on the value axis of this curve point (this is the \a mainValue)

  The container for storing multiple data points is \ref QCPCurveDataContainer. It is a typedef for
  \ref QCPDataContainer with \ref QCPCurveData as the DataType template parameter. See the
  documentation there for an explanation regarding the data type's generic methods.

  \see QCPCurveDataContainer
*/

/* start documentation of inline functions */

/*! \fn double QCPCurveData::sortKey() const

  Returns the \a t member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static QCPCurveData QCPCurveData::fromSortKey(double sortKey)

  Returns a data point with the specified \a sortKey (assigned to the data point's \a t member).
  All other members are set to zero.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn static static bool QCPCurveData::sortKeyIsMainKey()

  Since the member \a key is the data point key coordinate and the member \a t is the data ordering
  parameter, this method returns false.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPCurveData::mainKey() const

  Returns the \a key member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn double QCPCurveData::mainValue() const

  Returns the \a value member of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/*! \fn QCPRange QCPCurveData::valueRange() const

  Returns a QCPRange with both lower and upper boundary set to \a value of this data point.

  For a general explanation of what this method is good for in the context of the data container,
  see the documentation of \ref QCPDataContainer.
*/

/* end documentation of inline functions */

/*!
  Constructs a curve data point with t, key and value set to zero.
*/
    QCPCurveData::QCPCurveData() :
            t(0),
            key(0),
            value(0)
    {
    }

/*!
  Constructs a curve data point with the specified \a t, \a key and \a value.
*/
    QCPCurveData::QCPCurveData(double t, double key, double value) :
            t(t),
            key(key),
            value(value)
    {
    }


} // QCP