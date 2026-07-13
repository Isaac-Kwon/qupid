#include "QPMath.hpp"

// #include "QPMath.h"
#include "QPVector.hpp"
#include "QPMatrix.hpp"

#include <cmath>
#include <iostream>

double QPMath::Gaussian::Simple1D(double x,
                                  double integral,
                                  double mean,
                                  double sigma){
    if(sigma <= 0) return 0.;
    return QPMath::Gaus(x, mean, sigma, true) * integral;
}

double QPMath::Gaussian::Simple2D(double x,
                                  double y,
                                  double integral,
                                  double meanx,
                                  double meany,
                                  double sigma){
    if(sigma <= 0) return 0.;
    double chi = QPMath::Sqrt( QPMath::Power(x-meanx,2) + QPMath::Power(y-meany,2) ) / sigma;
    return QPMath::Gaussian::Simple1D(chi, integral*QPMath::InvSqrt2Pi/(sigma*sigma));
}

double QPMath::Gaussian::UniAxis2D(double x,
                                   double y,
                                   double integral,
                                   double meanx,
                                   double meany,
                                   double sigmax,
                                   double sigmay){
    if(sigmax<=0 || sigmay<=0) return 0.;
    double chi = QPMath::Sqrt(QPMath::Power(x-meanx,2)/QPMath::Power(sigmax,2) + QPMath::Power(y-meany,2)/QPMath::Power(sigmay,2));
    return QPMath::Gaussian::Simple1D(chi, integral*QPMath::InvSqrt2Pi/(sigmax*sigmay));
}

double QPMath::Gaussian::RotAxis2D(double x         ,
                                   double y         ,
                                   double integral  ,
                                   double meanx     ,
                                   double meany     ,
                                   double sigmax    ,
                                   double sigmay    ,
                                   double rotation  ){
    if(sigmax<=0 || sigmay<=0) return 0.;
    double xp = QPMath::Cos(rotation)*(x-meanx) + QPMath::Sin(rotation)*(y-meany);
    double yp = QPMath::Cos(rotation)*(y-meany) - QPMath::Sin(rotation)*(x-meanx);
    double chi = QPMath::Sqrt(QPMath::Power(xp,2)/QPMath::Power(sigmax,2) + QPMath::Power(yp,2)/QPMath::Power(sigmay,2));
    return QPMath::Gaussian::Simple1D(chi, integral*QPMath::InvSqrt2Pi/(sigmax*sigmay));
}

double QPMath::Gaussian::MatrixForm2D(QPVector& x, QPVector& x0, QPMatrix& S){
    QPVector vx = x - x0;
    QPMatrix X = QPMatrix(vx,2);

    // std::cout<<X.Print(true)<<std::endl;
    // std::cout<<X.Transpose().Print(true)<<std::endl;
    // std::cout<<S.Print(true)<<std::endl;
    // std::cout<<S.Inverse().Print(true)<<std::endl;

    double expo =  (X.Transpose() * (S.Inverse()) * X)[0];
    return QPMath::Exp(-0.5 * expo) * 0.5* (QPMath::InvPi) / QPMath::Sqrt(S.Det());
}

// double QPMath::Gaussian::UniCylinder3D(double x,
//                                          double y,
//                                          double z,
//                                          double dirx       ,
//                                          double diry       ,
//                                          double dirz       ,
//                                          double rotcx      ,
//                                          double rotcy      ,
//                                          double rotcx      ,
//                                          double stx        ,
//                                          double sty        ,
//                                          double stz        ,
//                                          double integral   ,
//                                          double sigmax     ,
//                                          double sigmay     ,
//                                          double rotation   ,
//                                          double length     ,
//                                          double dispersion ){
    
//     AXVector pos   = AXVector(x,y,z);
//     AXVector dir   = AXVector(dirx,diry,dirz).Unit();
//     AXVector stt   = AXVector(stx,sty,stz);

//     double pdist = ((stt-pos).Cross(dir)).Norm();
//     double ldist = QPMath::Sqrt((stt-pos).Norm2() - QPMath::Power(pdist,2));
    
    

//     return 0;
// }

double QPMath::Gaussian::Assymetry2D(double x,
                        double y,
                        double integral   ,
                        double meanx      ,
                        double meany      ,
                        double sigmax     ,
                        double sigmay     ,
                        double rotation   ,
                        double length     ,
                        double tilt       ,
                        double dispersion ){

    double xp = x-meanx;
    double yp = y-meany;

    // if( tilt > 0 && -1* length / QPMath::Sin(tilt) > xp) return 0;

    double z   = 0;
    double xp3 = xp*QPMath::Cos(tilt) + z*QPMath::Sin(tilt);
    double yp3 = yp;
    double zp3 = -1*xp*QPMath::Sin(tilt) + z*(tilt) - length;

    double sigmapx = -1* dispersion * zp3 + sigmax;
    double sigmapy = -1* dispersion * zp3 + sigmay;

    if(sigmapx<=0 || sigmapy<=0 ) return 0;

    return QPMath::Gaussian::RotAxis2D(xp3, yp3, integral, 0, 0, sigmapx, sigmapy, rotation);
}

