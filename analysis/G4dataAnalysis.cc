#include <iostream>
#include <fstream>
#include <TFile.h>
#include <TApplication.h>
#include <TCanvas.h>
#include <TPaveStats.h>
#include <TH1.h>
#include <TH2.h>
#include <TLegend.h>
#include <TMath.h>
#include <TRandom3.h>

#include "G4data.h"


TH1F *S1;

void createHistos()
{
  // nhits spectrum
  S1 = new TH1F("Snhits", "G4 spectrum, all hit events", 1200, 0, 120);
  S1->GetXaxis()->SetTitle("Energy (keV)");
  S1->GetYaxis()->SetTitle("Events/bin/s");
  S1->GetXaxis()->CenterTitle();
  S1->GetYaxis()->CenterTitle();
}
void displayHistos()
{
  // print nHits
  TCanvas* cS1 = new TCanvas("G4spectrum1hits","G4spectrum1hits");
  cS1->SetGrid();
  cS1->SetLogy();
  S1->Scale(1./304.);
  S1->Draw("");
}

void EnergyConversion(G4data* G4)
{
  TRandom3 random(0); // "TRandom3 is the best random engine in ROOT. 0 means seed from system clock."

  for (int x = 0; x < G4->fast->nhits; x++) {
    float E = G4->fast->Edep[x];

    // correction for scintillator non-linearity response
    E = E * (1.001 - 0.486 * exp(-0.0902 * E));
    // z position dependance
    E =  (1.0 - (G4->fast->Pz[x] - 150) * 0.0008) * E; // 150 is the mean photoabsorption position

    // -> sum energies
    if (x == 0) G4->fast->Edep[0] = E;
    else G4->fast->Edep[0] += E;
  }
  G4->fast->nhits = 1;

  // -> energy resolution
  float E = G4->fast->Edep[0];
  // correction for PMT photon-electron production (low energy cutoff)
  int nPhE = random.Poisson(E);// * 0.5);
  // correction for PMT gain fluctuation
  E = random.Gaus(nPhE, 0.35*sqrt(nPhE));// / 0.5;
  G4->fast->Edep[0] = E;

}

void fill1hits(G4data* G4)
{
    float E = G4->fast->Edep[0];
    S1->Fill(E);
}

int main(int argc, char* argv[])
{
  TApplication theApp("App", &argc, argv);

  // open the Tree and create objects to contain the branches
  TFile* f = new TFile("G4data.root","r");
  TTree* tree = (TTree*) f->Get("G4data");
  G4data* G4 = new G4data; //Declare the object
  G4->SetAddress(tree);    //Set every address explicitly (IMPORTANT!)

  // create histograms
  createHistos();

  // loop over all entries in the tree
  for (long i = 0; i < tree->GetEntries(); i++){
    tree->GetEntry(i);

    if (G4->fast->EdepTot > 0) {

      // energy conversion: keV to ADC
      EnergyConversion(G4);

      // 1hits
      fill1hits(G4);

    }
  }

  // display histograms
  displayHistos();

  theApp.Run();
  return 0;
}
