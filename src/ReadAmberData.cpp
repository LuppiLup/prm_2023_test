#include "ReadAmberData.h"
#include <cmath>
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TTree.h"
#include "TrackExtrapolationResult.h"
#include "TrackData.h"
#include <TGraph.h>
#include <TGraphErrors.h>
#include "TMatrix.h"

#include "TTreeReader.h"
#include "TTreeReaderValue.h"
#include "TTreeReaderArray.h"
#include "TEfficiency.h"

#include <TF1.h>
#include <TPaveText.h>
#include <TLatex.h>

#include <fstream>
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
  double x;
  double y;
  float TIS_float;
  double TIS_double;
  Long64_t s_last = 10000;
  std::vector<double> times;
  std::vector<float> TCSp;
  std::vector<double> mean_time;
  std::vector<double> sigma_time;
  std::vector<double> track_x;
  std::vector<double> track_x_sigma;
  std::vector<double> track_y;
  std::vector<double> track_sigma_x;
  std::vector<double> track_sigma_y;
  std::vector<bool> bound_cut;
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
  TTreeReaderValue<float> TCSphase = TTreeReaderValue<float>(*treeReader, "tcsPhase");
  TTreeReaderValue<Long64_t> s = TTreeReaderValue<Long64_t>(*treeReader, "spill");
  TTreeReaderValue<bool> isBeamTrigger = TTreeReaderValue<bool>(*treeReader, "isBeamTrigger");
  TTreeReaderValue<bool> isVITrigger = TTreeReaderValue<bool>(*treeReader, "isVITrigger");
  spectrometerTracks = new TTreeReaderValue<std::vector<TrackData>>(*treeReader, "SpectrometerData");

  TCanvas* c_test = new TCanvas("", "", 1000, 800);
  TH1D* h_xtest = new TH1D("", "", 1000, -20, 20);
  TH2D* h_test = new TH2D("", "", 1000, -20, 20, 1000, -20, 20);
  TMatrix covMat(5, 5);
            for (int i = 0; i < 5; ++i) {
                for (int j = 0; j < 5; ++j) {
                    covMat(i, j) = 0.0; // You can initialize with any default value
                }
            }

  while (treeReader->Next()) {
    if (s_last == 10000) {
        s_last = *s;
    }
    else if (*s != s_last) {
        PerSpill p;
        p.SpillNumber = s_last;
        p.timeStamps.swap(times);
        p.TCS.swap(TCSp);
        p.meanTrackTime.swap(mean_time);
        p.sigmaTrackTime.swap(sigma_time);
        p.tracks_x.swap(track_x);
        p.tracks_y.swap(track_y);
        p.tracks_sigma_x.swap(track_sigma_x);
        p.tracks_sigma_y.swap(track_sigma_y);
        p.boundaries_cut.swap(bound_cut);
        mPerSpillData.push_back(p);
        s_last = *s;
    }
    double delta_x = -8.3316 - 31e-4;
    double delta_y = -0.4436 + 56e-4;
    //double phi_z = 0;
    double phi_z = 0.0023;
    double mask_width = 0.10;

    for (const TrackData &track : **spectrometerTracks) {
      //track.Print();
      zMin = track.zMin;
      zMax = track.zMax;
      trackIsMatched = track.trackIsMatched;
      canBeMuon = track.canBeMuon;
      for (const TrackExtrapolationResult &extrapolation : track.extrapolationResults) {
        
        //mask
        //if (zMax > 2800 and (((extrapolation.x + delta_x) * (extrapolation.x + delta_x) + (extrapolation.y - delta_y) * (extrapolation.y - delta_y)) < 2 or ((extrapolation.x + delta_x) * (extrapolation.x + delta_x) + (extrapolation.y - delta_y) * (extrapolation.y - delta_y)) > 3.2) and (extrapolation.x > 7.68 + mask_width - delta_x or extrapolation.x < 7.68 - mask_width - delta_x) and (extrapolation.x > 5.12 + mask_width - delta_x or extrapolation.x < 5.12 - mask_width - delta_x) and (extrapolation.x > 2.56 + mask_width - delta_x or extrapolation.x < 2.56 - mask_width - delta_x) and (extrapolation.x > 0.00 + mask_width - delta_x or extrapolation.x < 0.00 - mask_width - delta_x) and (extrapolation.x > -2.56 + mask_width - delta_x or extrapolation.x < -2.56 - mask_width - delta_x) and (extrapolation.x > -5.12 + mask_width - delta_x or extrapolation.x < -5.12 - mask_width - delta_x) and (extrapolation.x > -7.68 + mask_width - delta_x or extrapolation.x < -7.68 - mask_width - delta_x) and (extrapolation.y > 7.68 + mask_width + delta_y or extrapolation.y < 7.68 - mask_width + delta_y) and (extrapolation.y > 5.12 + mask_width + delta_y or extrapolation.y < 5.12 - mask_width + delta_y) and (extrapolation.y > 2.56 + mask_width + delta_y or extrapolation.y < 2.56 - mask_width + delta_y) and (extrapolation.y > 0 + mask_width + delta_y or extrapolation.y < 0 - mask_width + delta_y) and (extrapolation.y > -2.56 + mask_width + delta_y or extrapolation.y < -2.56 - mask_width + delta_y) and (extrapolation.y > -5.12 + mask_width + delta_y or extrapolation.y < -5.12 - mask_width + delta_y) and (extrapolation.y > -7.68 + mask_width + delta_y or extrapolation.y < -7.68 - mask_width + delta_y)) {
        if (zMax > 2800) {
        //if (zMax > 0) {
          if ((((extrapolation.x + delta_x) * (extrapolation.x + delta_x) + (extrapolation.y - delta_y) * (extrapolation.y - delta_y)) < 2 or ((extrapolation.x + delta_x) * (extrapolation.x + delta_x) + (extrapolation.y - delta_y) * (extrapolation.y - delta_y)) > 3.2) and (extrapolation.x > 7.68 + mask_width - delta_x or extrapolation.x < 7.68 - mask_width - delta_x) and (extrapolation.x > 5.12 + mask_width - delta_x or extrapolation.x < 5.12 - mask_width - delta_x) and (extrapolation.x > 2.56 + mask_width - delta_x or extrapolation.x < 2.56 - mask_width - delta_x) and (extrapolation.x > 0.00 + mask_width - delta_x or extrapolation.x < 0.00 - mask_width - delta_x) and (extrapolation.x > -2.56 + mask_width - delta_x or extrapolation.x < -2.56 - mask_width - delta_x) and (extrapolation.x > -5.12 + mask_width - delta_x or extrapolation.x < -5.12 - mask_width - delta_x) and (extrapolation.x > -7.68 + mask_width - delta_x or extrapolation.x < -7.68 - mask_width - delta_x) and (extrapolation.y > 7.68 + mask_width + delta_y or extrapolation.y < 7.68 - mask_width + delta_y) and (extrapolation.y > 5.12 + mask_width + delta_y or extrapolation.y < 5.12 - mask_width + delta_y) and (extrapolation.y > 2.56 + mask_width + delta_y or extrapolation.y < 2.56 - mask_width + delta_y) and (extrapolation.y > 0 + mask_width + delta_y or extrapolation.y < 0 - mask_width + delta_y) and (extrapolation.y > -2.56 + mask_width + delta_y or extrapolation.y < -2.56 - mask_width + delta_y) and (extrapolation.y > -5.12 + mask_width + delta_y or extrapolation.y < -5.12 - mask_width + delta_y) and (extrapolation.y > -7.68 + mask_width + delta_y or extrapolation.y < -7.68 - mask_width + delta_y)) {
            bound_cut.push_back(1);
          }
          else {
            bound_cut.push_back(0);
          }
          times.push_back(*timeInSpill * (1.0 + alpha) * 1e09);
          TCSp.push_back(*TCSphase);
          mean_time.push_back(track.meanTime);
          sigma_time.push_back(track.sigmaTime);
          //tracks extrapolated to VMM GEM
          x = -extrapolation.x - delta_x;
          y = extrapolation.y - delta_y;
          track_x.push_back(x*cos(phi_z) - y*sin(phi_z));
          track_y.push_back(x*sin(phi_z) + y*cos(phi_z));
          //track_x.push_back(-extrapolation.x - delta_x);
          //track_y.push_back(extrapolation.y - delta_y);
          track_sigma_x.push_back(sqrt(extrapolation.covMatrix[0][0]));
          track_sigma_y.push_back(sqrt(extrapolation.covMatrix[1][1]));
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

void AmberData::TrackTime(Long64_t spill) const {
    const PerSpill* p = nullptr;
    for (auto& ps : mPerSpillData) {
        if (ps.SpillNumber == spill) {
            p = &ps;
            break;
        }
    }
    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    TCanvas* cmean = new TCanvas("", "", 1200, 700);
    TGraph* meanTime_TiS = new TGraph(p->timeStamps.size(), p->timeStamps.data(), p->meanTrackTime.data());
    meanTime_TiS->GetXaxis()->SetTitle("time in spill / ns");
    meanTime_TiS->GetYaxis()->SetTitle("meanTime / ns");
    meanTime_TiS->Draw("AP");
    cmean->SaveAs(Form("meanTime_vs_TiS_%lld.png", spill));
    TCanvas* csigma = new TCanvas("", "", 1200, 700);
    TGraph* sigmaTime_TiS = new TGraph(p->timeStamps.size(), p->timeStamps.data(), p->sigmaTrackTime.data());
    sigmaTime_TiS->GetXaxis()->SetTitle("time in spill / ns");
    sigmaTime_TiS->GetYaxis()->SetTitle("sigmaTime / ns");
    sigmaTime_TiS->Draw("AP");
    csigma->SaveAs(Form("meansigmaTime_vs_TiS_%lld.png", spill));
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
  TH1D *h_cal = new TH1D("", "; #Deltat / #mus; Entries per bin", 100, -2, 2);
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

std::tuple<double, std::vector<bool>, std::vector<double>, std::vector<double>, std::vector<double>> AmberData::CalibrateAmberTrigger(Long64_t VMM_scan_number, Long64_t AMBER_run_number, Long64_t spill, const std::vector<double> &TriggerTimeStamps, double spill_pos) {
    
    TString output_plots_time_calibration = Form("scan_%lld_run_%lld_spill_%lld_time_calibration.pdf", VMM_scan_number, AMBER_run_number, spill);
    const PerSpill* p = nullptr;
    double tdiff;
    double tdiff_calibrated;
    double cal_offset;
    double cal_alpha_prime;
    std::vector<double> TIS_timeStamps_calibrated;
    std::vector<double> tdiff_TIS;
    std::vector<double> tdiff_TIS_s;
    std::vector<double> tdiff_tdiff;
    std::vector<double> tdiff_tdiff_err;
    std::vector<double> tdiff_TIS_cal;
    std::vector<double> tdiff_tdiff_cal;

    std::vector<bool> trigger_matched;
    std::vector<double> trigger_tracktime;
    std::vector<double> trigger_TCS;
    std::vector<double> TimeInSpill_calibrated;

    int n;
    std::vector<int> match_count = {0,0,0,0,0,0,0,0,0};

    for (auto& ps : mPerSpillData) {
        if (ps.SpillNumber == spill) {
            p = &ps;
            break;
        }
    }
    std::cout << p->timeStamps.size() << std::endl;
    std::cout << TriggerTimeStamps.size() << std::endl;
    std::cout << "test2" << std::endl;
    TH1D* h1_cal = new TH1D("", "; #deltat / #mus; Entries per bin", 20e05, -10e06, 10e06);
    TH1D* h1_cal_plot = new TH1D("", "; #deltat / s; Entries per 10#mus bin", 15e05, -5, 10);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        //std::cout << j << std::endl;
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            h1_cal->Fill((p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos)/1000);   //time differences in us
            h1_cal_plot->Fill((p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos)/1e09);
        }
    }
    double peakpos_0 = h1_cal->GetMaximumBin() * 20e06 / 20e05 - 10e06;;   //highest bin should be at calibration peak
    TCanvas* c1_cal = new TCanvas("", "", 1200, 700);
    h1_cal->Draw();
    //c1_cal->SaveAs(Form("trigger_cal_1_spill_%lld.png", spill));
    c1_cal->Print(output_plots_time_calibration + "[");
    c1_cal->Print(output_plots_time_calibration);
    delete c1_cal;
    delete h1_cal;

    //plot for thesis
    TCanvas* c1_cal_plot = new TCanvas("", "", 800, 600);
    c1_cal_plot->SetLeftMargin(0.2);
    c1_cal_plot->SetBottomMargin(0.2);
    gStyle->SetOptStat(0);
    h1_cal_plot->GetXaxis()->CenterTitle();
    h1_cal_plot->GetXaxis()->SetTitleSize(0.06);
    h1_cal_plot->GetYaxis()->CenterTitle();
    h1_cal_plot->GetYaxis()->SetTitleSize(0.06);
    h1_cal_plot->Draw();
    c1_cal_plot->Update();
    c1_cal_plot->SaveAs(Form("trigger_cal_1_spill_%lld.png", spill));
    delete c1_cal_plot;
    delete h1_cal_plot;


    TH1D* h2_cal = new TH1D("", "; #deltat / #mus; Entries per bin", 40000, peakpos_0 - 20, peakpos_0 + 20);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            h2_cal->Fill((p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos) / 1000);   //time differences in us
        }
    }
    TCanvas* c2_cal = new TCanvas("", "", 1200, 700);
    h2_cal->Draw();
    //c2_cal->SaveAs(Form("trigger_cal_2_spill_%lld.png", spill));
    double peakpos_1 = h2_cal->GetMaximumBin() * 1 + 1000*(peakpos_0 - 20); // accuracy 1 ns
    delete c2_cal;
    delete h2_cal;
    TH1D* h3_cal = new TH1D("", "; #deltat / #mus; Entries per bin", 200, -2, 2);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
      n = 0;
      trigger_matched.push_back(0);
      trigger_tracktime.push_back(0);
      trigger_TCS.push_back(0);
      //std::cout << j << "   " << p->timeStamps.size() << std::endl;
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            tdiff = p->timeStamps[j] - TriggerTimeStamps[k] + spill_pos - peakpos_1 - p->TCS[j];
            h3_cal->Fill(tdiff / 1000);
            if (abs(tdiff/1000) < 1 and p->timeStamps[j] > 1.25e09) {
                tdiff_TIS.push_back(p->timeStamps[j]);
                tdiff_TIS_s.push_back(p->timeStamps[j]/1e09);
                tdiff_tdiff.push_back(tdiff);
                tdiff_tdiff_err.push_back(1.64);
            }
            if (tdiff > -1000 and tdiff < 1000) {
              trigger_matched[j] = 1;
              trigger_tracktime[j] = p->sigmaTrackTime[j];
              //trigger_tracktime[j] = TriggerTimeStamps[k];
              trigger_TCS[j] = TriggerTimeStamps[k] + 0* p->TCS[j]; //TCS phase not used here currently
              n++;
            }
        }
      match_count[n]++;
    }
    TCanvas* c3_cal = new TCanvas("", "", 1200, 700);
    h3_cal->Draw();
    //c3_cal->SaveAs(Form("trigger_cal_3_spill_%lld.png", spill));
    TCanvas* c_tdiff_TiS = new TCanvas("", "", 1200, 700);
    TGraph* tdiff_TiS = new TGraph(tdiff_TIS.size(), tdiff_TIS.data(), tdiff_tdiff.data());
    TGraphErrors* tdiff_TiS_s = new TGraphErrors(tdiff_TIS_s.size(), tdiff_TIS_s.data(), tdiff_tdiff.data(), nullptr, nullptr);
    //TGraphErrors* tdiff_TiS_s = new TGraphErrors(tdiff_TIS_s.size(), tdiff_TIS_s.data(), tdiff_tdiff.data(), nullptr, tdiff_tdiff_err.data());
    tdiff_TiS->GetXaxis()->SetTitle("time in spill / ns");
    tdiff_TiS->GetYaxis()->SetTitle("#deltat / ns");
    tdiff_TiS_s->GetXaxis()->SetTitle("Time in Spill / s");
    tdiff_TiS_s->GetYaxis()->SetTitle("#deltat / ns");
    TF1* f_clocks = new TF1("f_clocks", "[0] + [1] * x", 1.25e09, 5.6e09);
    f_clocks->SetParameters(-300, 3e-8);
    //TF1* f_clocks = new TF1("f_clocks", "[1] * pow(x-[0], 1) + [1] * pow(x-[0], 1) + [2] * pow(x-[0], 2) + [3] * pow(x-[0], 3) + [4] * pow(x-[0], 4) + [5] * pow(x-[0], 5) + [6] * pow(x-[0], 6)", 1.25e09, 5.6e09);
    //f_clocks->SetParameters(3.4e09, 3e-8, 11e-16, 1e-27, 1e-36, 1e-45, 1e-54);
    tdiff_TiS->Fit("f_clocks");
    TPaveText* paramclocks = new TPaveText(0.1, 0.75, 0.37, 0.85, "NDC");
    paramclocks->AddText(Form("offset: %.2f #pm %.2f ns", f_clocks->GetParameter(0), f_clocks->GetParError(0)));
    paramclocks->AddText(Form("#alpha': (%.6f #pm %.6f)e-05", f_clocks->GetParameter(1) * 1e05, f_clocks->GetParError(1) * 1e05));
    cal_offset = f_clocks->GetParameter(0);
    cal_alpha_prime = f_clocks->GetParameter(1);
    //paramclocks->AddText(Form("t0: %.7f #pm %.7f s", f_clocks->GetParameter(2)/1e09, f_clocks->GetParError(2)/1e09));
    tdiff_TiS->Draw("AP");
    c_tdiff_TiS->Update();
    paramclocks->Draw();
    c_tdiff_TiS->Print(output_plots_time_calibration);

    //plot for thesis
    TCanvas* c_tdiff_TiS_plot = new TCanvas("", "", 800, 600);
    c_tdiff_TiS_plot->SetBottomMargin(0.2);
    c_tdiff_TiS_plot->SetLeftMargin(0.2);
    tdiff_TiS_s->SetTitle(" ");
    tdiff_TiS_s->GetXaxis()->CenterTitle();
    tdiff_TiS_s->GetYaxis()->CenterTitle();
    tdiff_TiS_s->GetXaxis()->SetTitleSize(0.06);
    tdiff_TiS_s->GetYaxis()->SetTitleSize(0.06);
    tdiff_TiS_s->Draw("AP");
    TPaveText* paramclocks_s = new TPaveText(0.2, 0.75, 0.6, 0.9, "NDC");
    paramclocks_s->AddText(Form("offset: %.1f #pm %.1f ns", f_clocks->GetParameter(0), f_clocks->GetParError(0)));
    paramclocks_s->AddText(Form("#alpha': (%.6f #pm %.6f)e-05", f_clocks->GetParameter(1) * 1e05, f_clocks->GetParError(1) * 1e05));
    paramclocks_s->Draw();
    tdiff_TiS_s->GetYaxis()->SetRangeUser(-400, 200);
    TF1* f_clocks_s = new TF1("f_clocks", "[0] + [1] * x *1e09", -2, 8);
    f_clocks_s->SetParameter(0, f_clocks->GetParameter(0));
    f_clocks_s->SetParameter(1, f_clocks->GetParameter(1));
    f_clocks_s->Draw("same");
    c_tdiff_TiS_plot->SaveAs(Form("col_rec_triggerdiff_vs_TiS_%lld.png", spill));

    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        //TIS_timeStamps_calibrated.push_back(p->timeStamps[j] * (1 - cal_alpha_prime) - cal_offset);
        TIS_timeStamps_calibrated.push_back(p->timeStamps[j] - f_clocks->Eval(p->timeStamps[j]));
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            tdiff_calibrated = TIS_timeStamps_calibrated[j] - TriggerTimeStamps[k] + spill_pos - peakpos_1 - p->TCS[j];
            if (abs(tdiff_calibrated) < 35) {
                tdiff_TIS_cal.push_back(p->timeStamps[j]);
                tdiff_tdiff_cal.push_back(tdiff_calibrated);
            }
        }
    }
    TCanvas* c_tdiff_TiS_cal = new TCanvas("", "", 1200, 700);
    TGraph* tdiff_TiS_cal = new TGraph(tdiff_TIS_cal.size(), tdiff_TIS_cal.data(), tdiff_tdiff_cal.data());
    tdiff_TiS_cal->GetXaxis()->SetTitle("time in spill / ns");
    tdiff_TiS_cal->GetYaxis()->SetTitle("#deltat / ns");
    tdiff_TiS_cal->Draw("AP");
    //c_tdiff_TiS_cal->SaveAs(Form("triggerdiff_vs_TiS_%lld_calibrated.png", spill));
    c_tdiff_TiS_cal->Print(output_plots_time_calibration);

    TCanvas* c4_cal = new TCanvas("", "", 1200, 700);
    gStyle->SetOptStat(1111);
    TH1D* h4_cal = new TH1D("", "; #Deltat / ns; Entries per bin", 200, -10, 10);
    for (long unsigned int j = 0; j < p->timeStamps.size(); j++) {
        for (long unsigned int k = 0; k < TriggerTimeStamps.size(); k++) {
            h4_cal->Fill(TIS_timeStamps_calibrated[j] - TriggerTimeStamps[k] + spill_pos - peakpos_1 - p->TCS[j]);
        }
    }
    h4_cal->Draw();
    //c4_cal->SaveAs(Form("trigger_cal_final_spill_%lld.png", spill));
    c4_cal->Print(output_plots_time_calibration);
    c4_cal->Print(output_plots_time_calibration + "]");

    std::cout << "track-trigger-matching   " << 0 << "  " << match_count[0] << std::endl; 
    std::cout << "track-trigger-matching   " << 1 << "  " << match_count[1] << std::endl; 
    std::cout << "track-trigger-matching   " << 2 << "  " << match_count[2] << std::endl; 
    std::cout << "track-trigger-matching   " << 3 << "  " << match_count[3] << std::endl; 
    std::cout << "track-trigger-matching   " << 4 << "  " << match_count[4] << std::endl;
    return std::make_tuple(peakpos_1, trigger_matched, trigger_tracktime, trigger_TCS, TIS_timeStamps_calibrated);

}

