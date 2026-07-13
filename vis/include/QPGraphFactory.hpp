#ifndef __QPGRAPHFACTORY__
#define __QPGRAPHFACTORY__ 1

#include <string>

class QPDigitizerBase;
class TH2D;

/// Renders a digitizer's analog/digital/thresholded views as ROOT TH2D
/// histograms (libqupidVis). Friend of the digitizer classes.
/// Every histogram is heap-allocated and owned by the caller (subject to
/// the usual ROOT directory ownership rules).
/// \warning All three histogram builders take both axis ranges from
///          GetSizeY(), and DrawDigitalSignalText takes the y pixel count
///          from GetNPixelX(): the output is laid out correctly only for
///          square matrices.
class QPGraphFactory{
    public:
    /// \param digitizer digitizer to render; not owned
    /// \param name      tag used in the histogram names
    ///                  (`hfact_<name>_an` / `_dl` / `_dd`)
    QPGraphFactory(QPDigitizerBase* digitizer, std::string name);

    /// ndiv×ndiv sampling of GetAnalogSignal over the matrix.
    /// \param ndiv bins per axis
    /// \return caller-owned histogram of the analog signal density
    TH2D * AnalogSignal(int ndiv=100);
    /// One bin per pixel, filled with the pixel's integrated digital
    /// signal (electrons).
    /// \return caller-owned histogram of the digital signal
    TH2D * DigitalSignal();
    /// Binary fired-pixel map: 1 where the digital signal exceeds
    /// \p threshold, 0 elsewhere.
    /// \param threshold firing threshold, electrons
    /// \return caller-owned histogram of the thresholded signal
    TH2D * DigitizedSignal(double threshold=0.1);

    /// Draws each pixel's digital value as a TText on the current pad.
    /// The text objects are left to the pad.
    /// \param h histogram whose bin centers position the labels; when
    ///          nullptr the positions are computed from the digitizer
    ///          geometry
    void   DrawDigitalSignalText(TH2D* h=nullptr);
    /// Not implemented; the body is empty.
    /// \param h unused
    void   DrawPixelLinse(TH2D* h=nullptr);

    /// \return the rendered digitizer; not owned
    QPDigitizerBase* GetDigitizer() const {return fDigitizer;}

    protected:

    private:
    QPDigitizerBase* fDigitizer = nullptr;
    std::string fName = "fac1";

};



#endif