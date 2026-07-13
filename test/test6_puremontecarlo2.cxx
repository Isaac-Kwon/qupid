#include "iostream"
#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPDigitizerSquare.hpp"
#include "QPSignalGaussian.hpp"

#include "TRandom3.h"
#include "TTree.h"
#include "TH2D.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TPaveText.h"

struct Pos{
    double x;
    double y;
    Pos(const double& x_, const double& y_): x(x_), y(y_){;}
    Pos(Pos& other):x(other.x), y(other.y){;}
};

//VARIOUS ENERGY, VARIOUS POSITION

void test6_puremontecarlo2(){

    int nTrial = 50000;
    // int nTrial = 100;
    double maxsignal = 537280;

    //Define Digitizer
    double pitchx = 29.24;
    double pitchy = 26.88;
    int    npixx  = 10;
    int    npixy  = 10;
    double threshold = 250;
    
    QPDigitizerSquare * dig = new QPDigitizerSquare(pitchx * npixx, pitchy * npixy, npixx, npixy);

    // Define Independent Data Store
    double sigma = 0.83*900; //fixed
    double amp   = 537280;   
    Pos    pos   = {0., 0.};

    // Define Dependent Data Store
    int npix = 0;

    // Define TTree Structure
    TTree * tree = new TTree("tree", "qupid, (test6) pure monte-carlo test 1");
    tree->Branch("amp", &amp, "amp/D");
    tree->Branch("pos", &pos, "x/D:y/D");
    tree->Branch("sig", &sigma, "sig/D");
    tree->Branch("npix", &npix, "npix/I");

    tree->SetAlias("eV", "amp*3.6");
    tree->SetAlias("keV", "amp*3.6/1000");
    tree->SetAlias("MeV", "amp*3.6/1000000");

    // Define Random Generator
    TRandom3* rand = new TRandom3();

    // Variable
    // * Signal position X and Y
    //   - X : posx
    //   - Y : posy
    // * Amplitude
    //
    // Fixed
    // * Sigma
    
    
    int pin = nTrial>100 ? nTrial/100 : 1;

    for(int i=0; i<nTrial; i++){
        pos.x = rand->Uniform(-pitchx*0.5, pitchx*0.5);
        pos.y = rand->Uniform(-pitchy*0.5, pitchy*0.5);
        amp   = rand->Uniform(0., maxsignal);

        QPVector mu = QPVector(pos.x, pos.y);
        QPMatrix S  = QPMatrix(2,2,4, sigma, 0., 0., sigma);

        QPSignalGaussian  * sig = new QPSignalGaussian(mu, amp, S);
        dig->InjectSignal(sig);

        npix = dig->GetFiredPixel(threshold).size();

        tree->Fill();
        dig->ClearSignal();
        
        if( i%pin == 0){
            std::cout<<"i = "<<i<<std::endl;
        }
    }

    TH2D* h1 = new TH2D("h1", "Various amplitude and position", 100, 0, maxsignal, 49, 1, 50);
    tree->Draw("npix:amp >> h1", "", "goff");

    TCanvas * c1 = new TCanvas("c1", "Canvas 1", 1000, 800);
    c1->cd();
    h1->GetXaxis()->SetTitle("Amplitude [a.u.]");
    h1->GetYaxis()->SetTitle("# of pixel fired");
    h1->GetZaxis()->SetTitle("Entries");
    h1->Draw("COLZ");

    TPaveText * pave1 = new TPaveText(0.4, 0.10, 0.70, 0.35, "NDC NB");
    pave1->AddText("qupid Calculation");
    pave1->AddText(TString::Format("# of Signal Generated = %d", nTrial));
    pave1->AddText("Various Position");
    pave1->AddText(TString::Format("Pixel Pitch = [%.2f, %.2f]", pitchx, pitchy));
    pave1->AddText("Isometric Gaussian Signal");
    pave1->AddText(TString::Format("Amplitude = [0, %.0f]", maxsignal));
    pave1->AddText(TString::Format("Sigma = %.2f(fixed)", sigma));
    pave1->AddText(TString::Format("Firing Threshold = %.0f", threshold));

    pave1->SetTextAlign(11);
    pave1->SetTextFont(42);
    pave1->GetLine(0)->SetTextFont(72);
    pave1->SetFillColorAlpha(kWhite, 0.);
    pave1->SetLineWidth(0.);

    pave1->Draw();

    TCanvas * c2 = new TCanvas("c2", "Canvas 2", 1000, 800);
    c2->cd();
    c2->SetLogy();
    TH1D* h2 = new TH1D("h2", "Various amplitude and position", 49, 1, 50);
    tree->Draw("npix >> h2", "", "goff");
    h2->GetXaxis()->SetTitle("# of pixel fired [a.u.]");
    h2->GetYaxis()->SetTitle("Entries");
    h2->Draw();
    TPaveText * pave2 = new TPaveText(*pave1);
    pave2->SetX1NDC(0.1);
    pave2->SetY1NDC(0.7);
    pave2->SetX2NDC(0.4);
    pave2->SetY2NDC(0.9);
    pave2->Draw("NDC");

    
    c2->SaveAs("test6_c2.pdf");

    TFile * file = new TFile("./test6_output.root","RECREATE");
    file->Add(tree);
    file->Add(c1);
    file->Add(h1);
    file->Add(c2);
    file->Add(h2);
    file->Add(pave1);

    file->Write();

    
    
}

int main(){
    test6_puremontecarlo2();
    return 0;
}