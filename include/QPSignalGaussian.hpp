#ifndef __QPSIGNALGAUSSIAN__
#define __QPSIGNALGAUSSIAN__ 1

#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPSignalBase.hpp"

/// 2D Gaussian signal in covariance-matrix form,
/// \f[ p(\vec{x}) = \frac{1}{2\pi\sqrt{\det S}}
///     \exp\!\left(-\tfrac{1}{2}(\vec{x}-\vec{x}_0)^{T} S^{-1}
///     (\vec{x}-\vec{x}_0)\right) \f]
/// with mean \f$ \vec{x}_0 \f$ (µm) and 2×2 matrix \f$ S \f$.
class QPSignalGaussian: public QPSignalBase{
    public:
    /// \param x0        signal center, µm
    /// \param amplitude total deposited charge, electrons
    /// \param S         2×2 matrix used directly in the covariance form above
    QPSignalGaussian(const QPVector& x0, const double& amplitude, const QPMatrix& S);
    /// Builds \f$ S = \mathrm{diag}(s_x, s_y)\, R(s_{rad}) \f$ with
    /// QPMath::RotationMatrix — the plain product, not the similarity
    /// transform \f$ R\,\mathrm{diag}(s_x,s_y)\,R^{T} \f$, so \f$ S \f$ is
    /// not symmetric for \f$ s_{rad} \neq 0 \f$.
    /// \param x0        signal center x, µm
    /// \param y0        signal center y, µm
    /// \param amplitude total deposited charge, electrons
    /// \param sx        diagonal entry for x
    /// \param sy        diagonal entry for y
    /// \param srad      rotation angle, radians
    QPSignalGaussian(const double& x0, const double& y0, const double& amplitude, const double& sx, const double& sy, const double& srad);

    /// Evaluates the covariance form above at (x, y).
    /// \param x evaluation point x, µm
    /// \param y evaluation point y, µm
    /// \return probability density, 1/µm²
    virtual double GetPDF(const double& x, const double& y);

    protected:
    private:
    QPVector fx0;
    QPMatrix fS;
};

#endif