#include "QPGeneral.hpp"
#include "QPSignalBase.hpp"
#include <iostream>

QPSignalBase::QPSignalBase(const double& amplitude): fAmplitude(amplitude){
    ;
}

QPSignalBase::~QPSignalBase(){
    Delete();
}

void QPSignalBase::Delete(){;}

double QPSignalBase::GetPDF(const double& x, const double& y){
    std::cout<<"QPSignalBase::GetPDF - Calling pure function, return 0"<<std::endl;
    return 0;
}

double QPSignalBase::GetSignal(const double& x, const double& y){
    return GetPDF(x,y)*fAmplitude;
}

double QPSignalBase::Integral(const double& x0, const double& x1, const double& y0, const double& y1){
    // int ndiv = qupid::NIntegrateDivide;
    int ndiv = 5;
    double ans = 0;

    double sx = x0 +  (x1-x0) / (2*ndiv);
    double sy = y0 + (y1-y0) / (2*ndiv);

    double dx = (x1-x0) / (ndiv);
    double dy = (y1-y0) / (ndiv);

    // std::cout<<sx << "\t" << sy << "\t" <<dx<< "\t" <<dy<<std::endl;
    for(int i=0; i<ndiv; i++){
        for(int j=0; j<ndiv; j++){
            double x = dx*i+sx;
            double y = dy*j+sy;
            // std::cout<<x<<"\t"<<y<<"\t"<<GetSignal(x,y)<<std::endl;
            ans += GetSignal(x,y);
        }
    }
    return ans*dx*dy;

}