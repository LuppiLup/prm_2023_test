#include "ReadAmberData.h"

#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"
#include "TrackExtrapolationResult.h"
#include "TrackData.h"
#include <TGraph.h>

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"

#include <TF1.h>
#include <TPaveText.h>

#include <iostream>
AmberData::AmberData(const std::string &DataFile) {
    std::cout << " start reading " << std::endl;
  TFile f(DataFile.c_str());
  if (f.IsZombie()) {
    std::cerr << "Could not open amber datafile " << DataFile << std::endl;
    abort();
  }

  //Long64_t s;
  double time;
  Long64_t s_last = 10000;
  std::vector<double> times;
  std::vector<double> track_x;
  std::vector<double> track_y;
  double zMin;
  double zMax;
  Bool_t trackIsMatched;
  Bool_t canBeMuon;

  double pos1 = 2324;   //VMM_GEM
  //double pos2 = 3107;   //GM10
  double pos2 = 2116.7;

  TTree* t = (TTree*)f.Get("UserEvent7001");
  t->Print();
  treeReader = new TTreeReader("UserEvent7001", &f);
  TTreeReaderValue<double> timeInSpill = TTreeReaderValue<double>(*treeReader, "timeInSpill");
  TTreeReaderValue<Long64_t> s = TTreeReaderValue<Long64_t>(*treeReader, "spill");
  spectrometerTracks = new TTreeReaderValue<std::vector<TrackData>>(*treeReader, "SpectrometerData");

  TCanvas* c_test = new TCanvas("", "", 1000, 800);
  TH1D* h_xtest = new TH1D("", "", 1000, -20, 20);
  TH2D* h_test = new TH2D("", "", 1000, -20, 20, 1000, -20, 20);

  while (treeReader->Next()) {
    if (s_last == 10000) {
        s_last = *s;
    }
    else if (*s != s_last) {
        PerSpill p;
        p.SpillNumber = s_last;
        p.timeStamps.swap(times);
        p.tracks_x.swap(track_x);
        p.tracks_y.swap(track_y);
        mPerSpillData.push_back(p);
        s_last = *s;
    }
    //std::cout << "spill   " << *spill_number << std::endl;
    //std::cout << "tiS   " << (*timeInSpill) << std::endl;
    //std::cout << "amount tracks  " << (**spectrometerTracks).size() << std::endl;
    for (const TrackData &track : **spectrometerTracks) {
      //track.Print();
      zMin = track.zMin;
      zMax = track.zMax;
      trackIsMatched = track.trackIsMatched;
      canBeMuon = track.canBeMuon;
      for (const TrackExtrapolationResult &extrapolation : track.extrapolationResults) {
        //std::cout << "x_extrapolation:  " << extrapolation.x << "  " << *timeInSpill << std::endl;
        if (zMax > 2324) {
        //if (zMax > 0) {
          times.push_back(*timeInSpill * (1 + alpha) * 1e09);
          //tracks extrapolated to VMM GEM
          track_x.push_back(extrapolation.x);
          track_y.push_back(extrapolation.y);
          //tracks extrapolated to pos2
          h_test->Fill(extrapolation.x + (pos2 - pos1)*extrapolation.dxdz, extrapolation.y + (pos2 - pos1)*extrapolation.dydz);
          h_xtest->Fill(extrapolation.x);
        }
      }
    }
  }
  gStyle->SetPalette(1);
  h_test->Draw("colz");
  c_test->SaveAs("xy_test.png");
  TCanvas* c_xtest = new TCanvas("", "", 1000, 800);
  h_xtest->Draw();
  c_xtest->SaveAs("x_test.png");
  PerSpill p;
  p.SpillNumber = s_last;
  p.timeStamps.swap(times);
  p.tracks_x.swap(track_x);
  p.tracks_y.swap(track_y);
  f.Close();

  for (auto &s : mPerSpillData) {
    std::cout << "NAmbertimes for spill: " << s.SpillNumber << " " << s.timeStamps.size()
              << std::endl;
  }
  std::cout << "Amber trees finished" << std::endl;

  
  /*
  Long64_t s;
  double time;
  double x;
  double y;
  double z;
  double x_det;
  double y_det;
  double z_det = 23.24 * 100;
  double gradX;
  double gradY;
  double extrapolation;
  double x_test;
  std::vector<double> vx_det;
  std::vector<double> vy_det;
  std::vector<double> vz;
  //std::vector<double> extrapolation;
  std::vector<double> track_x;
  std::vector<double> track_y;
  std::vector<double> track_z;
  std::cout << " I am here A " << std::endl;
  TTree *t = (TTree *)f.Get("UserEvent7001");
  t->Print();
  std::cout << " I am here B " << std::endl;
  t->SetBranchAddress("spill", &s);
  std::cout << " I am here C " << std::endl;
  t->Print();
  t->SetBranchAddress("timeInSpill", &time);
  //size_t NEntries = t->GetEntries();
  TrackExtrapolationResult tracks;
  //t->SetBranchAddress("SpectrometerData.extrapolationResults.x", &tracks.x);
  //t->SetBranchAddress("y", &tracks.y);
  std::vector<TrackExtrapolationResult> extrapolationResults;
  t->SetBranchAddress("SpectrometerData.extrapolationResults", &extrapolationResults);
  Long64_t s_last = 10000;
  std::vector<double> times;
  std::cout << " I am here 1" << std::endl;
  for (size_t i = 100; i < 140; ++i) {
    t->GetEntry(i);
    if (s_last == 10000) {
      s_last = s;
    } else if (s != s_last) {
      PerSpill p;
      p.SpillNumber = s_last;
      p.timeStamps.swap(times);
      p.tracks_x.swap(track_x);
      mPerSpillData.push_back(p);
      s_last = s;
    }
    times.push_back(time * (1 + alpha) * 1e09);
    
    for (auto& trackResult : extrapolationResults ){
      x = trackResult.x;
    }
    
    //x = t->GetLeaf("SpectrometerData.extrapolationResults.x")->GetValue();
    //x = t->GetLeaf("SpectrometerData.x")->GetValue();
    y = t->GetLeaf("SpectrometerData.y")->GetValue();
    z = t->GetLeaf("SpectrometerData.z")->GetValue();
    gradX = t->GetLeaf("SpectrometerData.gradX")->GetValue();
    gradY = t->GetLeaf("SpectrometerData.gradY")->GetValue();
    //x_det = x + gradX * (z_det - z);
    x_det = x;
    y_det = y + gradY * (z_det - z);
    if (x * x > 0 and y * y > 0) {
        vx_det.push_back(x_det);
        vy_det.push_back(y_det);
        vz.push_back(z);
    }
    std::cout << i <<  "  " << x << "  " << y << "  " << z << std::endl;
  }

  TCanvas* c_test = new TCanvas("", "", 1000, 800);
  TH1D* h_test = new TH1D("", "", 1000, 400, 600);
  for (long unsigned int i_x = 0; i_x < vx_det.size(); i_x++) {
      h_test->Fill(vz[i_x]);
  }
  h_test->Draw();
  c_test->SaveAs("x_test.png");
  PerSpill p;
  p.SpillNumber = s_last;
  p.timeStamps.swap(times);
  p.tracks_x.swap(track_x);
  f.Close();
  // for (auto &s : mPerSpillData) {
  //   std::cout << "NAmbertimes for spill: " << s.SpillNumber << " " << s.timeStamps.size()
  //             << std::endl;
  // }
  // std::cout << "Amber trees finished" << std::endl;
  */
}


