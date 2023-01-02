#ifndef QCUSTOMPLOT_QCPCOLORMAPDATA_H
#define QCUSTOMPLOT_QCPCOLORMAPDATA_H

namespace QCP {

    class QCP_LIB_DECL QCPColorMapData
            {
                    public:
                    QCPColorMapData(int keySize, int valueSize, const QCPRange &keyRange, const QCPRange &valueRange);
                    ~QCPColorMapData();
                    QCPColorMapData(const QCPColorMapData &other);
                    QCPColorMapData &operator=(const QCPColorMapData &other);

                    // getters:
                    int keySize() const { return mKeySize; }
                    int valueSize() const { return mValueSize; }
                    QCPRange keyRange() const { return mKeyRange; }
                    QCPRange valueRange() const { return mValueRange; }
                    QCPRange dataBounds() const { return mDataBounds; }
                    double data(double key, double value);
                    double cell(int keyIndex, int valueIndex);
                    unsigned char alpha(int keyIndex, int valueIndex);

                    // setters:
                    void setSize(int keySize, int valueSize);
                    void setKeySize(int keySize);
                    void setValueSize(int valueSize);
                    void setRange(const QCPRange &keyRange, const QCPRange &valueRange);
                    void setKeyRange(const QCPRange &keyRange);
                    void setValueRange(const QCPRange &valueRange);
                    void setData(double key, double value, double z);
                    void setCell(int keyIndex, int valueIndex, double z);
                    void setAlpha(int keyIndex, int valueIndex, unsigned char alpha);

                    // non-property methods:
                    void recalculateDataBounds();
                    void clear();
                    void clearAlpha();
                    void fill(double z);
                    void fillAlpha(unsigned char alpha);
                    bool isEmpty() const { return mIsEmpty; }
                    void coordToCell(double key, double value, int *keyIndex, int *valueIndex) const;
                    void cellToCoord(int keyIndex, int valueIndex, double *key, double *value) const;

                    protected:
                    // property members:
                    int mKeySize, mValueSize;
                    QCPRange mKeyRange, mValueRange;
                    bool mIsEmpty;

                    // non-property members:
                    double *mData;
                    unsigned char *mAlpha;
                    QCPRange mDataBounds;
                    bool mDataModified;

                    bool createAlpha(bool initializeOpaque=true);

                    friend class QCPColorMap;
            };



} // QCP

#endif //QCUSTOMPLOT_QCPCOLORMAPDATA_H
