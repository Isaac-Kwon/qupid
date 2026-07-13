#include "QPGraphFactory.hpp"
#include "QPDigitizerBase.hpp"
#include "QPPixelBase.hpp"
#include "TH2D.h"
#include "TText.h"
#include "TLine.h"

QPGraphFactory::QPGraphFactory(QPDigitizerBase* digitizer, std::string name="fac1"): fDigitizer(digitizer), fName(name){;}



TH2D* QPGraphFactory::AnalogSignal(int ndiv){
    double sizeX = GetDigitizer()->GetSizeY();
    double sizeY = GetDigitizer()->GetSizeY();
    TH2D * h = new TH2D(TString::Format("hfact_%s_an", fName.c_str()), "AnalogSignal",
                        ndiv, -sizeX/2, sizeX/2, ndiv, -sizeY/2, sizeY/2);
    for(int i=1; i<=ndiv; i++){
        for(int j=1; j<=ndiv; j++){
            double x = h->GetXaxis()->GetBinCenter(i);
            double y = h->GetYaxis()->GetBinCenter(j);

            double val = GetDigitizer()->GetAnalogSignal(x,y);

            h->SetBinContent(i,j,val);
        }
    }
    h->GetZaxis()->SetRangeUser(0.1, h->GetBinContent(h->GetMaximumBin()));
    return h;
}

TH2D* QPGraphFactory::DigitalSignal(){
    double sizeX = GetDigitizer()->GetSizeY();
    double sizeY = GetDigitizer()->GetSizeY();
    int    ndivX = GetDigitizer()->GetNPixelX();
    int    ndivY = GetDigitizer()->GetNPixelY();
    TH2D * h = new TH2D(TString::Format("hfact_%s_dl", fName.c_str()), "DigitalSignal",
                        ndivX, -sizeX/2, sizeX/2, ndivY, -sizeY/2, sizeY/2);
    for(int i=0; i<ndivX; i++){
        for(int j=0; j<ndivY; j++){
            double val = GetDigitizer()->GetPixel(i,j)->GetDigitalSignal();
            h->SetBinContent(i+1,j+1,val);
        }
    }
    h->GetZaxis()->SetRangeUser(0.1, h->GetBinContent(h->GetMaximumBin()));
    return h;
}

TH2D* QPGraphFactory::DigitizedSignal(double threshold){
    double sizeX = GetDigitizer()->GetSizeY();
    double sizeY = GetDigitizer()->GetSizeY();
    int    ndivX = GetDigitizer()->GetNPixelX();
    int    ndivY = GetDigitizer()->GetNPixelY();
    TH2D * h = new TH2D(TString::Format("hfact_%s_dd", fName.c_str()), "DigitizedSignal",
                            ndivX, -sizeX/2, sizeX/2, ndivY, -sizeY/2, sizeY/2);
    for(int i=0; i<ndivX; i++){
        for(int j=0; j<ndivY; j++){
            double val = GetDigitizer()->GetPixel(i,j)->GetDigitalSignal();
            if(val > threshold){
                h->SetBinContent(i+1,j+1,1);
            }else{
                h->SetBinContent(i+1,j+1,0);
            }
        }
    }
    h->GetZaxis()->SetRangeUser(0, 1);
    return h;
}

void QPGraphFactory::DrawDigitalSignalText(TH2D* h){
    int binx = GetDigitizer()->GetNPixelX();
    int biny = GetDigitizer()->GetNPixelX();
    double pitx = GetDigitizer()->GetSizeX() / (binx);
    double pity = GetDigitizer()->GetSizeY() / (biny);
    double offsetx = -(GetDigitizer()->GetSizeX())*0.5 + pitx*0.5;
    double offsety = -(GetDigitizer()->GetSizeY())*0.5 + pity*0.5;

    for(int i=0; i<binx; i++){
        for(int j=0; j<biny; j++){
            
            double xc;
            double yc;

            if(h==nullptr){
                xc = pitx*i + offsetx;
                yc = pity*j + offsety;
            }else{
                xc = h->GetXaxis()->GetBinCenter(i+1);
                yc = h->GetYaxis()->GetBinCenter(j+1);
            }

            double val = GetDigitizer()->GetPixel(i,j)->GetDigitalSignal();
            
            // h1->SetBinContent(i,j,val);

            TText* t = new TText(xc, yc, TString::Format("%.0f", val));
            t->SetTextAlign(22);
            t->SetTextColor(kWhite);
            t->SetTextFont(43);
            t->SetTextSize(10);
            t->SetTextAngle(0);
            t->Draw();
        }
    }

}

void QPGraphFactory::DrawPixelLinse(TH2D* h){
    // Not yet implemented.
}
