//
//  PrimaryGeneratorAction.cc 2013-09-04  Maxime Chauvin
//

#include "PrimaryGeneratorAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction()
{	
  m_particleGun  = new G4GeneralParticleSource();
}  

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete m_particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{  
  // particules generation
  m_particleGun->GeneratePrimaryVertex(anEvent);
}
