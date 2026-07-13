/** \page tut_gaussian_signal Tutorial 1 — Gaussian charge-cloud signal
 *
 * qupid represents the charge a traversing particle leaves in the sensor as an
 * analytic distribution: a total collected charge (the amplitude, in electrons)
 * multiplied by a normalized 2D Gaussian probability density. QPSignalGaussian
 * is built from the impact point \f$\mu\f$ (µm) and the covariance matrix
 * \f$\Sigma\f$ (µm²); the density is
 * \f$ \frac{1}{2\pi\sqrt{|\Sigma|}}\,
 *     \exp\!\big[-\tfrac12 (x-\mu)^T \Sigma^{-1} (x-\mu)\big]. \f$
 *
 * Here the cloud is isometric: \f$\Sigma = \mathrm{diag}(\sigma^2,\sigma^2)\f$,
 * so a diagonal entry is the variance and \f$\sigma\f$ is the lateral spread in
 * µm. Unequal diagonal entries or off-diagonal terms would make the cloud
 * anisotropic or rotated. This program reads the charge density at a couple of
 * points and integrates it over pixel-sized rectangles; summed over a 3×3 block
 * of ALPIDE pixels the integral recovers essentially the whole amplitude.
 *
 * Next: \ref tut_cluster_size.
 */

#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPSignalGaussian.hpp"

#include <iostream>

int main(){
    // Impact point of the particle, µm.
    QPVector mu(0., 0.);

    // Lateral spread of the charge cloud. Sigma is the covariance matrix in
    // µm²; sigma = 8 µm gives the variance 64 µm² on each diagonal.
    double sigma_um = 8.0;
    double var      = sigma_um * sigma_um;
    QPMatrix S(2, 2, 4, var, 0., 0., var);

    // Total collected charge, electrons.
    double amplitude = 12000.;

    QPSignalGaussian signal(mu, amplitude, S);

    // GetPDF is the probability density (µm^-2); GetSignal is amplitude x PDF,
    // i.e. the charge density in electrons/µm^2.
    std::cout << "PDF at (0,0)          = " << signal.GetPDF(0., 0.)    << " um^-2\n";
    std::cout << "charge density (0,0)  = " << signal.GetSignal(0., 0.) << " e-/um^2\n";
    std::cout << "charge density (8,0)  = " << signal.GetSignal(sigma_um, 0.)
              << " e-/um^2 (one sigma out)\n";

    // Electrons collected in the ALPIDE pixel (29.24 x 26.88 µm) centered on
    // the impact point, and in its right-hand neighbour.
    double px = 29.24, py = 26.88;
    std::cout << "charge in central pixel  = "
              << signal.Integral(-px/2, px/2, -py/2, py/2) << " e-\n";
    std::cout << "charge in right neighbour = "
              << signal.Integral(px/2, 3*px/2, -py/2, py/2) << " e-\n";

    // Summing the per-cell charge over the 3x3 pixel block around the impact
    // point accounts for almost all of the amplitude.
    double total = 0.;
    for(int ix = -1; ix <= 1; ix++){
        for(int iy = -1; iy <= 1; iy++){
            total += signal.Integral((ix - 0.5) * px, (ix + 0.5) * px,
                                     (iy - 0.5) * py, (iy + 0.5) * py);
        }
    }
    std::cout << "charge over 3x3 pixels   = " << total
              << " e- (amplitude = " << amplitude << ")\n";

    return 0;
}
