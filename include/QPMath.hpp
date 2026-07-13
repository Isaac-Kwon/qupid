#ifndef __QPMATH__
#define __QPMATH__ 1

#include "QPVector.hpp"
#include "QPMatrix.hpp"

/// Mathematical functions for signal modelling: Gaussian and Woods-Saxon
/// profiles, ellipse geometry, and thin wrappers around the \<cmath\>
/// functions. Angles are in radians throughout.
namespace QPMath{

    const double InvPi        = 0.3183098861837906715377675267450287240689192; ///< \f$ 1/\pi \f$
    const double InvSqrtPi    = 0.5641895835477562869480794515607725858440506; ///< \f$ 1/\sqrt{\pi} \f$
    const double InvSqrt2Pi   = 0.3989422804014326779399460599343818684758586; ///< \f$ 1/\sqrt{2\pi} \f$
    const double Degree       = 0.01745329251994329576923690768488612713442871; ///< one degree in radians, \f$ \pi/180 \f$
    const double Pi           = 3.1415926535897932384626433832795028841971693; ///< \f$ \pi \f$

    /// Normalized Gaussian profiles. `integral` is the total integral of the
    /// distribution — the deposited charge in electrons when the profile is
    /// used as a signal.
    namespace Gaussian{
        /// 1D Gaussian
        /// \f$ \dfrac{I}{\sqrt{2\pi}\,\sigma}\, e^{-(x-\mu)^2/(2\sigma^2)} \f$.
        /// \param x        evaluation point
        /// \param integral total integral \f$ I \f$
        /// \param mean     mean \f$ \mu \f$
        /// \param sigma    width \f$ \sigma \f$
        /// \return density at \p x; 0 when \f$ \sigma \le 0 \f$
        double Simple1D(double x,
                          double integral        =1.,
                          double mean            =0.,
                          double sigma           =1.);
        /// Isotropic 2D Gaussian
        /// \f$ \dfrac{I}{2\pi\sigma^2}
        ///     \exp\!\left(-\dfrac{(x-\mu_x)^2+(y-\mu_y)^2}{2\sigma^2}\right) \f$.
        /// \param x        evaluation point x
        /// \param y        evaluation point y
        /// \param integral total integral \f$ I \f$
        /// \param meanx    mean \f$ \mu_x \f$
        /// \param meany    mean \f$ \mu_y \f$
        /// \param sigma    common width \f$ \sigma \f$
        /// \return density at (x, y); 0 when \f$ \sigma \le 0 \f$
        double Simple2D(double x,
                          double y,
                          double integral      =1.,
                          double meanx         =0,
                          double meany         =0,
                          double sigma         =1.);
        /// Axis-aligned elliptic 2D Gaussian
        /// \f$ \dfrac{I}{2\pi\sigma_x\sigma_y}
        ///     \exp\!\left(-\dfrac12\left[\dfrac{(x-\mu_x)^2}{\sigma_x^2}
        ///     +\dfrac{(y-\mu_y)^2}{\sigma_y^2}\right]\right) \f$.
        /// \param x        evaluation point x
        /// \param y        evaluation point y
        /// \param integral total integral \f$ I \f$
        /// \param meanx    mean \f$ \mu_x \f$
        /// \param meany    mean \f$ \mu_y \f$
        /// \param sigmax   width \f$ \sigma_x \f$
        /// \param sigmay   width \f$ \sigma_y \f$
        /// \return density at (x, y); 0 when either width \f$ \le 0 \f$
        double UniAxis2D(double x,
                           double y,
                           double integral     =1.,
                           double meanx        =0.,
                           double meany        =0.,
                           double sigmax       =1.,
                           double sigmay       =1.);
        /// Elliptic 2D Gaussian with principal axes rotated by \p rotation:
        /// the offsets are first turned into the ellipse frame,
        /// \f$ x' = (x-\mu_x)\cos\theta + (y-\mu_y)\sin\theta,\quad
        ///     y' = (y-\mu_y)\cos\theta - (x-\mu_x)\sin\theta \f$,
        /// then evaluated as in UniAxis2D.
        /// \param x        evaluation point x
        /// \param y        evaluation point y
        /// \param integral total integral \f$ I \f$
        /// \param meanx    mean \f$ \mu_x \f$
        /// \param meany    mean \f$ \mu_y \f$
        /// \param sigmax   width \f$ \sigma_x \f$ along the rotated x axis
        /// \param sigmay   width \f$ \sigma_y \f$ along the rotated y axis
        /// \param rotation axis rotation \f$ \theta \f$, radians
        /// \return density at (x, y); 0 when either width \f$ \le 0 \f$
        double RotAxis2D(double x,
                           double y,
                           double integral     = 1.,
                           double meanx        = 0.,
                           double meany        = 0.,
                           double sigmax       = 1.,
                           double sigmay       = 1.,
                           double rotation     = 0.);
        /// Gaussian of an inclined track with depth-dependent widening.
        /// With \f$ x_p = x-\mu_x,\ y_p = y-\mu_y \f$ the implementation
        /// evaluates
        /// \f$ x_3 = x_p\cos t,\quad y_3 = y_p,\quad z_3 = -x_p\sin t - L \f$
        /// and widens both sigmas linearly with depth,
        /// \f$ \sigma'_{x,y} = \sigma_{x,y} - D\,z_3 \f$,
        /// then returns RotAxis2D\f$(x_3, y_3, I, 0, 0,
        /// \sigma'_x, \sigma'_y, \theta)\f$.
        /// \param x          evaluation point x
        /// \param y          evaluation point y
        /// \param integral   total integral \f$ I \f$
        /// \param meanx      mean \f$ \mu_x \f$
        /// \param meany      mean \f$ \mu_y \f$
        /// \param sigmax     base width \f$ \sigma_x \f$
        /// \param sigmay     base width \f$ \sigma_y \f$
        /// \param rotation   axis rotation \f$ \theta \f$, radians
        /// \param length     depth offset \f$ L \f$
        /// \param tilt       track tilt \f$ t \f$, radians
        /// \param dispersion widening rate \f$ D \f$ per unit depth
        /// \return density at (x, y); 0 when either effective width \f$ \le 0 \f$
        double Assymetry2D(double x,
                             double y,
                             double integral   = 1.,
                             double meanx      = 0.,
                             double meany      = 0.,
                             double sigmax     = 1.,
                             double sigmay     = 1.,
                             double rotation   = 0.,
                             double length     = 0.,
                             double tilt       = 0.,
                             double dispersion = 1.);
        /// 2D Gaussian in covariance-matrix form,
        /// \f[ p(\vec{x}) = \frac{1}{2\pi\sqrt{\det S}}
        ///     \exp\!\left(-\tfrac12(\vec{x}-\vec{x}_0)^{T} S^{-1}
        ///     (\vec{x}-\vec{x}_0)\right) \f]
        /// evaluated with the hand-rolled QPMatrix inverse and determinant.
        /// \param x  evaluation point (first two components used)
        /// \param x0 mean \f$ \vec{x}_0 \f$
        /// \param S  2×2 matrix \f$ S \f$
        /// \return density at \p x
        double MatrixForm2D(QPVector& x, QPVector& x0, QPMatrix& S);
    };

