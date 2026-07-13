#ifndef __QPMONTECARLO__
#define __QPMONTECARLO__ 1

#include <string>
#include <vector>

class QPDigitizerBase;
class QPRandomBase;

/// Monte Carlo driver for cluster-size studies.
/// Per event: draw a signal position (and optionally an amplitude), inject
/// one isometric Gaussian signal into the digitizer, count the pixels above
/// threshold, clear the digitizer. Positions are in µm, amplitudes and
/// thresholds in electrons.
///
/// The per-event draw order is fixed by Shoot() and must not change:
/// x, then y, then amplitude (amplitude only when SetAmplitudeRange was
/// used; SetAmplitude keeps it fixed and draws nothing). Regression
/// baselines depend on this order.
class QPMonteCarlo{
    public:
    /// One generated event, plain data only.
    struct Record{
        double fAmp;   ///< signal amplitude, electrons
        double fX;     ///< signal position x, µm
        double fY;     ///< signal position y, µm
        double fSigma; ///< value placed on both diagonal entries of the covariance-form matrix S
        int    fNpix;  ///< pixels above threshold (cluster size)
    };

    /// \param digitizer target digitizer; not owned
    /// \param random    random engine; caller-owned, kept by reference
    QPMonteCarlo(QPDigitizerBase* digitizer, QPRandomBase& random);

    /// Uniform draw ranges for the signal position, µm.
    /// \param xmin lower x bound
    /// \param xmax upper x bound
    /// \param ymin lower y bound
    /// \param ymax upper y bound
    void SetPositionRange(const double& xmin, const double& xmax,
                          const double& ymin, const double& ymax);
    /// Fixed amplitude for every event; disables the amplitude draw.
    /// \param amp amplitude, electrons
    void SetAmplitude(const double& amp);                        // fixed amplitude
    /// Per-event uniform amplitude draw.
    /// \param min lower bound, electrons
    /// \param max upper bound, electrons
    void SetAmplitudeRange(const double& min, const double& max);// uniform per event
    /// Width of the injected Gaussian.
    /// \param sigma value placed on both diagonal entries of the
    ///              covariance-form matrix \f$ S = \mathrm{diag}(\sigma, \sigma) \f$
    void SetSigma(const double& sigma);
    /// Pixel firing threshold.
    /// \param threshold threshold, electrons
    void SetThreshold(const double& threshold);

    /// Generate one event.
    /// Draws x, then y, then the amplitude (only when SetAmplitudeRange is
    /// active); injects one QPSignalGaussian with
    /// \f$ S = \mathrm{diag}(\sigma, \sigma) \f$, counts the fired pixels at
    /// the threshold, then clears (and deletes) the injected signal.
    /// \warning Regression baselines are bit-identical only while this draw
    ///          order is preserved.
    /// \return the event record
    Record Shoot();                      // one event (see draw order above)
    /// Shoot() \p ntrial times.
    /// \param ntrial number of events
    /// \return accumulated records
    std::vector<Record> Run(int ntrial); // Shoot() ntrial times, accumulated

    /// Write records as CSV: header line `amp,x,y,sig,npix`, values with
    /// `%.17g` (round-trip double precision).
    /// \param records  events to write
    /// \param filename output path
    /// \return false when the file cannot be opened, true otherwise
    static bool WriteCSV(const std::vector<Record>& records,
                         const std::string& filename);

    /// \return the target digitizer; not owned
    QPDigitizerBase* GetDigitizer() const {return fDigitizer;}

    protected:
    private:
    QPDigitizerBase* fDigitizer = nullptr;
    QPRandomBase&    fRandom;

    double fXmin = 0.;
    double fXmax = 0.;
    double fYmin = 0.;
    double fYmax = 0.;
    double fAmp  = 0.;
    double fAmpMin = 0.;
    double fAmpMax = 0.;
    bool   fRandomAmp = false;
    double fSigma = 1.;
    double fThreshold = 0.;
};

#endif
