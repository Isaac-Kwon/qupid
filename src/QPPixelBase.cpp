#include "QPPixelBase.hpp"
#include "QPDigitizerBase.hpp"
#include "QPSignalBase.hpp"

#include <iostream>

QPPixelBase::QPPixelBase(QPDigitizerBase* digitizer): fDigitizer(digitizer){
    ;
}

void QPPixelBase::Delete(){
    ;
}

QPPixelBase::~QPPixelBase(){;}

double QPPixelBase::GetDigitalSignal(){
    int n = GetDigitizer()->GetNSignal();
    double value=0;

    for(int i=0; i<n; i++){
        QPSignalBase * signal = GetDigitizer()->GetSignal(i);
        value += GetDigitalSignalOne(signal);
    }
    return value;
}

double QPPixelBase::GetDigitalSignalOne(QPSignalBase* signal){
    std::cout<<"QPPixelBase::GetDigitalSignal - Calling Pure Function"<<std::endl;
    return 0;
}
