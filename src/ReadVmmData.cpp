#include "ReadVmmData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TLeaf.h"
#include "TStyle.h"
#include "TTree.h"
#include "TGraph.h"
#include "TGraphErrors.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include <TF1.h>
#include <TPaveText.h>

#include <iostream>

#include "DataStructures.h"
#include <TLatex.h>

VMMData::VMMData(const std::string &DataFile_cluster, const std::string &DataFile_trigger, int spill) {
  TFile f(DataFile_cluster.c_str());
  if (f.IsZombie()) {
    std::cerr << "Could not open vmm datafile " << DataFile_cluster << std::endl;
    abort();
  }
  // first the clusters...
  TTree *t = (TTree *)f.Get("clusters_plane;1");
  //TTree *t = (TTree *)f.Get(Form("clusters_plane_%d;63", spill - 96));
  if (!t) {
    std::cerr << "I could not find cluster plane tree in file!" << std::endl;
    abort();
  }
  t->Print();
  //int det_v;
  int len_cluster_plane;
  int len_hits;
  int size_cut = 200;
  int adc_cut = 0;
  
  //TLeaf* plane = t->GetLeaf("plane");
  TLeaf* time = t->GetLeaf("time");
  TLeaf* pos = t->GetLeaf("pos");
  TLeaf* size = t->GetLeaf("size_nn");
  TLeaf* adc = t->GetLeaf("adc");
  TLeaf* fec_c = t->GetLeaf("fec");
  //TLeaf* mst = t->GetLeaf("max_missing_strip");
  size_t NEntries = t->GetEntries();//1000000;//t->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
      t->GetEntry(i);
      len_cluster_plane = time->GetLen();
      for (int j = 0; j < len_cluster_plane; j++) {
          //det_v = det->GetValue(j);
          //det_v = det->GetValue(j);
              if (fec_c->GetValue(j) == 1) {
                  mCluster0Times.push_back(time->GetValue(j));
                  mCluster0adc.push_back(adc->GetValue(j));
                  mCluster0size.push_back(size->GetValue(j));
                  mCluster0Pos.push_back(pos->GetValue(j));
                  mCluster0fec.push_back(1);
              }
              if (fec_c->GetValue(j) == 4) {
                  mCluster0Times.push_back(time->GetValue(j));
                  mCluster0adc.push_back(adc->GetValue(j));
                  mCluster0size.push_back(size->GetValue(j));
                  mCluster0Pos.push_back(pos->GetValue(j));
                  mCluster0fec.push_back(4);
            

              }
              if (fec_c->GetValue(j) == 2) {
                  mCluster1Times.push_back(time->GetValue(j));
                  mCluster1adc.push_back(adc->GetValue(j));
                  mCluster1size.push_back(size->GetValue(j));
                  //mCluster1Pos.push_back(pos->GetValue(j) - 384);      //old files
                  mCluster1Pos.push_back(pos->GetValue(j));
                  mCluster1fec.push_back(2);
              }
              if (fec_c->GetValue(j) == 5) {
                  mCluster1Times.push_back(time->GetValue(j));
                  mCluster1adc.push_back(adc->GetValue(j));
                  mCluster1size.push_back(size->GetValue(j));
                  mCluster1Pos.push_back(pos->GetValue(j));
                  mCluster1fec.push_back(5);
              }
          
      }
  
  }
  //std::cout << "Time of last cluster is " << time->GetValue() << std::endl;
  //std::cout << "NClusterTimes: " << mClusterTimes.size() << std::endl;

  // now the physics triggers
  TFile f_t(DataFile_trigger.c_str());
  if (f_t.IsZombie()) {
    std::cerr << "Could not open vmm trigger datafile " << DataFile_trigger << std::endl;
    abort();
  }


  //TTree *t2 = (TTree *)f_t.Get(Form("hits_%d;1", spill - 96));
  TTree *t2 = (TTree *)f_t.Get("trigger;1");
  if (!t2) {
    std::cerr << "I could not find hits tree in file!" << std::endl;
    abort();
  }
  TLeaf* time_hits = t2->GetLeaf("time");
  pos = t2->GetLeaf("pos");
  TLeaf* adc_hits = t2->GetLeaf("adc");
  NEntries = t2->GetEntries();
  for (size_t i = 0; i < NEntries; ++i) {
    t2->GetEntry(i);
    if (pos->GetValue() == 63) {
      mTriggerTimes.push_back(time_hits->GetValue());
    }
  }
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

