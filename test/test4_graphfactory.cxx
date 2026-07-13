#include "deque"

#include "QPDigitizerSquare.hpp"
#include "QPSignalGaussian.hpp"
#include "QPVector.hpp"
#include "QPMatrix.hpp"
#include "QPGraphFactory.hpp"

#include "TCanvas.h"
#include "TPaveText.h"
#include "TH2D.h"

void test4_graphfactory(){
    QPVector mu = QPVector(0, 0);
    // QPVector mu = QPVector(0, 0);

    // double sigma = 0.83*900;
    double sigma = 0.83*900;
    QPMatrix S = QPMatrix(2, 2, 4, sigma, 0., 0., sigma);
    
    QPDigitizerSquare * dig = new QPDigitizerSquare(292.4*1.5, 268.8*1.5, 15, 15);
    QPSignalGaussian  * sig = new QPSignalGaussian(mu, 537280, S);
    dig->InjectSignal(sig);

    QPGraphFactory* factory = new QPGraphFactory(dig, "one");

    TCanvas * c1 = new TCanvas("c1", "c1", 2100, 1800);
    c1->Divide(2,2);
    c1->cd(1);
    c1->cd(1)->SetLogz();
    factory->AnalogSignal()->Draw("COLZ");
    c1->cd(2);
    c1->cd(2)->SetLogz();
    TH2D* h2 = factory->DigitalSignal();
    h2->Draw("COLZ");
    factory->DrawDigitalSignalText(h2);
    c1->cd(3);
    TH2D* h3 = factory->DigitizedSignal(250);
    h3->Draw("COLZ");
    factory->DrawDigitalSignalText(h3);

    // c1->cd(1)->Print("test4_p1.pdf");
    // c1->cd(2)->Print("test4_p2.pdf");
    // c1->cd(3)->Print("test4_p3.pdf");
    // c1->cd(4)->Print("test4_p4.pdf");

    c1->cd(1)->Print("test4_p1.eps");
    c1->cd(2)->Print("test4_p2.eps");
    c1->cd(3)->Print("test4_p3.eps");
    c1->cd(4)->Print("test4_p4.eps");

    std::cout<<"Total "<<dig->GetFiredPixel(250).size()<<" Pixel "<<std::endl;
    
}

int main(){
    return 0;
}