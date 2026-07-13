#ifndef __QPSIGNALBASE__
#define __QPSIGNALBASE__ 1

/// Abstract charge signal: a normalized 2D PDF scaled by an amplitude.
/// Coordinates are in µm, the amplitude in electrons (3.6 eV per e-h pair);
/// the analog signal is therefore a surface density in electrons/µm².
/// Concrete signals implement GetPDF.
class QPSignalBase{
    public:
    /// Calls Delete().
    virtual ~QPSignalBase();
    /// Cleanup hook called by the destructor; currently a no-op.
    virtual void   Delete();
    /// Signal density \f$ A\,p(x,y) \f$: amplitude times GetPDF.
    /// \param x evaluation point x, µm
    /// \param y evaluation point y, µm
    /// \return charge surface density, electrons/µm²
    double GetSignal(const double& x, const double& y);
    /// Midpoint-rule integral of GetSignal over the rectangle
    /// \f$[x_0,x_1]\times[y_0,y_1]\f$: the rectangle is divided into an
    /// \f$ n\times n \f$ grid sampled at the cell centers,
    /// \f[ \int \approx \sum_{i,j} A\,p(x_i, y_j)\,\Delta x\,\Delta y,
    ///     \qquad x_i = x_0 + (i+\tfrac12)\Delta x,\quad
    ///     \Delta x = (x_1-x_0)/n \f]
    /// \note \f$ n \f$ is currently hard-coded to 5; qupid::NIntegrateDivide
    ///       (= 100) is not used.
    /// \param x0 lower x bound, µm
    /// \param x1 upper x bound, µm
    /// \param y0 lower y bound, µm
    /// \param y1 upper y bound, µm
    /// \return integrated charge, electrons
    double Integral(const double& x0, const double& x1, const double& y0, const double& y1);

    /// Normalized PDF of the signal shape, 1/µm².
    /// The base implementation is a stand-in for a pure virtual: it prints a
    /// warning and returns 0.
    /// \param x evaluation point x, µm
    /// \param y evaluation point y, µm
    /// \return probability density at (x, y)
    virtual double GetPDF(const double& x, const double& y);

    protected:
    /// \param amplitude total deposited charge, electrons
    QPSignalBase(const double& amplitude);

    private:
    double fAmplitude;
};

#endif