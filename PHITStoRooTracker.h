//****************************************//
//  SimPHITS to ROOTracker
//  create by Ye YANG
//  Kyushu U.
//****************************************//

//  2015.04.28   Ye YANG

#ifndef ___SIMPHITS_TO_ROOT
#define ___SIMPHITS_TO_ROOT

#include <string>
#include <TFile.h>

class SimPHITSToRooTracker {
  public:
    SimPHITSToRooTracker(std::string argv);
    ~SimPHITSToRooTracker() {}
    void WriteToMyFormat(int bunch=1000, int event=1000);
    void ReadEnergyDeposition(int bunch=1000, int event=1000);
    void WriteToICEDUSTFormat(int bunch=1000, int event=1000);
    double GetMomentum(int PDGID, double E);
    std::string GetName();

  protected:
    std::string GetFileName(std::string name);
    double StringToDouble(std::string str);
    float  StringToFloat(std::string str);

  private:
    std::string filename;
};

#endif
