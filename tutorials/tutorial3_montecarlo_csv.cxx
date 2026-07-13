/** \page tut_montecarlo_csv Tutorial 3 — Monte Carlo cluster-size scan to CSV
 *
 * The cluster size depends on where inside a pixel the particle hits and on how
 * much charge it deposits. QPMonteCarlo scans this: per event it draws an
 * impact point (uniform over one pixel pitch) and an amplitude (uniform in a
 * range), injects one Gaussian cloud, counts the fired pixels, and clears the
 * digitizer. Each event becomes one record (amp, x, y, sigma, npix), written as
 * CSV for offline analysis.
 *
 * Random numbers come through the QPRandomBase interface; here the ROOT-free
 * QPRandom (std::mt19937_64) supplies them, so this program needs no ROOT. The
 * fixed seed makes the run reproducible. Previous: \ref tut_cluster_size.
 * Next: \ref tut_visualization.
 */

#include "QPDigitizerSquare.hpp"
#include "QPMonteCarlo.hpp"
#include "QPRandom.hpp"

#include <iostream>
#include <vector>

int main(){
    double pitchx    = 29.24, pitchy = 26.88;   // ALPIDE pitch, µm
    int    npixx     = 11,    npixy  = 11;
    double threshold = 250.;                     // electrons
    double sigma_um  = 8.0;                       // charge-cloud spread, µm
    double maxAmp    = 12000.;                     // electrons, top of the scan
    int    nTrial    = 2000;

    QPDigitizerSquare dig(pitchx * npixx, pitchy * npixy, npixx, npixy);

    QPRandom random(12345);   // fixed seed -> reproducible stream

    QPMonteCarlo mc(&dig, random);
    // Impact point uniform over one central pixel; amplitude uniform to maxAmp.
    mc.SetPositionRange(-pitchx / 2, pitchx / 2, -pitchy / 2, pitchy / 2);
    mc.SetAmplitudeRange(0., maxAmp);
    // SetSigma takes the covariance diagonal entry, i.e. the variance in µm².
    mc.SetSigma(sigma_um * sigma_um);
    mc.SetThreshold(threshold);

    std::vector<QPMonteCarlo::Record> records = mc.Run(nTrial);

    if(!QPMonteCarlo::WriteCSV(records, "tutorial3_output.csv")){
        std::cerr << "tutorial3: cannot write tutorial3_output.csv\n";
        return 1;
    }

    std::cout << "tutorial3: " << records.size()
              << " events -> tutorial3_output.csv\n";
    return 0;
}
