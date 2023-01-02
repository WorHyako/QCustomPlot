#ifndef QCUSTOMPLOT_QCPAXISTICKERPI_H
#define QCUSTOMPLOT_QCPAXISTICKERPI_H

namespace QCP {

    class QCP_LIB_DECL QCPAxisTickerPi : public QCPAxisTicker
{
    Q_GADGET
    public:
    /*!
    Defines how fractions should be displayed in tick labels.

    \see setFractionStyle
  */
    enum FractionStyle { fsFloatingPoint     ///< Fractions are displayed as regular decimal floating point numbers, e.g. "0.25" or "0.125".
        ,fsAsciiFractions   ///< Fractions are written as rationals using ASCII characters only, e.g. "1/4" or "1/8"
        ,fsUnicodeFractions ///< Fractions are written using sub- and superscript UTF-8 digits and the fraction symbol.
    };
    Q_ENUMS(FractionStyle)

    QCPAxisTickerPi();

    // getters:
    QString piSymbol() const { return mPiSymbol; }
    double piValue() const { return mPiValue; }
    bool periodicity() const { return mPeriodicity; }
    FractionStyle fractionStyle() const { return mFractionStyle; }

    // setters:
    void setPiSymbol(QString symbol);
    void setPiValue(double pi);
    void setPeriodicity(int multiplesOfPi);
    void setFractionStyle(FractionStyle style);

    protected:
    // property members:
    QString mPiSymbol;
    double mPiValue;
    int mPeriodicity;
    FractionStyle mFractionStyle;

    // non-property members:
    double mPiTickStep; // size of one tick step in units of mPiValue

    // reimplemented virtual methods:
    virtual double getTickStep(const QCPRange &range) Q_DECL_OVERRIDE;
    virtual int getSubTickCount(double tickStep) Q_DECL_OVERRIDE;
    virtual QString getTickLabel(double tick, const QLocale &locale, QChar formatChar, int precision) Q_DECL_OVERRIDE;

    // non-virtual methods:
    void simplifyFraction(int &numerator, int &denominator) const;
    QString fractionToString(int numerator, int denominator) const;
    QString unicodeFraction(int numerator, int denominator) const;
    QString unicodeSuperscript(int number) const;
    QString unicodeSubscript(int number) const;
};
Q_DECLARE_METATYPE(QCPAxisTickerPi::FractionStyle)


} // QCP

#endif //QCUSTOMPLOT_QCPAXISTICKERPI_H
