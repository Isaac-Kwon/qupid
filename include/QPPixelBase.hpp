#ifndef __QPPIXELBASE__
#define __QPPIXELBASE__ 1

class QPSignalBase;
class QPDigitizerBase;

/// Abstract pixel bound to a digitizer: pulls the digitizer's injected
/// signals and integrates them over its own cell. Signal values are in
/// electrons.
class QPPixelBase{
    public:
    /// Trivial; the digitizer back-pointer is not owned.
    virtual ~QPPixelBase();
    /// Cleanup hook; currently a no-op.
    virtual void Delete();
    /// Total collected charge of the pixel: GetDigitalSignalOne summed over
    /// every signal injected into the digitizer.
    /// \return integrated charge, electrons
    double GetDigitalSignal();
    /// \return non-owning back-pointer to the digitizer this pixel belongs to
    QPDigitizerBase* GetDigitizer() const {return fDigitizer;}

    /// Charge collected from a single signal. The base implementation
    /// prints a warning and returns 0 (stand-in for a pure virtual);
    /// concrete pixels integrate the signal over their own cell.
    /// \param signal signal to integrate; not owned
    /// \return integrated charge, electrons
    virtual double GetDigitalSignalOne(QPSignalBase* signal);

    protected:
    /// \param digitizer digitizer this pixel belongs to; not owned
    QPPixelBase(QPDigitizerBase* digitizer);
    private:
    QPDigitizerBase* fDigitizer=nullptr;
};

#endif