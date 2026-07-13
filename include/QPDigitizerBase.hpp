#ifndef __QPDIGITIZERBASE__
#define __QPDIGITIZERBASE__ 1

#include <deque>

class QPSignalBase;
class QPPixelBase;

/// Abstract pixel-matrix digitizer: owns the injected signals and the
/// matrix geometry; concrete classes define the pixel layout.
/// Lengths are in µm, amplitudes and thresholds in electrons.
/// \warning The destructor does not delete the injected signals; call
///          ClearSignal() before destroying the digitizer, or the signals
///          leak.
class QPDigitizerBase{
    public:
    /// Does not delete the injected signals; see the class warning.
    virtual ~QPDigitizerBase();
    /// Cleanup hook; currently a no-op.
    virtual void Delete();

    /// Hands a signal to the digitizer, which takes ownership: the default
    /// ClearSignal() deletes it.
    /// \param signal heap-allocated signal
    void InjectSignal(QPSignalBase* signal);
    /// Borrowed pointer to injected signal \p i; the digitizer keeps
    /// ownership.
    /// \param i signal index
    /// \return the signal, or nullptr when \p i is outside [0, GetNSignal())
    QPSignalBase * GetSignal(int i) const;
    /// \return number of injected signals
    int GetNSignal() const;
    /// Empties the signal store.
    /// \param removeall when true (default), every injected signal is
    ///                  deleted; when false, only the pointers are dropped
    ///                  and ownership returns to the caller
    void ClearSignal(bool removeall=true);

    /// Summed signal density of all injected signals at (x, y),
    /// \f$ \sum_k A_k\, p_k(x, y) \f$.
    /// \param x evaluation point x, µm
    /// \param y evaluation point y, µm
    /// \return charge surface density, electrons/µm²
    virtual double GetAnalogSignal(const double& x, const double& y);

    /// Pixel by linear index. The base implementation prints a warning and
    /// returns nullptr (stand-in for a pure virtual); concrete digitizers
    /// return a newly allocated pixel owned by the caller.
    /// \param index linear pixel index
    /// \return caller-owned pixel, or nullptr
    virtual QPPixelBase* GetPixel(const int index);
    /// Pixel by grid index. Same contract as GetPixel(int): the base prints
    /// a warning and returns nullptr; implementations return a caller-owned
    /// pixel.
    /// \param i pixel index along x
    /// \param j pixel index along y
    /// \return caller-owned pixel, or nullptr
    virtual QPPixelBase* GetPixel(const int i, const int j);

    /// Builds every pixel and keeps those whose digital signal exceeds
    /// \p threshold; the rest are deleted internally.
    /// \param threshold firing threshold, electrons
    /// \return deque of fired pixels, each owned by the caller (delete
    ///         them); its size is the cluster size
    std::deque<QPPixelBase*> GetFiredPixel(double threshold);

    /// \return total number of pixels
    int GetNPixel() const {return fNpix0;}

    /// \return matrix extent along x, µm
    double GetSizeX() const {return fSize[0];}
    /// \return matrix extent along y, µm
    double GetSizeY() const {return fSize[1];}
    /// \return number of pixels along x
    int GetNPixelX() const {return fNpix[0];}
    /// \return number of pixels along y
    int GetNPixelY() const {return fNpix[1];}

    protected:
    /// \param npix total number of pixels
    QPDigitizerBase(int npix);
    double fSize[2]; ///< matrix extent {x, y}, µm
    int    fNpix[2]; ///< number of pixels {x, y}

    private:
    int    fNpix0=0;

    std::deque<QPSignalBase*> fSignals;
};

#endif