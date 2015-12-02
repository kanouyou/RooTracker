// Example for edit the user defined root format
// Created by Y. Yang
// 2015-12-2

#ifndef TUserSimPHITSRooTracker_HH
#define TUserSimPHITSRooTracker_HH

#include <string>
#include <TFile.h>
#include <TTree.h>
#include "PHITStoRooTracker.h"

class TUserPHITSRooTracker : public SimPHITSToRooTracker {
  public:
    TUserPHITSRooTracker(std::string);
    virtual ~TUserPHITSRooTracker();
    virtual void WriteToMyFormat(int bunch=1000, int event=1000); 
    bool Initialize();
    void Fill();
    void Finalize();
    void Print();

  private:
    bool    fCheckFill;
    TFile*  fFile;
    TTree*  fTree;
    int     fEvent;
    int     fBunch;
    int     fPdg;
    double* fPos;
    double  fTime;
    double* fMomentum;
    double  fEnergy;
    int*    fCount;
};

#endif
