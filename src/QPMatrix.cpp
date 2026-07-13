#include <iostream>
#include <cstdlib>
#include <cstdarg>
#include <cmath>
#include <sstream>

#include "QPMatrix.hpp"


QPMatrix::QPMatrix(): fElement(0), fM(0), fN(0){
    // Init();
}

QPMatrix::QPMatrix(const int n): fElement(0), fM(n), fN(n){
    Init();
}

QPMatrix::QPMatrix(const int m, const int n): fElement(0), fM(m), fN(n){
    Init();
}


QPMatrix::QPMatrix(const int m, const int n, const int npassed, ...): fElement(0), fM(m), fN(n){
    Init();

    va_list vl;
    va_start( vl, npassed );

    for(int i=0; i<m*n && i<npassed; i++){
        fElement[i] = va_arg(vl, double);
    }

    va_end(vl);
}

QPMatrix::QPMatrix(QPVector& vec, int ncomp): fElement(0), fM(ncomp), fN(1){
    Init();
    for(int i=0; i<ncomp && i<3; i++){
        fElement[i] = vec(i);
    }
}

QPMatrix::QPMatrix(const QPMatrix& other): fElement(0), fM(other.fM), fN(other.fN){
    Init();
    
    int fMN = fM*fN;
    for(int i=0; i<fMN; i++){
        fElement[i] = other.fElement[i];
    }
}

QPMatrix& QPMatrix::operator=(const QPMatrix& other){
    Freeing();
    fM = other.fM;
    fN = other.fN;

    Init();
    for(int i=0; i<fM*fN; i++){
        fElement[i] = other.fElement[i];
        // std::cout<<"COPIED\t"<<fElement[i]<<"\t"<<At(i)<<std::endl;
    }
    
    return *this;
}

QPMatrix::~QPMatrix(){
    Freeing();
}

void QPMatrix::Init(){
    fElement = (double*) std::malloc(fM*fN*sizeof(double));
    for(int i=0; i<fN*fM; i++){
        fElement[i] = 0.;
    }
}

void QPMatrix::Freeing(){
    // if(fElement == nullptr){
    //     return;
    // }
    std::free(fElement);
}

bool     QPMatrix::IsInside(int i){
    if(i >= 0 && i< fN*fM) return true;
    return false;
}

bool     QPMatrix::IsInside(int i, int j){
    if (i>=0 && i<fM && j>=0 && j<fN){
        return true;
    }
    return false;
}

double  QPMatrix::At(int i){
    if( !IsInside(i) ){
        std::cout<<"QPMatrix::At(1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return -1;
    }
    return fElement[i];
}

double  QPMatrix::At(int i, int j){
    if( !IsInside(i,j) ){
        std::cout<<"QPMatrix::At(2) - Out if index "<<"["<<i<<","<<j<<"]"<<std::endl;
        return -1;
    }
    return fElement[i*fN+j];
}

bool     QPMatrix::IsSameShape(QPMatrix & other){
    if(fN == other.fN && fM == other.fM) return true;
    return false;
}

bool     QPMatrix::IsTransShape(QPMatrix & other){
    if(fN == other.fM && fM == other.fN) return true;
    return false;
}

double&  QPMatrix::operator[](int i){
    if( !IsInside(i) ){
        std::cout<<"QPMatrix::operator[](1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return nval;
    }
    return fElement[i];
}

double&  QPMatrix::operator()(int i){
    if( !IsInside(i) ){
        std::cout<<"QPMatrix::operator()(1) - Out if index"<<"["<<i<<"]"<<std::endl;
        return nval;
    }
    return fElement[i];
}

double&  QPMatrix::operator()(int i, int j){
    if( !IsInside(i, j) ){
        std::cout<<"QPMatrix::operator()(2) - Out if index"<<"["<<i<<"]"<<std::endl;
        return nval;
    }
    return fElement[i*fM+j];
}

double QPMatrix::Det(){
    if(fM != fN){
        std::cout<<"QPMatrix::Det() - Determinant can be defined only for square matrix "<<"["<<fM<<","<<fN<<"]"<<std::endl;
        return 0.;
    }
    if(fM==0) return 0.;

    if(fM==1) return At(0);

    if(fM==2){
        return At(0,0)*At(1,1) - At(0,1)*At(1,0);
    }

    double ans = 0;
    for(int i=0; i<fN; i++){
        ans += pow(-1, i)*At(0,i)*SubMatrix(0, i).Det();
    }

    return ans;
}

