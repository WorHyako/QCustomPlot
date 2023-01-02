#ifndef QCUSTOMPLOT_INLINES_H
#define QCUSTOMPLOT_INLINES_H

#include "enums.hpp"

namespace QCP {
/*! \internal

  Returns whether the specified \a value is considered an invalid data value for plottables (i.e.
  is \e nan or \e +/-inf). This function is used to check data validity upon replots, when the
  compiler flag \c QCUSTOMPLOT_CHECK_DATA is set.
*/
    inline bool isInvalidData(double value) {
        return qIsNaN(value) || qIsInf(value);
    }

/*! \internal
  \overload

  Checks two arguments instead of one.
*/
    inline bool isInvalidData(double value1, double value2) {
        return isInvalidData(value1) || isInvalidData(value2);
    }

/*! \internal

  Sets the specified \a side of \a margins to \a value

  \see getMarginValue
*/
    inline void setMarginValue(QMargins &margins, QCP::MarginSide side, int value) {
        switch (side) {
            case QCP::msLeft:
                margins.setLeft(value);
                break;
            case QCP::msRight:
                margins.setRight(value);
                break;
            case QCP::msTop:
                margins.setTop(value);
                break;
            case QCP::msBottom:
                margins.setBottom(value);
                break;
            case QCP::msAll:
                margins = QMargins(value, value, value, value);
                break;
            default:
                break;
        }
    }

/*! \internal

  Returns the value of the specified \a side of \a margins. If \a side is \ref QCP::msNone or
  \ref QCP::msAll, returns 0.

  \see setMarginValue
*/
    inline int getMarginValue(const QMargins &margins, QCP::MarginSide side) {
        switch (side) {
            case QCP::msLeft:
                return margins.left();
            case QCP::msRight:
                return margins.right();
            case QCP::msTop:
                return margins.top();
            case QCP::msBottom:
                return margins.bottom();
            default:
                break;
        }
        return 0;
    }
}
#endif
