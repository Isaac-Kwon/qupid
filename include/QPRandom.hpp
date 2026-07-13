#ifndef __QPRANDOM__
#define __QPRANDOM__ 1

#include "QPRandomBase.hpp"

#include <random>

/// Default ROOT-free random engine: std::mt19937_64 with the standard
/// library distributions.
/// A distribution object is constructed on every call, so the value
/// sequence is fully determined by the seed and the order of calls.
class QPRandom: public QPRandomBase{
    public:
    /// \param seed engine seed (default: std::mt19937_64::default_seed)
    QPRandom(unsigned long seed = std::mt19937_64::default_seed);
    /// Trivial.
    virtual ~QPRandom(){;}

    /// Re-seed the engine, restarting its sequence.
    /// \param seed engine seed value
    virtual void   SetSeed(unsigned long seed);

    /// Uniform deviate in \f$[a,b)\f$ via std::uniform_real_distribution.
    /// \param a lower bound (attainable)
    /// \param b upper bound (not attainable)
    /// \return uniform deviate
    virtual double Uniform(double a, double b);

    /// Normal deviate via std::normal_distribution.
    /// \param mean  mean of the distribution
    /// \param sigma standard deviation
    /// \return normal deviate
    virtual double Gaus(double mean, double sigma);

    protected:
    private:
    std::mt19937_64 fEngine;
};

#endif
