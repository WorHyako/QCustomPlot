#ifndef QCUSTOMPLOT_QCPDATARANGE_HPP
#define QCUSTOMPLOT_QCPDATARANGE_HPP

#include "defs.hpp"

#include <QtCore>

namespace QCP {

    class QCP_LIB_DECL QCPDataRange {
    public:
        QCPDataRange();

        QCPDataRange(int begin, int end);

        bool operator==(const QCPDataRange &other) const { return mBegin == other.mBegin && mEnd == other.mEnd; }

        bool operator!=(const QCPDataRange &other) const { return !(*this == other); }

        int begin() const { return mBegin; }

        int end() const { return mEnd; }

        int size() const { return mEnd - mBegin; }

        int length() const { return size(); }

        void setBegin(int begin) { mBegin = begin; }

        void setEnd(int end) { mEnd = end; }

        bool isValid() const { return (mEnd >= mBegin) && (mBegin >= 0); }

        bool isEmpty() const { return length() == 0; }

        QCPDataRange bounded(const QCPDataRange &other) const;

        QCPDataRange expanded(const QCPDataRange &other) const;

        QCPDataRange intersection(const QCPDataRange &other) const;

        QCPDataRange adjusted(int changeBegin, int changeEnd) const {
            return QCPDataRange(mBegin + changeBegin, mEnd + changeEnd);
        }

        bool intersects(const QCPDataRange &other) const;

        bool contains(const QCPDataRange &other) const;

    private:
        int mBegin, mEnd;
    };
}
Q_DECLARE_TYPEINFO(QCP::QCPDataRange, Q_MOVABLE_TYPE);

#endif
