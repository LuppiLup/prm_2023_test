#include "ReadVmmData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLeaf.h"
#include "TStyle.h"
#include "TTree.h"

#include <TF1.h>
#include <TPaveText.h>

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
  t->Print();
  int det_v;
  TLeaf* det = t->GetLeaf("det");
  TLeaf* plane = t->GetLeaf("plane");
  TLeaf* time = t->GetLeaf("time");
  TLeaf* pos = t->GetLeaf("pos");
  size_t NEntries = t->GetEntries();//1000000;//t->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
      t->GetEntry(i);
      det_v = det->GetValue();
      if (plane->GetValue() == 0 && det_v > 0 && det_v < 5) {
          //if (time->GetValue() > 1.5738e+12 && time->GetValue() < 1.5768e+12) { // cut for time in spill, cut out undebunched start of spill
          //mClusterTimes.push_back(time->GetValue());
          //}
          
          if (det_v == 1) {
              mCluster0Times.push_back(time->GetValue());
              mCluster0Pos.push_back(pos->GetValue() -384);
              mCluster0fec.push_back(1);
          }
          
          if (det_v == 2) {
              mCluster0Times.push_back(time->GetValue());
              mCluster0Pos.push_back(pos->GetValue());
              mCluster0fec.push_back(1);
          }
          if (det_v == 3) {
              mCluster0Times.push_back(time->GetValue());
              mCluster0Pos.push_back(pos->GetValue());
              mCluster0fec.push_back(4);
          }
          if (det_v == 4) {
              mCluster0Times.push_back(time->GetValue());
              mCluster0Pos.push_back(pos->GetValue() - 384);
              mCluster0fec.push_back(4);
          }
          
      }
      if (plane->GetValue() == 1 && det_v > 0 && det_v < 5) {
          if (det_v == 1) {
              mCluster1Times.push_back(time->GetValue());
              mCluster1Pos.push_back(pos->GetValue() - 384);
              mCluster1fec.push_back(5);
          }
          if (det_v == 2) {
              mCluster1Times.push_back(time->GetValue());
              mCluster1Pos.push_back(pos->GetValue() - 384);
              mCluster1fec.push_back(2);
          }
          if (det_v == 3) {
              mCluster1Times.push_back(time->GetValue());
              mCluster1Pos.push_back(pos->GetValue());
              mCluster1fec.push_back(2);
          }
          if (det_v == 4) {
              mCluster1Times.push_back(time->GetValue());
              mCluster1Pos.push_back(pos->GetValue());
              mCluster1fec.push_back(5);
          }
      }
  }
  std::cout << "Time of last cluster is " << time->GetValue() << std::endl;
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

void VMMData::PlotCluster0Pos() const {
    TCanvas* c_pos0 = new TCanvas("9", "10", 1400, 800);
    gStyle->SetOptStat(1);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    //gStyle->SetLegendTextSize(0.06); // does not work in root 5 -> commented out
    c_pos0->SetLeftMargin(0.10);
    c_pos0->SetBottomMargin(0.15);
    TH1D* pos_x = new TH1D("", "; x / cm; hits per bin", 500, -16, 16);
    for (double value : mCluster0Pos) {
        pos_x->Fill(value * 0.04);
    }
    pos_x->Draw();
    c_pos0->SaveAs("detector_cluster0_positions.png");
}

