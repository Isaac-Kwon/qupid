#include "QPMonteCarlo.hpp"

#include "QPRandomBase.hpp"
#include "QPDigitizerBase.hpp"
#include "QPSignalGaussian.hpp"
#include "QPVector.hpp"
#include "QPMatrix.hpp"

#include <cstdio>

QPMonteCarlo::QPMonteCarlo(QPDigitizerBase* digitizer, QPRandomBase& random)
: fDigitizer(digitizer), fRandom(random){;}

void QPMonteCarlo::SetPositionRange(const double& xmin, const double& xmax,
                                    const double& ymin, const double& ymax){
    fXmin = xmin;
    fXmax = xmax;
    fYmin = ymin;
    fYmax = ymax;
}

void QPMonteCarlo::SetAmplitude(const double& amp){
    fAmp = amp;
    fRandomAmp = false;
}

void QPMonteCarlo::SetAmplitudeRange(const double& min, const double& max){
    fAmpMin = min;
    fAmpMax = max;
    fRandomAmp = true;
}

void QPMonteCarlo::SetSigma(const double& sigma){
    fSigma = sigma;
}

void QPMonteCarlo::SetThreshold(const double& threshold){
    fThreshold = threshold;
}

QPMonteCarlo::Record QPMonteCarlo::Shoot(){
    Record rec;

    // Fixed draw order: x, y, then amplitude only when it is randomized.
    rec.fX   = fRandom.Uniform(fXmin, fXmax);
    rec.fY   = fRandom.Uniform(fYmin, fYmax);
    rec.fAmp = fRandomAmp ? fRandom.Uniform(fAmpMin, fAmpMax) : fAmp;
    rec.fSigma = fSigma;

    QPVector mu = QPVector(rec.fX, rec.fY);
    QPMatrix S  = QPMatrix(2,2,4, fSigma, 0., 0., fSigma);

    QPSignalGaussian * sig = new QPSignalGaussian(mu, rec.fAmp, S);
    fDigitizer->InjectSignal(sig);

    rec.fNpix = (int) fDigitizer->GetFiredPixel(fThreshold).size();

    fDigitizer->ClearSignal();
    return rec;
}

std::vector<QPMonteCarlo::Record> QPMonteCarlo::Run(int ntrial){
    std::vector<Record> records;
    records.reserve(ntrial > 0 ? ntrial : 0);
    for(int i=0; i<ntrial; i++){
        records.push_back(Shoot());
    }
    return records;
}

bool QPMonteCarlo::WriteCSV(const std::vector<Record>& records,
                            const std::string& filename){
    std::FILE* out = std::fopen(filename.c_str(), "w");
    if(out == nullptr) return false;
    std::fprintf(out, "amp,x,y,sig,npix\n");
    for(const Record& r : records){
        std::fprintf(out, "%.17g,%.17g,%.17g,%.17g,%d\n",
                     r.fAmp, r.fX, r.fY, r.fSigma, r.fNpix);
    }
    std::fclose(out);
    return true;
}
