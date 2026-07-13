#ifndef __QPPIXEL__
#define __QPPIXEL__ 1

#include "QPPixelBase.hpp"

class QPSignalBase;
class QPDigitizerSquare;

/// Rectangular pixel cell \f$ [x_0, x_1] \times [y_0, y_1] \f$ (µm) with
/// grid indices (i, j).
class QPPixel: public QPPixelBase{
    public:
    /// \param x0        lower cell corner x, µm
    /// \param y0        lower cell corner y, µm
    /// \param x1        upper cell corner x, µm
    /// \param y1        upper cell corner y, µm
    /// \param digitizer digitizer this pixel belongs to; not owned
    /// \param i         pixel index along x
    /// \param j         pixel index along y
    QPPixel(const double & x0,  const double & y0,
            const double & x1,  const double & y1,
            QPDigitizerBase* digitizer=nullptr,
            const int    & i=0, const int   & j=0);
    /// Trivial; the pixel owns nothing.
    virtual ~QPPixel();
    /// Cleanup hook; currently a no-op.
    virtual void    Delete();
    /// Charge collected from one signal: its Integral over this cell.
    /// \param signal signal to integrate; not owned
    /// \return integrated charge, electrons
    virtual double  GetDigitalSignalOne(QPSignalBase * signal);

    /// \return pixel index along x
    int GetPixX() const {return fIndX;}
    /// \return pixel index along y
    int GetPixY() const {return fIndY;}
    /// Cell extent along x.
    /// \warning Declared int: the double extent \f$ x_1 - x_0 \f$ is
    ///          truncated (the ALPIDE-like pitch 29.24 µm comes back as 29).
    /// \return truncated cell width, µm
    int GetPixelSizeX() const {return fx1-fx0;}
    /// Cell extent along y.
    /// \warning Same truncation as GetPixelSizeX (26.88 µm comes back
    ///          as 26).
    /// \return truncated cell height, µm
    int GetPixelSizeY() const {return fy1-fy0;}
    protected:
    private:
    double fx0=0.;
    double fy0=0.;
    double fx1=1.;
    double fy1=1.;

    int    fIndX = 0;
    int    fIndY = 0;
};

#endif