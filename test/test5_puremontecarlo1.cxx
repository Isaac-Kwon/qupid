#include "iostream"
#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPDigitizerSquare.hpp"
#include "QPSignalGaussian.hpp"
#include "QPMonteCarlo.hpp"
#include "QPRandomTRandom.hpp"

#include "TRandom3.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"

struct Pos{
    double x;
    double y;
    Pos(const double& x_, const double& y_): x(x_), y(y_){;}
    Pos(Pos& other):x(other.x), y(other.y){;}
};

//FIXED ENERGY, VARIOUS POSITION

void test5_puremontecarlo1(){

    int nTrial = 10000;

    //Define Digitizer
    double pitchx = 29.24;
    double pitchy = 26.88;
    int    npixx  = 10;
    int    npixy  = 10;
    double threshold = 250;
    
    QPDigitizerSquare * dig = new QPDigitizerSquare(pitchx * npixx, pitchy * npixy, npixx, npixy);

    // Define Independent Data Store
    double sigma = 0.83*900; //fixed
    double amp   = 537280;   //fixed
    Pos    pos   = {0., 0.};

    // Define Dependent Data Store
    int npix = 0;

    // Define TTree Structure
    TTree * tree = new TTree("tree", "qupid, (test5) pure monte-carlo test 1");
    tree->Branch("amp", &amp, "amp/D");
    tree->Branch("pos", &pos, "x/D:y/D");
    tree->Branch("sig", &sigma, "sig/D");
    tree->Branch("npix", &npix, "npix/I");

    tree->SetAlias("eV", "amp*3.6");
    tree->SetAlias("keV", "amp*3.6/1000");
    tree->SetAlias("MeV", "amp*3.6/1000000");

    // Define Random Generator (TRandom3 through the core RNG interface)
    TRandom3* rand = new TRandom3();
    QPRandomTRandom qprandom(rand);

    // Variable
    // * Signal position X and Y
    //   - X : posx
    //   - Y : posy
    //
    // Fixed
    // * Amplitude
    // * Sigma

    // Define Monte Carlo driver (draw order per event: x, y; amp is fixed)
    QPMonteCarlo mc(dig, qprandom);
    mc.SetPositionRange(-pitchx*0.5, pitchx*0.5, -pitchy*0.5, pitchy*0.5);
    mc.SetAmplitude(amp);
    mc.SetSigma(sigma);
    mc.SetThreshold(threshold);

    int pin = nTrial/100;

    for(int i=0; i<nTrial; i++){
        QPMonteCarlo::Record rec = mc.Shoot();
        pos.x = rec.fX;
        pos.y = rec.fY;
        npix  = rec.fNpix;

        tree->Fill();

        if( i%pin == 0){
            std::cout<<"i = "<<i<<std::endl;
        }
    }

    TH1D* h1 = new TH1D("h1", "h1", 49, 1, 50);
    tree->Draw("npix>>h1", "", "goff");

    h1->GetXaxis()->SetRangeUser(33, 42);

    h1->Draw();
    
}

int main(){
    test5_puremontecarlo1();
    return 0;
}