// double QPMath::Gaussian::AF::UniCylinder3D(double* x, double* par){
//     return QPMath::Gaussian::UniCylinder3D(x[0], x[1], x[2], par[0], par[1], par[2], par[3], par[4], par[5], par[6], par[7], par[8], par[9], par[10], par[11]);
// }

double QPMath::WoodSaxon::Simple1D(double x, double maximum, double mean, double a, double radius){
    double xp = x-mean;
    xp = x > 0 ? x : -1*x;
    return maximum / ( 1+QPMath::Exp((xp-radius)/a));
}

double QPMath::WoodSaxon::Simple2D(double x, double y,
                                     double maximum, double meanx, double meany, double a, double radius){
    double dist = QPMath::Sqrt(QPMath::Power(x-meanx,2)+QPMath::Power(y-meany,2));
    return QPMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

double QPMath::WoodSaxon::UniAxis2D(double x, double y, 
                   double maximum, double meanx, double meany, double ax, double ay, double radiusX, double radiusY){
    double dist = QPMath::Sqrt(QPMath::Power(x-meanx,2)+QPMath::Power(y-meany,2));
    double a      = QPMath::GeometryCalc::EllipseUniAxis(x-meanx, y-meany, ax, ay);
    double radius = QPMath::GeometryCalc::EllipseUniAxis(x-meanx, y-meany, radiusX, radiusY);
    return QPMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

double QPMath::WoodSaxon::RotAxis2D(double x, double y,
                   double maximum, double meanx, double meany,
                   double ax, double ay, double radiusX, double radiusY, double rotation){
    double dist = QPMath::Sqrt(QPMath::Power(x-meanx,2)+QPMath::Power(y-meany,2));
    double a      = QPMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, ax, ay, rotation);
    double radius = QPMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, radiusX, radiusY, rotation);
    return QPMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

double QPMath::WoodSaxon::RotAxis2DUnsync(double x, double y,
                   double maximum, double meanx, double meany,
                   double ax, double ay, double radiusX, double radiusY, double rotationA, double rotationR){
    double dist = QPMath::Sqrt(QPMath::Power(x-meanx,2)+QPMath::Power(y-meany,2));
    double a      = QPMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, ax, ay, rotationA);
    double radius = QPMath::GeometryCalc::EllipseRotAxis(x-meanx, y-meany, radiusX, radiusY, rotationR);
    return QPMath::WoodSaxon::Simple1D(dist, maximum, 0, a, radius);
}

double QPMath::GeometryCalc::EllipseUniAxis(double x, double y, double lx, double ly){
    return QPMath::GeometryCalc::EllipseRotAxis(x, y, lx, ly, 0);
}

double QPMath::GeometryCalc::EllipseRotAxis(double x, double y, double lx, double ly, double theta){
    double angle = QPMath::GeometryCalc::Angle(x,y) - theta;
    double xp = lx * QPMath::Cos(angle);
    double yp = ly * QPMath::Sin(angle);
    
    return QPMath::Sqrt(QPMath::Power(xp,2) + QPMath::Power(yp,2));
}

double QPMath::GeometryCalc::Angle(double x, double y, double centx, double centy){
    double xp = x - centx;
    double yp = y - centy;
    
    return QPMath::ATan2(yp,xp);

}

double QPMath::Sqrt(const double& x){
    return sqrt(x);
}

double QPMath::Power(const double& x, const double& exponent){
    return pow(x,exponent);
}

double QPMath::Sin(const double& x){
    return sin(x);
}

double QPMath::Cos(const double& x){
    return cos(x);
}

double QPMath::ATan(const double& x){
    return atan(x);
}

double QPMath::ATan2(const double& y, const double& x){
    return atan2(x, y);
}

double QPMath::Exp(const double& x){
    return exp(x);
}

double QPMath::Gaus(const double& x, const double& mean, const double& sigma, const bool& norm)
{
   if (sigma == 0) return 1.e30;
   double arg = (x-mean)/sigma;
   // for |arg| > 39  result is zero in double precision
   if (arg < -39.0 || arg > 39.0) return 0.0;
   double res = QPMath::Exp(-0.5*arg*arg);
   if (!norm) return res;
   return res/(2.50662827463100024*sigma); //sqrt(2*Pi)=2.50662827463100024
}


QPMatrix QPMath::RotationMatrix(double theta){
    QPMatrix ans = QPMatrix(2,2,4,QPMath::Cos(theta), -QPMath::Sin(theta), QPMath::Sin(theta), QPMath::Cos(theta));
    return ans;
}