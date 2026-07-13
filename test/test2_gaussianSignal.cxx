
#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPSignalGaussian.hpp"

#include "iostream"

#include "TH2D.h"
#include "TText.h"
#include "TCanvas.h"

void test2_gaussianSignal(){

    // QPVector mu = QPVector(15,15);
    QPVector mu = QPVector(0,0);
    double sigma = 0.83*900;
    QPMatrix S = QPMatrix(2,2, 4, sigma, 0., 0., sigma);

    QPSignalGaussian signal = QPSignalGaussian(mu, 537280, S);

    TCanvas* c1 = new TCanvas("c1", "c1", 1200, 1000);
    c1->Draw();
    TH2D* h1 = new TH2D("h1", "h1", 20, -292.4, 292.4, 20, -268.8, 268.8);
    h1->Draw("COLZ");

    for(int i=1; i<=h1->GetXaxis()->GetNbins(); i++){
        for(int j=1; j<=h1->GetYaxis()->GetNbins(); j++){
            double x0 = h1->GetXaxis()->GetBinLowEdge(i);
            double x1 = h1->GetXaxis()->GetBinUpEdge(i);
            double y0 = h1->GetYaxis()->GetBinLowEdge(j);
            double y1 = h1->GetYaxis()->GetBinUpEdge(j);

            double xc = h1->GetXaxis()->GetBinCenter(i);
            double yc = h1->GetYaxis()->GetBinCenter(j);

            double val = signal.Integral(x0,x1,y0,y1);
            
            h1->SetBinContent(i,j,val);

            TText* t = new TText(xc, yc, TString::Format("%.0f", val));
            t->SetTextAlign(22);
            t->SetTextColor(kWhite);
            t->SetTextFont(43);
            t->SetTextSize(10);
            t->SetTextAngle(0);
            t->Draw();
        }
    }
    h1->GetZaxis()->SetRangeUser(0.1, h1->GetBinContent(h1->GetMaximumBin()));
    // h1->Update();
    c1->Update();
    std::cout<<h1->Integral()<<std::endl;
    c1->SetLogz();

}

int main(){
    return 0;
}