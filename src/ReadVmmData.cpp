#include "ReadVmmData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLeaf.h"
#include "TStyle.h"
#include "TTree.h"

#include <iostream>

#include "DataStructures.h"

VMMData::VMMData(const std::string &DataFile) {
  TFile f(DataFile.c_str());
  if (f.IsZombie()) {
    std::cerr << "Could not open vmm datafile " << DataFile << std::endl;
    abort();
  }
  // first the clusters...
  TTree *t = (TTree *)f.Get("clusters_plane");
  if (!t) {
    std::cerr << "I could not find cluster plane tree in file!" << std::endl;
    abort();
  }

  TLeaf *det = t->GetLeaf("det");
  TLeaf *plane = t->GetLeaf("plane");
  TLeaf *time = t->GetLeaf("time");
  size_t NEntries = t->GetEntries();//1000000;//t->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
    t->GetEntry(i);
    int det_v = det->GetValue();
    if (plane->GetValue() == 0 && det_v > 0 && det_v < 5) {
      //if (time->GetValue() > 1.5738e+12 && time->GetValue() < 1.5768e+12) { // cut for time in spill, cut out undebunched start of spill
        mClusterTimes.push_back(time->GetValue());
      //}
    }
  }
  std::cout << "Time of last cluseṛ is " << time->GetValue() << std::endl;
  //   std::cout << "NClusterTimes: " << mClusterTimes.size() << std::endl;

  // now the physics triggers
  TTree *t2 = (TTree *)f.Get("hits");
  if (!t2) {
    std::cerr << "I could not find hits tree in file!" << std::endl;
    abort();
  }

  TLeaf *ch = t2->GetLeaf("ch");
  det = t2->GetLeaf("det");
  time = t2->GetLeaf("time");
  NEntries = t2->GetEntries();//8000000;//t2->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
    t2->GetEntry(i);
    if (det->GetValue() == 0 && ch->GetValue() == 63) {
      //if (time->GetValue() > 1.5738e+12 && time->GetValue() < 1.5768e+12) { // cut for time in spill, cut out undebunched start of spill
        mTriggerTimes.push_back(time->GetValue());
      //}
    }
  }
     std::cout << "NTriggerTimes: " << mTriggerTimes.size() << std::endl;
}

void VMMData::PlotClusterTimes() const {
  TCanvas *c = new TCanvas("1", "2", 1200, 700); // in root 5 TCanvas names are mandatory, otherwise , root will complain that a canvas with the same name is created later
  gStyle->SetOptStat(1);
  gStyle->SetTitleSize(0.06, "xyz");
  gStyle->SetTitleFontSize(0.06);
  gStyle->SetLabelSize(0.06, "xyz");
  //gStyle->SetLegendTextSize(0.06); // does not work in root 5 -> commented out
  c->SetLeftMargin(0.20);
  c->SetBottomMargin(0.15);
  TH1D *h = new TH1D("", "; t / ns; clusters per bin", 1000, mClusterTimes[0],
                     mClusterTimes[mClusterTimes.size() - 1]);
  for (double value : mClusterTimes) {
    h->Fill(value);
  }
  h->Draw();
  c->SaveAs("detector_cluster0_times.png");
}

void VMMData::PlotTriggerTimes() const {
  TCanvas *c = new TCanvas("3", "4", 1200, 700);
  TH1D *h = new TH1D("", "; t / ns; trigger per bin", 1000, mTriggerTimes[0],
                     mTriggerTimes[mTriggerTimes.size() - 1]);
  for (double value : mTriggerTimes) {
    h->Fill(value);
  }
  h->Draw();
  c->SaveAs("trigger_times.png");
}

void VMMData::PlotClusterTriggerDiff() const {
  TCanvas *c = new TCanvas("5", "6", 1200, 700);
  TH1D *h = new TH1D("", "; t / #mus", 2000, -3, -1);
  for (long unsigned int i = 0; i < mTriggerTimes.size(); i++) {
    for (long unsigned int j = 0; j < mClusterTimes.size(); j++) {
      h->Fill((mClusterTimes[j] - mTriggerTimes[i]) / 1000);
    }
  }
  h->Draw();
  c->SaveAs("trigger_cal.root");
  c->SaveAs("trigger_cal.png");
}

void VMMData::PlotClusterTrigger2D() const {
  TCanvas *c = new TCanvas("7", "8", 1200, 700);
  c->Divide(2, 2);

  std::vector<double> startEnds;
  TH1D *h = new TH1D("", "; t / ns; clusters per bin", 1000, mClusterTimes[0],
                     mClusterTimes[mClusterTimes.size() - 1]);
  for (double value : mClusterTimes) {
    h->Fill(value);
  }
  double max = h->GetMaximum();
  bool Larger = true;
  for (int i = 1; i <= h->GetNbinsX(); ++i) {
    if (Larger && h->GetBinContent(i) > max / 2) {
      Larger = false;
      startEnds.push_back(h->GetBinLowEdge(i));
    }
    if (!Larger && h->GetBinContent(i) < max / 2) {
      Larger = true;
      startEnds.push_back(h->GetBinCenter(i) + h->GetBinWidth(i));
    }
  }

  //   for (auto &d : startEnds) {
  //     std::cout << d << std::endl;
  //   }
  if (startEnds.size() / 2 != startEnds.size() / 2.) {
    std::cout << "Adding end ... why?" << std::endl;
    startEnds.push_back(h->GetBinCenter(h->GetNbinsX()) + h->GetBinWidth(h->GetNbinsX()));
  }

  TH1D *h1 = new TH1D("", "cluster time, moved and only spill", 1000, 0,
                      startEnds[startEnds.size() - 1] - startEnds[0]);
  bool fill = false;
  auto it = startEnds.cbegin();
  for (double value : mClusterTimes) {
    // assume sorted...
    if (!fill && value > *it) {
      fill = true;
      h1->Fill(value - startEnds[0]);
      ++it;
    }
    if (fill) {
      if (value > *it) {
        fill = false;
        ++it;
      } else {
        h1->Fill(value - startEnds[0]);
      }
    }
  }

  TH1D *h2 = new TH1D("", "trigger time", 1000, 0,
                      mTriggerTimes[mTriggerTimes.size() - 1] - mTriggerTimes[0]);
  for (double value : mTriggerTimes) {
    h2->Fill(value - mTriggerTimes[0]);
  }
  TH1D *diff = (TH1D *)h1->Clone();
  diff->SetTitle("diff");
  diff->Add(h2, -1);

  TH1D *ratio = (TH1D *)h1->Clone();
  ratio->SetTitle("ratio");
  ratio->Divide(h2);

  c->cd(1);
  h1->Draw();

  c->cd(2);
  h2->Draw();

  c->cd(3);
  diff->Draw();

  c->cd(4);
  ratio->Draw();

  c->SaveAs("ClusterTrigger2D.png");
}
