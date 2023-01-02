#ifndef QCUSTOMPLOT_QCPCOLORGRADIENT_HPP
#define QCUSTOMPLOT_QCPCOLORGRADIENT_HPP

#include "defs.hpp"
#include "QCPRange.hpp"

#include <QObject>
#include <QMap>
#include <QColor>
#include <QVector>

namespace QCP {

    class QCP_LIB_DECL QCPColorGradient {
    Q_GADGET
    public:
        /*!
        Defines the color spaces in which color interpolation between gradient stops can be performed.

        \see setColorInterpolation
      */
        enum ColorInterpolation {
            ciRGB,  ///< Color channels red, green and blue are linearly interpolated
            ciHSV ///< Color channels hue, saturation and value are linearly interpolated (The hue is interpolated over the shortest angle distance)
        };
        Q_ENUM(ColorInterpolation)

        /*!
        Defines how NaN data points shall appear in the plot.

        \see setNanHandling, setNanColor
      */
        enum NanHandling {
            nhNone, ///< NaN data points are not explicitly handled and shouldn't occur in the data (this gives slight performance improvement)
            nhLowestColor,  ///< NaN data points appear as the lowest color defined in this QCPColorGradient
            nhHighestColor, ///< NaN data points appear as the highest color defined in this QCPColorGradient
            nhTransparent, ///< NaN data points appear transparent
            nhNanColor ///< NaN data points appear as the color defined with \ref setNanColor
        };
        Q_ENUM(NanHandling)

        /*!
        Defines the available presets that can be loaded with \ref loadPreset. See the documentation
        there for an image of the presets.
      */
        enum GradientPreset {
            gpGrayscale,  ///< Continuous lightness from black to white (suited for non-biased data representation)
            gpHot,       ///< Continuous lightness from black over firey colors to white (suited for non-biased data representation)
            gpCold,      ///< Continuous lightness from black over icey colors to white (suited for non-biased data representation)
            gpNight,     ///< Continuous lightness from black over weak blueish colors to white (suited for non-biased data representation)
            gpCandy,     ///< Blue over pink to white
            gpGeography, ///< Colors suitable to represent different elevations on geographical maps
            gpIon,     ///< Half hue spectrum from black over purple to blue and finally green (creates banding illusion but allows more precise magnitude estimates)
            gpThermal,   ///< Colors suitable for thermal imaging, ranging from dark blue over purple to orange, yellow and white
            gpPolar,  ///< Colors suitable to emphasize polarity around the center, with blue for negative, black in the middle and red for positive values
            gpSpectrum, ///< An approximation of the visible light spectrum (creates banding illusion but allows more precise magnitude estimates)
            gpJet, ///< Hue variation similar to a spectrum, often used in numerical visualization (creates banding illusion but allows more precise magnitude estimates)
            gpHues, ///< Full hue cycle, with highest and lowest color red (suitable for periodic data, such as angles and phases, see \ref setPeriodic)
        };
        Q_ENUM(GradientPreset)

        QCPColorGradient();

        QCPColorGradient(GradientPreset preset);

        bool operator==(const QCPColorGradient &other) const;

        bool operator!=(const QCPColorGradient &other) const { return !(*this == other); }

        int levelCount() const { return mLevelCount; }

        QMap<double, QColor> colorStops() const { return mColorStops; }

        ColorInterpolation colorInterpolation() const { return mColorInterpolation; }

        NanHandling nanHandling() const { return mNanHandling; }

        QColor nanColor() const { return mNanColor; }

        bool periodic() const { return mPeriodic; }

        void setLevelCount(int n);

        void setColorStops(const QMap<double, QColor> &colorStops);

        void setColorStopAt(double position, const QColor &color);

        void setColorInterpolation(ColorInterpolation interpolation);

        void setNanHandling(NanHandling handling);

        void setNanColor(const QColor &color);

        void setPeriodic(bool enabled);

        void colorize(const double *data, const QCPRange &range, QRgb *scanLine, int n, int dataIndexFactor = 1,
                      bool logarithmic = false);

        void colorize(const double *data, const unsigned char *alpha, const QCPRange &range, QRgb *scanLine, int n,
                      int dataIndexFactor = 1, bool logarithmic = false);

        QRgb color(double position, const QCPRange &range, bool logarithmic = false);

        void loadPreset(GradientPreset preset);

        void clearColorStops();

        QCPColorGradient inverted() const;

    protected:
        int mLevelCount;
        QMap<double, QColor> mColorStops;
        ColorInterpolation mColorInterpolation;
        NanHandling mNanHandling;
        QColor mNanColor;
        bool mPeriodic;

        QVector<QRgb> mColorBuffer; // have colors premultiplied with alpha (for usage with QImage::Format_ARGB32_Premultiplied)
        bool mColorBufferInvalidated;

        bool stopsUseAlpha() const;

        void updateColorBuffer();
    };
}
Q_DECLARE_METATYPE(QCP::QCPColorGradient::ColorInterpolation)

Q_DECLARE_METATYPE(QCP::QCPColorGradient::NanHandling)

Q_DECLARE_METATYPE(QCP::QCPColorGradient::GradientPreset)

#endif
