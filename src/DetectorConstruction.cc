//
//  DetectorConstruction.cc 2013-09-04  Maxime Chauvin
//

#include "DetectorConstruction.hh"
#include "TrackerSD.hh"

#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4PVPlacement.hh"
#include "G4NistManager.hh"
#include "G4Material.hh"
#include "G4VisAttributes.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction(){}

DetectorConstruction::~DetectorConstruction(){}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4NistManager * nist_manager = G4NistManager::Instance();
  G4Material * air = nist_manager->FindOrBuildMaterial("G4_AIR");
  G4Material * Pb = nist_manager->FindOrBuildMaterial("G4_Pb");
  G4Material * PE = nist_manager->FindOrBuildMaterial("G4_POLYETHYLENE");
  G4Material * Al = nist_manager->FindOrBuildMaterial("G4_Al");
  // Steel as non-NIST material
  G4Element * elFe = nist_manager->FindOrBuildElement("Fe");
  G4Element * elNi = nist_manager->FindOrBuildElement("Ni");
  G4Element * elCr = nist_manager->FindOrBuildElement("Cr");
  G4Material * steel = new G4Material("StainlessSteel", 7.80 * g/cm3, 3 /* components */);
  steel -> AddElement(elFe, 70 * perCent);
  steel -> AddElement(elCr, 18 * perCent);
  steel -> AddElement(elNi, 12 * perCent);

  G4RotationMatrix* xRot90 = new G4RotationMatrix();
  xRot90->rotateX(90.*deg);
  G4RotationMatrix* xRot270 = new G4RotationMatrix();
  xRot270->rotateX(270.*deg);
  G4RotationMatrix* zRot90 = new G4RotationMatrix();
  zRot90->rotateZ(90.*deg);
  G4RotationMatrix* zRot270 = new G4RotationMatrix();
  zRot270->rotateZ(270.*deg);
  G4RotationMatrix* Rotz90x90 = new G4RotationMatrix();
  Rotz90x90->rotateZ(90.*deg);
  Rotz90x90->rotateX(-90.*deg);

  // WORLD
  WorldSize  = 300.0*cm;
  G4Box* s_World = new G4Box("World", WorldSize/2, WorldSize/2, WorldSize/2);
  G4LogicalVolume* l_World = new G4LogicalVolume(s_World, air, "World");
  G4VPhysicalVolume* p_World = new G4PVPlacement(0,	G4ThreeVector(), "World", l_World, NULL, false, 0);

  // Source casing mother volume
  const G4String SourceName = "SourceSetup";
  G4Box* s_Source = new G4Box(SourceName, 2.45*cm, 2.45*cm, 2.45*cm);
  G4LogicalVolume* l_Source = new G4LogicalVolume(s_Source, air, SourceName);
  G4VPhysicalVolume* p_Source = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,85.0*cm),  // for the source with no collimators 2016
  //G4VPhysicalVolume* p_Source = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,94.95*cm),
  //G4VPhysicalVolume* p_Source = new G4PVPlacement(0, G4ThreeVector(0.0*cm,-4.95*cm,85.0*cm),  // for the scatter piece 2012
  //G4VPhysicalVolume* p_Source = new G4PVPlacement(zRot270, G4ThreeVector(4.95*cm,0.0*cm,85.0*cm),  // for the scatter piece 2016
                                                  SourceName, l_Source, p_World, false, 0);

    // source lead case, y direction for the scatter piece
    const G4String SrcPbCaseName = "SrcPbCase";
    G4Box* s1_SrcPbCase = new G4Box(SrcPbCaseName, 2.45*cm, 2.45*cm, 2.45*cm);
    G4Tubs* s2_SrcPbCase = new G4Tubs(SrcPbCaseName, 0.*cm, 1.3*cm, 0.5*cm, 0.*deg, 360.*deg);
    G4Tubs* s3_SrcPbCase = new G4Tubs(SrcPbCaseName, 0.*cm, 0.25*cm, 1.*cm, 0.*deg, 360.*deg);
    G4SubtractionSolid* s12_SrcPbCase = new G4SubtractionSolid(SrcPbCaseName, s1_SrcPbCase, s2_SrcPbCase, 0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
    G4SubtractionSolid* s_SrcPbCase = new G4SubtractionSolid(SrcPbCaseName, s12_SrcPbCase, s3_SrcPbCase, 0, G4ThreeVector(0.0*cm,0.0*cm,-1.5*cm));
    G4LogicalVolume* l_SrcPbCase = new G4LogicalVolume(s_SrcPbCase, Pb, SrcPbCaseName);
    G4VPhysicalVolume* p_SrcPbCase = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),  // for the source with no collimators 2016
    // G4VPhysicalVolume* p_SrcPbCase = new G4PVPlacement(xRot270, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),  // for the scatter piece 2016
                                                       l_SrcPbCase, SrcPbCaseName, l_Source, false, 0);

    //// source lead case, z direction
    //const G4String SrcPbCaseName = "SrcPbCase";
    //G4Box* s1_SrcPbCase = new G4Box(SrcPbCaseName, 2.45*cm, 2.45*cm, 2.45*cm);
    //G4Tubs* s2_SrcPbCase = new G4Tubs(SrcPbCaseName, 0.*cm, 1.3*cm, 0.5*cm, 0.*deg, 360.*deg);
    //G4Tubs* s3_SrcPbCase = new G4Tubs(SrcPbCaseName, 0.*cm, 0.25*cm, 1.*cm, 0.*deg, 360.*deg);
    //G4SubtractionSolid* s12_SrcPbCase = new G4SubtractionSolid(SrcPbCaseName, s1_SrcPbCase, s2_SrcPbCase, 0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
    //G4SubtractionSolid* s_SrcPbCase = new G4SubtractionSolid(SrcPbCaseName, s12_SrcPbCase, s3_SrcPbCase, 0, G4ThreeVector(0.0*cm,0.0*cm,-1.5*cm));
    //G4LogicalVolume* l_SrcPbCase = new G4LogicalVolume(s_SrcPbCase, Pb, SrcPbCaseName);
    //G4VPhysicalVolume* p_SrcPbCase = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),
                                                       //l_SrcPbCase, SrcPbCaseName, l_Source, false, 0);

    //// source stainless steel casing
    //const G4String SrcSteelCaseName = "SrcSteelCase";
    //G4Tubs* s1_SrcSteelCase = new G4Tubs(SrcSteelCaseName, 0.*cm, 0.54*cm, 0.3*cm, 0.*deg, 360.*deg);
    //G4Tubs* s2_SrcSteelCase = new G4Tubs(SrcSteelCaseName, 0.*cm, 0.375*cm, 0.125*cm, 0.*deg, 360.*deg);
    //G4SubtractionSolid* s_SrcSteelCase = new G4SubtractionSolid(SrcSteelCaseName, s1_SrcSteelCase, s2_SrcSteelCase, 0, G4ThreeVector(0.0*cm,0.0*cm,-0.155*cm));
    //G4LogicalVolume* l_SrcSteelCase = new G4LogicalVolume(s_SrcSteelCase, steel, SrcSteelCaseName);
    //G4VPhysicalVolume* p_SrcSteelCase = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),
                                                          //l_SrcSteelCase, SrcSteelCaseName, l_Source, false, 0);

  // // Scatter piece mother volume
  // const G4String ScatterName = "ScatterPieceSetup";
  // G4Box* s_Scatter = new G4Box(ScatterName, 2.5*cm, 2.5*cm, 2.5*cm);
  // G4LogicalVolume* l_Scatter = new G4LogicalVolume(s_Scatter, air, ScatterName);
  // G4VPhysicalVolume* p_Scatter = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,85.0*cm),
  //                                                  ScatterName, l_Scatter, p_World, false, 0);
  //   //// lead block 2012
  //   //const G4String PbBlockName = "PbBlock";
  //   //G4Box* s1_PbBlock = new G4Box(PbBlockName, 2.5*cm, 2.5*cm, 2.5*cm);
  //   //G4Tubs* s2_PbBlock = new G4Tubs(PbBlockName, 0.*cm, 1.3*cm, 1.3251*cm, 0.*deg, 360.*deg);
  //   //G4Tubs* s3_PbBlock = new G4Tubs(PbBlockName, 0.*cm, 0.35*cm, 1.*cm, 0.*deg, 360.*deg);
  //   //G4SubtractionSolid* s12_PbBlock = new G4SubtractionSolid(PbBlockName, s1_PbBlock, s2_PbBlock, 0, G4ThreeVector(0.0*cm,0.0*cm,-1.125*cm));
  //   //G4SubtractionSolid* s_PbBlock = new G4SubtractionSolid(PbBlockName, s12_PbBlock, s3_PbBlock, xRot90, G4ThreeVector(0.0*cm,-1.875*cm,-0.15*cm));
  //   //G4LogicalVolume* l_PbBlock = new G4LogicalVolume(s_PbBlock, Pb, PbBlockName);
  //   //G4VPhysicalVolume* p_PbBlock = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),
  //   //                                                 l_PbBlock, PbBlockName, l_Scatter, false, 0);
  //   //// scatter piece 2012
  //   //const G4String ScatterPieceName = "ScatterPiece2012";
  //   //G4Tubs* s1_ScatterPiece = new G4Tubs(ScatterPieceName, 0.*cm, 1.3*cm, 1.*cm, 0.*deg, 360.*deg);
  //   //G4Tubs* s2_ScatterPiece = new G4Tubs(ScatterPieceName, 0.25*cm, 1.3*cm, 0.85*cm, 0.*deg, 360.*deg);
  //   //G4SubtractionSolid* s_ScatterPiece = new G4SubtractionSolid(ScatterPieceName, s1_ScatterPiece, s2_ScatterPiece, 0, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm));
  //   //G4LogicalVolume* l_ScatterPiece = new G4LogicalVolume(s_ScatterPiece, PE, ScatterPieceName);
  //   //G4VPhysicalVolume* p_ScatterPiece = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-0.8*cm),
  //   //                                                      l_ScatterPiece, ScatterPieceName, l_Scatter, false, 0);
  //   //// scatter piece 2013, 2015
  //   //const G4String ScatterPieceName = "ScatterPiece2013";
  //   //G4Box* s_ScatterPiece = new G4Box(ScatterPieceName, 1.16*cm, 0.55*cm, 0.45*cm);
  //   //G4LogicalVolume* l_ScatterPiece = new G4LogicalVolume(s_ScatterPiece, PE, ScatterPieceName);
  //   ////G4VPhysicalVolume* p_ScatterPiece = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,-0.25*cm),				// PoGO 2013
  //   //G4VPhysicalVolume* p_ScatterPiece = new G4PVPlacement(zRot90, G4ThreeVector(0.0*cm,0.0*cm,-0.25*cm),			// Håkan tests 2015
  //   //                                                      l_ScatterPiece, ScatterPieceName, l_Scatter, false, 0);
  //
  //   // lead block 2016
  //   const G4String PbBlockName = "PbBlock2016";
  //   G4Box* s1_PbBlock = new G4Box(PbBlockName, 2.5*cm, 2.5*cm, 2.5*cm);
  //   G4Tubs* s2_PbBlock = new G4Tubs(PbBlockName, 0.*cm, 1.3*cm, 1.5*cm, 0.*deg, 360.*deg);
  //   G4Tubs* s3_PbBlock = new G4Tubs(PbBlockName, 0.*cm, 0.5*cm, 1.*cm, 0.*deg, 360.*deg);
  //   G4SubtractionSolid* s12_PbBlock = new G4SubtractionSolid(PbBlockName, s1_PbBlock, s2_PbBlock, 0, G4ThreeVector(0.0*cm,0.0*cm,-1.0*cm));
  //   G4SubtractionSolid* s_PbBlock = new G4SubtractionSolid(PbBlockName, s12_PbBlock, s3_PbBlock, xRot90, G4ThreeVector(0.0*cm,-2.0*cm,0.0*cm));
  //   G4LogicalVolume* l_PbBlock = new G4LogicalVolume(s_PbBlock, Pb, PbBlockName);
  //   G4VPhysicalVolume* p_PbBlock = new G4PVPlacement(Rotz90x90, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),
  //                                                    l_PbBlock, PbBlockName, l_Scatter, false, 0);
  //   // scatter piece 2016
  //   const G4String ScatterPieceName = "ScatterPiece2016";
  //   G4Tubs* s_ScatterPiece = new G4Tubs(ScatterPieceName, 0.*cm, 1.3*cm, 0.5*cm, 0.*deg, 360.*deg);
  //   G4LogicalVolume* l_ScatterPiece = new G4LogicalVolume(s_ScatterPiece, PE, ScatterPieceName);
  //   G4VPhysicalVolume* p_ScatterPiece = new G4PVPlacement(Rotz90x90, G4ThreeVector(0.0*cm,0.0*cm,0.0*cm),
  //                                                         l_ScatterPiece, ScatterPieceName, l_Scatter, false, 0);


  //// lead collimator
  //const G4String SrcShieldName = "SrcShield";
  ////G4Tubs* s_SrcShield = new G4Tubs(SrcShieldName, 0.1*cm, 2.*cm, 0.625*cm, 0.*deg, 360.*deg); // 1 collimator
  //G4Tubs* s_SrcShield = new G4Tubs(SrcShieldName, 0.1*cm, 2.*cm, 3.75*cm, 0.*deg, 360.*deg);  // 6 collimators
  //G4LogicalVolume* l_SrcShield = new G4LogicalVolume(s_SrcShield, Pb, SrcShieldName);
  ////G4VPhysicalVolume* p_SrcShield = new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,85.625*cm),   // 1 collimator
  //G4VPhysicalVolume* p_SrcShield = new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,88.750*cm),   // 6 collimators
  ////G4VPhysicalVolume* p_SrcShield = new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,93.25*cm),   // 6 collimators 2014/09/01
  ////G4VPhysicalVolume* p_SrcShield = new G4PVPlacement(0, G4ThreeVector(0.84*cm,0.*cm,93.25*cm),   // 6 collimators 2014/09/01 tilt=0.6degrees
                                                     //SrcShieldName, l_SrcShield, p_World, false, 0);

  // Rotating frame 2016
  const G4String RotatingFrameName = "RotatingFrame";
  G4Tubs* s_Rtube = new G4Tubs(RotatingFrameName, 1.95*cm, 2.25*cm, 3.0*cm, 0.*deg, 360.*deg);
  G4Tubs* s_Rdisk = new G4Tubs(RotatingFrameName, 0.5*cm, 2.25*cm, 0.15*cm, 0.*deg, 360.*deg);
  G4UnionSolid* s_Rlead = new G4UnionSolid(RotatingFrameName, s_Rtube, s_Rdisk, 0, G4ThreeVector(0.0*cm,0.0*cm,-3.85*cm));
  G4LogicalVolume* l_Rlead = new G4LogicalVolume(s_Rlead, Pb, RotatingFrameName);
  G4VPhysicalVolume* p_Rlead = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,78.5*cm), RotatingFrameName, l_Rlead, p_World, false, 0);

  G4Tubs* s1_Rplate = new G4Tubs(RotatingFrameName, 0.5*cm, 6.0*cm, 0.25*cm, 0.*deg, 360.*deg);
  G4Tubs* s2_Rplate = new G4Tubs(RotatingFrameName, 0.5*cm, 2.25*cm, 0.15*cm, 0.*deg, 360.*deg);
  G4SubtractionSolid* s_Rplate = new G4SubtractionSolid(RotatingFrameName, s1_Rplate, s2_Rplate, 0, G4ThreeVector(0.0*cm,0.0*cm,0.1*cm));
  G4LogicalVolume* l_Rplate = new G4LogicalVolume(s_Rplate, Al, RotatingFrameName);
  G4VPhysicalVolume* p_Rplate = new G4PVPlacement(0, G4ThreeVector(0.0*cm,0.0*cm,74.55*cm), RotatingFrameName, l_Rplate, p_World, false, 0);


  // perfect detector
  G4Tubs* s_det = new G4Tubs("Detector", 0.*cm, 20.*cm, 5.*cm, 0.*deg, 360.*deg);
  G4LogicalVolume* l_det = new G4LogicalVolume(s_det, Pb, "Detector");
  //G4VPhysicalVolume* p_det = new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,77.5*cm), "Detector", l_det, p_World, false, 0);
  G4VPhysicalVolume* p_det = new G4PVPlacement(0, G4ThreeVector(0.*cm,0.*cm,69.3*cm), "Detector", l_det, p_World, false, 0);  //position with rotating frame 2016

  // sensitive detectors
  G4SDManager* SDman = G4SDManager::GetSDMpointer();

      TrackerSD* testSD = new TrackerSD("testSD");
      SDman->AddNewDetector(testSD);
      l_det->SetSensitiveDetector(testSD);

  // visualization
  G4VisAttributes* colourBlue = new G4VisAttributes(G4Colour(0.,0.,1.));
  //l_SrcPbCase->SetVisAttributes(colourBlue);
  //l_SrcColl->SetVisAttributes(colourBlue);
  //l_ScatterPiece->SetVisAttributes(colourBlue);

  return p_World;
}
