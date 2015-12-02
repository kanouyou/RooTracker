//****************************************//
//  SimPHITS to ROOTracker
//  create by Ye YANG
//  Kyushu U.
//****************************************//

//  2015.04.28   Ye YANG

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <cmath>
#include <TTree.h>
#include <TBits.h>
#include <TParticle.h>
#include "PHITStoRooTracker.h"

using namespace std;


SimPHITSToRooTracker::SimPHITSToRooTracker(string argv) {
  filename = argv;
}


void SimPHITSToRooTracker::WriteToMyFormat(int bunch, int event) {
  string rootname = GetFileName(filename) + ".root";
  TFile* file = new TFile(rootname.c_str(), "RECREATE");
  TTree* tree = new TTree("RooTracker", "SimPHITS ROOTracker");
  
  int    EvtID;           // current event: event + event*(bunch-1)
  int    BchID;           // bunch
  int    EvtPDG;          // pdgcode
  double EvtV4[4];        // position: (x, y, z, t)
  double EvtP4[4];        // momentum: (Px, Py, Pz, E)
  double EvtEnergy;       // energy
  double EvtWgt;          // weight
  int    Counter[3];      // counter: (c1, c2, c3)
  
  tree->Branch(  "EvtID",  &EvtID,      "EvtID/I");
  tree->Branch(  "BchID",  &BchID,      "BchID/I");
  tree->Branch( "EvtPDG", &EvtPDG,     "EvtPDG/I");
  tree->Branch(  "EvtV4",   EvtV4,   "EvtV4[4]/D");
  tree->Branch(  "EvtP4",   EvtP4,   "EvtP4[4]/D");
  tree->Branch("EvtEnergy", &EvtEnergy, "EvtEnergy/D");
  tree->Branch( "EvtWgt", &EvtWgt,     "EvtWgt/D");
  tree->Branch("Counter", Counter, "Counter[3]/D");
  
  ifstream input(filename.c_str());

  string pdg, x, y, z, px, py, pz, e, wt, t, c1, c2, c3, sx, sy, sz, n0, nc, nb, no;
  int evt;
  if (!input) cerr << "error: failed to open this file " << filename.c_str() << endl;
  else {
    while (true) {
      input >> pdg >> x >> y >> z >> px >> py >> pz >> e >> wt >> t >> c1 >> c2 >> c3 >> sx >> sy >> sz >> n0 >> nc >> nb >> no;
	  if (!input) break;
      EvtID     = static_cast<int>(StringToFloat(nc));
	  BchID     = static_cast<int>(StringToFloat(nb));
      EvtPDG    = static_cast<int>(StringToFloat(pdg));
      EvtV4[0]  = static_cast<double>(StringToFloat(x));
	  EvtV4[1]  = static_cast<double>(StringToFloat(y));
	  EvtV4[2]  = static_cast<double>(StringToFloat(z));
	  EvtV4[3]  = static_cast<double>(StringToFloat(t));
	  EvtP4[0]  = static_cast<double>(StringToFloat(px));
	  EvtP4[1]  = static_cast<double>(StringToFloat(py));
	  EvtP4[2]  = static_cast<double>(StringToFloat(pz));
	  EvtP4[3]  = GetMomentum(static_cast<int>(StringToFloat(pdg)), static_cast<double>(StringToFloat(e)));
	  EvtEnergy = static_cast<double>(StringToFloat(e));
	  EvtWgt    = static_cast<double>(StringToFloat(wt));
	  Counter[0]= static_cast<int>(StringToFloat(c1));
	  Counter[1]= static_cast<int>(StringToFloat(c2));
	  Counter[2]= static_cast<int>(StringToFloat(c3));
      evt = EvtID + event*(BchID - 1);
	  if (evt%event==0) 
	    cout << "Filling data now  -->  " << (float)evt/(event*bunch)*100 << "% finished" << endl;
	  tree->Fill();
	}
  }
  
  file->Write();
  file->Close();
}

