//
//  EventAction.hh 2013-09-04  Maxime Chauvin
//

#ifndef EventAction_h
#define EventAction_h 1

#include "TrackerHit.hh"

#include "G4UserEventAction.hh"
#include "G4Event.hh"
#include "G4ThreeVector.hh"
#include "TFile.h"
#include "TTree.h"
#include "TLeaf.h"
#include <fstream>

class EventAction : public G4UserEventAction
{
  public:
    EventAction();
   ~EventAction();

    void BeginOfEventAction(const G4Event*);
    void EndOfEventAction(const G4Event*);

    void CreateASCIIFile(const G4String name) {asciiFile = new std::ofstream(name);}
    void CreateROOTFile(const G4String name);

  private:
    struct Prim_event{
      int Id;
      float E;
      float Px, Py, Pz;
      float Dx, Dy, Dz;
      float Polx, Poly, Polz;
    };
    struct Det_event{
      int nhits; //Number of hits determines the size of arrays when writing the TTree
      float EdepTot;
      float Edep[1000000];
      float Px[1000000];
      float Py[1000000];
      float Pz[1000000];
      float Dx[1000000];
      float Dy[1000000];
      float Dz[1000000];
      float Polx[1000000];
      float Poly[1000000];
      float Polz[1000000];
      inline void SetAddress(TBranch* branch){
        TObjArray* leaf_array = branch->GetListOfLeaves();
        ((TLeaf*)leaf_array->At(0))->SetAddress(&nhits);
        ((TLeaf*)leaf_array->At(1))->SetAddress(&EdepTot);
        ((TLeaf*)leaf_array->At(2))->SetAddress(Edep);
        ((TLeaf*)leaf_array->At(3))->SetAddress(Px);
        ((TLeaf*)leaf_array->At(4))->SetAddress(Py);
        ((TLeaf*)leaf_array->At(5))->SetAddress(Pz);
        ((TLeaf*)leaf_array->At(3))->SetAddress(Dx);
        ((TLeaf*)leaf_array->At(4))->SetAddress(Dy);
        ((TLeaf*)leaf_array->At(5))->SetAddress(Dz);
        ((TLeaf*)leaf_array->At(3))->SetAddress(Polx);
        ((TLeaf*)leaf_array->At(4))->SetAddress(Poly);
        ((TLeaf*)leaf_array->At(5))->SetAddress(Polz);
      }
    };
    void WriteToASCII(const G4Event*);
    void WriteToROOT(const G4Event*);
    void readHitsCollection(const TrackerHitsCollection*, Det_event*);

    G4int testCollID;
    G4int printModulo;

    Prim_event primEvent;
    Det_event testEvent;

    std::ofstream* asciiFile;
    TFile *rootFile;
    TTree *G4dataTree;
};

#endif
