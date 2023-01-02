#include "include/QCPErrorBarsData.hpp"

namespace QCP {
/*! \class QCPErrorBarsData
  \brief Holds the data of one single error bar for QCPErrorBars.

  The stored data is:
  \li \a errorMinus: how much the error bar extends towards negative coordinates from the data
  point position
  \li \a errorPlus: how much the error bar extends towards positive coordinates from the data point
  position

  The container for storing the error bar information is \ref QCPErrorBarsDataContainer. It is a
  typedef for <tt>QVector<\ref QCPErrorBarsData></tt>.

  \see QCPErrorBarsDataContainer
*/

/*!
  Constructs an error bar with errors set to zero.
*/
    QCPErrorBarsData::QCPErrorBarsData() :
            errorMinus(0),
            errorPlus(0)
    {
    }

/*!
  Constructs an error bar with equal \a error in both negative and positive direction.
*/
    QCPErrorBarsData::QCPErrorBarsData(double error) :
            errorMinus(error),
            errorPlus(error)
    {
    }

/*!
  Constructs an error bar with negative and positive errors set to \a errorMinus and \a errorPlus,
  respectively.
*/
    QCPErrorBarsData::QCPErrorBarsData(double errorMinus, double errorPlus) :
            errorMinus(errorMinus),
            errorPlus(errorPlus)
    {
    }

} // QCP