#include <iostream>
#include <fstream>
#include "TUserPHITSRooTracker.h"

using std::string;
using std::endl;
using std::cerr;

TUserPHITSRooTracker::TUserPHITSRooTracker(string file) : SimPHITSToRooTracker(file) {
  fCheckFill = false;
  fFile = NULL;
}

TUserPHITSRooTracker::~TUserPHITSRooTracker() {}

void TUserPHITSRooTracker::WriteToMyFormat(int bunch, int event) {
  // Initialization
  bool fInit = Initialize();
  
  if (fInit) {
    Fill();
    Print();
    Finalize();
  }
  
}

bool TUserPHITSRooTracker::Initialize() {
  string rootName = GetFileName(GetName()) + ".root";
  fFile = new TFile(rootName.c_str(), "recreate");
  // you can change the tree name here
  if (fFile) {
    fTree = new TTree("tree", "phits tree");
    
	// you can also change the branch name here
    fPos      = new double[3];
    fMomentum = new double[3];
    fCount    = new int[3];
    fTree->Branch(   "event", &fEvent,          "event/I");
    fTree->Branch(   "bunch", &fBunch,          "bunch/I");
    fTree->Branch( "pdgcode", &fPdg,          "pdgcode/I");
    fTree->Branch("position",  fPos,      "position[3]/D");
    fTree->Branch(    "time", &fTime,            "time/D");
    fTree->Branch("momentum",  fMomentum, "momentum[3]/D");
    fTree->Branch(  "energy", &fEnergy,        "energy/D");
    fTree->Branch( "counter", fCount,     "counter[3]/I");
  }
  else
    return false;

  return true;
}

void TUserPHITSRooTracker::Fill() {
  ifstream input(GetName().c_str());
  
  string pdg, x, y, z, px, py, pz, e, wt, t, c1, c2, c3, sx, sy, sz, n0, nc, nb, no;
  if (!input) cerr << "error: failed to open this file!!! " << GetName().c_str() << endl;
  else {
    while (true) {
      input >> pdg >> x >> y >> z >> px >> py >> pz >> e >> wt >> t >> c1 >> c2 >> c3 >> sx >> sy >> sz >> n0 >> nc >> nb >> no;
	  if (!input) break;
      fEvent  = static_cast<int>(StringToFloat(nc));
      fBunch  = static_cast<int>(StringToFloat(nb));
      fPdg    = static_cast<int>(StringToFloat(pdg));
      fPos[0] = static_cast<double>(StringToFloat(x));
      fPos[1] = static_cast<double>(StringToFloat(y));
      fPos[2] = static_cast<double>(StringToFloat(z));
      fTime   = static_cast<double>(StringToFloat(t));
      fMomentum[0]  = static_cast<double>(StringToFloat(px));
      fMomentum[1]  = static_cast<double>(StringToFloat(py));
      fMomentum[2]  = static_cast<double>(StringToFloat(pz));
      fEnergy   = static_cast<double>(StringToFloat(e));
      fCount[0] = static_cast<int>(StringToFloat(c1));
      fCount[1] = static_cast<int>(StringToFloat(c2));
      fCount[2] = static_cast<int>(StringToFloat(c3));
	  fTree->Fill();
	}
  }
}

void TUserPHITSRooTracker::Finalize() {
  fFile->Write();
  fFile->Close();
}

void TUserPHITSRooTracker::Print() {
  fTree->Print();
}
