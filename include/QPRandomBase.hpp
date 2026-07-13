#ifndef __QPRANDOMBASE__
#define __QPRANDOMBASE__ 1

/// Abstract random number source.
/// The only RNG type core code may reference; concrete engines (the
/// std-based QPRandom, the TRandom adapter in vis/include) are chosen and
/// owned by the caller.
class QPRandomBase{
    public:
    /// Virtual destructor for deletion through the interface.
    virtual ~QPRandomBase(){;}

    /// Seed the underlying engine.
    /// \param seed engine seed value
    virtual void   SetSeed(unsigned long seed) = 0;

    /// Uniform deviate between \p a and \p b.
    /// Whether the endpoints themselves can be returned follows the concrete
    /// engine (QPRandom: \f$[a,b)\f$; the TRandom adapter: TRandom semantics).
    /// \param a lower bound
    /// \param b upper bound
    /// \return uniform deviate
    virtual double Uniform(double a, double b) = 0;

    /// Normal (Gaussian) deviate.
    /// \param mean  mean of the distribution
    /// \param sigma standard deviation
    /// \return normal deviate
    virtual double Gaus(double mean, double sigma) = 0;

    protected:
    private:
};

#endif