    /// Woods-Saxon (Fermi-function) profiles.
    namespace WoodSaxon{
        /// 1D Woods-Saxon profile
        /// \f$ \dfrac{M}{1+e^{(|x|-R)/a}} \f$.
        /// \warning \p mean has no effect: the implementation overwrites the
        ///          mean-shifted coordinate with \f$ |x| \f$.
        /// \param x       evaluation point
        /// \param maximum plateau value \f$ M \f$
        /// \param mean    accepted but ignored (see warning)
        /// \param a       edge diffuseness \f$ a \f$
        /// \param radius  half-width \f$ R \f$
        /// \return profile value at \p x
        double Simple1D(double x,
                          double maximum = 1.0,
                          double mean    = 0.0,
                          double a       = 1.0,
                          double radius  = 1.0);
        /// Radial 2D Woods-Saxon profile
        /// \f$ \dfrac{M}{1+e^{(r-R)/a}} \f$ with
        /// \f$ r = \sqrt{(x-\mu_x)^2+(y-\mu_y)^2} \f$.
        /// \param x       evaluation point x
        /// \param y       evaluation point y
        /// \param maximum plateau value \f$ M \f$
        /// \param meanx   center \f$ \mu_x \f$
        /// \param meany   center \f$ \mu_y \f$
        /// \param a       edge diffuseness \f$ a \f$
        /// \param radius  half-width \f$ R \f$
        /// \return profile value at (x, y)
        double Simple2D(double x,
                          double y,
                          double maximum = 1.0,
                          double meanx   = 0.0,
                          double meany   = 0.0,
                          double a       = 1.0,
                          double radius  = 1.0);
        /// Elliptic 2D Woods-Saxon profile with axis-aligned axes:
        /// \f$ M/(1+e^{(r-R(\phi))/a(\phi)}) \f$ where \f$ r \f$ is the
        /// distance to the center and both the diffuseness and the radius
        /// are evaluated in the offset direction \f$ \phi \f$ via
        /// GeometryCalc::EllipseUniAxis, e.g.
        /// \f$ R(\phi) = \sqrt{R_x^2\cos^2\phi + R_y^2\sin^2\phi} \f$.
        /// \param x       evaluation point x
        /// \param y       evaluation point y
        /// \param maximum plateau value \f$ M \f$
        /// \param meanx   center \f$ \mu_x \f$
        /// \param meany   center \f$ \mu_y \f$
        /// \param ax      diffuseness semi-axis along x
        /// \param ay      diffuseness semi-axis along y
        /// \param radiusX radius semi-axis \f$ R_x \f$
        /// \param radiusY radius semi-axis \f$ R_y \f$
        /// \return profile value at (x, y)
        double UniAxis2D(double x,
                           double y,
                           double maximum = 1.0,
                           double meanx   = 0.0,
                           double meany   = 0.0,
                           double ax      = 1.0,
                           double ay      = 1.0,
                           double radiusX = 1.0,
                           double radiusY = 1.0); //Non-Isometric but same with axis XY
        /// Elliptic 2D Woods-Saxon profile with both ellipses rotated by
        /// \p rotation (via GeometryCalc::EllipseRotAxis); otherwise as
        /// UniAxis2D.
        /// \param x        evaluation point x
        /// \param y        evaluation point y
        /// \param maximum  plateau value \f$ M \f$
        /// \param meanx    center \f$ \mu_x \f$
        /// \param meany    center \f$ \mu_y \f$
        /// \param ax       diffuseness semi-axis along the rotated x axis
        /// \param ay       diffuseness semi-axis along the rotated y axis
        /// \param radiusX  radius semi-axis \f$ R_x \f$
        /// \param radiusY  radius semi-axis \f$ R_y \f$
        /// \param rotation ellipse rotation, radians
        /// \return profile value at (x, y)
        double RotAxis2D(double x,
                           double y,
                           double maximum  = 1.0,
                           double meanx    = 0.0,
                           double meany    = 0.0,
                           double ax       = 1.0,
                           double ay       = 1.0,
                           double radiusX  = 1.0,
                           double radiusY  = 1.0,
                           double rotation = 0.0); //Non-Isometric but same with axis XY
        /// As RotAxis2D, but the diffuseness ellipse and the radius ellipse
        /// rotate independently.
        /// \param x         evaluation point x
        /// \param y         evaluation point y
        /// \param maximum   plateau value \f$ M \f$
        /// \param meanx     center \f$ \mu_x \f$
        /// \param meany     center \f$ \mu_y \f$
        /// \param ax        diffuseness semi-axis along its rotated x axis
        /// \param ay        diffuseness semi-axis along its rotated y axis
        /// \param radiusX   radius semi-axis \f$ R_x \f$
        /// \param radiusY   radius semi-axis \f$ R_y \f$
        /// \param rotationA rotation of the diffuseness ellipse, radians
        /// \param rotationR rotation of the radius ellipse, radians
        /// \return profile value at (x, y)
        double RotAxis2DUnsync(double x,
                                 double y,
                                 double maximum  = 1.0,
                                 double meanx    = 0.0,
                                 double meany    = 0.0,
                                 double ax       = 1.0,
                                 double ay       = 1.0,
                                 double radiusX  = 1.0,
                                 double radiusY  = 1.0,
                                 double rotationA = 0.0,
                                 double rotationR = 0.0); //Non-Isometric but same with axis XY
    };

