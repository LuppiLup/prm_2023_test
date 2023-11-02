#include "ReadAmberData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TStyle.h"
#include "TTree.h"

#include <iostream>

AmberData::AmberData(const std::string &DataFile) {
    std::cout << " start reading " << std::endl;
  TFile f(DataFile.c_str());
  if (f.IsZombie()) {
    std::cerr << "Could not open amber datafile " << DataFile << std::endl;
    abort();
  }

  Long64_t s;
  double time;
std::cout << " I am here A " << std::endl;
  TTree *t = (TTree *)f.Get("UserEvent7001");
  std::cout << " I am here B " << std::endl;
  t->SetBranchAddress("spill", &s);
  std::cout << " I am here C " << std::endl;
  t->SetBranchAddress("timeInSpill", &time);
  size_t NEntries = t->GetEntries();
  Long64_t s_last = 10000;
  std::vector<double> times;
  std::cout << " I am here 1" << std::endl;
  for (size_t i = 0; i < NEntries; ++i) {
    t->GetEntry(i);
    if (s_last == 10000) {
      s_last = s;
    } else if (s != s_last) {
      PerSpill p;
      p.SpillNumber = s_last;
      p.timeStamps.swap(times);
      mPerSpillData.push_back(p);
      s_last = s;
    }
    times.push_back(time * (1 + alpha) * 1e09);
  }
  std::cout << " I am here 2 " << std::endl;
  PerSpill p;
  p.SpillNumber = s_last;
  p.timeStamps.swap(times);
  f.Close();
  // for (auto &s : mPerSpillData) {
  //   std::cout << "NAmbertimes for spill: " << s.SpillNumber << " " << s.timeStamps.size()
  //             << std::endl;
  // }
  // std::cout << "Amber trees finished" << std::endl;
}

void AmberData::PlotTimeStamps(Long64_t spill) const {
  TCanvas *c = new TCanvas("", "", 1200, 700);
  TH1D *h = new TH1D("", "; t / ns; timestamps per bin", 1000, 0, 6e09);

  const PerSpill *p = nullptr;
  for (auto &ps : mPerSpillData) {
    if (ps.SpillNumber == spill) {
      p = &ps;
      break;
    }
  }

  for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
    h->Fill(p->timeStamps[j]);
  }
  h->Draw();
  c->SaveAs(Form("spill_%lld.png", spill));
}

void AmberData::PlotClusterTimeDiff(Long64_t spill, const std::vector<double> &VMMTimeStamps) {
  const PerSpill *p = nullptr;
  for (auto &ps : mPerSpillData) {
    if (ps.SpillNumber == spill) {
      p = &ps;
      break;
    }
  }

  TCanvas *c_cal = new TCanvas("", "", 1200, 700);
  gStyle->SetTitleSize(0.04, "xyz");
  gStyle->SetTitleFontSize(0.04);
  gStyle->SetLabelSize(0.04, "xyz");
  std::cout << " I am here " << std::endl;
  //gStyle->SetLegendTextSize(0.04);
  TH1D *h_cal = new TH1D("", "; #Deltat / #mus; entrys per bin", 150, -1.5, 1.5);
  for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
    for (long unsigned int k = 0; k < 1200000; k++) {
      h_cal->Fill((p->timeStamps[j] - VMMTimeStamps[k] + 1375e09 + 1.826e09 + 50e03 - 5880) / 1000);
    }
  }
  h_cal->Draw();
  c_cal->SaveAs(Form("cal_spill_%lld.png", spill));
}

double AmberData::CalibrateAmberTrigger(Long64_t spill, const std::vector<double> &TriggerTimeStamps, double spill_pos) {
    const PerSpill* p = nullptr;
    for (auto& ps : mPerSpillData) {
        if (ps.SpillNumber == spill) {
            p = &ps;
            break;
        }
    }

    TH1D* h1_cal = new TH1D("", "; #Deltat / #mus; entrys per bin", 20e05, -10e06, 10e06);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            h1_cal->Fill((p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos)/1000);   //time differences in us
        }
    }
    double peakpos_0 = h1_cal->GetMaximumBin() * 20e06 / 20e05 - 10e06;;   //highest bin should be at calibration peak
    TCanvas* c1_cal = new TCanvas("", "", 1200, 700);
    h1_cal->Draw();
    c1_cal->SaveAs(Form("trigger_cal_1_spill_%lld.png", spill));
    TH1D* h2_cal = new TH1D("", "; #Deltat / #mus; entrys per bin", 40000, peakpos_0 - 20, peakpos_0 + 20);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            h2_cal->Fill((p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos) / 1000);   //time differences in us
        }
    }
    double peakpos_1 = h2_cal->GetMaximumBin() * 1 - 10000; // accuracy 1 ns
    TCanvas* c2_cal = new TCanvas("", "", 1200, 700);
    h2_cal->Draw();
    c2_cal->SaveAs(Form("trigger_cal_2_spill_%lld.png", spill));
    return peakpos_1;

}

