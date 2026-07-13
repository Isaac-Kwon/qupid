#include "QPVector.hpp"
#include "QPMath.hpp"

#include <cmath>

#include <iostream>
#include <string>
#include <sstream>

QPVector::QPVector(): fX(0), fY(0), fZ(0){}
QPVector::QPVector(double x, double y): fX(x), fY(y), fZ(0){}
QPVector::QPVector(double x, double y, double z): fX(x), fY(y), fZ(z){}
QPVector::QPVector(const QPVector& other): fX(other.fX), fY(other.fY), fZ(other.fZ){;}

//Get
double    QPVector::X(){return fX;}
double    QPVector::Y(){return fY;}
double    QPVector::Z(){return fZ;}
void      QPVector::Get(double& x, double& y, double& z){x = fX; y = fY; z=fZ;}

QPVector  QPVector::Unit(){
    double norm = Norm();
    return QPVector(fX/norm, fY/norm, fZ/norm);
}

//Operator, returning result

QPVector QPVector::Scale(double& number){
    // return QPVector(number*fX, number*fY, number*fZ);
    return QPVector(number*fX, number*fY, number*fZ);
}

double    QPVector::Norm(){
    return sqrt(fX*fX + fY*fY + fZ*fZ);
}

double    QPVector::Norm2(){
    return fX*fX + fY*fY + fZ*fZ;
}

//Operator, Self Closed operation 
void      QPVector::AddThis(const QPVector& other){
    // fX += other.fX;
    // fY += other.fY;
    // fZ += other.fZ;
    (*this)+=other;
}
void      QPVector::SubstractThis(const QPVector& other){
    // fX -= other.fX;
    // fY -= other.fY;
    // fZ -= other.fZ;
    (*this)-=other;
}
void      QPVector::CrossThis(const QPVector& other){
    (*this)^=other;
}

void      QPVector::ScaleThis(const double& number){
    (*this).operator*=(number);
}

//Operator, Coveniences

// double    QPVector::operator*(const QPVector& other){
//     return sqrt(fX*other.fX + fY*other.fY + fZ*other.fZ);
// }

// QPVector& QPVector::operator*(const double&   other){
//     return * new QPVector(fX*other, fY*other, fZ*other);
// }

// QPVector& QPVector::operator^(const QPVector& other){
//     double x = fY*other.fZ-fY*other.fZ;
//     double y = fZ*other.fX-fX*other.fZ;
//     double z = fX*other.fY-fY*other.fX;
//     return * new QPVector(x,y,z);
// }

double    QPVector::GetAngle(QPVector other){ //Get Angle [0, Pi]
    return acos(Unit().Dot(other.Unit()));
}

double    QPVector::GetAngleDegree(QPVector other){
    return GetAngle(other)*180/QPMath::Pi;
}

std::string QPVector::Print(bool quite){
    std::ostringstream anss;

    anss << "[" << fX << "," << fY << "," << fZ << "]";

    std::string ans = anss.str();
    if(!quite){
        // std::cout<<fX<<"\t"<<fY<<"\t"<<fZ<<std::endl;
        std::cout<<"[x,y,z] = " << ans << std::endl;
    }
    
    return ans;
}

// QPVector operator+ ( QPVector& a,  QPVector& b) {
//     return QPVector(a.fX+b.fX, a.fY+b.fY, a.fZ+b.fZ);
// }

QPVector operator+ ( QPVector  a,  QPVector  b) {
    return QPVector(a.fX+b.fX, a.fY+b.fY, a.fZ+b.fZ);
}

// QPVector operator- ( QPVector& a,  QPVector& b) {
//     return QPVector(a.fX-b.fX, a.fY-b.fY, a.fZ-b.fZ);
// }

QPVector operator- ( QPVector  a,  QPVector  b) {
    return QPVector(a.fX-b.fX, a.fY-b.fY, a.fZ-b.fZ);
}

// QPVector operator^ ( QPVector& a,  QPVector& b) {
//     double x = a.fY*b.fZ-a.fY*b.fZ;
//     double y = a.fZ*b.fX-a.fX*b.fZ;
//     double z = a.fX*b.fY-a.fY*b.fX;
//     return QPVector(x,y,z);
// }

QPVector operator^ ( QPVector  a,  QPVector  b) {
    double x = a.fY*b.fZ-a.fZ*b.fY;
    double y = a.fZ*b.fX-a.fX*b.fZ;
    double z = a.fX*b.fY-a.fY*b.fX;
    return QPVector(x,y,z);
}

// QPVector operator* ( QPVector &vec, double other) {
//     return QPVector(other*vec.fX, other*vec.fY, other*vec.fZ);
// }

QPVector operator* ( QPVector vec, double other) {
    return QPVector(other*vec.fX, other*vec.fY, other*vec.fZ);
}

// double   operator* ( QPVector& other,  QPVector& vec) {
//     return vec.Dot(other);
// }

// QPVector operator* (double other,  QPVector &vec) {
//     return QPVector(other*vec.fX, other*vec.fY, other*vec.fZ);
// }

QPVector operator* (double other,  QPVector vec) {
    return QPVector(other*vec.fX, other*vec.fY, other*vec.fZ);
}

double   operator* ( QPVector  other,  QPVector  vec) {
    return vec.Dot(other);
}