    /// Ellipse radii and plane angles used by the Woods-Saxon profiles.
    namespace GeometryCalc{
        /// EllipseRotAxis with \f$ \theta = 0 \f$: radius of the
        /// axis-aligned ellipse with semi-axes \f$ l_x, l_y \f$ in the
        /// direction of (x, y).
        /// \param x  direction component x
        /// \param y  direction component y
        /// \param lx semi-axis along x
        /// \param ly semi-axis along y
        /// \return \f$ \sqrt{l_x^2\cos^2\phi + l_y^2\sin^2\phi} \f$ with
        ///         \f$ \phi = \mathrm{Angle}(x,y) \f$
        double EllipseUniAxis(double x, double y, double lx, double ly);
        /// Radius of the ellipse with semi-axes \f$ l_x, l_y \f$ rotated by
        /// \f$ \theta \f$, evaluated in the direction of (x, y):
        /// \f$ \sqrt{l_x^2\cos^2\phi + l_y^2\sin^2\phi} \f$ with
        /// \f$ \phi = \mathrm{Angle}(x,y) - \theta \f$.
        /// \param x     direction component x
        /// \param y     direction component y
        /// \param lx    semi-axis along the rotated x axis
        /// \param ly    semi-axis along the rotated y axis
        /// \param theta ellipse rotation, radians
        /// \return ellipse radius in that direction
        double EllipseRotAxis(double x, double y, double lx, double ly, double theta);
        /// Quadrant-aware angle of the point (x, y) about (centx, centy).
        /// \warning Through ATan2's swapped forwarding this evaluates
        ///          std::atan2(x-centx, y-centy): the angle is measured from
        ///          the +y axis, not from the +x axis as in the usual polar
        ///          convention.
        /// \param x     point x
        /// \param y     point y
        /// \param centx center x
        /// \param centy center y
        /// \return angle in radians, measured from the +y axis
        double Angle(double x, double y, double centx=0., double centy=0.);
    };