//calibration with triggers for each fec using clusters
void VMMData::PlotClusterTriggerDiff() const {
  TCanvas *c = new TCanvas("5", "6", 1400, 1100);
  gStyle->SetOptStat(0);
  c->Divide(2, 2);
  TH1D *h1 = new TH1D("", "FEC_1; #deltat / #mus; Entries per 2.5ns bin", 160, -2.100, -1.700);
  TH1D* h4 = new TH1D("", "FEC_4; #deltat / #mus; Entries per 2.5ns bin", 160, -2.100, -1.700);
  TH1D* h2 = new TH1D("", "FEC_2; #deltat / #mus; Entries per 2.5ns bin", 160, -2.100, -1.700);
  TH1D* h5 = new TH1D("", "FEC_5; #deltat / #mus; Entries per 2.5ns bin", 160, -2.100, -1.700);
  //for (long unsigned int i = 0; i < mTriggerTimes.size(); i++) {
  for (long unsigned int i = 2000; i < 5000; i++) {
  //for (long unsigned int i = 20000; i < 24000; i++) {
    for (long unsigned int j = 0; j < mCluster0Times.size(); j++) {
        if (mCluster0fec[j] == 1) {
            h1->Fill((mCluster0Times[j] - mTriggerTimes[i])/1000);
        }
        if (mCluster0fec[j] == 4) {
            h4->Fill((mCluster0Times[j] - mTriggerTimes[i])/1000);
        }
    }
    for (long unsigned int j = 0; j < mCluster1Times.size(); j++) {
        if (mCluster1fec[j] == 2) {
            h2->Fill((mCluster1Times[j] - mTriggerTimes[i])/1000);
        }
        if (mCluster1fec[j] == 5) {
            h5->Fill((mCluster1Times[j] - mTriggerTimes[i])/1000);
        }
    }
    std::cout << "trigger cal   " << i << std::endl;
  }
  c->cd(1);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  TF1* func_1 = new TF1("func_1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h1->GetBinLowEdge(1), h1->GetBinLowEdge(h1->GetNbinsX() + 1));
  func_1->SetParameters(100, -1.850, 0.01, 10);
  func_1->SetNpx(1000);
  h1->Fit("func_1", "", "", -2.100, -1.700);
  h1->SetTitleSize(0.12);
  h1->GetXaxis()->CenterTitle();
  h1->GetXaxis()->SetTitleSize(0.06);
  h1->GetYaxis()->CenterTitle();
  h1->GetYaxis()->SetTitleSize(0.06);
  TPaveText* fit_fec1 = new TPaveText(0.15, 0.70, 0.4, 0.9, "NDC");
  fit_fec1->SetTextAlign(12);
  fit_fec1->AddText(Form("A1 = %.0f #pm %.0f", func_1->GetParameter(0), func_1->GetParError(0)));
  fit_fec1->AddText(Form("#mu1 = (%.1f #pm %.1f) ns", 1000*func_1->GetParameter(1), 1000*func_1->GetParError(1)));
  fit_fec1->AddText(Form("#sigma1 = (%.1f #pm %.1f) ns", 1000*func_1->GetParameter(2), 1000*func_1->GetParError(2)));
  fit_fec1->AddText(Form(" "));
  fit_fec1->AddText(Form("offset = %.1f #pm %.1f", func_1->GetParameter(3), func_1->GetParError(3)));
  h1->Draw();
  fit_fec1->Draw();
  //c->SaveAs("trigger_cal_fec1.png");
  //c->SaveAs("trigger_cal_fec1.root");
  c->cd(2);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  TF1* func_4 = new TF1("func_4", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h4->GetBinLowEdge(1), h4->GetBinLowEdge(h4->GetNbinsX() + 1));
  func_4->SetParameters(100, -1.850, 0.01, 10);
  func_4->SetNpx(1000);
  h4->Fit("func_4", "", "", -2.100, -1.700);
  h4->GetXaxis()->CenterTitle();
  h4->GetXaxis()->SetTitleSize(0.06);
  h4->GetYaxis()->CenterTitle();
  h4->GetYaxis()->SetTitleSize(0.06);
  TPaveText* fit_fec4 = new TPaveText(0.15, 0.70, 0.4, 0.9, "NDC");
  fit_fec4->SetTextAlign(12);
  fit_fec4->AddText(Form("A4 = %.0f #pm %.0f", func_4->GetParameter(0), func_4->GetParError(0)));
  fit_fec4->AddText(Form("#mu4 = (%.1f #pm %.1f) ns", 1000*func_4->GetParameter(1), 1000*func_4->GetParError(1)));
  fit_fec4->AddText(Form("#sigma4 = (%.1f #pm %.1f) ns", 1000*func_4->GetParameter(2), 1000*func_4->GetParError(2)));
  fit_fec4->AddText(Form(" "));
  fit_fec4->AddText(Form("offset = %.1f #pm %.1f", func_4->GetParameter(3), func_4->GetParError(3)));
  h4->Draw();
  fit_fec4->Draw();
  c->cd(3);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  TF1* func_2 = new TF1("func_2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h2->GetBinLowEdge(1), h2->GetBinLowEdge(h2->GetNbinsX() + 1));
  func_2->SetParameters(100, -1.850, 0.01, 10);
  func_2->SetNpx(1000);
  h2->Fit("func_2", "", "", -2.100, -1.700);
  h2->GetXaxis()->CenterTitle();
  h2->GetXaxis()->SetTitleSize(0.06);
  h2->GetYaxis()->CenterTitle();
  h2->GetYaxis()->SetTitleSize(0.06);
  TPaveText* fit_fec2 = new TPaveText(0.15, 0.70, 0.4, 0.9, "NDC");
  fit_fec2->SetTextAlign(12);
  fit_fec2->AddText(Form("A2 = %.0f #pm %.0f", func_2->GetParameter(0), func_2->GetParError(0)));
  fit_fec2->AddText(Form("#mu2 = (%.1f #pm %.1f) ns", 1000*func_2->GetParameter(1), 1000*func_2->GetParError(1)));
  fit_fec2->AddText(Form("#sigma2 = (%.1f #pm %.1f) ns", 1000*func_2->GetParameter(2), 1000*func_2->GetParError(2)));
  fit_fec2->AddText(Form(" "));
  fit_fec2->AddText(Form("offset = %.1f #pm %.1f", func_2->GetParameter(3), func_2->GetParError(3)));
  h2->Draw();
  fit_fec2->Draw();
  c->cd(4);
  gPad->SetLeftMargin(0.15);
  gPad->SetBottomMargin(0.15);
  TF1* func_5 = new TF1("func_5", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h5->GetBinLowEdge(1), h5->GetBinLowEdge(h5->GetNbinsX() + 1));
  func_5->SetParameters(100, -1.850, 0.01, 10);
  func_5->SetNpx(1000);
  h5->Fit("func_5", "", "", -2.100, -1.700);
  h5->GetXaxis()->CenterTitle();
  h5->GetXaxis()->SetTitleSize(0.06);
  h5->GetYaxis()->CenterTitle();
  h5->GetYaxis()->SetTitleSize(0.06);
  TPaveText* fit_fec5 = new TPaveText(0.15, 0.70, 0.4, 0.9, "NDC");
  fit_fec5->SetTextAlign(12);
  fit_fec5->AddText(Form("A5 = %.0f #pm %.0f", func_5->GetParameter(0), func_5->GetParError(0)));
  fit_fec5->AddText(Form("#mu5 = (%.1f #pm %.1f) ns", 1000*func_5->GetParameter(1), 1000*func_5->GetParError(1)));
  fit_fec5->AddText(Form("#sigma5 = (%.1f #pm %.1f) ns", 1000*func_5->GetParameter(2), 1000*func_5->GetParError(2)));
  fit_fec5->AddText(Form(" "));
  fit_fec5->AddText(Form("offset = %.1f #pm %.1f", func_5->GetParameter(3), func_5->GetParError(3)));
  h5->Draw();
  fit_fec5->Draw();
  c->SaveAs("trigger_cal.png");
  TCanvas* c_fec2 = new TCanvas("5", "6", 3000, 2000);
  gStyle->SetOptStat(0);
  h2->Draw();
  TLatex* tex = new TLatex(-2050., 80, "Work in Progress");
  tex->SetTextColor(17);
  tex->SetTextSize(0.155);
  tex->SetTextAngle(26.15998);
  tex->SetLineWidth(2);
  tex->Draw();
  //graph_y->Draw("P");
  gPad->RedrawAxis();
  fit_fec2->Draw();
  c_fec2->SaveAs("trigger_cal_fec2.png");

}

void VMMData::Synch_fecs(double shift_1, double shift_2, double shift_4, double shift_5) {
  for (long unsigned int i = 0; i < mCluster0Times.size(); i++) {
    if (mCluster0fec[i] == 1) {
      mCluster0Times[i] = mCluster0Times[i] - shift_1;
    }
    if (mCluster0fec[i] == 4) {
      mCluster0Times[i] = mCluster0Times[i] - shift_4;
    }
  }
  for (long unsigned int i = 0; i < mCluster1Times.size(); i++) {
    if (mCluster1fec[i] == 2) {
      mCluster1Times[i] = mCluster1Times[i] - shift_2;
    }
    if (mCluster1fec[i] == 5) {
      mCluster1Times[i] = mCluster1Times[i] - shift_5;
    }
  }
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
