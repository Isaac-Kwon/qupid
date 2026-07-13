/** \page tut_cluster_size Tutorial 2 — Fired pixels and cluster size
 *
 * A digitizer holds the pixel matrix and integrates the analog charge over each
 * pixel cell to get the charge that pixel collects. Pixels whose collected
 * charge exceeds the firing threshold (250 electrons for ALPIDE) are the fired
 * pixels, and their count is the cluster size. This program injects one
 * Gaussian charge cloud onto a square matrix with the ALPIDE pitch
 * (29.24 × 26.88 µm) and lists the fired pixels with their collected charge.
 *
 * The matrix is centered on the origin and has an odd pixel count, so one pixel
 * sits directly under the impact point; the fired region is symmetric around
 * it. Previous: \ref tut_gaussian_signal. Next: \ref tut_montecarlo_csv.
 */

#include "QPDigitizerSquare.hpp"
#include "QPSignalGaussian.hpp"
#include "QPPixel.hpp"
#include "QPVector.hpp"
#include "QPMatrix.hpp"

#include <deque>
#include <iostream>

int main(){
    // ALPIDE pixel pitch (µm) and an odd-sized matrix centered on the origin.
    double pitchx = 29.24, pitchy = 26.88;
    int    npixx  = 11,    npixy  = 11;
    QPDigitizerSquare dig(pitchx * npixx, pitchy * npixy, npixx, npixy);

    // Charge cloud: 12000 electrons, sigma = 8 µm, at the matrix center.
    double sigma_um = 8.0, var = sigma_um * sigma_um;
    QPVector mu(0., 0.);
    QPMatrix S(2, 2, 4, var, 0., 0., var);
    dig.InjectSignal(new QPSignalGaussian(mu, 12000., S));

    // Firing threshold, electrons. The fired pixels are the cluster.
    double threshold = 250.;
    std::deque<QPPixelBase*> fired = dig.GetFiredPixel(threshold);

    std::cout << "pixels above " << threshold << " e-:\n";
    for(QPPixelBase* p : fired){
        QPPixel* pix = (QPPixel*) p;
        std::cout << "  pixel [" << pix->GetPixX() << "," << pix->GetPixY()
                  << "]  charge = " << pix->GetDigitalSignal() << " e-\n";
    }
    std::cout << "cluster size = " << fired.size() << " pixels\n";

    return 0;
}