    /// std::sqrt wrapper.
    /// \param x operand
    /// \return \f$ \sqrt{x} \f$
    double Sqrt(const double& x);
    /// std::pow wrapper.
    /// \param x        base
    /// \param exponent exponent
    /// \return \f$ x^{\mathrm{exponent}} \f$
    double Power(const double& x, const double& exponent);

    /// std::sin wrapper.
    /// \param x angle, radians
    /// \return \f$ \sin x \f$
    double Sin(const double& x);
    /// std::cos wrapper.
    /// \param x angle, radians
    /// \return \f$ \cos x \f$
    double Cos(const double& x);
    /// std::atan wrapper.
    /// \param x operand
    /// \return \f$ \arctan x \in (-\pi/2, \pi/2) \f$, radians
    double ATan(const double& x);
    /// Quadrant-aware arctangent with swapped argument forwarding.
    /// \warning Forwards its parameters to std::atan2 in reversed order:
    ///          ATan2(y, x) returns std::atan2(x, y), i.e. the
    ///          quadrant-aware \f$ \arctan(x/y) \f$ — the angle measured
    ///          from the +y axis — instead of the conventional
    ///          \f$ \arctan(y/x) \f$. GeometryCalc::Angle depends on this
    ///          behavior.
    /// \param y first argument (passed to std::atan2 as its second argument)
    /// \param x second argument (passed to std::atan2 as its first argument)
    /// \return angle in radians
    double ATan2(const double& y, const double& x);
    /// std::exp wrapper.
    /// \param x operand
    /// \return \f$ e^{x} \f$
    double Exp(const double& x);
    /// Gaussian kernel \f$ e^{-(x-\mu)^2/(2\sigma^2)} \f$, divided by
    /// \f$ \sqrt{2\pi}\,\sigma \f$ when \p norm is true.
    /// Returns \f$ 10^{30} \f$ when \f$ \sigma = 0 \f$ and 0 when
    /// \f$ |x-\mu|/\sigma > 39 \f$ (underflows double precision).
    /// \param x     evaluation point
    /// \param mean  mean \f$ \mu \f$
    /// \param sigma width \f$ \sigma \f$
    /// \param norm  normalize to unit integral
    /// \return kernel value
    double Gaus(const double& x, const double& mean, const double& sigma, const bool& norm);

    /// Counterclockwise 2D rotation matrix
    /// \f[ R(\theta) = \begin{pmatrix} \cos\theta & -\sin\theta \\ \sin\theta & \cos\theta \end{pmatrix} \f]
    /// \param theta rotation angle, radians
    /// \return the 2×2 matrix, by value
    QPMatrix RotationMatrix(double theta);

};

#endif