void AmberData::PlotTimeStamps(Long64_t spill) const {
    std::cout << "test1" << std::endl;
  TCanvas *c = new TCanvas("", "", 1200, 700);
  TH1D *h = new TH1D("", "; t / ns; timestamps per bin", 1000, 0, 6e09);
  std::cout << "test2" << std::endl;
  const PerSpill *p = nullptr;
  for (auto &ps : mPerSpillData) {
    if (ps.SpillNumber == spill) {
      p = &ps;
      break;
    }
  }
  std::cout << "test3" << std::endl;
  for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
    h->Fill(p->timeStamps[j]);
  }
  std::cout << "test4" << std::endl;
  h->Draw();
  c->SaveAs(Form("spill_%lld.png", spill));
  std::cout << "test5" << std::endl;
}

void AmberData::PlotClusterTimeDiff(Long64_t spill, const std::vector<double> &VMMTimeStamps, double timeshift) {
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
  TH1D *h_cal = new TH1D("", "; #Deltat / #mus; entrys per bin", 100, -2, 2);
  //for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
  for (long unsigned int j = 5000; j < 6000; j++) {
      std::cout << "amber_cal16  " << j << std::endl;
    for (long unsigned int k = 0; k < 1200000; k++) {
      //h_cal->Fill((p->timeStamps[j] - VMMTimeStamps[k] + 1375e09 + 1.826e09 + 50e03 - 5880) / 1000);
      h_cal->Fill((p->timeStamps[j] - VMMTimeStamps[k] - timeshift) / 1000);
      //std::cout << p->timeStamps[j] - VMMTimeStamps[k] - timeshift << std::endl;
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
    double peakpos_1 = h2_cal->GetMaximumBin() * 1 + 1000*(peakpos_0 - 20); // accuracy 1 ns
    TCanvas* c2_cal = new TCanvas("", "", 1200, 700);
    h2_cal->Draw();
    c2_cal->SaveAs(Form("trigger_cal_2_spill_%lld.png", spill));
    return peakpos_1;

}

void AmberData::Residuals(Long64_t spill, const std::vector<double>& VMMxTimeStamps, const std::vector<double>& VMMyTimeStamps, const std::vector<double>& VMM_xpos, const std::vector<double>& VMM_ypos, double timeshift) {
    const PerSpill* p = nullptr;
    for (auto& ps : mPerSpillData) {
        if (ps.SpillNumber == spill) {
            p = &ps;
            break;
        }
    }
    int N_tracks;
    std::vector<int> hits_per_track_x = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_track_y = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<double> Time_In_Spill;
    std::vector<double> time_differences;
    int hits_count;
    double timediff;
    double xdiff;
    double ydiff;
    TCanvas* c_res_time = new TCanvas("", "", 1200, 700);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    std::cout << " I am here " << std::endl;
    //gStyle->SetLegendTextSize(0.04);
    TH1D* h_res_time = new TH1D("", "; #deltat / #mus; entrys per bin", 200, -1, 1);
    TH1D* h_res_x = new TH1D("", "; #deltax / #mum; entrys per bin", 200, -1500, 1500);
    TH1D* h_res_y = new TH1D("", "; #deltay / #mum; entrys per bin", 200, -1500, 1500);
    TH2D* h_missing = new TH2D("", "", 1000, -20, 20, 1000, -20, 20);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        N_tracks = p->timeStamps.size();
        std::cout << "amber_cal_residuals  " << j << " out of " << N_tracks << std::endl;
        hits_count = 0;
        for (long unsigned int k = 0; k < VMMxTimeStamps.size(); k++) {
            timediff = p->timeStamps[j] - VMMxTimeStamps[k] - timeshift;
            xdiff = p->tracks_x[j] + 0.04*VMM_xpos[k] - 8.34;
            if (std::abs(timediff) < 500) {
                h_res_x->Fill(1000*10*xdiff);
                if (std::abs(xdiff) < 0.1) {
                    hits_count++;
                    Time_In_Spill.push_back(p->timeStamps[j]);
                    time_differences.push_back(timediff);
                }
            }
            if (std::abs(xdiff) < 0.1) {
                h_res_time->Fill(timediff / 1000);
            }
        }
        if (hits_count == 0) {
            h_missing->Fill(p->tracks_x[j], p->tracks_y[j]);
        }
        hits_per_track_x[hits_count]++;
        hits_count = 0;
        for (long unsigned int K = 0; K < VMMyTimeStamps.size(); K++) {
            timediff = p->timeStamps[j] - VMMyTimeStamps[K] - timeshift;
            ydiff = p->tracks_y[j] - 0.04 * VMM_ypos[K] + 0.445;
            if (std::abs(timediff) < 500) {
                h_res_y->Fill(1000*10*ydiff);
                if (std::abs(ydiff) < 0.1) {
                    hits_count++;
                }
            }
        }
        hits_per_track_y[hits_count]++;
    }
    h_res_time->Draw();
    c_res_time->SaveAs(Form("res_time_spill_%lld.png", spill));

    TCanvas* c_missing = new TCanvas("", "", 1200, 700);
    h_missing->Draw();
    c_missing->SaveAs(Form("missing_tracks_%lld.png", spill));

    TCanvas* timediff_vs_TiS = new TCanvas("", "", 1200, 700);
    TGraph* timediff_TiS = new TGraph(Time_In_Spill.size(), Time_In_Spill.data(), time_differences.data());
    timediff_TiS->GetXaxis()->SetTitle("time in spill / s");
    timediff_TiS->GetYaxis()->SetTitle("#deltat / ns");
    timediff_TiS->Draw("AP");
    timediff_vs_TiS->SaveAs(Form("timediff_vs_TiS_%lld.png", spill));

    TCanvas* c_res_x = new TCanvas("", "", 1200, 700);

    TF1* func_x = new TF1("func_x", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]*exp(-(x-[7])*(x-[7])/(2*[8]*[8])) + [9]", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x->SetParameters(300, 1, 100, 80, 200, 400, 30, 1, 400, 10);
    h_res_x->Fit("func_x", " ", " ", -1500, 1500);
    TPaveText* fit_x = new TPaveText(0.1, 0.5, 0.37, 0.85, "NDC");
    fit_x->SetTextAlign(12);
    fit_x->AddText(Form("A1 = %.1f #pm %.1f", func_x->GetParameter(0), func_x->GetParError(0)));
    fit_x->AddText(Form("#mu1 = (%.0f #pm %.0f) #mum", func_x->GetParameter(1), func_x->GetParError(1)));
    fit_x->AddText(Form("#sigma1 = (%.0f #pm %.0f) #mum", func_x->GetParameter(2), func_x->GetParError(2)));
    fit_x->AddText(Form(" "));
    fit_x->AddText(Form("A2 = %.1f #pm %.1f", func_x->GetParameter(3), func_x->GetParError(3)));
    fit_x->AddText(Form("#mu2 = (%.0f #pm %.0f) #mum", func_x->GetParameter(4), func_x->GetParError(4)));
    fit_x->AddText(Form("#sigma2 = (%.0f #pm %.0f) #mum", func_x->GetParameter(5), func_x->GetParError(5)));
    fit_x->AddText(Form(" "));
    fit_x->AddText(Form("A3 = %.1f #pm %.1f", func_x->GetParameter(6), func_x->GetParError(6)));
    fit_x->AddText(Form("#mu3 = (%.0f #pm %.0f) #mum", func_x->GetParameter(7), func_x->GetParError(7)));
    fit_x->AddText(Form("#sigma3 = (%.0f #pm %.0f) #mum", func_x->GetParameter(8), func_x->GetParError(8)));
    fit_x->AddText(Form(" "));
    fit_x->AddText(Form("offset = %.1f #pm %.1f", func_x->GetParameter(9), func_x->GetParError(9)));
    h_res_x->Draw();
    fit_x->Draw();
    c_res_x->SaveAs(Form("res_x_spill_%lld.png", spill));

    TCanvas* c_res_y = new TCanvas("", "", 1200, 700);

    TF1* func_y = new TF1("func_y", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h_res_y->GetBinLowEdge(1), h_res_y->GetBinLowEdge(h_res_y->GetNbinsX() + 1));
    func_y->SetParameters(300, 1, 100, 80, 200, 400, 10);
    h_res_y->Fit("func_y", " ", " ", -1500, 1500);
    TPaveText* fit_y = new TPaveText(0.1, 0.5, 0.37, 0.85, "NDC");
    fit_y->SetTextAlign(12);
    fit_y->AddText(Form("A1 = %.1f #pm %.1f", func_y->GetParameter(0), func_y->GetParError(0)));
    fit_y->AddText(Form("#mu1 = (%.0f #pm %.0f) #mum", func_y->GetParameter(1), func_y->GetParError(1)));
    fit_y->AddText(Form("#sigma1 = (%.0f #pm %.0f) #mum", func_y->GetParameter(2), func_y->GetParError(2)));
    fit_y->AddText(Form(" "));
    fit_y->AddText(Form("A2 = %.1f #pm %.1f", func_y->GetParameter(3), func_y->GetParError(3)));
    fit_y->AddText(Form("#mu2 = (%.0f #pm %.0f) #mum", func_y->GetParameter(4), func_y->GetParError(4)));
    fit_y->AddText(Form("#sigma2 = (%.0f #pm %.0f) #mum", func_y->GetParameter(5), func_y->GetParError(5)));
    fit_y->AddText(Form(" "));
    fit_y->AddText(Form("offset = %.1f #pm %.1f", func_y->GetParameter(6), func_y->GetParError(6)));
    h_res_y->Draw();
    fit_y->Draw();
    c_res_y->SaveAs(Form("res_y_spill_%lld.png", spill));

    std::cout << "amount tacks    " << N_tracks << std::endl;
    std::cout << "x-plane   " << std::endl;
    std::cout << "0  " << hits_per_track_x[0] << std::endl;
    std::cout << "1  " << hits_per_track_x[1] << std::endl;
    std::cout << "2  " << hits_per_track_x[2] << std::endl;
    std::cout << "3  " << hits_per_track_x[3] << std::endl;
    std::cout << "y-plane   " << std::endl;
    std::cout << "0  " << hits_per_track_y[0] << std::endl;
    std::cout << "1  " << hits_per_track_y[1] << std::endl;
    std::cout << "2  " << hits_per_track_y[2] << std::endl;
    std::cout << "3  " << hits_per_track_y[3] << std::endl;
}

