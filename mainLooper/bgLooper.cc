#define mt2tree_cxx
#include "bgLooper.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>

using namespace std;

TH1F *htH = new TH1F("ht", "ht", 100,0,3000);
TH1F *met_ptH = new TH1F("met_pt", "met_pt", 100, 0, 2500);
TH1F *mt2H = new TH1F("mt2", "mt2", 100, 0, 1500);
TH1F *nJet30H = new TH1F("nJet30", "nJet30", 11, 0, 11);

void bgLooper::Loop() {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();

  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;

    // Applying selections
    if((nMuons10==0 && nElectrons10==0 && nPFLep5LowMT == 0 && nPFHad10LowMT == 0) && // lepveto cut                                                                                                           
       (HLT_PFHT900 || HLT_PFJet450 || HLT_PFHT300_PFMET110 || HLT_PFMET120_PFMHT120 || HLT_PFMETNoMu120_PFMHTNoMu120) && // trigs cut                                                                         
       ((nVert > 0) && (nJet30 > 1) && (deltaPhiMin > 0.3) && (diffMetMht/met_pt < 0.5) && (mt2 > 200)
	&& ( (ht > 250 && met_pt > 250) || (ht > 1000 && met_pt > 30) )) && // srbase cut                                                                                                                      
       (Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_globalTightHalo2016Filter
        && Flag_badMuonFilter && Flag_goodVertices && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_badChargedHadronFilter
	&& nJet30FailId == 0 && met_miniaodPt / met_caloPt < 5.0 && nJet200MuFrac50DphiMet == 0)) { // filters cut
        // Fill the histograms
      if(!std::isnan(ht)) htH->Fill(ht, evt_scale1fb);
        if(!std::isnan(met_pt)) met_ptH->Fill(met_pt, evt_scale1fb);
        if(!std::isnan(mt2)) mt2H->Fill(mt2, evt_scale1fb);
        if(!std::isnan(nJet30)) nJet30H->Fill(nJet30, evt_scale1fb);
    }
    
  }
  cout << "Finished current TChain" << endl;
}

void bgLooper::saveHists(const char* fileName) {
  TFile *f = new TFile(fileName, "RECREATE");
  if ( f->IsOpen() ) printf("File opened successfully\n");

  htH->Write("ht");
  met_ptH->Write("met_pt");
  mt2H->Write("mt2");
  nJet30H->Write("nJet30");
  
  f->Write();
  f->Close();
  cout << "Created " << fileName << endl;
}
