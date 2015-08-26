#include "TMath.h"
#include "Math/VectorUtil.h"
#include "TROOT.h" 

#include "utils.h"

#include <math.h>
#include <stdlib.h>
#include <vector>
#include <map>

#include "CMS3/CMS3.h"

using namespace std;
using namespace tas;

int getFileType(const char* fname){
    // map background types to an index
    // 0 = ttbar, 1 = dyjets, 2 = WJets                                
 
    int filetype = -1;

    if (strstr(fname, "ttbar") != NULL)
        filetype = 0;
    else if (strstr(fname, "WJets") != NULL)
        filetype = 1;
    else if (strstr(fname, "ttzjets") != NULL)
        filetype = 2;
    else if (strstr(fname, "t_tW") != NULL || strstr(fname, "tbar_tW") != NULL)
        filetype = 3;

    if(filetype == -1){
        cout << "ERROR: unrecognized input file " << fname << endl;
        return 0;
    }

    return filetype;

}

// int getBkgType(int fileType){

//     // int ngenlep = 0;
//     // for(unsigned int i=0; i<genels_id().size(); i++){
//     //     if(genels_status()[i] == 23){
//     //         ngenlep++;
//     //     }
//     // }
//     // for(unsigned int i=0; i<genmus_id().size(); i++){
//     //     if(genmus_status()[i] == 23){
//     //         ngenlep++;
//     //     }
//     // }

//     // if(ngenlep==0)
//     //     return 0;
//     // else if(ngenlep==1)
//     //     return 1;
//     // else if (ngenlep==2)
//     //     return 2;
//     // else
//     //     return 3;


// }


/////////////////////////////
////// root math stuff //////
/////////////////////////////
double deltaR(const LorentzVector& p1, const LorentzVector& p2) {
    return ROOT::Math::VectorUtil::DeltaR(p1,p2);
}

double deltaPhi(float phi1, float phi2) {
    double dPhi = fabs( phi1 - phi2 );
    if( dPhi > M_PI ) dPhi = 2.0*M_PI - dPhi;
    return dPhi;
}

double ComputeMT(LorentzVector p1, float met, float metphi) {
    double dPhi = deltaPhi(p1.Phi(), metphi);
    // 43.61 of pdg.lbl.gov/2013/reviews/rpp2012-rev-kinematics.pdf
    return sqrt( 2.0 * p1.Pt() * met * (1.0-cos(dPhi)) );
}