/* a class to read the energy depostion output format to my root format */
void SimPHITSToRooTracker::ReadEnergyDeposition(int bunch, int event) {
  string rootname = GetFileName(filename) + ".root";
  TFile* file = new TFile(rootname.c_str(), "RECREATE");
  TTree* tree = new TTree("RooTracker", "SimPHITS energy deposition data");

  int    EvtID;          // Event ID
  int    EvtPDG;         // Event PDG code
  int    EvtGeo;         // Geometry ID
  double EvtDep;         // Energy deposition
  double EvtV4[4];       // position: (x, y, z, E)
  double EvtV4C[4];      // current position: (Xc, Yc, Zc, Ec)
  double EvtWgt;         // Event weight

  tree->Branch( "EvtID",  &EvtID,     "EvtID/I");
  tree->Branch("EvtPDG", &EvtPDG,    "EvtPDG/I");
  tree->Branch("EvtGeo", &EvtGeo,    "EvtGeo/I");
  tree->Branch("EvtDep", &EvtDep,    "EvtDep/D");
  tree->Branch( "EvtV4",   EvtV4,  "EvtV4[4]/D");
  tree->Branch("EvtV4C",  EvtV4C, "EvtV4C[4]/D");
  tree->Branch("EvtWgt", &EvtWgt,    "EvtWgt/D");
  
  /* read file */
  ifstream input(filename.c_str());
  int evt, bch, col, pdg, geo;
  double dep, wgt, e, x, y, z, ec, xc, yc, zc;
  string buff;
  if (!input) cerr << "Error: cannot open it" << endl;
  else {
	// remove first comment line
    getline(input, buff);
	while (true) {
	  input >> evt >> bch >> col >> pdg >> geo >> dep >> wgt >> e >> x >> y >> z >> ec >> xc >> yc >> zc;
	  if (!input) break;
	  EvtID    = evt;
	  EvtPDG   = pdg;
	  EvtGeo   = geo;
	  EvtDep   = dep;
	  EvtV4[0] = x;
	  EvtV4[1] = y;
	  EvtV4[2] = z;
	  EvtV4[3] = e;
	  EvtV4C[0]= xc;
	  EvtV4C[1]= yc;
	  EvtV4C[2]= zc;
	  EvtV4C[3]= ec;
	  EvtWgt   = wgt;
	  if (evt%event==0) 
	    cout << "Filling data now  -->  " << (float)evt/(event*bunch)*100 << "% finished" << endl;
	  tree->Fill();
	}
  }
  
  file->Write();
  file->Close();
}


