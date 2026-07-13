#ifndef __QPRANDOMTRANDOM__
#define __QPRANDOMTRANDOM__ 1

#include "QPRandomBase.hpp"

#include "TRandom.h"

/// Header-only adapter: implements QPRandomBase on top of a caller-owned
/// TRandom. Delegates Uniform(a,b) / Gaus(mean,sigma) one-to-one, so the
/// TRandom stream (and any baseline built on it) is preserved exactly.
/// Neither libqupid nor libqupidVis includes this header; only executables
/// that include it link ROOT.
class QPRandomTRandom: public QPRandomBase{
    public:
    /// \param random TRandom instance to delegate to; caller-owned, not
    ///               deleted by the adapter
    QPRandomTRandom(TRandom* random): fRandom(random){;}
    /// Does not delete the wrapped TRandom.
    virtual ~QPRandomTRandom(){;}

    /// Delegates to TRandom::SetSeed.
    /// \param seed engine seed value
    virtual void   SetSeed(unsigned long seed){ fRandom->SetSeed(seed); }
    /// Delegates to TRandom::Uniform(a, b).
    /// \param a lower bound
    /// \param b upper bound
    /// \return uniform deviate with TRandom's endpoint semantics
    virtual double Uniform(double a, double b){ return fRandom->Uniform(a, b); }
    /// Delegates to TRandom::Gaus(mean, sigma).
    /// \param mean  mean of the distribution
    /// \param sigma standard deviation
    /// \return normal deviate
    virtual double Gaus(double mean, double sigma){ return fRandom->Gaus(mean, sigma); }

    protected:
    private:
    TRandom* fRandom = nullptr;
};

#endif
