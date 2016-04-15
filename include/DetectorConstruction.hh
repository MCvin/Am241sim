//
//  DetectorConstruction.hh 2013-09-04  Maxime Chauvin
//

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
   ~DetectorConstruction();

    G4VPhysicalVolume* Construct();

  private:
    G4double WorldSize;
};

#endif