/* a function to convert data to ICEDUST format */
void SimPHITSToRooTracker::WriteToICEDUSTFormat(int bunch, int event) {
  string rootname = GetFileName(filename) + ".root";
  TFile* file = new TFile(rootname.c_str(), "RECREATE");
  TTree* tree = new TTree("gRooTracker", "SimPHITS RooTracker data");
  const int kNPmax = event;
  const int fEvent = 500000;

  TBits*      brEvtFlags = 0; // Generator-specific event flags
  TObjString* brEvtCode  = 0; // Generator-specific event string with 'event code'
  int         brEvtNum;       // Event number
  double      brEvtXSec;      // Cross section for selected event (1E-38 cm2)
  double      brEvtDXSec;     // Cross section for selected event (1E-38 cm2 /{K^n})
  double      brEvtWght;      // Weight of the event
  double      brEvtProb;      // Probability for that event (gicen cross-sections, path lengths etc.)
  double      brEvtVtx[4];    // Event vertex position in detector coordinate system (SI)
  int         brStdHepN;      // Number of particles in particle array
  // std-hep like particle array
  int         brStdHepPdg   [kNPmax];     // Pdg codes (& generator specific codes for pseudoparticles)
  int         brStdHepStatus[kNPmax];     // Generator-specific status code
  int         brStdHepRescat[kNPmax];     // Hadron transport model - specific rescattering code
  double      brStdHepX4    [kNPmax][4];  // 4-x (x, y, z, t) of particle in hit nucleus frame (fm)
  double      brStdHepP4    [kNPmax][4];  // 4-p (px,py,pz,E) of particle in LAB frame (GeV)
  double      brStdHepPolz  [kNPmax][3];  // Polarization vector
  int         brStdHepFd    [kNPmax];     // First daughter
  int         brStdHepLd    [kNPmax];     // Last  daughter
  int         brStdHepFm    [kNPmax];     // First mother
  int         brStdHepLm    [kNPmax];     // Last  mother
  // Added for COMET
  double      brWeight      [kNPmax];     // weight of the particle
  TObjArray*  brMonitorName = new TObjArray();     // the name of the monitor

  const int n_monitors = 6;
  TObjString fort_81("3mMonitor");
  TObjString fort_83("90degreeMonitor");
  TObjString fort_88("ProtonTargetMonitor_Cylinder");
  TObjString fort_89("ProtonTargetMonitor_Upstream");
  TObjString fort_90("ProtonTargetMonitor_Downstream");
  TObjString fort_unknown("Unknown");

  TObjString* monitor_names[n_monitors] = { &fort_81,
					  &fort_83,
					  &fort_88,
					  &fort_89,
					  &fort_90,
					  &fort_unknown};
  
  int iParticle = 0;
  tree->Branch("EvtFlags", "TBits",      &brEvtFlags, 32000, 1);           
  tree->Branch("EvtCode",  "TObjString", &brEvtCode,  32000, 1);           
  tree->Branch("EvtNum",          &brEvtNum,          "EvtNum/I");             
  tree->Branch("EvtXSec",         &brEvtXSec,         "EvtXSec/D");           
  tree->Branch("EvtDXSec",        &brEvtDXSec,        "EvtDXSec/D");           
  tree->Branch("EvtWght",         &brEvtWght,         "EvtWght/D");           
  tree->Branch("EvtProb",         &brEvtProb,         "EvtProb/D");           
  tree->Branch("EvtVtx",           brEvtVtx,          "EvtVtx[4]/D");             
  tree->Branch("StdHepN",         &brStdHepN,         "StdHepN/I");             
  tree->Branch("StdHepPdg",        brStdHepPdg,       "StdHepPdg[StdHepN]/I"); 
  tree->Branch("StdHepStatus",     brStdHepStatus,    "StdHepStatus[StdHepN]/I");
  tree->Branch("StdHepRescat",     brStdHepRescat,    "StdHepRescat[StdHepN]/I");
  tree->Branch("StdHepX4",         brStdHepX4,        "StdHepX4[StdHepN][4]/D");
  tree->Branch("StdHepP4",         brStdHepP4,        "StdHepP4[StdHepN][4]/D");
  tree->Branch("StdHepPolz",       brStdHepPolz,      "StdHepPolz[StdHepN][3]/D");
  tree->Branch("StdHepFd",         brStdHepFd,        "StdHepFd[StdHepN]/I");
  tree->Branch("StdHepLd",         brStdHepLd,        "StdHepLd[StdHepN]/I");
  tree->Branch("StdHepFm",         brStdHepFm,        "StdHepFm[StdHepN]/I");
  tree->Branch("StdHepLm",         brStdHepLm,        "StdHepLm[StdHepN]/I");
		  
  // Added for COMET
  tree->Branch("Weight",         brWeight,        "Weight[StdHepN]/D");
  tree->Branch("MonitorName", "TObjArray", brMonitorName);
  brEvtNum = 1;

  ifstream input(filename.c_str());

  string pdg, x, y, z, px, py, pz, e, wt, t, c1, c2, c3, sx, sy, sz, n0, nc, nb, no;
  int    evt;
  double ptot;

  if (!input) cerr << "error: failed to open this file " << filename.c_str() << endl;
  else {
    while (true) {
      input >> pdg >> x >> y >> z >> px >> py >> pz >> e >> wt >> t >> c1 >> c2 >> c3 >> sx >> sy >> sz >> n0 >> nc >> nb >> no;
	  //if (!input) break;
	  evt = static_cast<int>(StringToFloat(nc) + fEvent*(StringToFloat(nb)-1));
      if (brEvtNum != evt) {
        if (evt%event==0) {
		  cout << "Filling data now  -->  " << (float)evt/(bunch*event)*100 << "% finished" << endl;
		  //cout << "Now " << evt << endl;
		}
		brEvtNum   = evt;
		brEvtXSec  = -999;
		brEvtDXSec = -999;
		brEvtWght  = -999;
		brEvtProb  = -999;
		brEvtVtx[0]= 0; 
		brEvtVtx[1]= 0;
		brEvtVtx[2]= 0;
		brEvtVtx[3]= 0;
        brStdHepN  = iParticle;
		tree->Fill();
		iParticle = 0;
	  }
      
	  ptot = GetMomentum(static_cast<int>(StringToFloat(pdg)), static_cast<double>(StringToFloat(e)));

      brStdHepPdg   [iParticle] = static_cast<int>(StringToFloat(pdg));
      brStdHepStatus[iParticle] = 1; // set to 1 since that is what SimG4 is expecting for particles that it can track
	  brStdHepRescat[iParticle] = -999;  
	  brStdHepX4    [iParticle][0] = static_cast<double>(StringToFloat(x));
	  brStdHepX4    [iParticle][1] = static_cast<double>(StringToFloat(y));
	  brStdHepX4    [iParticle][2] = static_cast<double>(StringToFloat(z));
	  brStdHepX4    [iParticle][3] = static_cast<double>(StringToFloat(t));
		
	  brStdHepP4    [iParticle][0] = static_cast<double>(StringToFloat(px))*ptot;
	  brStdHepP4    [iParticle][1] = static_cast<double>(StringToFloat(py))*ptot;
	  brStdHepP4    [iParticle][2] = static_cast<double>(StringToFloat(pz))*ptot;
	  brStdHepP4    [iParticle][3] = ptot;
		
	  brStdHepPolz  [iParticle][0] = -999;
	  brStdHepPolz  [iParticle][1] = -999;
      brStdHepPolz  [iParticle][2] = -999;
		
	  brStdHepFd    [iParticle] = -999;     // First daughter
	  brStdHepLd    [iParticle] = -999;     // Last  daughter
	  brStdHepFm    [iParticle] = -999;     // First mother
      brStdHepLm    [iParticle] = -999;     // Last  mother
		
	  brWeight      [iParticle] = static_cast<double>(StringToFloat(wt));
	  //if (brStdHepX4[iParticle][2]==-300.) brMonitorName->Add(monitor_names[0]);
	  //else brMonitorName->Add(monitor_names[5]);

	  ++iParticle;
	  if (!input) break;
	}
  }
  
  //tree->Fill();
  file->Write();
  file->Close();
}