void AmberData::Residuals(Long64_t VMM_scan_number, Long64_t AMBER_run_number, Long64_t spill, const std::vector<int>& VMMxfec, const std::vector<int>& VMMyfec, const std::vector<double>& VMMxTimeStamps, const std::vector<double>& VMMyTimeStamps, const std::vector<double>& VMM_xpos, const std::vector<double>& VMM_ypos, const std::vector<int>& x_adc, const std::vector<int>& y_adc, const std::vector<int>& x_size, const std::vector<int>& y_size, double timeshift, const std::vector<bool>& tracktime_matched, const std::vector<double>& tracktime, const std::vector<double>& trigger_TCS, const std::vector<double>& TimeInSpill_calibrated) {
    
    TString output_root_file = Form("scan_%lld_run_%lld_spill_%lld.root", VMM_scan_number, AMBER_run_number, spill);
    TString output_plots_residuals = Form("scan_%lld_run_%lld_spill_%lld_residuals.pdf", VMM_scan_number, AMBER_run_number, spill);
    TString output_efficiency = Form("scan_%lld_run_%lld_spill_%lld_efficiency.txt", VMM_scan_number, AMBER_run_number, spill);
    
    const PerSpill* p = nullptr;
    for (auto& ps : mPerSpillData) {
        if (ps.SpillNumber == spill) {
            p = &ps;
            break;
        }
    }
    //double x_pitch = 0.04;
    //double y_pitch = 0.04;
    double x_pitch = 0.04/1.000521*1.001;
    double y_pitch = 0.04/1.000861*1.0007;


    double time_cut = 80;
    double spacial_cut = 0.07;


    std::vector<std::string> residual_data_lines;
    std::string residual_data_line;
    std::ifstream residual_data_file(Form("scan_%lld_residual_data.txt", VMM_scan_number));
    if (!residual_data_file.is_open()) {
      residual_data_lines.push_back(Form("Voltage_Scan_%lld_Plot_Data", VMM_scan_number));
      residual_data_lines.push_back("int amount_tracks_data[20] = {");
      residual_data_lines.push_back("int amount_p0_x_data[20] = {");
      residual_data_lines.push_back("int amount_p0_y_data[20] = {");
      residual_data_lines.push_back("int amount_tracks_mask_data[20] = {");
      residual_data_lines.push_back("int amount_p0_x_mask_data[20] = {");
      residual_data_lines.push_back("int amount_p0_y_mask_data[20] = {");
      residual_data_lines.push_back("double fec1_data[20] = {");
      residual_data_lines.push_back("double fec1_err_data[20] = {");
      residual_data_lines.push_back("double fec1_mask_data[20] = {");
      residual_data_lines.push_back("double fec1_mask_err_data[20] = {");
      residual_data_lines.push_back("double fec2_data[20] = {");
      residual_data_lines.push_back("double fec2_err_data[20] = {");
      residual_data_lines.push_back("double fec2_mask_data[20] = {");
      residual_data_lines.push_back("double fec2_mask_err_data[20] = {");
      residual_data_lines.push_back("double fec4_data[20] = {");
      residual_data_lines.push_back("double fec4_err_data[20] = {");
      residual_data_lines.push_back("double fec4_mask_data[20] = {");
      residual_data_lines.push_back("double fec4_mask_err_data[20] = {");
      residual_data_lines.push_back("double fec5_data[20] = {");
      residual_data_lines.push_back("double fec5_err_data[20] = {");
      residual_data_lines.push_back("double fec5_mask_data[20] = {");
      residual_data_lines.push_back("double fec5_mask_err_data[20] = {");
      residual_data_lines.push_back("double spat_res_x_data[20] = {");
      residual_data_lines.push_back("double spat_res_x_err_data[20] = {");
      residual_data_lines.push_back("double spat_res_x_mask_data[20] = {");
      residual_data_lines.push_back("double spat_res_x_mask_err_data[20] = {");
      residual_data_lines.push_back("double spat_res_y_data[20] = {");
      residual_data_lines.push_back("double spat_res_y_err_data[20] = {");
      residual_data_lines.push_back("double spat_res_y_mask_data[20] = {");
      residual_data_lines.push_back("double spat_res_y_mask_err_data[20] = {");
      residual_data_lines.push_back("int amount_BG_x_data[20] = {");
      residual_data_lines.push_back("int amount_BG_y_data[20] = {");
      residual_data_lines.push_back("int amount_BG_x_mask_data[20] = {");
      residual_data_lines.push_back("int amount_BG_y_mask_data[20] = {");
      
    } else {
      while (std::getline(residual_data_file, residual_data_line)) {
        residual_data_lines.push_back(residual_data_line);
      }
    }
    residual_data_file.close();


    int N_tracks;
    std::vector<int> hits_per_track_x = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_track_y = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_track_x_BG = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_track_y_BG = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_mask_track_x = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_mask_track_y = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_mask_track_x_BG = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<int> hits_per_mask_track_y_BG = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
    std::vector<double> Time_In_Spill;
    std::vector<double> time_differences;
    std::vector<double> x_differences;
    int hits_count;
    int hits_count_BG;
    double timediff;
    double xdiff;
    double ydiff;
    int progress_tracker = 0;
    std::cout << " I am here " << std::endl;
    //gStyle->SetLegendTextSize(0.04);
    gStyle->SetFuncWidth(1.7);

    std::vector<double> time_fit;
    for (int a = 0; a < 20000; a++) {
      time_fit.push_back(8.024 + (-0.149 - 8.024)/(1 + pow(a/538.325, 3.697)));
    }

    TH2D* h_amber_2d_profile = new TH2D("reference beam profile", "Reference Beam Profile; x/cm; y/cm", 100, -20, 20, 100, -20, 20);
    TH1D* h_amber_time_profile = new TH1D("reference spill profile", "Reference Spill Profile; t/s; Entries", 200, 0, 6);
    TH1D* h_VMM_time_profile = new TH1D("VMM Spill Profile", "VMM Spill Profile; t/s; Entries", 200, VMMxTimeStamps[0]/1e09, VMMxTimeStamps[VMMxTimeStamps.size() - 1]/1e09);

    TH1D* h_amber_time_profile_cut = new TH1D("reference spill profile", "Reference Spill Profile; TimeInSpill/s; Entries", 500, 3, 3.5);
    TH1D* h_VMM_time_profile_cut = new TH1D("VMM Spill Profile", "VMM Spill Profile; TimeInSpill/s; Entries", 500, 3, 3.5);

    TH1D* h_amber_time_profile_cut2 = new TH1D("reference spill profile", "Reference Spill Profile; TimeInSpill/ms; Entries", 50, 0, 100);
    TH1D* h_VMM_time_profile_cut2 = new TH1D("VMM Spill Profile", "VMM Spill Profile; TimeInSpill/#mus; Entries", 200, 0, 400);

    for (long unsigned int i_amber_tracks = 0; i_amber_tracks < p->timeStamps.size(); i_amber_tracks++) {
      h_amber_2d_profile->Fill(p->tracks_x[i_amber_tracks], p->tracks_y[i_amber_tracks]);
      h_amber_time_profile->Fill(p->timeStamps[i_amber_tracks]/1e09);
      h_amber_time_profile_cut->Fill(p->timeStamps[i_amber_tracks]/1e09);
      h_amber_time_profile_cut2->Fill((p->timeStamps[i_amber_tracks]/1e09 - 3.0)*1000);
    }

    for (long unsigned int i_VMM_tracks = 0; i_VMM_tracks < VMMxTimeStamps.size(); i_VMM_tracks++) {
      h_VMM_time_profile->Fill(VMMxTimeStamps[i_VMM_tracks]/1e09);
      h_VMM_time_profile_cut->Fill((VMMxTimeStamps[i_VMM_tracks] + timeshift)/1e09);
      h_VMM_time_profile_cut2->Fill(((VMMxTimeStamps[i_VMM_tracks] + timeshift)/1e09 - 3.0)*1000000);
    }

    TCanvas* c_beam_profile = new TCanvas("", "", 1800, 1500);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_amber_2d_profile->GetXaxis()->CenterTitle();
    h_amber_2d_profile->GetXaxis()->SetTitleSize(0.05);
    h_amber_2d_profile->GetYaxis()->CenterTitle();
    h_amber_2d_profile->GetYaxis()->SetTitleSize(0.05);
    h_amber_2d_profile->Draw();
    c_beam_profile->Print(output_plots_residuals + "[");
    c_beam_profile->Print(output_plots_residuals);


    TCanvas* c_spill_profies = new TCanvas("", "", 1200, 500);
    c_spill_profies->Divide(2);
    c_spill_profies->cd(1);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_amber_time_profile->GetXaxis()->CenterTitle();
    h_amber_time_profile->GetXaxis()->SetTitleSize(0.05);
    h_amber_time_profile->GetYaxis()->CenterTitle();
    h_amber_time_profile->GetYaxis()->SetTitleSize(0.05);
    h_amber_time_profile->Draw();
    c_spill_profies->cd(2);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_VMM_time_profile->GetXaxis()->CenterTitle();
    h_VMM_time_profile->GetXaxis()->SetTitleSize(0.05);
    h_VMM_time_profile->GetYaxis()->CenterTitle();
    h_VMM_time_profile->GetYaxis()->SetTitleSize(0.05);
    h_VMM_time_profile->Draw();
    c_spill_profies->Print(output_plots_residuals);

    TCanvas* c_spill_profies_cut = new TCanvas("", "", 1200, 500);
    c_spill_profies_cut->Divide(2);
    c_spill_profies_cut->cd(1);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_amber_time_profile_cut->GetXaxis()->CenterTitle();
    h_amber_time_profile_cut->GetXaxis()->SetTitleSize(0.05);
    h_amber_time_profile_cut->GetYaxis()->CenterTitle();
    h_amber_time_profile_cut->GetYaxis()->SetTitleSize(0.05);
    h_amber_time_profile_cut->Draw();
    c_spill_profies_cut->cd(2);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_VMM_time_profile_cut->GetXaxis()->CenterTitle();
    h_VMM_time_profile_cut->GetXaxis()->SetTitleSize(0.05);
    h_VMM_time_profile_cut->GetYaxis()->CenterTitle();
    h_VMM_time_profile_cut->GetYaxis()->SetTitleSize(0.05);
    h_VMM_time_profile_cut->Draw();
    c_spill_profies_cut->Print(output_plots_residuals);

    TCanvas* c_spill_profies_cut2 = new TCanvas("", "", 1200, 500);
    c_spill_profies_cut2->Divide(2);
    c_spill_profies_cut2->cd(1);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_amber_time_profile_cut2->GetXaxis()->CenterTitle();
    h_amber_time_profile_cut2->GetXaxis()->SetTitleSize(0.05);
    h_amber_time_profile_cut2->GetYaxis()->CenterTitle();
    h_amber_time_profile_cut2->GetYaxis()->SetTitleSize(0.05);
    h_amber_time_profile_cut2->Draw();
    c_spill_profies_cut2->cd(2);
    gStyle->SetOptStat(0);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_VMM_time_profile_cut2->GetXaxis()->CenterTitle();
    h_VMM_time_profile_cut2->GetXaxis()->SetTitleSize(0.05);
    h_VMM_time_profile_cut2->GetYaxis()->CenterTitle();
    h_VMM_time_profile_cut2->GetYaxis()->SetTitleSize(0.05);
    h_VMM_time_profile_cut2->Draw();
    //c_spill_profies_cut2->Print(output_plots_residuals);



    TH1D* h_res_time_x_fec1 = new TH1D("dt_fec1", "#deltat_fec1; #deltat_x_fec1 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_time_x_fec4 = new TH1D("dt_fec4", "#deltat_fec4; #deltat_x_fec4 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_time_y_fec2 = new TH1D("dt_fec2", "#deltat_fec2; #deltat_y_fec2 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_time_y_fec5 = new TH1D("dt_fec5", "#deltat_fec5; #deltat_y_fec5 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_time = new TH1D("dt", "#deltat; #deltat / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_x = new TH1D("dx", "#deltax; #deltax / #mum; Entries per 16 #mum bin", 100, -800, 800);
    TH1D* h_res_y = new TH1D("dy", "#deltay; #deltay / #mum; Entries per 16 #mum bin", 100, -800, 800);
    TH1D* h_res_mask_time_x_fec1 = new TH1D("dt_fec1_mask", "#deltat_fec1_mask; #deltat_x_fec1 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_mask_time_x_fec4 = new TH1D("dt_fec4_mask", "#deltat_fec4_mask; #deltat_x_fec4 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_mask_time_y_fec2 = new TH1D("dt_fec2_mask", "#deltat_fec2_mask; #deltat_y_fec2 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_mask_time_y_fec5 = new TH1D("dt_fec5_mask", "#deltat_fec5_mask; #deltat_y_fec5 / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_mask_time = new TH1D("dt_mask", "#deltat_mask; #deltat / ns; Entries per 2.4ns bin", 100, -120, 120);
    TH1D* h_res_mask_x = new TH1D("dx_mask", "#deltax_mask; #deltax / #mum; Entries per 16 #mum bin", 100, -800, 800);
    TH1D* h_res_mask_y = new TH1D("dy_mask", "#deltay_mask; #deltay / #mum; Entries per 16 #mum bin", 100, -800, 800);
    TH2D* h_missing_x = new TH2D("", "; x / cm; y / cm", 1000, -5, 10, 1000, -5, 10);
    TH2D* h_missing_y = new TH2D("", "; x / cm; y / cm", 1000, -5, 10, 1000, -5, 10);
    TH2D* h_missing_x_mask = new TH2D("", "; x / cm; y / cm", 1000, -5, 10, 1000, -5, 10);
    TH2D* h_missing_y_mask = new TH2D("", "; x / cm; y / cm", 1000, -5, 10, 1000, -5, 10);
    TH2D* h_x_adc = new TH2D("", "", 300, 0, 3000, 300, 0, 1000);
    TH2D* h_y_adc = new TH2D("", "", 300, 0, 3000, 300, 0, 1000);
    TH2D* h_x_size = new TH2D("", "", 14, 0, 14, 100, 0, 1000);
    TH2D* h_y_size = new TH2D("", "", 14, 0, 14, 100, 0, 1000);
    TH2D* h_dx_x = new TH2D("", "#deltax(x)", 400, -16, 16, 250, -2000, 2000);
    TH2D* h_dy_y = new TH2D("", "#deltay(y)", 400, -16, 16, 250, -2000, 2000);
    TH2D* h_dx_y = new TH2D("", "#deltax(y)", 400, -16, 16, 250, -2000, 2000);
    TH2D* h_dy_x = new TH2D("", "#deltay(x)", 400, -16, 16, 250, -2000, 2000);
    TH2D* h_dx_sigmatrack = new TH2D("", "", 300, 0, 200, 300, 0, 1000);
    TH2D* h_dy_sigmatrack = new TH2D("", "", 300, 0, 200, 300, 0, 1000);
    TH2D* h_size_adc_x = new TH2D("", "", 14, 0, 14, 40, 0, 5000);
    TH2D* h_size_adc_y = new TH2D("", "", 14, 0, 14, 40, 0, 5000);
    TH2D* h_size_tot_adc_x = new TH2D("", "", 14, 0, 14, 40, 0, 10000);
    TH2D* h_size_tot_adc_y = new TH2D("", "", 14, 0, 14, 40, 0, 10000);
    TH2D* h_adc_dt = new TH2D("", "",300, 0, 3000, 400 ,-80, 80);
    TH2D* h_adc_dt_cor = new TH2D("", "",300, 0, 3000, 400 ,-250, 100);
    //TH2D* h_adc_dt_cor = new TH2D("", "",300, 0, 3000, 400 ,-80, 80);
    TH2D* h_size_dt = new TH2D("", "",300, -150, 150, 15 ,0, 15);
    TH2D* h_size_size = new TH2D("", "",10, 0, 10, 10 ,0, 10);
    TH2D* h_adc_adc = new TH2D("", "",100, 0, 3000, 100 ,0, 3000);
    h_adc_dt->GetYaxis()->SetTitle("#deltat/ns");
    h_adc_dt->GetXaxis()->SetTitle("ADC");
    h_adc_dt_cor->GetYaxis()->SetTitle("#deltat/ns");
    h_adc_dt_cor->GetXaxis()->SetTitle("ADC");
    h_size_dt->GetXaxis()->SetTitle("#deltat/ns");
    h_size_dt->GetYaxis()->SetTitle("size");
    h_x_adc->GetXaxis()->SetTitle("ADC");
    h_x_adc->GetYaxis()->SetTitle("|#deltax|");
    h_x_size->GetXaxis()->SetTitle("cluster_size");
    h_x_size->GetYaxis()->SetTitle("|#deltax|");
    h_dx_x->GetXaxis()->SetTitle("x/cm");
    h_dx_x->GetYaxis()->SetTitle("#deltax/#mum");
    h_dy_y->GetXaxis()->SetTitle("y/cm");
    h_dy_y->GetYaxis()->SetTitle("#deltay/#mum");
    h_dx_y->GetXaxis()->SetTitle("y/cm");
    h_dx_y->GetYaxis()->SetTitle("#deltax/#mum");
    h_dy_x->GetXaxis()->SetTitle("x/cm");
    h_dy_x->GetYaxis()->SetTitle("#deltay/#mum");
    h_dx_sigmatrack->GetXaxis()->SetTitle("sigmaX_track/#mum");
    h_dx_sigmatrack->GetYaxis()->SetTitle("|#deltax|/#mum");
    h_dy_sigmatrack->GetXaxis()->SetTitle("sigmaY_track/#mum");
    h_dy_sigmatrack->GetYaxis()->SetTitle("|#deltay|/#mum");
    h_size_adc_x->GetXaxis()->SetTitle("cluster_size");
    h_size_adc_x->GetYaxis()->SetTitle("ADC");
    h_size_adc_y->GetXaxis()->SetTitle("cluster_size");
    h_size_adc_y->GetYaxis()->SetTitle("ADC");
    h_size_tot_adc_x->GetXaxis()->SetTitle("cluster_size");
    h_size_tot_adc_x->GetYaxis()->SetTitle("ADC");
    h_size_tot_adc_y->GetXaxis()->SetTitle("cluster_size");
    h_size_tot_adc_y->GetYaxis()->SetTitle("ADC");
    h_size_size->GetXaxis()->SetTitle("x_size");
    h_size_size->GetYaxis()->SetTitle("y_size");
    h_adc_adc->GetXaxis()->SetTitle("x_adc");
    h_adc_adc->GetYaxis()->SetTitle("y_adc");

    TFile *output_file = new TFile(output_root_file, "RECREATE");
    TTree *output_tree_1 = new TTree("Amber_Tracks", "");

    int spill_number_out1;
    double TimeInSpill_out1;
    double AMBER_time_out;
    double x_position_out1;
    double y_position_out1;
    std::vector<double> VMM_time_x_out;
    std::vector<double> VMM_x_out;
    std::vector<int> VMM_size_x_out;
    std::vector<double> VMM_time_y_out;
    std::vector<double> VMM_y_out;
    std::vector<int> VMM_size_y_out;
    int VMM_xHit_out1;
    int VMM_yHit_out1;
    int VMM_BG_xHit_out1;
    int VMM_BG_yHit_out1;
    int x_size_last = 0;
    int y_size_last = 0;
    int x_adc_last = 0;
    int y_adc_last = 0;
    int x_size_first = 0;
    int y_size_first = 0;
    int x_adc_first = 0;
    int y_adc_first = 0;
    int amount_used_tracks = 0;
    int amount_used_tracks_mask = 0;
    bool bound_mask;
    double dt1_out, dt2_out, dt4_out, dt5_out, dt_all_out;
    double dx_out, dy_out;
    double dt1_mask_out, dt2_mask_out, dt4_mask_out, dt5_mask_out, dt_all_mask_out;
    double dx_mask_out, dy_mask_out;
    output_tree_1->Branch("spill", &spill_number_out1, "spill/I");
    output_tree_1->Branch("TimeInSpill", &TimeInSpill_out1, "TimeInSpill/D");
    output_tree_1->Branch("amber_x", &x_position_out1, "amber_x/D");
    output_tree_1->Branch("amber_y", &y_position_out1, "amber_y/D");
    output_tree_1->Branch("amber_time", &AMBER_time_out, "amber_time/D");
    output_tree_1->Branch("VMM_time_x", &VMM_time_x_out);
    output_tree_1->Branch("VMM_time_y", &VMM_time_y_out);
    output_tree_1->Branch("VMM_pos_x", &VMM_x_out);
    output_tree_1->Branch("VMM_pos_y", &VMM_y_out);
    output_tree_1->Branch("VMM_size_x", &VMM_size_x_out);
    output_tree_1->Branch("VMM_size_y", &VMM_size_y_out);
    output_tree_1->Branch("VMM_xHits_per_track", &VMM_xHit_out1, "VMM_xHit/I");
    output_tree_1->Branch("VMM_yHits_per_track", &VMM_yHit_out1, "VMM_yHit/I");
    output_tree_1->Branch("VMM_BG_xHits_per_track", &VMM_BG_xHit_out1, "VMM_BG_xHit/I");
    output_tree_1->Branch("VMM_BG_yHits_per_track", &VMM_BG_yHit_out1, "VMM_BG_yHit/I");
    output_tree_1->Branch("bound_mask", &bound_mask, "bound_mask/O");
    
    output_tree_1->Branch("dt1", &dt1_out, "dt1/D");
    output_tree_1->Branch("dt2", &dt2_out, "dt2/D");
    output_tree_1->Branch("dt4", &dt4_out, "dt4/D");
    output_tree_1->Branch("dt5", &dt5_out, "dt5/D");
    output_tree_1->Branch("dt_all", &dt_all_out, "dt1/D");
    output_tree_1->Branch("dx", &dx_out, "dx/D");
    output_tree_1->Branch("dy", &dy_out, "dy/D");
    output_tree_1->Branch("dt1_mask", &dt1_mask_out, "dt1_mask/D");
    output_tree_1->Branch("dt2_mask", &dt2_mask_out, "dt2_mask/D");
    output_tree_1->Branch("dt4_mask", &dt4_mask_out, "dt4_mask/D");
    output_tree_1->Branch("dt5_mask", &dt5_mask_out, "dt5_mask/D");
    output_tree_1->Branch("dt_all_mask", &dt_all_mask_out, "dt_all_mask/D");
    output_tree_1->Branch("dx_mask", &dx_mask_out, "dx_mask/D");
    output_tree_1->Branch("dy_mask", &dy_mask_out, "dy_mask/D");
    

    //for (long unsigned int j = 500; j < (p->timeStamps.size()); j++) {
    for (long unsigned int j = 2000; j < (p->timeStamps.size()); j++) {
    //for (long unsigned int j = 2000; j < 4000; j++) {
        amount_used_tracks++;
        if (p->boundaries_cut[j] == true) {
          amount_used_tracks_mask++;
        }
        N_tracks = p->timeStamps.size();

        spill_number_out1 = spill;
        TimeInSpill_out1 = TimeInSpill_calibrated[j];
        x_position_out1 = p->tracks_x[j];
        y_position_out1 = p->tracks_y[j];
        AMBER_time_out = TimeInSpill_calibrated[j] - timeshift - 1*p->TCS[j] + 1*p->meanTrackTime[j];
        bound_mask = p->boundaries_cut[j];

        if (progress_tracker == 100) {
          std::cout << "amber_cal_residuals spill  " << spill << ":  " << j << " out of " << N_tracks << std::endl;
          progress_tracker = 1;
        }
        else {
          progress_tracker++;
        }
        hits_count = 0;
        hits_count_BG = 0;
        for (long unsigned int k = 0; k < VMMxTimeStamps.size(); k++) {
            timediff = TimeInSpill_calibrated[j] - VMMxTimeStamps[k] - timeshift - 1*p->TCS[j] + 1*p->meanTrackTime[j] - time_fit[x_adc[k]];
            xdiff = x_pitch*VMM_xpos[k] - p->tracks_x[j];
	          if (std::abs(xdiff) < spacial_cut) {
                h_size_dt->Fill(timediff, x_size[k]);
                h_adc_dt->Fill(x_adc[k], timediff + time_fit[x_adc[k]]);
                h_adc_dt_cor->Fill(x_adc[k], timediff);
            }
            if (std::abs(xdiff) < spacial_cut && std::abs(timediff) < 2.5*time_cut) {
              if (VMMxfec[k] == 1) {
                h_res_time_x_fec1->Fill(timediff);
                dt1_out = timediff;
                output_tree_1->GetBranch("dt1")->Fill();
                if (p->boundaries_cut[j] == true) {
                  h_res_mask_time_x_fec1->Fill(timediff);
                  dt1_mask_out = timediff;
                  output_tree_1->GetBranch("dt1_mask")->Fill();
                }
              }
              if (VMMxfec[k] == 4) {
                h_res_time_x_fec4->Fill(timediff);
                dt4_out = timediff;
                output_tree_1->GetBranch("dt4")->Fill();
                if (p->boundaries_cut[j] == true) {
                  h_res_mask_time_x_fec4->Fill(timediff);
                  dt4_mask_out = timediff;
                  output_tree_1->GetBranch("dt4_mask")->Fill();
                }
              }
              h_res_time->Fill(timediff);
              dt_all_out = timediff;
              output_tree_1->GetBranch("dt_all")->Fill();
              if (p->boundaries_cut[j] == true) {
                h_res_mask_time->Fill(timediff);
                dt_all_mask_out = timediff;
                output_tree_1->GetBranch("dt_all_mask")->Fill();
              }
            }
            if (std::abs(timediff) < time_cut) {
                h_res_x->Fill(1000*10*xdiff);
                dx_out = 1000*10*xdiff;
                output_tree_1->GetBranch("dx")->Fill();
                if (p->boundaries_cut[j] == true) {
                  h_res_mask_x->Fill(1000*10*xdiff);
                  dx_mask_out = 1000*10*xdiff;
                  output_tree_1->GetBranch("dx_mask")->Fill();
                }
                h_x_adc->Fill(x_adc[k], std::abs(1000*10*xdiff));
                h_x_size->Fill(x_size[k], std::abs(1000*10*xdiff));
                h_dx_x->Fill(p->tracks_x[j], 1000*10*xdiff);
                if (std::abs(5*p->tracks_x[j]) < std::abs(p->tracks_y[j]) && std::abs(p->tracks_x[j]) > 0.2) {
                  h_dx_y->Fill(p->tracks_y[j], 1000*10*xdiff);
                }
                h_dx_sigmatrack->Fill(1000*10*p->tracks_sigma_x[j], std::abs(1000*10*xdiff));
                if (std::abs(xdiff) < spacial_cut) {
                    VMM_time_x_out.push_back(VMMxTimeStamps[k] + time_fit[x_adc[k]]);
                    VMM_x_out.push_back(x_pitch*VMM_xpos[k]);
                    VMM_size_x_out.push_back(x_size[k]);
                    h_size_adc_x->Fill(x_size[k], x_adc[k]);
                    hits_count++;
                    Time_In_Spill.push_back(p->timeStamps[j]);
                    time_differences.push_back(timediff);
                    x_differences.push_back(1000*10*xdiff);
                    if (hits_count == 2) {
                      x_size_first = x_size_last;
                      x_adc_first = x_adc_last;
                    }
                    x_size_last = x_size[k];
                    x_adc_last = x_adc[k];
                }
            }
            if (std::abs(timediff - 10000) < time_cut) {
                if (std::abs(xdiff) < spacial_cut) {
                    hits_count_BG++;
                }
            }
        }
        if (hits_count == 0) {
          h_missing_x->Fill(p->tracks_x[j], p->tracks_y[j]);
          if (p->boundaries_cut[j] == true) {
            h_missing_x_mask->Fill(p->tracks_x[j], p->tracks_y[j]);
          } 
        }
        VMM_xHit_out1 = hits_count;
        VMM_BG_xHit_out1 = hits_count_BG;
        hits_per_track_x[hits_count]++;
        hits_per_track_x_BG[hits_count_BG]++;
        if (p->boundaries_cut[j] == true) {
          hits_per_mask_track_x[hits_count]++;
          hits_per_mask_track_x_BG[hits_count_BG]++;        
        }
        hits_count = 0;
        hits_count_BG = 0;
        for (long unsigned int K = 0; K < VMMyTimeStamps.size(); K++) {
            //time_fit = 8.024 + (-0.149 - 8.024)/(1 + pow(y_adc[K]/538.325, 3.697));
            //time_fit = 0;
            timediff = TimeInSpill_calibrated[j] - VMMyTimeStamps[K] - timeshift - 1*p->TCS[j] + 1*p->meanTrackTime[j] - time_fit[y_adc[K]];
            ydiff = y_pitch * VMM_ypos[K] - p->tracks_y[j];
            if (std::abs(ydiff) < spacial_cut && std::abs(timediff) < 2.5*time_cut) {
                if (VMMyfec[K] == 2) {
                    h_res_time_y_fec2->Fill(timediff);
                    dt2_out = timediff;
                    output_tree_1->GetBranch("dt2")->Fill();
                    if (p->boundaries_cut[j] == true) {
                        h_res_mask_time_y_fec2->Fill(timediff);
                        dt2_mask_out = timediff;
                        output_tree_1->GetBranch("dt2_mask")->Fill();
                    }
                }
                if (VMMyfec[K] == 5) {
                    h_res_time_y_fec5->Fill(timediff);
                    dt5_out = timediff;
                    output_tree_1->GetBranch("dt5")->Fill();
                    if (p->boundaries_cut[j] == true) {
                        h_res_mask_time_y_fec5->Fill(timediff);
                        dt5_mask_out = timediff;
                        output_tree_1->GetBranch("dt5_mask")->Fill();
                    }
                }
                h_res_time->Fill(timediff);
                dt_all_out = timediff;
                output_tree_1->GetBranch("dt_all")->Fill();
                if (p->boundaries_cut[j] == true) {
                    h_res_mask_time->Fill(timediff);
                    dt_all_mask_out = timediff;
                    output_tree_1->GetBranch("dt_all_mask")->Fill();
                }
            }
            if (std::abs(timediff) < time_cut) {
                h_res_y->Fill(1000*10*ydiff);
                dy_out = 1000*10*ydiff;
                output_tree_1->GetBranch("dy")->Fill();
                if (p->boundaries_cut[j] == true) {
                  h_res_mask_y->Fill(1000*10*ydiff);
                  dy_mask_out = 1000*10*ydiff;
                  output_tree_1->GetBranch("dy_mask")->Fill();
                }
                h_dy_y->Fill(p->tracks_y[j], 1000*10*ydiff);
                if (std::abs(p->tracks_x[j]) > std::abs(5*p->tracks_y[j]) && std::abs(p->tracks_y[j]) > 0.2) {
                  h_dy_x->Fill(p->tracks_x[j], 1000*10*ydiff);
                }
                h_dy_sigmatrack->Fill(1000*10*p->tracks_sigma_y[j], std::abs(1000*10*ydiff));
                if (std::abs(ydiff) < spacial_cut) {
                  VMM_time_y_out.push_back(VMMyTimeStamps[K] + time_fit[y_adc[K]]);
                  VMM_y_out.push_back(y_pitch*VMM_ypos[K]);
                  VMM_size_y_out.push_back(y_size[K]);
                  hits_count++;
                  h_size_adc_y->Fill(y_size[K], y_adc[K]);
                  if (hits_count == 2) {
                    y_size_first = y_size_last;
                    y_adc_first = y_adc_last;
                  }
                  y_size_last = y_size[K];
                  y_adc_last = y_adc[K];
                }
            }
            if (std::abs(timediff - 10000) < time_cut) {
                if (std::abs(ydiff) < spacial_cut) {
                    hits_count_BG++;
                }
            }
        }
        if (hits_count == 0) {
          h_missing_y->Fill(p->tracks_x[j], p->tracks_y[j]);
          if (p->boundaries_cut[j] == true) {
            h_missing_y_mask->Fill(p->tracks_x[j], p->tracks_y[j]);
          } 
        }
        VMM_yHit_out1 = hits_count;
        VMM_BG_yHit_out1 = hits_count_BG;
        hits_per_track_y[hits_count]++;
        hits_per_track_y_BG[hits_count_BG]++;
        if (p->boundaries_cut[j] == true) {
          hits_per_mask_track_y[hits_count]++;
          hits_per_mask_track_y_BG[hits_count_BG]++;        
        }
        output_tree_1->Fill();
        if ( VMM_xHit_out1 == 1 && VMM_yHit_out1 == 1) {
          h_size_size->Fill(x_size_last, y_size_last);
          h_adc_adc->Fill(x_adc_last, y_adc_last);
          //h_size_adc_x->Fill(x_size_last, x_adc_last);
          //h_size_adc_y->Fill(y_size_last, y_adc_last);
          h_size_tot_adc_x->Fill(x_size_last, x_adc_last + y_adc_last);
          h_size_tot_adc_y->Fill(y_size_last, x_adc_last + y_adc_last);
          //h_size_size->Fill(x_size_last, y_size_first);
          //h_adc_adc->Fill(x_adc_last, y_adc_first);
          //h_size_adc_x->Fill(x_size_last, x_adc_first);
          //h_size_adc_y->Fill(y_size_last, y_adc_first);
        }
        VMM_time_x_out.clear();
        VMM_x_out.clear();
        VMM_size_x_out.clear();
        VMM_time_y_out.clear();
        VMM_y_out.clear();
        VMM_size_y_out.clear();
    }
    output_file->Write();


    /*
    TCanvas* c_res_time = new TCanvas("", "", 3600, 3000);
    c_res_time->Divide(2,3);
    c_res_time->cd(1);


    //TCanvas* c_res_time_x_fec1 = new TCanvas("", "", 1200, 700);
    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    TF1* f_time_x_fec1 = new TF1("f_time_x_fec1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_time_x_fec1->GetBinLowEdge(1), h_res_time_x_fec1->GetBinLowEdge(h_res_time_x_fec1->GetNbinsX() + 1));
    f_time_x_fec1->SetNpx(1000);
    f_time_x_fec1->SetParameters(600, -1, 14, 10);
    h_res_time_x_fec1->Fit("f_time_x_fec1", " ", " ", -150, 150);
    TPaveText* fit_time_x_fec1 = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
    fit_time_x_fec1->SetTextAlign(12);
    fit_time_x_fec1->AddText(Form("A = %.1f #pm %.1f", f_time_x_fec1->GetParameter(0), f_time_x_fec1->GetParError(0)));
    fit_time_x_fec1->AddText(Form("#mu = (%.2f #pm %.2f) ns", f_time_x_fec1->GetParameter(1), f_time_x_fec1->GetParError(1)));
    fit_time_x_fec1->AddText(Form("#sigma = (%.2f #pm %.2f) ns", f_time_x_fec1->GetParameter(2), f_time_x_fec1->GetParError(2)));
    fit_time_x_fec1->AddText(Form(" "));
    fit_time_x_fec1->AddText(Form("offset = %.2f #pm %.2f", f_time_x_fec1->GetParameter(3), f_time_x_fec1->GetParError(3)));
    h_res_time_x_fec1->Draw();
    fit_time_x_fec1->Draw();
    //c_res_time_x_fec1->SaveAs(Form("spill_%lld_res_time_x_fec1.png", spill));

    c_res_time->cd(2);
    //TCanvas* c_res_time_x_fec4 = new TCanvas("", "", 1200, 700);
    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    TF1* f_time_x_fec4 = new TF1("f_time_x_fec4", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_time_x_fec4->GetBinLowEdge(1), h_res_time_x_fec4->GetBinLowEdge(h_res_time_x_fec4->GetNbinsX() + 1));
    f_time_x_fec4->SetNpx(1000);
    f_time_x_fec4->SetParameters(600, -1, 14, 10);
    h_res_time_x_fec4->Fit("f_time_x_fec4", " ", " ", -150, 150);
    TPaveText* fit_time_x_fec4 = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
    fit_time_x_fec4->SetTextAlign(12);
    fit_time_x_fec4->AddText(Form("A = %.1f #pm %.1f", f_time_x_fec4->GetParameter(0), f_time_x_fec4->GetParError(0)));
    fit_time_x_fec4->AddText(Form("#mu = (%.2f #pm %.2f) ns", f_time_x_fec4->GetParameter(1), f_time_x_fec4->GetParError(1)));
    fit_time_x_fec4->AddText(Form("#sigma = (%.2f #pm %.2f) ns", f_time_x_fec4->GetParameter(2), f_time_x_fec4->GetParError(2)));
    fit_time_x_fec4->AddText(Form(" "));
    fit_time_x_fec4->AddText(Form("offset = %.2f #pm %.2f", f_time_x_fec4->GetParameter(3), f_time_x_fec4->GetParError(3)));
    h_res_time_x_fec4->Draw();
    fit_time_x_fec4->Draw();
    //c_res_time_x_fec4->SaveAs(Form("spill_%lld_res_time_x_fec4.png", spill));


    c_res_time->cd(3);
    //TCanvas* c_res_time_y_fec2 = new TCanvas("", "", 1200, 700);
    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    TF1* f_time_y_fec2 = new TF1("f_time_y_fec2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_time_y_fec2->GetBinLowEdge(1), h_res_time_y_fec2->GetBinLowEdge(h_res_time_y_fec2->GetNbinsX() + 1));
    f_time_y_fec2->SetNpx(1000);
    f_time_y_fec2->SetParameters(600, -1, 14, 10);
    h_res_time_y_fec2->Fit("f_time_y_fec2", " ", " ", -150, 150);
    TPaveText* fit_time_y_fec2 = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
    fit_time_y_fec2->SetTextAlign(12);
    fit_time_y_fec2->AddText(Form("A = %.1f #pm %.1f", f_time_y_fec2->GetParameter(0), f_time_y_fec2->GetParError(0)));
    fit_time_y_fec2->AddText(Form("#mu = (%.2f #pm %.2f) ns", f_time_y_fec2->GetParameter(1), f_time_y_fec2->GetParError(1)));
    fit_time_y_fec2->AddText(Form("#sigma = (%.2f #pm %.2f) ns", f_time_y_fec2->GetParameter(2), f_time_y_fec2->GetParError(2)));
    fit_time_y_fec2->AddText(Form(" "));
    fit_time_y_fec2->AddText(Form("offset = %.2f #pm %.2f", f_time_y_fec2->GetParameter(3), f_time_y_fec2->GetParError(3)));
    h_res_time_y_fec2->Draw();
    fit_time_y_fec2->Draw();
    //c_res_time_y_fec2->SaveAs(Form("spill_%lld_res_time_y_fec2.png", spill));

    c_res_time->cd(4);
    //TCanvas* c_res_time_y_fec5 = new TCanvas("", "", 1200, 700);
    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");
    TF1* f_time_y_fec5 = new TF1("f_time_y_fec5", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_time_y_fec5->GetBinLowEdge(1), h_res_time_y_fec5->GetBinLowEdge(h_res_time_y_fec5->GetNbinsX() + 1));
    f_time_y_fec5->SetNpx(1000);
    f_time_y_fec5->SetParameters(400, -1, 11.5, 3);
    h_res_time_y_fec5->Fit("f_time_y_fec5", " ", " ", -150, 150);
    TPaveText* fit_time_y_fec5 = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
    fit_time_y_fec5->SetTextAlign(12);
    fit_time_y_fec5->AddText(Form("A = %.1f #pm %.1f", f_time_y_fec5->GetParameter(0), f_time_y_fec5->GetParError(0)));
    fit_time_y_fec5->AddText(Form("#mu = (%.2f #pm %.2f) ns", f_time_y_fec5->GetParameter(1), f_time_y_fec5->GetParError(1)));
    fit_time_y_fec5->AddText(Form("#sigma = (%.2f #pm %.2f) ns", f_time_y_fec5->GetParameter(2), f_time_y_fec5->GetParError(2)));
    fit_time_y_fec5->AddText(Form(" "));
    fit_time_y_fec5->AddText(Form("offset = %.2f #pm %.2f", f_time_y_fec5->GetParameter(3), f_time_y_fec5->GetParError(3)));
    h_res_time_y_fec5->Draw();
    fit_time_y_fec5->Draw();
    //c_res_time_y_fec5->SaveAs(Form("spill_%lld_res_time_y_fec5.png", spill));


    c_res_time->cd(5);
    //TCanvas* c_res_time = new TCanvas("", "", 1200, 700);
    TF1* f_time = new TF1("f_time", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_time->GetBinLowEdge(1), h_res_time->GetBinLowEdge(h_res_time->GetNbinsX() + 1));
    f_time->SetNpx(1000);
    f_time->SetParameters(1000, -1, 11, 10);
    h_res_time->Fit("f_time", " ", " ", -150, 150);
    TPaveText* fit_time = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
    fit_time->SetTextAlign(12);
    fit_time->AddText(Form("A = %.1f #pm %.1f", f_time->GetParameter(0), f_time->GetParError(0)));
    fit_time->AddText(Form("#mu = (%.2f #pm %.2f) ns", f_time->GetParameter(1), f_time->GetParError(1)));
    fit_time->AddText(Form("#sigma = (%.2f #pm %.2f) ns", f_time->GetParameter(2), f_time->GetParError(2)));
    fit_time->AddText(Form(" "));
    fit_time->AddText(Form("offset = %.2f #pm %.2f", f_time->GetParameter(3), f_time->GetParError(3)));
    h_res_time->Draw();
    fit_time->Draw();
    c_res_time->Print(Form("spill_%lld.pdf[", spill));
    c_res_time->Print(Form("spill_%lld.pdf", spill));
    //c_res_time->SaveAs(Form("spill_%lld_res_time.png", spill));
    */
    TCanvas* c_res_time = new TCanvas("", "", 3600, 3000);
    c_res_time->Divide(2, 3);

    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");

    TH1D* hist_time_res[5] = {h_res_time_x_fec1, h_res_time_x_fec4, h_res_time_y_fec2, h_res_time_y_fec5, h_res_time};

    for (int i_hist = 0; i_hist < 5; i_hist++) {
      c_res_time->cd(i_hist + 1);

      TF1* fit_func = new TF1(Form("f_time_%d", i_hist + 1), "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]",
                              hist_time_res[i_hist]->GetBinLowEdge(1),
                              hist_time_res[i_hist]->GetBinLowEdge(hist_time_res[i_hist]->GetNbinsX() + 1));
      fit_func->SetNpx(1000);
      fit_func->SetParameters(400, -1, 14, 10);

      gPad->SetLeftMargin(0.1);
      gPad->SetBottomMargin(0.1);
      hist_time_res[i_hist]->GetXaxis()->CenterTitle();
      hist_time_res[i_hist]->GetXaxis()->SetTitleSize(0.05);
      hist_time_res[i_hist]->GetYaxis()->CenterTitle();
      hist_time_res[i_hist]->GetYaxis()->SetTitleSize(0.05);
      hist_time_res[i_hist]->Fit(fit_func, " ", " ", -150, 150);
      hist_time_res[i_hist]->Draw();

      TPaveText* fit_text = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
      fit_text->SetTextAlign(12);
      fit_text->AddText(Form("A = %.0f #pm %.0f", fit_func->GetParameter(0), fit_func->GetParError(0)));
      fit_text->AddText(Form("#mu = (%.2f #pm %.2f) ns", fit_func->GetParameter(1), fit_func->GetParError(1)));
      fit_text->AddText(Form("#sigma = (%.2f #pm %.2f) ns", fit_func->GetParameter(2), fit_func->GetParError(2)));
      fit_text->AddText(" ");
      fit_text->AddText(Form("offset = %.1f #pm %.1f", fit_func->GetParameter(3), fit_func->GetParError(3)));
      fit_text->Draw();
      if (i_hist == 0) {
        residual_data_lines[7] = residual_data_lines[7] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[8] = residual_data_lines[8] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_hist == 1) {
        residual_data_lines[15] = residual_data_lines[15] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[16] = residual_data_lines[16] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_hist == 2) {
        residual_data_lines[11] = residual_data_lines[11] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[12] = residual_data_lines[12] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_hist == 3) {
        residual_data_lines[19] = residual_data_lines[19] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[20] = residual_data_lines[20] + Form("%.2f, ", fit_func->GetParError(2));
      }
    }
    //c_res_time->Print(output_plots_residuals + "[");
    c_res_time->Print(output_plots_residuals);


    TCanvas* c_res_mask_time = new TCanvas("", "", 3600, 3000);
    c_res_mask_time->Divide(2, 3);

    gStyle->SetOptStat(0);
    gStyle->SetTitleSize(0.04, "xyz");
    gStyle->SetTitleFontSize(0.04);
    gStyle->SetLabelSize(0.04, "xyz");

    TH1D* h_time_res_mask[5] = {h_res_mask_time_x_fec1, h_res_mask_time_x_fec4, h_res_mask_time_y_fec2, h_res_mask_time_y_fec5, h_res_mask_time};
    const char* h_time_res_mask_titles[5] = {"h_res_mask_time_x_fec1", "h_res_mask_time_x_fec4", "h_res_mask_time_y_fec2", "h_res_mask_time_y_fec5", "h_res_mask_time"};

    for (int i_mask = 0; i_mask < 5; i_mask++) {
      c_res_mask_time->cd(i_mask + 1);

      TF1* fit_func = new TF1(Form("f_time_%d", i_mask + 1), "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]",
                              h_time_res_mask[i_mask]->GetBinLowEdge(1),
                              h_time_res_mask[i_mask]->GetBinLowEdge(h_time_res_mask[i_mask]->GetNbinsX() + 1));
      fit_func->SetNpx(1000);
      fit_func->SetParameters(00, -1, 14, 10);
      gPad->SetLeftMargin(0.1);
      gPad->SetBottomMargin(0.1);
      h_time_res_mask[i_mask]->GetXaxis()->CenterTitle();
      h_time_res_mask[i_mask]->GetXaxis()->SetTitleSize(0.05);
      h_time_res_mask[i_mask]->GetYaxis()->CenterTitle();
      h_time_res_mask[i_mask]->GetYaxis()->SetTitleSize(0.05);
      h_time_res_mask[i_mask]->Fit(fit_func, " ", " ", -150, 150);
      h_time_res_mask[i_mask]->Draw();

      TPaveText* fit_text = new TPaveText(0.1, 0.65, 0.28, 0.85, "NDC");
      fit_text->SetTextAlign(12);
      fit_text->AddText(Form("A = %.0f #pm %.0f", fit_func->GetParameter(0), fit_func->GetParError(0)));
      fit_text->AddText(Form("#mu = (%.2f #pm %.2f) ns", fit_func->GetParameter(1), fit_func->GetParError(1)));
      fit_text->AddText(Form("#sigma = (%.2f #pm %.2f) ns", fit_func->GetParameter(2), fit_func->GetParError(2)));
      fit_text->AddText(" ");
      fit_text->AddText(Form("offset = %.1f #pm %.1f", fit_func->GetParameter(3), fit_func->GetParError(3)));
      fit_text->Draw();
      if (i_mask == 0) {
        residual_data_lines[9] = residual_data_lines[9] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[10] = residual_data_lines[10] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_mask == 1) {
        residual_data_lines[17] = residual_data_lines[17] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[18] = residual_data_lines[18] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_mask == 2) {
        residual_data_lines[13] = residual_data_lines[13] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[14] = residual_data_lines[14] + Form("%.2f, ", fit_func->GetParError(2));
      }
      if (i_mask == 3) {
        residual_data_lines[21] = residual_data_lines[21] + Form("%.2f, ", fit_func->GetParameter(2));
        residual_data_lines[22] = residual_data_lines[22] + Form("%.2f, ", fit_func->GetParError(2));
      }
    }

    c_res_mask_time->Print(output_plots_residuals);


    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "timewalk fit" << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    TCanvas* c_adc_dt_cor = new TCanvas("", "", 1800, 1000);
    /*c_adc_dt->Divide(1,2);
    c_adc_dt->cd(1);
    TF1* f_adc_dt = new TF1("f_adc_dt", "[3] + ([0] - [3])/(1 + pow(x/[2], [1]))", 0, 1600);
    f_adc_dt->SetParameters(-18.5, 1.9, 169, 2.8);
    h_adc_dt->Fit("f_adc_dt", " ", " ", 0, 1600);
    //TF1* f_adc_dt = new TF1("f_adc_dt", "[1]*(x - [0])", 2500, 4000);
    //f_adc_dt->SetParameters(2225, 0.05);
    //h_adc_dt->Fit("f_adc_dt", " ", " ", 2500, 4000);
    h_adc_dt->Draw("colz");
    f_adc_dt->Draw("same");

    c_adc_dt->cd(2);*/
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_adc_dt_cor->SetTitleSize(0.12);
    h_adc_dt_cor->GetXaxis()->CenterTitle();
    h_adc_dt_cor->GetXaxis()->SetTitleSize(0.06);
    h_adc_dt_cor->GetYaxis()->CenterTitle();
    h_adc_dt_cor->GetYaxis()->SetTitleSize(0.06);
    h_adc_dt_cor->Draw("colz");
    c_adc_dt_cor->Print(output_plots_residuals);
    c_adc_dt_cor->SaveAs("col_rec_adc_dt_cor.png");

    TCanvas* c_adc_dt = new TCanvas("", "", 1800, 1000);
    TF1 *f_timew = new TF1("f_timew", "8.024 + (-0.149 - 8.024)/(1 + pow(x/538.325, 3.697))", 0, 1500);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_adc_dt->SetTitleSize(0.12);
    h_adc_dt->GetXaxis()->CenterTitle();
    h_adc_dt->GetXaxis()->SetTitleSize(0.06);
    h_adc_dt->GetYaxis()->CenterTitle();
    h_adc_dt->GetYaxis()->SetTitleSize(0.06);
    h_adc_dt->Draw("colz");
    f_timew->SetLineColor(kRed);
    f_timew->SetLineWidth(7);
    f_timew->Draw("same");
    c_adc_dt->Print(output_plots_residuals);
    c_adc_dt->SaveAs("col_rec_adc_dt.png");

    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    TCanvas* c_missing = new TCanvas("", "", 1200, 1000);
    c_missing->Divide(2,2);
    c_missing->cd(1);
    h_missing_x->Draw("colz");
    c_missing->cd(2);
    h_missing_y->Draw("colz");
    c_missing->cd(3);
    h_missing_x_mask->Draw("colz");
    c_missing->cd(4);
    h_missing_y_mask->Draw("colz");
    c_missing->Print(output_plots_residuals);


    TCanvas* c_adc_adc = new TCanvas("", "", 1600, 1000);
    h_adc_adc->Draw("colz");
    //c_adc_adc->SaveAs(Form("spill_%lld_adc_adc.png", spill));
    gPad->SetLogz();
    //c_adc_adc->SaveAs(Form("spill_%lld_adc_adc_log.png", spill));
    c_adc_adc->Print(output_plots_residuals);

    TCanvas* c_size_adc_x = new TCanvas("", "", 1600, 1000);
    h_size_adc_x->Draw("colz");
    gPad->SetLogz();
    //c_size_adc_x->SaveAs(Form("spill_%lld_size_adc_x.png", spill));

    TCanvas* c_size_adc_y = new TCanvas("", "", 1600, 1000);
    h_size_adc_y->Draw("colz");
    gPad->SetLogz();
    //c_size_adc_y->SaveAs(Form("spill_%lld_size_adc_y.png", spill));

    TCanvas* c_size_tot_adc_x = new TCanvas("", "", 1600, 1000);
    h_size_tot_adc_x->Draw("colz");
    gPad->SetLogz();
    //c_size_tot_adc_x->SaveAs(Form("spill_%lld_size_tot_adc_x.png", spill));

    TCanvas* c_size_tot_adc_y = new TCanvas("", "", 1600, 1000);
    h_size_tot_adc_y->Draw("colz");
    gPad->SetLogz();
    //c_size_tot_adc_y->SaveAs(Form("spill_%lld_size_tot_adc_y.png", spill));



    TCanvas* c_spatial_calib = new TCanvas("", "", 1900, 1000);
    c_spatial_calib->Divide(2,2);
    int nBinsX = h_dx_x->GetNbinsX();
    int nBinsY = h_dx_x->GetNbinsY();
    //TCanvas* c_dx_x = new TCanvas("", "", 1900, 1000);
    c_spatial_calib->cd(1);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_dx_x->SetTitleSize(0.06);
    h_dx_x->GetXaxis()->CenterTitle();
    h_dx_x->GetXaxis()->SetTitleSize(0.06);
    h_dx_x->GetYaxis()->CenterTitle();
    h_dx_x->GetYaxis()->SetTitleSize(0.06);
    TF1* f_dx_x = new TF1("f_dx_x", "[0] + [1]*x ", h_dx_x->GetXaxis()->GetXmin(), h_dx_x->GetXaxis()->GetXmax());
    /*for (int i = 1; i <= nBinsX; i++) {
      for (int j = 1; j <= nBinsY; j++) {
        double bin_x = h_dx_x->GetBinContent(i, j);
        double x_xPoint = h_dx_x->GetXaxis()->GetBinCenter(i);
        double x_yPoint = h_dx_x->GetYaxis()->GetBinCenter(j);
        f_dx_x->SetPoint(i*nBinsY + j - 1, x_xPoint, x_yPoint, bin_x);
      }
    }*/
    h_dx_x->Fit("f_dx_x");
    TPaveText* p_dx_x = new TPaveText(0.15, 0.75, 0.42, 0.85, "NDC");
    p_dx_x->AddText(Form("offset: %.0f #pm %.0f #mum", f_dx_x->GetParameter(0), f_dx_x->GetParError(0)));
    p_dx_x->AddText(Form("slope: %.1f #pm %.1f #mum/cm", f_dx_x->GetParameter(1), f_dx_x->GetParError(1)));
    h_dx_x->Draw("colz");
    f_dx_x->Draw("same");
    p_dx_x->Draw();
    //c_dx_x->SaveAs(Form("spill_%lld_dx_x.png", spill));

    //TCanvas* c_dx_y = new TCanvas("", "", 1900, 1000);
    c_spatial_calib->cd(3);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_dx_y->SetTitleSize(0.16);
    h_dx_y->GetXaxis()->CenterTitle();
    h_dx_y->GetXaxis()->SetTitleSize(0.06);
    h_dx_y->GetYaxis()->CenterTitle();
    h_dx_y->GetYaxis()->SetTitleSize(0.06);
    TF1* f_dx_y = new TF1("f_dx_y", "[0] + [1]*x ", -10, 10);
    h_dx_y->Fit("f_dx_y");
    TPaveText* p_dx_y = new TPaveText(0.15, 0.75, 0.42, 0.85, "NDC");
    p_dx_y->AddText(Form("offset: %.0f #pm %.0f #mum", f_dx_y->GetParameter(0), f_dx_y->GetParError(0)));
    p_dx_y->AddText(Form("slope: %.1f #pm %.1f #mum/cm", f_dx_y->GetParameter(1), f_dx_y->GetParError(1)));
    h_dx_y->Draw("colz");
    f_dx_y->Draw("same");
    p_dx_y->Draw();
    //c_dx_y->SaveAs(Form("spill_%lld_dx_y.png", spill));

    //TCanvas* c_dy_y = new TCanvas("", "", 1900, 1000);
    c_spatial_calib->cd(2);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_dy_y->SetTitleSize(0.16);
    h_dy_y->GetXaxis()->CenterTitle();
    h_dy_y->GetXaxis()->SetTitleSize(0.06);
    h_dy_y->GetYaxis()->CenterTitle();
    h_dy_y->GetYaxis()->SetTitleSize(0.06);
    TF1* f_dy_y = new TF1("f_dy_y", "[0] + [1]*x ", -10, 10);
    h_dy_y->Fit("f_dy_y");
    TPaveText* p_dy_y = new TPaveText(0.15, 0.75, 0.42, 0.85, "NDC");
    p_dy_y->AddText(Form("offset: %.0f #pm %.0f #mum", f_dy_y->GetParameter(0), f_dy_y->GetParError(0)));
    p_dy_y->AddText(Form("slope: %.1f #pm %.1f #mum/cm", f_dy_y->GetParameter(1), f_dy_y->GetParError(1)));
    h_dy_y->Draw("colz");
    f_dy_y->Draw("same");
    p_dy_y->Draw();
    //c_dy_y->SaveAs(Form("spill_%lld_dy_y.png", spill));

    //TCanvas* c_dy_x = new TCanvas("", "", 1900, 1000);
    c_spatial_calib->cd(4);
    gPad->SetLeftMargin(0.15);
    gPad->SetBottomMargin(0.15);
    h_dy_x->SetTitleSize(0.16);
    h_dy_x->GetXaxis()->CenterTitle();
    h_dy_x->GetXaxis()->SetTitleSize(0.06);
    h_dy_x->GetYaxis()->CenterTitle();
    h_dy_x->GetYaxis()->SetTitleSize(0.06);
    TF1* f_dy_x = new TF1("f_dy_x", "[0] + [1]*x ", -10, 10);
    h_dy_x->Fit("f_dy_x");
    TPaveText* p_dy_x = new TPaveText(0.15, 0.75, 0.42, 0.85, "NDC");
    p_dy_x->AddText(Form("offset: %.0f #pm %.0f #mum", f_dy_x->GetParameter(0), f_dy_x->GetParError(0)));
    p_dy_x->AddText(Form("slope: %.1f #pm %.1f #mum/cm", f_dy_x->GetParameter(1), f_dy_x->GetParError(1)));
    h_dy_x->Draw("colz");
    f_dy_x->Draw("same");
    p_dy_x->Draw();
    //c_dy_x->SaveAs(Form("spill_%lld_dy_x.png", spill));
    c_spatial_calib->Print(output_plots_residuals);


    TCanvas* c_dx_x_thesis = new TCanvas("", "", 1000, 700);
    c_dx_x_thesis->SetLeftMargin(0.15);
    c_dx_x_thesis->SetBottomMargin(0.15);
    h_dx_x->GetXaxis()->CenterTitle();
    h_dx_x->GetXaxis()->SetTitleSize(0.05);
    h_dx_x->GetYaxis()->CenterTitle();
    h_dx_x->GetYaxis()->SetTitleSize(0.05);
    h_dx_x->Draw("colz");
    f_dx_x->Draw("same");
    p_dx_x->Draw();
    c_dx_x_thesis->SaveAs("col_rec_dx_x_align.png");


    TCanvas* timediff_vs_TiS = new TCanvas("", "", 1200, 700);
    TGraph* timediff_TiS = new TGraph(Time_In_Spill.size(), Time_In_Spill.data(), time_differences.data());
    timediff_TiS->GetXaxis()->SetTitle("time in spill / ns");
    timediff_TiS->GetYaxis()->SetTitle("#deltat / ns");
    timediff_TiS->Draw("AP");
    timediff_vs_TiS->Print(output_plots_residuals);

    TCanvas* x_res_vs_TiS = new TCanvas("", "", 1200, 700);
    TGraph* x_res_TiS = new TGraph(Time_In_Spill.size(), Time_In_Spill.data(), x_differences.data());
    x_res_TiS->GetXaxis()->SetTitle("time in spill / ns");
    x_res_TiS->GetYaxis()->SetTitle("#deltax / #mum");
    x_res_TiS->SetMinimum(-5000);
    x_res_TiS->SetMaximum(5000);
    x_res_TiS->Draw("AP");
    x_res_vs_TiS->Print(output_plots_residuals);




    TCanvas* c_res_spatial = new TCanvas("", "", 1800, 1500);
    c_res_spatial->Divide(2,3);
    c_res_spatial->cd(5);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_x->GetXaxis()->CenterTitle();
    h_res_x->GetXaxis()->SetTitleSize(0.05);
    h_res_x->GetYaxis()->CenterTitle();
    h_res_x->GetYaxis()->SetTitleSize(0.05);
    //TCanvas* c_res_x = new TCanvas("", "", 1200, 700);
    TF1* func_x = new TF1("func_x", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x->SetNpx(1000);
    func_x->SetParameters(1000, 0, 100, 80, 0, 400, 10);
    h_res_x->Fit("func_x", " ", " ", -1500, 1500);
    double param0 = func_x->GetParameter(0);
    double param1 = func_x->GetParameter(1);
    double param2 = func_x->GetParameter(2);
    TF1* func_x_P1 = new TF1("func_x_P1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x_P1->SetParameter(0, func_x->GetParameter(0));
    func_x_P1->SetParameter(1, func_x->GetParameter(1));
    func_x_P1->SetParameter(2, func_x->GetParameter(2));
    func_x_P1->SetLineColor(kBlack);
    TF1* func_x_P2 = new TF1("func_x_P2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x_P2->SetParameter(0, func_x->GetParameter(3));
    func_x_P2->SetParameter(1, func_x->GetParameter(4));
    func_x_P2->SetParameter(2, func_x->GetParameter(5));
    func_x_P2->SetLineColor(kBlack);
    TF1* func_x_BG = new TF1("func_x_BG", "[0]", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x_BG->SetParameter(0, func_x->GetParameter(6));
    func_x_BG->SetLineColor(kBlack);
    func_x_P1->SetNpx(1000);
    func_x_P2->SetNpx(1000);
    func_x_BG->SetNpx(1000);
    TPaveText* fit_x = new TPaveText(0.2, 0.49, 0.38, 0.85, "NDC");
    fit_x->SetTextAlign(12);
    fit_x->AddText(Form("A1 = %.1f #pm %.1f", func_x->GetParameter(0), func_x->GetParError(0)));
    fit_x->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_x->GetParameter(1), func_x->GetParError(1)));
    fit_x->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_x->GetParameter(2), func_x->GetParError(2)));
    fit_x->AddText(Form(" "));
    fit_x->AddText(Form("A2 = %.1f #pm %.1f", func_x->GetParameter(3), func_x->GetParError(3)));
    fit_x->AddText(Form("#mu2 = (%.0f #pm %.0f) #mum", func_x->GetParameter(4), func_x->GetParError(4)));
    fit_x->AddText(Form("#sigma2 = (%.0f #pm %.0f) #mum", func_x->GetParameter(5), func_x->GetParError(5)));
    fit_x->AddText(Form(" "));
    fit_x->AddText(Form("offset = %.1f #pm %.1f", func_x->GetParameter(6), func_x->GetParError(6)));
    h_res_x->Draw();
    func_x_P1->Draw("same");
    func_x_P2->Draw("same");
    func_x_BG->Draw("same");
    fit_x->Draw();
    TLatex* tex = new TLatex(-1100., 200, "Work in Progress");
    tex->SetTextColor(17);
    tex->SetTextSize(0.155);
    tex->SetTextAngle(26.15998);
    tex->SetLineWidth(2);
    gPad->RedrawAxis();
    fit_x->Draw();
    //c_res_x->Print(Form("spill_%lld.pdf[", spill));
    //c_res_x->Print(Form("spill_%lld.pdf", spill));


    c_res_spatial->cd(1);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_x->GetXaxis()->CenterTitle();
    h_res_x->GetXaxis()->SetTitleSize(0.05);
    h_res_x->GetYaxis()->CenterTitle();
    h_res_x->GetYaxis()->SetTitleSize(0.05);
    //TCanvas* c_res_x_sg = new TCanvas("", "", 1200, 700);
    TF1* func_x_sg = new TF1("func_x_sg", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_x_sg->SetNpx(1000);
    func_x_sg->SetParameters(1000, 0, 100, 10);
    h_res_x->Fit("func_x_sg", " ", " ", -1500, 1500);
    TPaveText* fit_x_sg = new TPaveText(0.14, 0.65, 0.32, 0.85, "NDC");
    fit_x_sg->SetTextAlign(12);
    fit_x_sg->AddText(Form("A1 = %.0f #pm %.0f", func_x_sg->GetParameter(0), func_x_sg->GetParError(0)));
    fit_x_sg->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_x_sg->GetParameter(1), func_x_sg->GetParError(1)));
    fit_x_sg->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_x_sg->GetParameter(2), func_x_sg->GetParError(2)));
    fit_x_sg->AddText(Form(" "));
    fit_x_sg->AddText(Form("offset = %.1f #pm %.1f", func_x_sg->GetParameter(3), func_x_sg->GetParError(3)));
    h_res_x->Draw();
    fit_x_sg->Draw();
    residual_data_lines[23] = residual_data_lines[23] + Form("%.1f, ", func_x_sg->GetParameter(2));
    residual_data_lines[24] = residual_data_lines[24] + Form("%.1f, ", func_x_sg->GetParError(2));
    //c_res_x_sg->Print(Form("spill_%lld.pdf", spill));


    c_res_spatial->cd(3);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_mask_x->GetXaxis()->CenterTitle();
    h_res_mask_x->GetXaxis()->SetTitleSize(0.05);
    h_res_mask_x->GetYaxis()->CenterTitle();
    h_res_mask_x->GetYaxis()->SetTitleSize(0.05);
    TF1* func_x_sg_mask = new TF1("func_x_sg_mask", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_mask_x->GetBinLowEdge(1), h_res_mask_x->GetBinLowEdge(h_res_mask_x->GetNbinsX() + 1));
    func_x_sg_mask->SetNpx(1000);
    func_x_sg_mask->SetParameters(1000, 0, 100, 10);
    h_res_mask_x->Fit("func_x_sg_mask", " ", " ", -1500, 1500);
    TPaveText* fit_x_sg_mask = new TPaveText(0.14, 0.65, 0.32, 0.85, "NDC");
    fit_x_sg_mask->SetTextAlign(12);
    fit_x_sg_mask->AddText(Form("A1 = %.0f #pm %.0f", func_x_sg_mask->GetParameter(0), func_x_sg_mask->GetParError(0)));
    fit_x_sg_mask->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_x_sg_mask->GetParameter(1), func_x_sg_mask->GetParError(1)));
    fit_x_sg_mask->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_x_sg_mask->GetParameter(2), func_x_sg_mask->GetParError(2)));
    fit_x_sg_mask->AddText(Form(" "));
    fit_x_sg_mask->AddText(Form("offset = %.1f #pm %.1f", func_x_sg_mask->GetParameter(3), func_x_sg_mask->GetParError(3)));
    h_res_mask_x->Draw();
    fit_x_sg_mask->Draw();
    residual_data_lines[25] = residual_data_lines[25] + Form("%.1f, ", func_x_sg_mask->GetParameter(2));
    residual_data_lines[26] = residual_data_lines[26] + Form("%.1f, ", func_x_sg_mask->GetParError(2));


    c_res_spatial->cd(6);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_y->GetXaxis()->CenterTitle();
    h_res_y->GetXaxis()->SetTitleSize(0.05);
    h_res_y->GetYaxis()->CenterTitle();
    h_res_y->GetYaxis()->SetTitleSize(0.05);
    //TCanvas* c_res_y = new TCanvas("", "", 1200, 700);
    TF1* func_y = new TF1("func_y", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]*exp(-(x-[4])*(x-[4])/(2*[5]*[5])) + [6]", h_res_y->GetBinLowEdge(1), h_res_y->GetBinLowEdge(h_res_y->GetNbinsX() + 1));
    func_y->SetNpx(1000);
    func_y->SetParameters(1000, 0, 100, 80, 0, 400, 10);
    h_res_y->Fit("func_y", " ", " ", -1500, 1500);
    TF1* func_y_P1 = new TF1("func_y_P1", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))", h_res_y->GetBinLowEdge(1), h_res_y->GetBinLowEdge(h_res_y->GetNbinsX() + 1));
    func_y_P1->SetParameter(0, func_y->GetParameter(0));
    func_y_P1->SetParameter(1, func_y->GetParameter(1));
    func_y_P1->SetParameter(2, func_y->GetParameter(2));
    func_y_P1->SetLineColor(kBlack);
    TF1* func_y_P2 = new TF1("func_y_P2", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2]))", h_res_y->GetBinLowEdge(1), h_res_y->GetBinLowEdge(h_res_y->GetNbinsX() + 1));
    func_y_P2->SetParameter(0, func_y->GetParameter(3));
    func_y_P2->SetParameter(1, func_y->GetParameter(4));
    func_y_P2->SetParameter(2, func_y->GetParameter(5));
    func_y_P2->SetLineColor(kBlack);
    TF1* func_y_BG = new TF1("func_y_BG", "[0]", h_res_x->GetBinLowEdge(1), h_res_x->GetBinLowEdge(h_res_x->GetNbinsX() + 1));
    func_y_BG->SetParameter(0, func_y->GetParameter(6));
    func_y_BG->SetLineColor(kBlack);
    func_y_P1->SetNpx(1000);
    func_y_P2->SetNpx(1000);
    func_y_BG->SetNpx(1000);
    TPaveText* fit_y = new TPaveText(0.1, 0.49, 0.28, 0.85, "NDC");
    fit_y->SetTextAlign(12);
    fit_y->AddText(Form("A1 = %.1f #pm %.1f", func_y->GetParameter(0), func_y->GetParError(0)));
    fit_y->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_y->GetParameter(1), func_y->GetParError(1)));
    fit_y->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_y->GetParameter(2), func_y->GetParError(2)));
    fit_y->AddText(Form(" "));
    fit_y->AddText(Form("A2 = %.1f #pm %.1f", func_y->GetParameter(3), func_y->GetParError(3)));
    fit_y->AddText(Form("#mu2 = (%.0f #pm %.0f) #mum", func_y->GetParameter(4), func_y->GetParError(4)));
    fit_y->AddText(Form("#sigma2 = (%.0f #pm %.0f) #mum", func_y->GetParameter(5), func_y->GetParError(5)));
    fit_y->AddText(Form(" "));
    fit_y->AddText(Form("offset = %.1f #pm %.1f", func_y->GetParameter(6), func_y->GetParError(6)));
    h_res_y->Draw();
    func_y_P1->Draw("same");
    func_y_P2->Draw("same");
    func_y_BG->Draw("same");
    fit_y->Draw();
    //c_res_y->Print(Form("spill_%lld.pdf", spill));


    c_res_spatial->cd(2);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_y->GetXaxis()->CenterTitle();
    h_res_y->GetXaxis()->SetTitleSize(0.05);
    h_res_y->GetYaxis()->CenterTitle();
    h_res_y->GetYaxis()->SetTitleSize(0.05);
    //TCanvas* c_res_y_sg = new TCanvas("", "", 1200, 700);
    TF1* func_y_sg = new TF1("func_y_sg", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_y->GetBinLowEdge(1), h_res_y->GetBinLowEdge(h_res_y->GetNbinsX() + 1));
    func_y_sg->SetNpx(1000);
    func_y_sg->SetParameters(1000, 0, 100, 10);
    h_res_y->Fit("func_y_sg", " ", " ", -1500, 1500);
    TPaveText* fit_y_sg = new TPaveText(0.14, 0.65, 0.32, 0.85, "NDC");
    fit_y_sg->SetTextAlign(12);
    fit_y_sg->AddText(Form("A1 = %.0f #pm %.0f", func_y_sg->GetParameter(0), func_y_sg->GetParError(0)));
    fit_y_sg->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_y_sg->GetParameter(1), func_y_sg->GetParError(1)));
    fit_y_sg->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_y_sg->GetParameter(2), func_y_sg->GetParError(2)));
    fit_y_sg->AddText(Form(" "));
    fit_y_sg->AddText(Form("offset = %.1f #pm %.1f", func_y_sg->GetParameter(3), func_y_sg->GetParError(3)));
    h_res_y->Draw();
    fit_y_sg->Draw();
    residual_data_lines[27] = residual_data_lines[27] + Form("%.1f, ", func_y_sg->GetParameter(2));
    residual_data_lines[28] = residual_data_lines[28] + Form("%.1f, ", func_y_sg->GetParError(2));


    c_res_spatial->cd(4);
    gPad->SetLeftMargin(0.14);
    gPad->SetBottomMargin(0.14);
    h_res_mask_y->GetXaxis()->CenterTitle();
    h_res_mask_y->GetXaxis()->SetTitleSize(0.05);
    h_res_mask_y->GetYaxis()->CenterTitle();
    h_res_mask_y->GetYaxis()->SetTitleSize(0.05);
    TF1* func_y_sg_mask = new TF1("func_y_sg_mask", "[0]*exp(-(x-[1])*(x-[1])/(2*[2]*[2])) + [3]", h_res_mask_y->GetBinLowEdge(1), h_res_mask_y->GetBinLowEdge(h_res_mask_y->GetNbinsX() + 1));
    func_y_sg_mask->SetNpx(1000);
    func_y_sg_mask->SetParameters(1000, 0, 100, 10);
    h_res_mask_y->Fit("func_y_sg_mask", " ", " ", -1500, 1500);
    TPaveText* fit_y_sg_mask = new TPaveText(0.14, 0.65, 0.32, 0.85, "NDC");
    fit_y_sg_mask->SetTextAlign(12);
    fit_y_sg_mask->AddText(Form("A1 = %.0f #pm %.0f", func_y_sg_mask->GetParameter(0), func_y_sg_mask->GetParError(0)));
    fit_y_sg_mask->AddText(Form("#mu1 = (%.1f #pm %.1f) #mum", func_y_sg_mask->GetParameter(1), func_y_sg_mask->GetParError(1)));
    fit_y_sg_mask->AddText(Form("#sigma1 = (%.1f #pm %.1f) #mum", func_y_sg_mask->GetParameter(2), func_y_sg_mask->GetParError(2)));
    fit_y_sg_mask->AddText(Form(" "));
    fit_y_sg_mask->AddText(Form("offset = %.1f #pm %.1f", func_y_sg_mask->GetParameter(3), func_y_sg_mask->GetParError(3)));
    h_res_mask_y->Draw();
    fit_y_sg_mask->Draw();
    residual_data_lines[29] = residual_data_lines[29] + Form("%.1f, ", func_y_sg_mask->GetParameter(2));
    residual_data_lines[30] = residual_data_lines[30] + Form("%.1f, ", func_y_sg_mask->GetParError(2));

    c_res_spatial->Print(output_plots_residuals);
    c_res_spatial->Print(output_plots_residuals + "]");



    /*h_res_time->Write();
    h_res_time_x_fec1->Write();
    h_res_time_x_fec4->Write();
    h_res_time_y_fec2->Write();
    h_res_time_y_fec5->Write();
    h_res_mask_time->Write();
    h_res_mask_time_x_fec1->Write();
    h_res_mask_time_x_fec4->Write();
    h_res_mask_time_y_fec2->Write();
    h_res_mask_time_y_fec5->Write();
    h_res_x->Write();
    h_res_y->Write();
    h_res_mask_x->Write();
    h_res_mask_y->Write();
    */
    output_file->Close();



    std::cout << " " << std::endl;
    std::cout << "amount tacks    " << N_tracks << std::endl;
    std::cout << "amount tacks used    " << amount_used_tracks << std::endl;
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
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "x-plane_BG   " << std::endl;
    std::cout << "0  " << hits_per_track_x_BG[0] << std::endl;
    std::cout << "1  " << hits_per_track_x_BG[1] << std::endl;
    std::cout << "2  " << hits_per_track_x_BG[2] << std::endl;
    std::cout << "3  " << hits_per_track_x_BG[3] << std::endl;
    std::cout << "y-plane_BG   " << std::endl;
    std::cout << "0  " << hits_per_track_y_BG[0] << std::endl;
    std::cout << "1  " << hits_per_track_y_BG[1] << std::endl;
    std::cout << "2  " << hits_per_track_y_BG[2] << std::endl;
    std::cout << "3  " << hits_per_track_y_BG[3] << std::endl;



    double p0_x = 1.0*hits_per_track_x[0]/amount_used_tracks;
    double p0_y = 1.0*hits_per_track_y[0]/amount_used_tracks;
    double dp0_x = sqrt(p0_x*(1-p0_x)/amount_used_tracks);
    double dp0_y = sqrt(p0_y*(1-p0_y)/amount_used_tracks);
    double eff_app_x = 1.0*(amount_used_tracks - hits_per_track_x[0])/amount_used_tracks;
    double err_x_low = eff_app_x - TEfficiency::ClopperPearson(amount_used_tracks, amount_used_tracks - hits_per_track_x[0], 0.68, false);
    double err_x_up = -eff_app_x + TEfficiency::ClopperPearson(amount_used_tracks, amount_used_tracks - hits_per_track_x[0], 0.68, true);
    double eff_app_y = 1.0*(amount_used_tracks - hits_per_track_y[0])/amount_used_tracks;
    double err_y_low = eff_app_y - TEfficiency::ClopperPearson(amount_used_tracks, amount_used_tracks - hits_per_track_y[0], 0.68, false);
    double err_y_up = -eff_app_y + TEfficiency::ClopperPearson(amount_used_tracks, amount_used_tracks - hits_per_track_y[0], 0.68, true);
    double B_x = 1.0*(hits_per_track_x_BG[1] + 2*hits_per_track_x_BG[2] + 3*hits_per_track_x_BG[3] + 4*hits_per_track_x_BG[4])/amount_used_tracks;
    double B_y = 1.0*(hits_per_track_y_BG[1] + 2*hits_per_track_y_BG[2] + 3*hits_per_track_y_BG[3] + 4*hits_per_track_y_BG[4])/amount_used_tracks;
    //double eff_x = 1.0*(eff_app_x - B_x)/(1 - B_x);
    //double eff_y = 1.0*(eff_app_y - B_y)/(1 - B_y);
    double eff_x = 1.0 - p0_x*exp(B_x);
    double eff_y = 1.0 - p0_y*exp(B_y);
    double dB_x = sqrt(hits_per_track_x_BG[1] + 2*hits_per_track_x_BG[2] + 3*hits_per_track_x_BG[3] + 4*hits_per_track_x_BG[4])/amount_used_tracks;
    double dB_y = sqrt(hits_per_track_y_BG[1] + 2*hits_per_track_y_BG[2] + 3*hits_per_track_y_BG[3] + 4*hits_per_track_y_BG[4])/amount_used_tracks;

    double p0_x_mask = 1.0*hits_per_mask_track_x[0]/amount_used_tracks_mask;
    double p0_y_mask = 1.0*hits_per_mask_track_y[0]/amount_used_tracks_mask;
    double dp0_x_mask = sqrt(p0_x_mask*(1-p0_x_mask)/amount_used_tracks_mask);
    double dp0_y_mask = sqrt(p0_y_mask*(1-p0_y_mask)/amount_used_tracks_mask);
    double eff_app_x_mask = 1.0*(amount_used_tracks_mask - hits_per_mask_track_x[0])/amount_used_tracks_mask;
    double err_x_low_mask = eff_app_x_mask - TEfficiency::ClopperPearson(amount_used_tracks_mask, amount_used_tracks_mask - hits_per_mask_track_x[0], 0.68, false);
    double err_x_up_mask = -eff_app_x_mask + TEfficiency::ClopperPearson(amount_used_tracks_mask, amount_used_tracks_mask - hits_per_mask_track_x[0], 0.68, true);
    double eff_app_y_mask = 1.0*(amount_used_tracks_mask - hits_per_mask_track_y[0])/amount_used_tracks_mask;
    double err_y_low_mask = eff_app_y_mask - TEfficiency::ClopperPearson(amount_used_tracks_mask, amount_used_tracks_mask - hits_per_mask_track_y[0], 0.68, false);
    double err_y_up_mask = -eff_app_y_mask + TEfficiency::ClopperPearson(amount_used_tracks_mask, amount_used_tracks_mask - hits_per_mask_track_y[0], 0.68, true);
    double B_x_mask = 1.0*(hits_per_mask_track_x_BG[1] + 2*hits_per_mask_track_x_BG[2] + 3*hits_per_mask_track_x_BG[3] + 4*hits_per_mask_track_x_BG[4])/amount_used_tracks_mask;
    double B_y_mask = 1.0*(hits_per_mask_track_y_BG[1] + 2*hits_per_mask_track_y_BG[2] + 3*hits_per_mask_track_y_BG[3] + 4*hits_per_mask_track_y_BG[4])/amount_used_tracks_mask;
    //double eff_x = 1.0*(eff_app_x - B_x)/(1 - B_x);
    //double eff_y = 1.0*(eff_app_y - B_y)/(1 - B_y);
    double eff_x_mask = 1.0 - p0_x_mask*exp(B_x_mask);
    double eff_y_mask = 1.0 - p0_y_mask*exp(B_y_mask);
    double dB_x_mask = sqrt(hits_per_mask_track_x_BG[1] + 2*hits_per_mask_track_x_BG[2] + 3*hits_per_mask_track_x_BG[3] + 4*hits_per_mask_track_x_BG[4])/amount_used_tracks_mask;
    double dB_y_mask = sqrt(hits_per_mask_track_y_BG[1] + 2*hits_per_mask_track_y_BG[2] + 3*hits_per_mask_track_y_BG[3] + 4*hits_per_mask_track_y_BG[4])/amount_used_tracks_mask;

    std::cout << " " << std::endl;
    std::cout << " " << std::endl;
    std::cout << "app efficicency x-plane  " << 100*eff_app_x << " +" << 100*err_x_up << " -" << 100*err_x_low << " %" << std::endl;
    std::cout << "app efficicency y-plane  " << 100*eff_app_y << " +" << 100*err_y_up << " -" << 100*err_y_low << " %" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "background x-plane       " << 100*B_x << " +/-" << 100*dB_x << " %" << std::endl;
    std::cout << "background y-plane       " << 100*B_y << " +/-" << 100*dB_y << " %" << std::endl;
    std::cout << " " << std::endl;
    std::cout << "efficicency x-plane      " << 100*eff_x << std::endl;
    std::cout << "efficicency y-plane      " << 100*eff_y << std::endl;
    std::cout << " " << std::endl;
    std::cout << " " << std::endl;

    residual_data_lines[1] = residual_data_lines[1] + std::to_string(amount_used_tracks) + ", ";
    residual_data_lines[2] = residual_data_lines[2] + std::to_string(hits_per_track_x[0]) + ", ";
    residual_data_lines[3] = residual_data_lines[3] + std::to_string(hits_per_track_y[0]) + ", ";
    residual_data_lines[4] = residual_data_lines[4] + std::to_string(amount_used_tracks_mask) + ", ";
    residual_data_lines[5] = residual_data_lines[5] + std::to_string(hits_per_mask_track_x[0]) + ", ";
    residual_data_lines[6] = residual_data_lines[6] + std::to_string(hits_per_mask_track_y[0]) + ", ";

    residual_data_lines[31] = residual_data_lines[31] + std::to_string(hits_per_track_x_BG[1] + 2*hits_per_track_x_BG[2] + 3*hits_per_track_x_BG[3] + 4*hits_per_track_x_BG[4]) + ", ";
    residual_data_lines[32] = residual_data_lines[32] + std::to_string(hits_per_track_y_BG[1] + 2*hits_per_track_y_BG[2] + 3*hits_per_track_y_BG[3] + 4*hits_per_track_y_BG[4]) + ", ";
    residual_data_lines[33] = residual_data_lines[33] + std::to_string(hits_per_mask_track_x_BG[1] + 2*hits_per_mask_track_x_BG[2] + 3*hits_per_mask_track_x_BG[3] + 4*hits_per_mask_track_x_BG[4]) + ", ";
    residual_data_lines[34] = residual_data_lines[34] + std::to_string(hits_per_mask_track_y_BG[1] + 2*hits_per_mask_track_y_BG[2] + 3*hits_per_mask_track_y_BG[3] + 4*hits_per_mask_track_y_BG[4]) + ", ";
    std::ofstream residual_data_file_out(Form("scan_%lld_residual_data.txt", VMM_scan_number));
    for (const auto& current_line : residual_data_lines) {
        residual_data_file_out << current_line << std::endl;
    }
    residual_data_file_out.close();


    //const char* output_txt = Form(output_efficiency);
    std::ofstream outFile(output_efficiency);

    outFile << " " << std::endl;
    outFile << "amount tacks    " << N_tracks << std::endl;
    outFile << "amount tacks used    " << amount_used_tracks << std::endl;
    outFile << "x-plane   " << std::endl;
    outFile << "0  " << hits_per_track_x[0] << std::endl;
    outFile << "1  " << hits_per_track_x[1] << std::endl;
    outFile << "2  " << hits_per_track_x[2] << std::endl;
    outFile << "3  " << hits_per_track_x[3] << std::endl;
    outFile << "y-plane   " << std::endl;
    outFile << "0  " << hits_per_track_y[0] << std::endl;
    outFile << "1  " << hits_per_track_y[1] << std::endl;
    outFile << "2  " << hits_per_track_y[2] << std::endl;
    outFile << "3  " << hits_per_track_y[3] << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "x-plane_BG   " << std::endl;
    outFile << "0  " << hits_per_track_x_BG[0] << std::endl;
    outFile << "1  " << hits_per_track_x_BG[1] << std::endl;
    outFile << "2  " << hits_per_track_x_BG[2] << std::endl;
    outFile << "3  " << hits_per_track_x_BG[3] << std::endl;
    outFile << "y-plane_BG   " << std::endl;
    outFile << "0  " << hits_per_track_y_BG[0] << std::endl;
    outFile << "1  " << hits_per_track_y_BG[1] << std::endl;
    outFile << "2  " << hits_per_track_y_BG[2] << std::endl;
    outFile << "3  " << hits_per_track_y_BG[3] << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "app efficicency x-plane  " << 100*eff_app_x << " +" << 100*err_x_up << " -" << 100*err_x_low << " %" << std::endl;
    outFile << "app efficicency y-plane  " << 100*eff_app_y << " +" << 100*err_y_up << " -" << 100*err_y_low << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << "background x-plane       " << 100*B_x << " +/-" << 100*dB_x << " %" << std::endl;
    outFile << "background y-plane       " << 100*B_y << " +/-" << 100*dB_y << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << "efficicency x-plane      " << 100*eff_x << " %" << std::endl;
    outFile << "efficicency y-plane      " << 100*eff_y << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "Masked section boundaries" << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "amount tacks    " << N_tracks << std::endl;
    outFile << "amount tacks used    " << amount_used_tracks_mask << std::endl;
    outFile << "x-plane   " << std::endl;
    outFile << "0  " << hits_per_mask_track_x[0] << std::endl;
    outFile << "1  " << hits_per_mask_track_x[1] << std::endl;
    outFile << "2  " << hits_per_mask_track_x[2] << std::endl;
    outFile << "3  " << hits_per_mask_track_x[3] << std::endl;
    outFile << "y-plane   " << std::endl;
    outFile << "0  " << hits_per_mask_track_y[0] << std::endl;
    outFile << "1  " << hits_per_mask_track_y[1] << std::endl;
    outFile << "2  " << hits_per_mask_track_y[2] << std::endl;
    outFile << "3  " << hits_per_mask_track_y[3] << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "x-plane_BG   " << std::endl;
    outFile << "0  " << hits_per_mask_track_x_BG[0] << std::endl;
    outFile << "1  " << hits_per_mask_track_x_BG[1] << std::endl;
    outFile << "2  " << hits_per_mask_track_x_BG[2] << std::endl;
    outFile << "3  " << hits_per_mask_track_x_BG[3] << std::endl;
    outFile << "y-plane_BG   " << std::endl;
    outFile << "0  " << hits_per_mask_track_y_BG[0] << std::endl;
    outFile << "1  " << hits_per_mask_track_y_BG[1] << std::endl;
    outFile << "2  " << hits_per_mask_track_y_BG[2] << std::endl;
    outFile << "3  " << hits_per_mask_track_y_BG[3] << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
    outFile << "app efficicency x-plane  " << 100*eff_app_x_mask << " +" << 100*err_x_up_mask << " -" << 100*err_x_low_mask << " %" << std::endl;
    outFile << "app efficicency y-plane  " << 100*eff_app_y_mask << " +" << 100*err_y_up_mask << " -" << 100*err_y_low_mask << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << "background x-plane       " << 100*B_x_mask << " +/-" << 100*dB_x_mask << " %" << std::endl;
    outFile << "background y-plane       " << 100*B_y_mask << " +/-" << 100*dB_y_mask << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << "efficicency x-plane      " << 100*eff_x_mask << " %" << std::endl;
    outFile << "efficicency y-plane      " << 100*eff_y_mask << " %" << std::endl;
    outFile << " " << std::endl;
    outFile << " " << std::endl;
}


