#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"

void wiggle_pileup_corrected_ROOT()
{

TFile *f = new TFile("run2d-yolo.root", "r");

TH1F *wiggle_full = (TH1F *) gDirectory->Get("MuonCoincidence/TIME_FULL");
TH1F *wiggle_d = (TH1F *) gDirectory->Get("MuonCoincidence/TIME_D");

TH1F *wiggle_pileup_corrected = (TH1F *) wiggle_full->Clone();
wiggle_pileup_corrected ->Add(wiggle_d, -1);

TCanvas *c = new TCanvas("c", "Wiggle_pileup_corrected", 1000, 800);
c->SetLogy();

wiggle_full->Draw();
wiggle_d->Draw("same");
wiggle_d->SetLineColor(2);

wiggle_pileup_corrected->Draw("same");
wiggle_pileup_corrected->SetLineColor(6);


}