void VMMData::PlotCluster1Pos() const {
    TCanvas* c_pos1 = new TCanvas("11", "12", 1400, 800);
    gStyle->SetOptStat(1);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    //gStyle->SetLegendTextSize(0.06); // does not work in root 5 -> commented out
    c_pos1->SetLeftMargin(0.1);
    c_pos1->SetBottomMargin(0.15);
    TH1D* pos_y = new TH1D("", "; y / cm; hits per bin", 500, -16, 16);
    for (double value : mCluster1Pos) {
        pos_y->Fill(value*0.04);
    }
    pos_y->Draw();
    c_pos1->SaveAs("detector_cluster1_positions.png");
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
  TH1D *h = new TH1D("", "; t / ns; clusters per bin", 1000, mCluster0Times[0],
                     mCluster0Times[mCluster0Times.size() - 1]);
  for (double value : mCluster0Times) {
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
  TCanvas *c = new TCanvas("5", "6", 3000, 2000);
  c->Divide(2, 2);
  TH1D *h1 = new TH1D("", "fec1; t / #mus", 400, -3, -1);
  TH1D* h4 = new TH1D("", "fec4; t / #mus", 400, -3, -1);
  TH1D* h2 = new TH1D("", "fec2; t / #mus", 400, -3, -1);
  TH1D* h5 = new TH1D("", "fec5; t / #mus", 400, -3, -1);
  //for (long unsigned int i = 0; i < mTriggerTimes.size(); i++) {
  for (long unsigned int i = 2500; i < 26000; i++) {
    for (long unsigned int j = 0; j < mCluster0Times.size(); j++) {
        if (mCluster0fec[j] == 1) {
            h1->Fill((mCluster0Times[j] - mTriggerTimes[i]) / 1000);
        }
        if (mCluster0fec[j] == 4) {
            h4->Fill((mCluster0Times[j] - mTriggerTimes[i]) / 1000);
        }
    }
    for (long unsigned int j = 0; j < mCluster1Times.size(); j++) {
        if (mCluster1fec[j] == 2) {
            h2->Fill((mCluster1Times[j] - mTriggerTimes[i]) / 1000);
        }
        if (mCluster1fec[j] == 5) {
            h5->Fill((mCluster1Times[j] - mTriggerTimes[i]) / 1000);
        }
    }
    std::cout << i << std::endl;
  }
  c->cd(1);
  TF1* func_1 = new TF1("func_1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h1->GetBinLowEdge(1), h1->GetBinLowEdge(h1->GetNbinsX() + 1));
  func_1->SetParameters(100, -1.9, 0.1, 10);
  h1->Fit("func_1", "", "", -3, -1);
  TPaveText* fit_fec1 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec1->SetTextAlign(12);
  fit_fec1->AddText(Form("A1 = %.1f #pm %.1f", func_1->GetParameter(0), func_1->GetParError(0)));
  fit_fec1->AddText(Form("#mu1 = (%.4f #pm %.4f) #mus", func_1->GetParameter(1), func_1->GetParError(1)));
  fit_fec1->AddText(Form("#sigma1 = (%.4f #pm %.4f) #mus", func_1->GetParameter(2), func_1->GetParError(2)));
  fit_fec1->AddText(Form(""));
  fit_fec1->AddText(Form("offset = %.1f #pm %.1f", func_1->GetParameter(3), func_1->GetParError(3)));
  h1->Draw();
  fit_fec1->Draw();
  //c->SaveAs("trigger_cal_fec1.png");
  //c->SaveAs("trigger_cal_fec1.root");
  c->cd(2);
  TF1* func_4 = new TF1("func_4", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h4->GetBinLowEdge(1), h4->GetBinLowEdge(h4->GetNbinsX() + 1));
  func_4->SetParameters(100, -1.9, 0.1, 10);
  h4->Fit("func_4", "", "", -3, -1);
  TPaveText* fit_fec4 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec4->SetTextAlign(12);
  fit_fec4->AddText(Form("A4 = %.1f #pm %.1f", func_4->GetParameter(0), func_4->GetParError(0)));
  fit_fec4->AddText(Form("#mu4 = (%.4f #pm %.4f) #mus", func_4->GetParameter(1), func_4->GetParError(1)));
  fit_fec4->AddText(Form("#sigma4 = (%.4f #pm %.4f) #mus", func_4->GetParameter(2), func_4->GetParError(2)));
  fit_fec4->AddText(Form(""));
  fit_fec4->AddText(Form("offset = %.1f #pm %.1f", func_4->GetParameter(3), func_4->GetParError(3)));
  h4->Draw();
  fit_fec4->Draw();
  c->cd(3);
  TF1* func_2 = new TF1("func_2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h2->GetBinLowEdge(1), h2->GetBinLowEdge(h2->GetNbinsX() + 1));
  func_2->SetParameters(100, -1.9, 0.1, 10);
  h2->Fit("func_2", "", "", -3, -1);
  TPaveText* fit_fec2 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec2->SetTextAlign(12);
  fit_fec2->AddText(Form("A2 = %.1f #pm %.1f", func_2->GetParameter(0), func_2->GetParError(0)));
  fit_fec2->AddText(Form("#mu2 = (%.4f #pm %.4f) #mus", func_2->GetParameter(1), func_2->GetParError(1)));
  fit_fec2->AddText(Form("#sigma2 = (%.4f #pm %.4f) #mus", func_2->GetParameter(2), func_2->GetParError(2)));
  fit_fec2->AddText(Form(""));
  fit_fec2->AddText(Form("offset = %.1f #pm %.1f", func_2->GetParameter(3), func_2->GetParError(3)));
  h2->Draw();
  fit_fec2->Draw();
  c->cd(4);
  TF1* func_5 = new TF1("func_5", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h5->GetBinLowEdge(1), h5->GetBinLowEdge(h5->GetNbinsX() + 1));
  func_5->SetParameters(100, -1.9, 0.1, 10);
  h5->Fit("func_5", "", "", -3, -1);
  TPaveText* fit_fec5 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec5->SetTextAlign(12);
  fit_fec5->AddText(Form("A5 = %.1f #pm %.1f", func_5->GetParameter(0), func_5->GetParError(0)));
  fit_fec5->AddText(Form("#mu5 = (%.4f #pm %.4f) #mus", func_5->GetParameter(1), func_5->GetParError(1)));
  fit_fec5->AddText(Form("#sigma5 = (%.4f #pm %.4f) #mus", func_5->GetParameter(2), func_5->GetParError(2)));
  fit_fec5->AddText(Form(""));
  fit_fec5->AddText(Form("offset = %.1f #pm %.1f", func_5->GetParameter(3), func_5->GetParError(3)));
  h5->Draw();
  fit_fec5->Draw();
  c->SaveAs("trigger_cal.png");

}

void VMMData::TimeResolution() const {
  TCanvas *c = new TCanvas("", "", 2000, 1400);
  TH1D *h = new TH1D("", "; #deltat / ns", 300, -300, 300);
  for (long unsigned int i = 1000000; i < 1050000; i++) {
    for (long unsigned int j = 0; j < 3500000 ; j++) {
      if (mCluster0fec[j] == 1 and mCluster1fec[i] == 2) {
        h->Fill(mCluster0Times[j] - mCluster1Times[i] + 1861 - 1896);
      }
      if (mCluster0fec[j] == 1 and mCluster1fec[i] == 5) {
        h->Fill(mCluster0Times[j] - mCluster1Times[i] + 1861 - 1909);
      }
      if (mCluster0fec[j] == 4 and mCluster1fec[i] == 2) {
        h->Fill(mCluster0Times[j] - mCluster1Times[i] + 1934 - 1896);
      }
      if (mCluster0fec[j] == 4 and mCluster1fec[i] == 5) {
        h->Fill(mCluster0Times[j] - mCluster1Times[i] + 1934 - 1909);
      }

      //h->Fill(mCluster0Times[j] - mCluster1Times[i]);
    }
    std::cout << i - 1000000 << std::endl;
  }
  TF1* func = new TF1("func", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h->GetBinLowEdge(1), h->GetBinLowEdge(h->GetNbinsX() + 1));
  func->SetParameters(1000, 1, 30, 500, 40, 30, 100);
  h->Fit("func", "", "", -300, 300);
  TPaveText* fit_param = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_param->SetTextAlign(12);
  fit_param->AddText(Form("A1 = %.1f #pm %.1f", func->GetParameter(0), func->GetParError(0)));
  fit_param->AddText(Form("#mu1 = (%.4f #pm %.4f) #mus", func->GetParameter(1), func->GetParError(1)));
  fit_param->AddText(Form("#sigma1 = (%.4f #pm %.4f) #mus", func->GetParameter(2), func->GetParError(2)));
  fit_param->AddText(Form(""));
  fit_param->AddText(Form("A1 = %.1f #pm %.1f", func->GetParameter(3), func->GetParError(3)));
  fit_param->AddText(Form("#mu1 = (%.4f #pm %.4f) #mus", func->GetParameter(4), func->GetParError(4)));
  fit_param->AddText(Form("#sigma1 = (%.4f #pm %.4f) #mus", func->GetParameter(5), func->GetParError(5)));
  fit_param->AddText(Form(""));
  fit_param->AddText(Form("offset = %.1f #pm %.1f", func->GetParameter(6), func->GetParError(6)));
  h->Draw();
  fit_param->Draw();
  c->SaveAs("TimeRes.png");
}

void VMMData::PlotClusterTrigger2D() const {
  TCanvas *c = new TCanvas("7", "8", 1200, 700);
  c->Divide(2, 2);

  std::vector<double> startEnds;
  TH1D *h = new TH1D("", "; t / ns; clusters per bin", 1000, mCluster0Times[0],
                     mCluster0Times[mCluster0Times.size() - 1]);
  for (double value : mCluster0Times) {
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
  for (double value : mCluster0Times) {
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
