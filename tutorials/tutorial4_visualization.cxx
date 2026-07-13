/** \page tut_visualization Tutorial 4 — Visualization and the TRandom adapter
 *
 * QPGraphFactory renders a digitizer's views as ROOT TH2D histograms: the
 * analog charge field sampled on a fine grid, the per-pixel collected charge,
 * and the thresholded fired/not-fired map. This program draws all three for one
 * Gaussian cloud on the ALPIDE matrix (pitch 29.24 × 26.88 µm, threshold
 * 250 electrons) and saves them to a PDF.
 *
 * It then runs the same Monte Carlo as \ref tut_montecarlo_csv, but driven by
 * ROOT's TRandom3 through the header-only QPRandomTRandom adapter: QPMonteCarlo
 * still sees only the QPRandomBase interface, and ROOT is linked by this
 * executable alone. Visualization is optional (QUPID_WITH_ROOT), so this
 * tutorial builds only when ROOT is available. Previous: \ref tut_montecarlo_csv.
 */

#include "QPDigitizerSquare.hpp"
#include "QPSignalGaussian.hpp"
#include "QPMonteCarlo.hpp"
#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPGraphFactory.hpp"
#include "QPRandomTRandom.hpp"

#include "TROOT.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TRandom3.h"

#include <iostream>
#include <vector>

int main(){
    gROOT->SetBatch(kTRUE);   // headless: render to file, open no display

    double pitchx    = 29.24, pitchy = 26.88;
    int    npixx     = 11,    npixy  = 11;
    double threshold = 250.;
    double sigma_um  = 8.0, var = sigma_um * sigma_um;
    double amplitude = 12000.;

    // One charge cloud on the ALPIDE matrix.
    QPDigitizerSquare* dig = new QPDigitizerSquare(pitchx * npixx, pitchy * npixy,
                                                   npixx, npixy);
    QPVector mu(0., 0.);
    QPMatrix S(2, 2, 4, var, 0., 0., var);
    dig->InjectSignal(new QPSignalGaussian(mu, amplitude, S));

    // Analog field, per-pixel charge, and the thresholded map, side by side.
    QPGraphFactory factory(dig, "tut4");
    TCanvas c("c", "qupid views", 1800, 600);
    c.Divide(3, 1);
    c.cd(1); c.cd(1)->SetLogz();
    factory.AnalogSignal()->Draw("COLZ");
    c.cd(2);
    TH2D* hd = factory.DigitalSignal();
    hd->Draw("COLZ");
    factory.DrawDigitalSignalText(hd);
    c.cd(3);
    TH2D* ht = factory.DigitizedSignal(threshold);
    ht->Draw("COLZ");
    factory.DrawDigitalSignalText(ht);
    c.Print("tutorial4_views.pdf");

    std::cout << "tutorial4: cluster size = "
              << dig->GetFiredPixel(threshold).size()
              << " -> tutorial4_views.pdf\n";

    // The same Monte Carlo as tutorial 3, but with TRandom3 behind the adapter.
    TRandom3 rootrng(12345);
    QPRandomTRandom qprng(&rootrng);
    QPMonteCarlo mc(dig, qprng);
    mc.SetPositionRange(-pitchx / 2, pitchx / 2, -pitchy / 2, pitchy / 2);
    mc.SetAmplitudeRange(0., amplitude);
    mc.SetSigma(var);
    mc.SetThreshold(threshold);
    std::vector<QPMonteCarlo::Record> recs = mc.Run(500);
    std::cout << "tutorial4: TRandom-driven MC produced " << recs.size()
              << " events\n";

    return 0;
}
