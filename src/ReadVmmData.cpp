#include "ReadVmmData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLeaf.h"
#include "TStyle.h"
#include "TTree.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

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
  TTree *t = (TTree *)f.Get("events");
  if (!t) {
    std::cerr << "I could not find cluster plane tree in file!" << std::endl;
    abort();
  }
  t->Print();
  int det_v;
  int len_cluster_plane;
  int len_hits;
  TLeaf* det = t->GetLeaf("clusters_plane.det");
  TLeaf* plane = t->GetLeaf("clusters_plane.plane");
  TLeaf* time = t->GetLeaf("clusters_plane.time");
  TLeaf* pos = t->GetLeaf("clusters_plane.pos");
  size_t NEntries = t->GetEntries();//1000000;//t->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
      t->GetEntry(i);
      len_cluster_plane = time->GetLen();
      for (int j = 0; j < len_cluster_plane; j++) {
          det_v = det->GetValue(j);
          //det_v = det->GetValue(j);
          if (plane->GetValue(j) == 0 && det_v > 0 && det_v < 5) {
              if (det_v == 1) {
                  mCluster0Times.push_back(time->GetValue(j));
                  mCluster0Pos.push_back(pos->GetValue(j) - 384);
                  mCluster0fec.push_back(1);
              }

              if (det_v == 2) {
                  mCluster0Times.push_back(time->GetValue(j));
                  mCluster0Pos.push_back(pos->GetValue(j));
                  mCluster0fec.push_back(1);
              }
              if (det_v == 3) {
                  mCluster0Times.push_back(time->GetValue(j));
                  //mCluster0Pos.push_back(pos->GetValue(j));      //old files
                  mCluster0Pos.push_back(383 - pos->GetValue(j));
                  mCluster0fec.push_back(4);
              }
              if (det_v == 4) {
                  mCluster0Times.push_back(time->GetValue(j));
                  //mCluster0Pos.push_back(pos->GetValue(j) - 384);      //old files
                  mCluster0Pos.push_back(pos->GetValue(j) * (-1) - 1);
                  mCluster0fec.push_back(4);
              }

          }
          if (plane->GetValue(j) == 1 && det_v > 0 && det_v < 5) {
              if (det_v == 1) {
                  mCluster1Times.push_back(time->GetValue(j));
                  //mCluster1Pos.push_back(pos->GetValue(j) - 384);      //old files
                  mCluster1Pos.push_back(pos->GetValue(j) * (-1) - 1);
                  mCluster1fec.push_back(5);
              }
              if (det_v == 2) {
                  mCluster1Times.push_back(time->GetValue(j));
                  mCluster1Pos.push_back(pos->GetValue(j) - 384);
                  mCluster1fec.push_back(2);
              }
              if (det_v == 3) {
                  mCluster1Times.push_back(time->GetValue(j));
                  mCluster1Pos.push_back(pos->GetValue(j));
                  mCluster1fec.push_back(2);
              }
              if (det_v == 4) {
                  mCluster1Times.push_back(time->GetValue(j));
                  //mCluster1Pos.push_back(pos->GetValue(j));      //old files
                  mCluster1Pos.push_back(383 - pos->GetValue(j));
                  mCluster1fec.push_back(5);
              }
          }
      }
  }
  //std::cout << "Time of last cluster is " << time->GetValue() << std::endl;
  //std::cout << "NClusterTimes: " << mClusterTimes.size() << std::endl;

  // now the physics triggers
  TTree *t2 = (TTree *)f.Get("events");
  if (!t2) {
    std::cerr << "I could not find hits tree in file!" << std::endl;
    abort();
  }

  TLeaf* vmm = t2->GetLeaf("hits.vmm");
  TLeaf* fec = t2->GetLeaf("hits.fec");
  TLeaf *ch = t2->GetLeaf("hits.ch");
  det = t2->GetLeaf("hits.det");
  time = t2->GetLeaf("hits.time");
  vmm = t2->GetLeaf("hits.vmm");
  fec = t2->GetLeaf("hits.fec");
  NEntries = t2->GetEntries();//8000000;//t2->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
    t2->GetEntry(i);
    len_hits = t2->GetLeaf("hits.time")->GetLen();
    for (int j = 0; j < len_hits; j++) {
        //if (det->GetValue() == 0) {
        //    std::cout << "det 4 -> fec/VMM: " << fec->GetValue() << "  " << vmm->GetValue() << std::endl;
        //}
        if (det->GetValue(j) == 0 && ch->GetValue(j) == 63) {
            //if (time->GetValue() > 1.5738e+12 && time->GetValue() < 1.5768e+12) { // cut for time in spill, cut out undebunched start of spill
            mTriggerTimes.push_back(time->GetValue(j));
            //}
        }
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

//calibration with triggers for each fec
void VMMData::PlotClusterTriggerDiff() const {
  TCanvas *c = new TCanvas("5", "6", 3000, 2000);
  c->Divide(2, 2);
  TH1D *h1 = new TH1D("", "fec1; t / ns", 160, -2100, -1700);
  TH1D* h4 = new TH1D("", "fec4; t / ns", 160, -2100, -1700);
  TH1D* h2 = new TH1D("", "fec2; t / ns", 160, -2100, -1700);
  TH1D* h5 = new TH1D("", "fec5; t / ns", 160, -2100, -1700);
  //for (long unsigned int i = 0; i < mTriggerTimes.size(); i++) {
  for (long unsigned int i = 2500; i < 10000; i++) {
    for (long unsigned int j = 0; j < mCluster0Times.size(); j++) {
        if (mCluster0fec[j] == 1) {
            h1->Fill((mCluster0Times[j] - mTriggerTimes[i]));
        }
        if (mCluster0fec[j] == 4) {
            h4->Fill((mCluster0Times[j] - mTriggerTimes[i]));
        }
    }
    for (long unsigned int j = 0; j < mCluster1Times.size(); j++) {
        if (mCluster1fec[j] == 2) {
            h2->Fill((mCluster1Times[j] - mTriggerTimes[i]));
        }
        if (mCluster1fec[j] == 5) {
            h5->Fill((mCluster1Times[j] - mTriggerTimes[i]));
        }
    }
    std::cout << i << std::endl;
  }
  c->cd(1);
  TF1* func_1 = new TF1("func_1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h1->GetBinLowEdge(1), h1->GetBinLowEdge(h1->GetNbinsX() + 1));
  func_1->SetParameters(100, -1900, 10, 10);
  h1->Fit("func_1", "", "", -2100, -1700);
  TPaveText* fit_fec1 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec1->SetTextAlign(12);
  fit_fec1->AddText(Form("A1 = %.1f #pm %.1f", func_1->GetParameter(0), func_1->GetParError(0)));
  fit_fec1->AddText(Form("#mu1 = (%.1f #pm %.1f) ns", func_1->GetParameter(1), func_1->GetParError(1)));
  fit_fec1->AddText(Form("#sigma1 = (%.1f #pm %.1f) ns", func_1->GetParameter(2), func_1->GetParError(2)));
  fit_fec1->AddText(Form(""));
  fit_fec1->AddText(Form("offset = %.1f #pm %.1f", func_1->GetParameter(3), func_1->GetParError(3)));
  h1->Draw();
  fit_fec1->Draw();
  //c->SaveAs("trigger_cal_fec1.png");
  //c->SaveAs("trigger_cal_fec1.root");
  c->cd(2);
  TF1* func_4 = new TF1("func_4", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h4->GetBinLowEdge(1), h4->GetBinLowEdge(h4->GetNbinsX() + 1));
  func_4->SetParameters(100, -1900, 10, 10);
  h4->Fit("func_4", "", "", -2100, -1700);
  TPaveText* fit_fec4 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec4->SetTextAlign(12);
  fit_fec4->AddText(Form("A4 = %.1f #pm %.1f", func_4->GetParameter(0), func_4->GetParError(0)));
  fit_fec4->AddText(Form("#mu4 = (%.1f #pm %.1f) ns", func_4->GetParameter(1), func_4->GetParError(1)));
  fit_fec4->AddText(Form("#sigma4 = (%.1f #pm %.1f) ns", func_4->GetParameter(2), func_4->GetParError(2)));
  fit_fec4->AddText(Form(""));
  fit_fec4->AddText(Form("offset = %.1f #pm %.1f", func_4->GetParameter(3), func_4->GetParError(3)));
  h4->Draw();
  fit_fec4->Draw();
  c->cd(3);
  TF1* func_2 = new TF1("func_2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h2->GetBinLowEdge(1), h2->GetBinLowEdge(h2->GetNbinsX() + 1));
  func_2->SetParameters(100, -1900, 10, 10);
  h2->Fit("func_2", "", "", -2100, -1700);
  TPaveText* fit_fec2 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec2->SetTextAlign(12);
  fit_fec2->AddText(Form("A2 = %.1f #pm %.1f", func_2->GetParameter(0), func_2->GetParError(0)));
  fit_fec2->AddText(Form("#mu2 = (%.1f #pm %.1f) ns", func_2->GetParameter(1), func_2->GetParError(1)));
  fit_fec2->AddText(Form("#sigma2 = (%.1f #pm %.1f) ns", func_2->GetParameter(2), func_2->GetParError(2)));
  fit_fec2->AddText(Form(""));
  fit_fec2->AddText(Form("offset = %.1f #pm %.1f", func_2->GetParameter(3), func_2->GetParError(3)));
  h2->Draw();
  fit_fec2->Draw();
  c->cd(4);
  TF1* func_5 = new TF1("func_5", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h5->GetBinLowEdge(1), h5->GetBinLowEdge(h5->GetNbinsX() + 1));
  func_5->SetParameters(100, -1950, 10, 10);
  h5->Fit("func_5", "", "", -2100, -1700);
  TPaveText* fit_fec5 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_fec5->SetTextAlign(12);
  fit_fec5->AddText(Form("A5 = %.1f #pm %.1f", func_5->GetParameter(0), func_5->GetParError(0)));
  fit_fec5->AddText(Form("#mu5 = (%.1f #pm %.1f) ns", func_5->GetParameter(1), func_5->GetParError(1)));
  fit_fec5->AddText(Form("#sigma5 = (%.1f #pm %.1f) ns", func_5->GetParameter(2), func_5->GetParError(2)));
  fit_fec5->AddText(Form(""));
  fit_fec5->AddText(Form("offset = %.1f #pm %.1f", func_5->GetParameter(3), func_5->GetParError(3)));
  h5->Draw();
  fit_fec5->Draw();
  c->SaveAs("trigger_cal.png");

}

void VMMData::PlotClusterTriggerDiffVMMs() const {
    TCanvas* c = new TCanvas("5", "6", 3000, 2000);
    c->Divide(2, 2);
    TH1D* h1 = new TH1D("", "fec1-VMM5; t / ns", 160, -2100, -1700);
    TH1D* h2 = new TH1D("", "fec1-VMM6; t / ns", 160, -2100, -1700);
    TH1D* h3 = new TH1D("", "fec1-VMM7; t / ns", 160, -2100, -1700);
    TH1D* h4 = new TH1D("", "fec1-VMM8; t / ns", 160, -2100, -1700);
    //for (long unsigned int i = 0; i < mTriggerTimes.size(); i++) {
    for (long unsigned int i = 2500; i < 50000; i++) {
        for (long unsigned int j = 0; j < mCluster0Times.size(); j++) {
            if (mCluster0fec[j] == 1 and mCluster0Pos[j] > -126 and mCluster0Pos[j] < -67) {
                h1->Fill((mCluster0Times[j] - mTriggerTimes[i]));
            }
            if (mCluster0fec[j] == 1 and mCluster0Pos[j] > -64 and mCluster0Pos[j] < -3) {
                h2->Fill((mCluster0Times[j] - mTriggerTimes[i]));
            }
            if (mCluster0fec[j] == 1 and mCluster0Pos[j] > 2 and mCluster0Pos[j] < 61) {
                h3->Fill((mCluster0Times[j] - mTriggerTimes[i]));
            }
            if (mCluster0fec[j] == 1 and mCluster0Pos[j] > 66 and mCluster0Pos[j] < 125) {
                h4->Fill((mCluster0Times[j] - mTriggerTimes[i]));
            }
        }
        std::cout << i << std::endl;
    }
    c->cd(1);
    TF1* func_1 = new TF1("func_1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h1->GetBinLowEdge(1), h1->GetBinLowEdge(h1->GetNbinsX() + 1));
    func_1->SetParameters(100, -1900, 10, 10);
    h1->Fit("func_1", "", "", -2100, -1700);
    TPaveText* fit_1 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
    fit_1->SetTextAlign(12);
    fit_1->AddText(Form("A = %.1f #pm %.1f", func_1->GetParameter(0), func_1->GetParError(0)));
    fit_1->AddText(Form("#mu = (%.1f #pm %.1f) ns", func_1->GetParameter(1), func_1->GetParError(1)));
    fit_1->AddText(Form("#sigma = (%.1f #pm %.1f) ns", func_1->GetParameter(2), func_1->GetParError(2)));
    fit_1->AddText(Form(""));
    fit_1->AddText(Form("offset = %.1f #pm %.1f", func_1->GetParameter(3), func_1->GetParError(3)));
    h1->Draw();
    fit_1->Draw();
    c->cd(2);
    TF1* func_2 = new TF1("func_2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h2->GetBinLowEdge(1), h2->GetBinLowEdge(h2->GetNbinsX() + 1));
    func_2->SetParameters(100, -1900, 10, 10);
    h2->Fit("func_2", "", "", -2100, -1700);
    TPaveText* fit_2 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
    fit_2->SetTextAlign(12);
    fit_2->AddText(Form("A = %.1f #pm %.1f", func_2->GetParameter(0), func_2->GetParError(0)));
    fit_2->AddText(Form("#mu = (%.1f #pm %.1f) ns", func_2->GetParameter(1), func_2->GetParError(1)));
    fit_2->AddText(Form("#sigma = (%.1f #pm %.1f) ns", func_2->GetParameter(2), func_2->GetParError(2)));
    fit_2->AddText(Form(""));
    fit_2->AddText(Form("offset = %.1f #pm %.1f", func_2->GetParameter(3), func_2->GetParError(3)));
    h2->Draw();
    fit_2->Draw();
    c->cd(3);
    TF1* func_3 = new TF1("func_3", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h3->GetBinLowEdge(1), h3->GetBinLowEdge(h3->GetNbinsX() + 1));
    func_3->SetParameters(100, -1900, 10, 10);
    h3->Fit("func_3", "", "", -2100, -1700);
    TPaveText* fit_3 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
    fit_3->SetTextAlign(12);
    fit_3->AddText(Form("A = %.1f #pm %.1f", func_3->GetParameter(0), func_3->GetParError(0)));
    fit_3->AddText(Form("#mu = (%.1f #pm %.1f) ns", func_3->GetParameter(1), func_3->GetParError(1)));
    fit_3->AddText(Form("#sigma = (%.1f #pm %.1f) ns", func_3->GetParameter(2), func_3->GetParError(2)));
    fit_3->AddText(Form(""));
    fit_3->AddText(Form("offset = %.1f #pm %.1f", func_3->GetParameter(3), func_3->GetParError(3)));
    h3->Draw();
    fit_3->Draw();
    c->cd(4);
    TF1* func_4 = new TF1("func_4", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h4->GetBinLowEdge(1), h4->GetBinLowEdge(h4->GetNbinsX() + 1));
    func_4->SetParameters(100, -1950, 10, 10);
    h4->Fit("func_4", "", "", -2100, -1700);
    TPaveText* fit_4 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
    fit_4->SetTextAlign(12);
    fit_4->AddText(Form("A = %.1f #pm %.1f", func_4->GetParameter(0), func_4->GetParError(0)));
    fit_4->AddText(Form("#mu = (%.1f #pm %.1f) ns", func_4->GetParameter(1), func_4->GetParError(1)));
    fit_4->AddText(Form("#sigma = (%.1f #pm %.1f) ns", func_4->GetParameter(2), func_4->GetParError(2)));
    fit_4->AddText(Form(""));
    fit_4->AddText(Form("offset = %.1f #pm %.1f", func_4->GetParameter(3), func_4->GetParError(3)));
    h4->Draw();
    fit_4->Draw();
    c->SaveAs("trigger_cal_VMMs.png");

}

//estimate time resolution of readout
void VMMData::TimeResolution() const {
  TCanvas *c = new TCanvas("", "", 1900, 1500);
  c->Divide(2, 3);
  TH1D *h12 = new TH1D("fec1-2", "; #deltat / ns", 200, -300, 300);
  TH1D *h15 = new TH1D("fec1-5", "; #deltat / ns", 200, -300, 300);
  TH1D *h42 = new TH1D("fec4-2", "; #deltat / ns", 200, -300, 300);
  TH1D *h45 = new TH1D("fec4-5", "; #deltat / ns", 200, -300, 300);
  //check for split clusters
  TH1D* h14 = new TH1D("fec1-4", "; #deltat / ns", 200, -300, 300);
  TH1D* h25 = new TH1D("fec2-5", "; #deltat / ns", 200, -300, 300);
  for (long unsigned int i = 1000000; i < 1250000; i++) {
    for (long unsigned int j = 100000; j < 2000000 ; j++) {
      if (mCluster0fec[j] == 1 and mCluster1fec[i] == 2) {
        h12->Fill(mCluster0Times[j] - mCluster1Times[i]);
      }
      if (mCluster0fec[j] == 1 and mCluster1fec[i] == 5) {
        h15->Fill(mCluster0Times[j] - mCluster1Times[i]);
      }
      if (mCluster0fec[j] == 4 and mCluster1fec[i] == 2) {
        h42->Fill(mCluster0Times[j] - mCluster1Times[i]);
      }
      if (mCluster0fec[j] == 4 and mCluster1fec[i] == 5) {
        h45->Fill(mCluster0Times[j] - mCluster1Times[i]);
      }
      if (mCluster0fec[j] == 1 and mCluster0fec[i] == 4) {
          h14->Fill(mCluster0Times[j] - mCluster0Times[i]);
      }
      if (mCluster1fec[j] == 2 and mCluster1fec[i] == 5) {
          h25->Fill(mCluster1Times[j] - mCluster1Times[i]);
      }
      //h->Fill(mCluster0Times[j] - mCluster1Times[i]);
    }
    std::cout << "timeres   " << i - 1000000 << std::endl;
  }
  c->cd(1);
  TF1* func12 = new TF1("func12", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h12->GetBinLowEdge(1), h12->GetBinLowEdge(h12->GetNbinsX() + 1));
  func12->SetParameters(1000, 44, 11, 200, 44, 30, 500);
  h12->Fit("func12", "", "", -300, 300);
  TPaveText* fit_param12 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_param12->SetTextAlign(12);
  fit_param12->AddText(Form("A1 = %.1f #pm %.1f", func12->GetParameter(0), func12->GetParError(0)));
  fit_param12->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func12->GetParameter(1), func12->GetParError(1)));
  fit_param12->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func12->GetParameter(2), func12->GetParError(2)));
  fit_param12->AddText(Form(""));
  fit_param12->AddText(Form("A2 = %.1f #pm %.1f", func12->GetParameter(3), func12->GetParError(3)));
  fit_param12->AddText(Form("#mu2 = (%.4f #pm %.4f) ns", func12->GetParameter(4), func12->GetParError(4)));
  fit_param12->AddText(Form("#sigma2 = (%.4f #pm %.4f) ns", func12->GetParameter(5), func12->GetParError(5)));
  fit_param12->AddText(Form(""));
  fit_param12->AddText(Form("offset = %.1f #pm %.1f", func12->GetParameter(6), func12->GetParError(6)));
  h12->Draw();
  fit_param12->Draw();

  c->cd(2);
  TF1* func15 = new TF1("func15", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h15->GetBinLowEdge(1), h15->GetBinLowEdge(h15->GetNbinsX() + 1));
  func15->SetParameters(1000, 50, 11, 50, 50, 30, 100);
  h15->Fit("func15", "", "", -300, 300);
  TPaveText* fit_param15 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_param15->SetTextAlign(12);
  fit_param15->AddText(Form("A1 = %.1f #pm %.1f", func15->GetParameter(0), func15->GetParError(0)));
  fit_param15->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func15->GetParameter(1), func15->GetParError(1)));
  fit_param15->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func15->GetParameter(2), func15->GetParError(2)));
  fit_param15->AddText(Form(""));
  fit_param15->AddText(Form("A2 = %.1f #pm %.1f", func15->GetParameter(3), func15->GetParError(3)));
  fit_param15->AddText(Form("#mu2 = (%.4f #pm %.4f) ns", func15->GetParameter(4), func15->GetParError(4)));
  fit_param15->AddText(Form("#sigma2 = (%.4f #pm %.4f) ns", func15->GetParameter(5), func15->GetParError(5)));
  fit_param15->AddText(Form(""));
  fit_param15->AddText(Form("offset = %.1f #pm %.1f", func15->GetParameter(6), func15->GetParError(6)));
  h15->Draw();
  fit_param15->Draw();

  c->cd(3);
  TF1* func42 = new TF1("func42", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h42->GetBinLowEdge(1), h42->GetBinLowEdge(h42->GetNbinsX() + 1));
  //TF1* func42 = new TF1("func42", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [6]", h42->GetBinLowEdge(1), h42->GetBinLowEdge(h42->GetNbinsX() + 1));
  func42->SetParameters(3000, -50, 11, 200, -50, 30, 100);
  h42->Fit("func42", "", "", -300, 300);
  TPaveText* fit_param42 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_param42->SetTextAlign(12);
  fit_param42->AddText(Form("A1 = %.1f #pm %.1f", func42->GetParameter(0), func42->GetParError(0)));
  fit_param42->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func42->GetParameter(1), func42->GetParError(1)));
  fit_param42->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func42->GetParameter(2), func42->GetParError(2)));
  fit_param42->AddText(Form(""));
  fit_param42->AddText(Form("A2 = %.1f #pm %.1f", func42->GetParameter(3), func42->GetParError(3)));
  fit_param42->AddText(Form("#mu2 = (%.4f #pm %.4f) ns", func42->GetParameter(4), func42->GetParError(4)));
  fit_param42->AddText(Form("#sigma2 = (%.4f #pm %.4f) ns", func42->GetParameter(5), func42->GetParError(5)));
  fit_param42->AddText(Form(""));
  fit_param42->AddText(Form("offset = %.1f #pm %.1f", func42->GetParameter(6), func42->GetParError(6)));
  h42->Draw();
  fit_param42->Draw();

  c->cd(4);
  TF1* func45 = new TF1("func45", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h45->GetBinLowEdge(1), h45->GetBinLowEdge(h45->GetNbinsX() + 1));
  func45->SetParameters(1000, -20, 11, 50, -20, 30, 100);
  h45->Fit("func45", "", "", -300, 300);
  TPaveText* fit_param45 = new TPaveText(0.1, 0.57, 0.37, 0.85, "NDC");
  fit_param45->SetTextAlign(12);
  fit_param45->AddText(Form("A1 = %.1f #pm %.1f", func45->GetParameter(0), func45->GetParError(0)));
  fit_param45->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func45->GetParameter(1), func45->GetParError(1)));
  fit_param45->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func45->GetParameter(2), func45->GetParError(2)));
  fit_param45->AddText(Form(""));
  fit_param45->AddText(Form("A2 = %.1f #pm %.1f", func45->GetParameter(3), func45->GetParError(3)));
  fit_param45->AddText(Form("#mu2 = (%.4f #pm %.4f) ns", func45->GetParameter(4), func45->GetParError(4)));
  fit_param45->AddText(Form("#sigma2 = (%.4f #pm %.4f) ns", func45->GetParameter(5), func45->GetParError(5)));
  fit_param45->AddText(Form(""));
  fit_param45->AddText(Form("offset = %.1f #pm %.1f", func45->GetParameter(6), func45->GetParError(6)));
  h45->Draw();
  fit_param45->Draw();

  c->cd(5);
  TF1* func14 = new TF1("func14", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h14->GetBinLowEdge(1), h14->GetBinLowEdge(h14->GetNbinsX() + 1));
  func14->SetParameters(1000, 50, 30, 100);
  h14->Fit("func14", "", "", -300, 300);
  TPaveText* fit_param14 = new TPaveText(0.1, 0.67, 0.37, 0.85, "NDC");
  fit_param14->SetTextAlign(12);
  fit_param14->AddText(Form("A1 = %.1f #pm %.1f", func14->GetParameter(0), func14->GetParError(0)));
  fit_param14->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func14->GetParameter(1), func14->GetParError(1)));
  fit_param14->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func14->GetParameter(2), func14->GetParError(2)));
  fit_param14->AddText(Form(""));
  fit_param14->AddText(Form("offset = %.1f #pm %.1f", func14->GetParameter(3), func14->GetParError(3)));
  h14->Draw();
  fit_param14->Draw();

  c->cd(6);
  TF1* func25 = new TF1("func25", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h25->GetBinLowEdge(1), h25->GetBinLowEdge(h25->GetNbinsX() + 1));
  func25->SetParameters(1000, 50, 30, 100);
  h25->Fit("func25", "", "", -300, 300);
  TPaveText* fit_param25 = new TPaveText(0.1, 0.67, 0.37, 0.85, "NDC");
  fit_param25->SetTextAlign(22);
  fit_param25->AddText(Form("A1 = %.1f #pm %.1f", func25->GetParameter(0), func25->GetParError(0)));
  fit_param25->AddText(Form("#mu1 = (%.4f #pm %.4f) ns", func25->GetParameter(1), func25->GetParError(1)));
  fit_param25->AddText(Form("#sigma1 = (%.4f #pm %.4f) ns", func25->GetParameter(2), func25->GetParError(2)));
  fit_param25->AddText(Form(""));
  fit_param25->AddText(Form("offset = %.1f #pm %.1f", func25->GetParameter(3), func25->GetParError(3)));
  h25->Draw();
  fit_param25->Draw();
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
