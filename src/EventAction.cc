//
//  EventAction.cc 2013-09-04  Maxime Chauvin
//

#include "EventAction.hh"

#include "G4SDManager.hh"

EventAction::EventAction(): testCollID(-1)
{
  printModulo = 10000;
}

EventAction::~EventAction()
{
  G4dataTree->Write();
  asciiFile->close();
  rootFile->Close();
}

void EventAction::BeginOfEventAction(const G4Event* evt)
{
  if (evt->GetEventID()%printModulo == 0){ 
    G4cout << "    ---> Begin of event: " << evt->GetEventID() << "\n" << G4endl;
  }

  G4SDManager * SDman = G4SDManager::GetSDMpointer();
  if (testCollID==-1) testCollID = SDman->GetCollectionID("testSD");
}

void EventAction::EndOfEventAction(const G4Event* evt)
{
  // write to ASCII file
  WriteToASCII(evt);
  // write to ROOT file
  WriteToROOT(evt);
}

void EventAction::CreateROOTFile(const G4String name)
{
  rootFile = new TFile(name,"RECREATE");
  G4dataTree = new TTree( "G4data" , "Source data" );
  G4dataTree->Branch("Events", &primEvent, "Id/I:Energy/F:Px/F:Py/F:Pz/F:Dx/F:Dy/F:Dz/F");
  TBranch* test_branch = G4dataTree->Branch("Detector", &testEvent, "nhits/I:EdepTot/F:Edep[nhits]/F:Px[nhits]/F:Py[nhits]/F:Pz[nhits]/F:Dx[nhits]/F:Dy[nhits]/F:Dz[nhits]/F:Polx[nhits]/F:Poly[nhits]/F:Polz[nhits]/F");
  testEvent.SetAddress(test_branch); //Set the address for each leaf explicitly (IMPORTANT!)
  G4dataTree->SetAutoSave(1000000000); // to avoid getting many trees in the root file
}

void EventAction::WriteToASCII(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary(0);
  const G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex();

  // test detector hits
  TrackerHitsCollection* testHC = 0;
  if (HCE) testHC = (TrackerHitsCollection*)HCE->GetHC(testCollID);
  if (testHC->entries() > 0) {
    // print primary particule to use later as an input
    (*asciiFile).setf(std::ios::fixed);
    (*asciiFile).precision(10);
    (*asciiFile) << (*testHC)[0]->GetKineticEnergy()/keV << " "
                 << (*testHC)[0]->GetPosition().getX() << " " << (*testHC)[0]->GetPosition().getY() << " " << (*testHC)[0]->GetPosition().getZ() << " "
                 << (*testHC)[0]->GetMomentum().getX() << " " << (*testHC)[0]->GetMomentum().getY() << " " << (*testHC)[0]->GetMomentum().getZ() << " "
                 << (*testHC)[0]->GetPolarization().getX() << " " << (*testHC)[0]->GetPolarization().getY() << " " << (*testHC)[0]->GetPolarization().getZ()
                 << G4endl;
  }
}

void EventAction::WriteToROOT(const G4Event* evt)
{
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  G4PrimaryParticle* primary = evt->GetPrimaryVertex()->GetPrimary(0);
  const G4PrimaryVertex* primaryVertex = evt->GetPrimaryVertex();

  // get everything we want to output
    // primary particule
    //primEvent.Id = evt->GetEventID();
    //primEvent.E  = primary->GetTotalEnergy()/keV;
    //primEvent.Px = primaryVertex->GetX0()/mm;
    //primEvent.Py = primaryVertex->GetY0()/mm;
    //primEvent.Pz = primaryVertex->GetZ0()/mm;
    //primEvent.Dx = primary->GetPx();
    //primEvent.Dy = primary->GetPy();
    //primEvent.Dz = primary->GetPz();
    // test detector hits
    TrackerHitsCollection* testHC = 0;
    if (HCE) testHC = (TrackerHitsCollection*)HCE->GetHC(testCollID);
    if (testHC) readHitsCollection(testHC, &testEvent);
    if (testHC->entries() > 0) {
      primEvent.Id = evt->GetEventID();
      primEvent.E  = (*testHC)[0]->GetKineticEnergy()/keV;
      primEvent.Px = (*testHC)[0]->GetPosition().getX();
      primEvent.Py = (*testHC)[0]->GetPosition().getY();
      primEvent.Pz = (*testHC)[0]->GetPosition().getZ();
      primEvent.Dx = (*testHC)[0]->GetMomentum().getX();
      primEvent.Dy = (*testHC)[0]->GetMomentum().getY();
      primEvent.Dz = (*testHC)[0]->GetMomentum().getZ();
      primEvent.Polx = (*testHC)[0]->GetPolarization().getX();
      primEvent.Poly = (*testHC)[0]->GetPolarization().getY();
      primEvent.Polz = (*testHC)[0]->GetPolarization().getZ();

      G4dataTree->Fill();
    }
}

void EventAction::readHitsCollection(const TrackerHitsCollection* HC, Det_event* detEvt)
{
  G4double Edep;
  G4int nhits = 0;

  detEvt->EdepTot = 0;
  for (G4int i = 0; i < HC->entries(); i++){
    Edep = (*HC)[i]->GetEdep()/keV;
    detEvt->EdepTot += Edep;
    detEvt->Edep[nhits] = Edep;
    detEvt->Px[nhits] = (*HC)[i]->GetPostPosition().x()/mm;
    detEvt->Py[nhits] = (*HC)[i]->GetPostPosition().y()/mm;
    detEvt->Pz[nhits] = (*HC)[i]->GetPostPosition().z()/mm;
    detEvt->Dx[nhits] = (*HC)[i]->GetMomentum().x()/mm;
    detEvt->Dy[nhits] = (*HC)[i]->GetMomentum().y()/mm;
    detEvt->Dz[nhits] = (*HC)[i]->GetMomentum().z()/mm;
    detEvt->Polx[nhits] = (*HC)[i]->GetPolarization().x()/mm;
    detEvt->Poly[nhits] = (*HC)[i]->GetPolarization().y()/mm;
    detEvt->Polz[nhits] = (*HC)[i]->GetPolarization().z()/mm;
    nhits++;
  }
  detEvt->nhits = nhits;

  return ;
}