double QPMatrix::Trace(){
    double ans = 0;
    int ii = fN>fM ? fM : fN;

    for(int i=0; i<ii; i++){
        ans += operator()(i,i);
    }
    return ans;    
}

QPMatrix QPMatrix::SubMatrix(const int i_, const int j_){
    
    QPMatrix ans = QPMatrix(fM-1, fN-1);

    for(int i=0; i<fM-1; i++){
        for(int j=0; j<fN-1; j++){
            ans(i,j) = At(
                i < i_ ? i : i+1,
                j < j_ ? j : j+1
            );
        }
    }
    return ans;
}

double QPMatrix::Cofactor(const int i, const int j){
    if(fM != fN){
        std::cout<<"QPMatrix::Cofactor() - Cofactor can be defined only for square matrix "<<"["<<fM<<","<<fN<<"]"<<std::endl;
        return 0;
    }
    return pow(-1, i+j) * (SubMatrix(i,j).Det());
}

QPMatrix QPMatrix::Inverse(){
    if(fM != fN){
        std::cout<<"QPMatrix::Inverse() - Inverse can be defined only for square matrix "<<"["<<fM<<","<<fN<<"]"<<std::endl;
        return QPMatrix();
    }

    double det = Det();

    if(det<0.0000000001 && det>-0.0000000001 ){
        std::cout<<"QPMatrix::Inverse() - Determinant is near zero. No inverse matrix "<<"["<<det<<"]"<<std::endl;
        return QPMatrix();
    }

    QPMatrix ans = QPMatrix(fM, fN);
    for(int i=0; i<fM; i++){
        for(int j=0; j<fN; j++){
            ans(i,j) = Cofactor(j,i);
        }
    }

    ans*=(1/Det());

    return ans;
}



QPMatrix QPMatrix::Transpose(){
    QPMatrix ans(fN, fM);

    for(int i=0; i<fM; i++){
        for(int j=0; j<fN; j++){
            ans(j,i) = At(i,j);
        }
    }
    return ans;
} 

QPMatrix operator*(QPMatrix self, double  other){
    QPMatrix ans = QPMatrix(self);
    for(int i=0; i<(self.fN*self.fM); i++){
        ans(i) *= other;
    }
    return ans;
}

QPMatrix operator*(double other, QPMatrix self){
    QPMatrix ans = QPMatrix(self);
    for(int i=0; i<(self.fN*self.fM); i++){
        ans(i) *= other;
    }
    return ans;
}

QPMatrix operator*(QPMatrix self, QPMatrix  other){
    if(self.fN != other.fM){
        std::cout<<"QPMatrix::operator*()(3) - inner part of 2 matrices are not same : "<<self.fN<<", "<<other.fM<<std::endl;
        return QPMatrix();
    }
    QPMatrix ans = QPMatrix(self.fM, other.fN);
    for(int i=0; i<(ans.fM); i++){
        for(int j=0; j<(ans.fN); j++){
            for(int k=0; k<(self.fN); k++){
                ans(i,j)+= self.At(i,k) * other.At(k,j);
            }
        }
    }
    return ans;
}

QPMatrix operator+(QPMatrix &  self, QPMatrix & other){
    if(! self.IsSameShape(other) ){
        std::cout<<"operator+ (QPMatrix&, QPMatrix&) - Not same shape"<<std::endl;
        return QPMatrix();
    }

    QPMatrix ans = QPMatrix(self.fM, self.fN);

    int length = self.fN * self.fM;
    for(int i=0; i<length; i++){
        ans[i] = self[i] + other[i];
    }

    return ans;

}

QPMatrix operator-(QPMatrix & self, QPMatrix & other){
    if(! self.IsSameShape(other) ){
        std::cout<<"operator- (QPMatrix&, QPMatrix&) - Not same shape"<<std::endl;
        return QPMatrix();
    }

    QPMatrix ans(self.fM, self.fN);

    int length = self.fN * self.fM;
    for(int i=0; i<length; i++){
        ans[i] = self[i] - other[i];
    }

    return ans;

}

std::string QPMatrix::Print(bool quite){
    
    std::ostringstream anss;

    anss.precision(4);

    for(int i=0; i<fM; i++){
        for(int j=0; j<fN; j++){
            anss << At(i,j) << "\t";
        }
        anss<<std::endl;
    }
    anss.unsetf(std::ios::fixed);

    std::string ans = anss.str();

    if(!quite){
        std::cout<<ans;
    }

    return ans;
}

