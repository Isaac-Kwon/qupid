#include "QPDigitizerSquare.hpp"
#include "QPMonteCarlo.hpp"
#include "QPRandom.hpp"

#include <iostream>
#include <vector>

// ROOT-free Monte Carlo: core QPRandom (std::mt19937_64) + QPMonteCarlo,
// CSV output. Same study as test6 (various amplitude, various position)
// at reduced statistics; no ROOT headers anywhere.

int main(){

    int nTrial = 1000;
    double maxsignal = 537280;

    //Define Digitizer
    double pitchx = 29.24;
    double pitchy = 26.88;
    int    npixx  = 10;
    int    npixy  = 10;
    double threshold = 250;

    double sigma = 0.83*900; //fixed

    QPDigitizerSquare * dig = new QPDigitizerSquare(pitchx * npixx, pitchy * npixy, npixx, npixy);

    QPRandom random;

    QPMonteCarlo mc(dig, random);
    mc.SetPositionRange(-pitchx*0.5, pitchx*0.5, -pitchy*0.5, pitchy*0.5);
    mc.SetAmplitudeRange(0., maxsignal);
    mc.SetSigma(sigma);
    mc.SetThreshold(threshold);

    std::vector<QPMonteCarlo::Record> records = mc.Run(nTrial);

    if(!QPMonteCarlo::WriteCSV(records, "test7_output.csv")){
        std::cout<<"test7: cannot write test7_output.csv"<<std::endl;
        return 1;
    }

    std::cout<<"test7: "<<records.size()<<" events -> test7_output.csv"<<std::endl;
    return 0;
}
