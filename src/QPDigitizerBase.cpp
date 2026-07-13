#include "QPDigitizerBase.hpp"
#include "QPSignalBase.hpp"
#include "QPPixelBase.hpp"
#include <iostream>

QPDigitizerBase::QPDigitizerBase(int npix): fNpix0(npix){;}

QPDigitizerBase::~QPDigitizerBase(){;}

void QPDigitizerBase::Delete(){;}

void QPDigitizerBase::InjectSignal(QPSignalBase* signal){
    fSignals.emplace_back(signal);
}

QPPixelBase* QPDigitizerBase::GetPixel(const int index){
    std::cout<<"QPPixelBase::GetPixel(index) - Calling pure function"<<std::endl;
    return nullptr;
}
QPPixelBase* QPDigitizerBase::GetPixel(const int i, const int j){
    std::cout<<"QPPixelBase::GetPixel(i,j) - Calling pure function"<<std::endl;
    return nullptr;
}

QPSignalBase* QPDigitizerBase::GetSignal(int i) const{
    if(i < 0 || i >= GetNSignal()) return nullptr;
    return fSignals.at(i);
}

int QPDigitizerBase::GetNSignal() const{
    return fSignals.size();
}

void QPDigitizerBase::ClearSignal(bool removeall){
    if(removeall){
        for(QPSignalBase* signal: fSignals){
            delete signal;
        }
    }
    fSignals.clear();
}

std::deque<QPPixelBase*> QPDigitizerBase::GetFiredPixel(double threshold){
    // std::cout<<"GET IN FIRED"<<std::endl;
    int npix = GetNPixel();
    std::deque<QPPixelBase*> ans;
    for(int i=0; i<npix; i++){
        // std::cout<<"PIXN = "<<i<<std::endl;
        QPPixelBase* pix = GetPixel(i);
        if(pix->GetDigitalSignal() > threshold){
            ans.emplace_back(pix);
        }else{
            delete pix;
        }
    }
    return ans;
}

double QPDigitizerBase::GetAnalogSignal(const double& x, const double& y){
    int n = GetNSignal();
    double value=0;

    for(int i=0; i<n; i++){
        QPSignalBase * signal = GetSignal(i);
        value += signal->GetSignal(x,y);
    }
    return value;
}