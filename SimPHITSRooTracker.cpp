#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "TUserPHITSRooTracker.h"

using namespace std;
void usage();
void MyFormat(string);
void Deposit(string);
void ICEDUSTFormat(string);
static int bunch = 10000;
static int event = 1000;

int main(int argc, char** argv) {
  
  if (argc<1) cerr << "Error: there is no input file or options!" << endl;

  vector<string> input;
  for (int i=1; i<argc; i++) input.push_back(string(argv[i]));
  
  // option:
  /*for (int i=0; i<input.size(); i++) {
	if (input[i]=="-p" || input[i]=="--proton")
	if else (input[i]=="-e" || input[i]=="--event") {
      istringstream ist(input[i+1]);
	  ist >> event;
	}
  }*/
  
  for (int i=0; i<input.size(); i++) {
    if (input[i]=="-h" || input[i]=="--help")   usage();
	else if (input[i]=="-m" || input[i]=="--my") {
      for (int j=i+1; j<input.size(); j++) MyFormat(input[j]);
	}
	else if (input[i]=="-d" || input[i]=="--deposit") {
      for (int j=i+1; j<input.size(); j++) Deposit(input[j]);
	}
	else if (input[i]=="-i" || input[i]=="--icedust") {
      for (int j=i+1; j<input.size(); j++) ICEDUSTFormat(input[j]);
	}
	else if (!input[i].find("-") && !input[i].find("=")) {
      for (int j=i+1; j<input.size(); j++) MyFormat(input[j]);
	}
    //else {
    //  cerr << "Error: Unkown option!!! " << endl;
    //  return 0;
    //}
  }

  return 0;
}


void usage() {
  cout << "\nUsage\n";
  cout << "       SimPHITSToRooTracker.exe  <option>  [input file]" << endl;
  cout << "\n  RooTracker is a simple program to convert PHITS dump file into a root file," << endl;
  cout << "then it will be read out by GEANT4 to simulate physics.\n";
  cout << "\nOptions:\n";
  cout << "\n  -h, --help              help" << endl;
  cout << "  -m, --myformat          convert dump file to my format" << endl;
  cout << "  -d, --deposit           convert energy depostion data to root file" << endl;
  cout << "  -i, --icedust           convert dump file to icedust format\n" << endl;
}


void MyFormat(string file) {
  cout << "\n file: " << file << " --> convert to my format" << endl;
  cout << "reading... " << endl;
  TUserPHITSRooTracker* rt = new TUserPHITSRooTracker(file);
  rt->WriteToMyFormat(bunch, event);
}


void Deposit(string file) {
  cout << "\n file: " << file << " --> convert to root file" << endl;
  cout << "reading..." << endl;
  TUserPHITSRooTracker* rt = new TUserPHITSRooTracker(file);
  rt->ReadEnergyDeposition(bunch, event);
}


void ICEDUSTFormat(string file) {
  cout << "\n file: " << file << " --> convert to icedust format" << endl;
  cout << "reading..." << endl;
  TUserPHITSRooTracker* rt = new TUserPHITSRooTracker(file);
  rt->WriteToICEDUSTFormat(bunch, event);
}

