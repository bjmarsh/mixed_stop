#include "TMath.h"
#include "TROOT.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

int getFileType(const char*);
int getBkgType();
int getStopDecayMode();
double deltaR(const LorentzVector& p1, const LorentzVector& p2);
double deltaPhi(float phi1, float phi2);
double ComputeMT(LorentzVector p1, float met, float metphi);
