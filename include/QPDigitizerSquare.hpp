#ifndef __QPDIGITIZERSQUARE__
#define __QPDIGITIZERSQUARE__ 1

#include "QPDigitizerBase.hpp"
#include "QPPixel.hpp"

class QPGraphFactory;

/// Uniform rectangular pixel grid centered on the origin: pixel (i, j)
/// covers \f$ [i\,p_x - S_x/2,\ (i+1)\,p_x - S_x/2] \times
/// [j\,p_y - S_y/2,\ (j+1)\,p_y - S_y/2] \f$ with pitch
/// \f$ p_x = S_x/n_x \f$, \f$ p_y = S_y/n_y \f$. Lengths in µm.
class QPDigitizerSquare: public QPDigitizerBase{
    public:
    /// \param sizex matrix extent along x, µm
    /// \param sizey matrix extent along y, µm
    /// \param npixx number of pixels along x
    /// \param npixy number of pixels along y
    QPDigitizerSquare(double sizex=1, double sizey=1, int npixx=1, int npixy=1);

    // virtual double GetAnalogSignal(const double& x, const double& y);

    /// Pixel by linear index, decomposed as
    /// (index / npixx, index % npixx) and forwarded to GetPixel(int,int).
    /// \warning The bounds check accepts index == npixx·npixy (it uses >
    ///          instead of >=), and the quotient is validated against npixx
    ///          although it ranges over the y count: the mapping is a clean
    ///          bijection only for square grids (npixx == npixy).
    /// \param index linear pixel index
    /// \return caller-owned new pixel, or nullptr when rejected
    virtual QPPixel* GetPixel(const int index);
    /// Allocates and returns the pixel cell (i, j); every call produces a
    /// new QPPixel owned by the caller.
    /// \warning The upper-bound checks use `fNpix < i` rather than `<=`,
    ///          so i == npixx and j == npixy are accepted and yield a
    ///          one-past-the-end pixel outside the matrix.
    /// \param i pixel index along x
    /// \param j pixel index along y
    /// \return caller-owned new pixel, or nullptr when out of bounds
    virtual QPPixel* GetPixel(const int i, const int j);

    protected:

    private:

    /// Grants the visualization module access to the digitizer internals.
    friend class QPGraphFactory;
};

#endif