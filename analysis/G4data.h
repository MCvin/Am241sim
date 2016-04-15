#ifndef G4data_h
#define G4data_h
#include <TTree.h>
#include <TLeaf.h>

struct Prim_event{
  int Id;
  float E;
  float Px, Py, Pz;
  float Dx, Dy, Dz;
  inline void SetAddress(TBranch* branch){
    branch->SetAddress(this);
  }
};
struct Det_event{
  int nhits;
  float EdepTot;
  float Edep[1000];
  float Px[1000];
  float Py[1000];
  float Pz[1000];
  inline void SetAddress(TBranch* branch){
    branch->SetAddress(this);
    TObjArray* leaf_array = branch->GetListOfLeaves();
    ((TLeaf*)leaf_array->At(0))->SetAddress(&nhits);
    ((TLeaf*)leaf_array->At(1))->SetAddress(&EdepTot);
    ((TLeaf*)leaf_array->At(2))->SetAddress(Edep);
    ((TLeaf*)leaf_array->At(3))->SetAddress(Px);
    ((TLeaf*)leaf_array->At(4))->SetAddress(Py);
    ((TLeaf*)leaf_array->At(5))->SetAddress(Pz);
  }
};
struct G4data{
  Prim_event* primary;
  Det_event* fast;
  G4data(){
  primary = new Prim_event;
  fast = new Det_event;
  }
  inline void SetAddress(TTree* tree){
    primary->SetAddress(tree->GetBranch("primary"));
    fast->SetAddress(tree->GetBranch("fast"));
  }
};

#endif