// get the input file's name
string SimPHITSToRooTracker::GetName() {
  return filename;
}


/* a function to convert the fortran double format with D to float*/
float SimPHITSToRooTracker::StringToFloat(string str) {
  str.replace(str.find("D"), 1, "e");
  istringstream iss(str);
  float buff = 0.;
  iss >> buff;
  return buff;
}


/* a function to convert the fortran double format with D to double*/
double SimPHITSToRooTracker::StringToDouble(string str) {
  str.replace(str.find("D"), 1, "e");
  istringstream iss(str);
  double buff = 0.;
  iss >> buff;
  return buff;
}


/* a function to change the file extension */
string SimPHITSToRooTracker::GetFileName(string name) {
  // remove the filename extension
  int size = name.size();
  int exts = name.find(".");
  string outputname  = name.erase(exts, size-1);
  return outputname;
}


/* a function to convert energy to momentum */
double SimPHITSToRooTracker::GetMomentum(int PDGID, double E) {
  float mass;
  /*switch (PDGID) {
    case  2212:
	case -2212: mass = 938.272; break;    // proton
	case  2112:
	case -2112: mass = 939.565; break;    // neutron
	case   211:
	case  -211: mass = 139.579; break;    // pion+-
	case   111: mass = 134.977; break;    // pion0
    case    13:
	case   -13: mass = 106.658; break;    // muon+-
	case   321:
	case  -321: mass = 493.667; break;    // kaon+-
	case   311:
	case  -311: mass = 497.667; break;    // kaon0
	case    11:
	case   -11: mass = 0.511;   break;    // electron, prosition
	case    12:
	case   -12: mass = 1.0e-6;  break;    // nu_e
	case    14:
	case   -14: mass = 0.2;     break;    // nu_mu
	case   221:
	case  -221: mass = 547.3;   break;    // eta+-
	case   331: mass = 957.8;   break;    // eta'
	case  3122:
	case -3122: mass = 1115.683;break;    // lambda0
	case  3222:
	case -3222: mass = 1189.37; break;    // sigma+
	case  3212:
	case -3212: mass = 1192.642;break;    // sigma0
	case  3112:
	case -3112: mass = 1197.449;break;    // sigma-
	case  3322:
	case -3322: mass = 1314.83; break;    // xi0
	case  3312:
	case -3312: mass = 1321.31; break;    // xi-
	case  3334:
	case -3334: mass = 1672.45; break;    // omega-
	case 2000004: mass = 3727.379; break; // alpha
	default:    mass = 0.0;
  }*/
  
  if (PDGID<=3000000) {
    TParticle* particle = new TParticle();
    particle->SetPdgCode(PDGID);
    mass = particle->GetMass() * 1e+3;
  }
  else
    mass = 0.;

  double p = sqrt(pow(E+mass, 2) - pow(mass, 2));
  return p;
}

