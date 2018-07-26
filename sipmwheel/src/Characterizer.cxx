// 
// File: Characterizer.cxx
//
// Author: Hunter Sullivan
//
// Discription: Structure to characterize sipms.
//

#include "Characterizer.h"

namespace wheel {

Characterizer::Characterizer()
{}

Characterizer::~Characterizer()
{}

void Characterizer::CharacterizeSiPMs(const SiPMToHitCandVecMap& sipmToHitCandVecMap, const Configuration& config)
{
  TCanvas masterAmpDist("masterAmpDist",  "All Amplitude Distributions", 1000, 1000);
  TCanvas masterGainPlot("masterGainPlot", "All Gains", 1000, 1000);
  
  // Divide the canvases
  masterAmpDist.Divide(NBiases,nSiPMs);
  masterGainPlot.Divide(nBiases,nSiPMs);
  
  // Loop over the sipms
  unsigned sipmCounter = 0;
  for (const auto& sipm : SiPMToHitCandVecMap)
  {
    TH1D ampDist();
    MakeHistogram(ampDist);
    masterAmpDist.cd(sipmCounter + 1);
    gStyle->SetOptStat(0);
    ampDist.Draw("apl");

    auto ampPeaks = FitGain(ampDist);
    masterGainPlot.cd(sipmCounter + 1);
    
    ampPeaks.SetMarkerStyle(20);
    ampPeaks.SetMarkerColor(1);
    ampPeaks.Draw("AP");
    ampPeaks.GetXaxis()->SetLimits(0, 5);
    ampPeaks.SetMinimum(0);
  }
  
  // Plot versus bias
  TCanvas breakdownPlot("breakdownPlot", "SiPM Breakdown", 800, 800);
  TGraph g1(sipmToHitCandVecMap.size(), bias, &gains[0]);
  g1.SetTitle("Breakdown of SiPM");
  g1.GetXaxis()->SetTitle("Bias (V)");
  g1.GetYaxis()->SetTitle("Gain (mV/p.e.)");
  g1.GetXaxis()->SetLimits(70,75);
  g1.SetMinimum(0);
  g1.SetMaximum( 1.1*(*std::max_element(gains.begin(), gains.end())) );
  g1.SetMarkerStyle(20);
  g1.SetMarkerSize(2);
  g1.Draw("AP");

  // Fit for bias plot
  TF1 fit1("fit1", "[0] + [1]*x", biases.min, biases.max);

  fit1.SetParName(1,"Slope");
  fit1.SetParName(0, "YInt");
  g1.Fit(fit1, "QR");
  gStyle->SetOptFit();

  float breakdown = -fit1.GetParameter(0)/fit1.GetParameter(1);

  std::cout << "\nBreakdown is... " << breakdown << "\n";

  ///Plot versus overvoltage
  double bias_overvoltage[nFiles];
  for (int i = 0; i < nFiles; i++) {
    bias_overvoltage[i] = bias[i] - breakdown;
  }
  TCanvas *overvoltage_plot  = new TCanvas("overvoltage_plot", "Overvoltage Plot", 800, 800);
  TGraph *g2 = new TGraph(nFiles, bias_overvoltage, &gains[0]);
  g2->SetTitle("SiPM Gain");
  g2->GetXaxis()->SetTitle("Over-voltage (#DeltaV)");
  g2->GetYaxis()->SetTitle("Gain (mV/p.e.)");
  g2->GetXaxis()->SetLimits(0,4);
  g2->SetMinimum(0);
  g2->SetMaximum(4);
  g2->SetMarkerStyle(24);
  g2->SetMarkerSize(2);
  g2->Draw("AP");

  ///Fit for overvoltage plot
  TF1 *fit2 = new TF1("fit2", "[0] + [1]*x", 0, 4);

  fit2->SetParName(1,"Slope");
  fit2->SetParName(0, "YInt");
  g2->Fit(fit2, "QR");
  gStyle->SetOptFit();

  //std::cout << fit2->GetParError(0) << "   " << fit2->GetParError(1) <<  std::endl;
  for (unsigned i = 0; i < nFiles; i++) std::cout << "Gain " << i + 1 << " = " << gains[i] << std::endl;
  std::cout << "\n\nGain is... " << std::setprecision(3) << fit2->GetParameter(1) << " mV/p.e./O.V.\n\n";

  ///Place the quantities on plot
  std::string bd = std::to_string(breakdown);
  //double breakdown_error = breakdown*std::sqrt( std::pow(fit2->GetParError(0)/fit1->GetParameter(0), 2) + std::pow( fit2->GetParError(1)/fit2->GetParameter(1), 2) );
  //std::string bd_e = std::to_string(breakdown_error);
  std::string text1 = "V_{BD} = " + bd.substr(0,4) + " V";
  TLatex *bd_label = new TLatex(1,0.4,text1.c_str());
  bd_label->SetTextSize(0.025);
  bd_label->Draw("same");

  std::string g = std::to_string(fit2->GetParameter(1));
  std::string e = std::to_string(fit2->GetParError(1));
  std::string text2 = "G = " + g.substr(0,5) + " #pm " + e.substr(0,5) + " mV/p.e./#DeltaV";
  TLatex *gain_label = new TLatex(1,0.35,text2.c_str());
  gain_label->SetTextSize(0.025);
  gain_label->Draw("same");
}

void Characterizer::MakeHistogram(TH1D& ampDist, const HitCandidateVec& hitCandVec)
{
  // Find min and max in data set
  double xMin = std::numeric_limits<float>::max();
  double xMax = 0;
  for (const auto& hit : hitCandVec) 
  {
    if (hit.hitHeight > xMax) xMax = hit.hitHeight;
    if (hit.hitHeight < xMin) xMin = hit.hitHeight;
  }

  // Make histogram
  ampDist->SetBins(dataSet.first.size(), xMin, xMax);
  unsigned counter(0);
  for (const auto& hit : hitCandVec) {
    ampDist.SetBinContent(counter + 1, hit.hitHeight);
    counter++;
  }

  const std::string s = "SiPM Response in Dark Box " + std::to_string(type) + " V";
  ampDist.SetTitle(s.c_str());
  ampDist.GetXaxis()->SetTitle("Amplitude/Volts");
}

TGraphErrors Characterizer::FitGain(TH1D& hs)
{
  Float_t gain=0;
  TSpectrum s(3);
  //float threshold = ampThreshold;

  Int_t nfound = s.Search(hs, ampSig, "", ampThreshold);
  Int_t npeaks = s.GetNPeaks();
  printf("Found %d peaks to fit\n",npeaks);

  Double_t peaks;
  Double_t altpeaks;
  peaks = s.GetPositionX();

  Double_t x[npeaks], ex[npeaks];
  Double_t y[npeaks], ey[npeaks];

  Double_t gx[npeaks], gex[npeaks];
  Double_t gy[npeaks], gey[npeaks];

  //Sort peaks
  double temp;
  int nchanges=0;
  do 
  {
    nchanges=0;
    for(int p = 0; p < nfound - 1; p++) 
    {
      if(peaks[p] > peaks[p+1]) 
      {
        temp=peaks[p];
        peaks[p]=peaks[p+1];
        peaks[p+1]=temp;
        nchanges++;
      }
    }
  } while(nchanges != 0);

  for (int j = 0; j < npeaks; j++) 
  {
    x[j] =j+1;
    y[j]=peaks[j];
  }

  int point = 1;
  //double fitRange = ampFitRange;
 
  for (int g = 0; g < npeaks; g++) 
  {
    TF1 gfit("gfit", "gaus", y[g] - ampFitRange/2, y[g] + ampFitRange/2);
    hs.Fit(gfit,"QR+");
    
    gx[point - 1] = point;
    gy[point - 1] = gfit.GetParameter(1);  //Mean
    gey[point - 1] = gfit.GetParError(1);
    point++;
  }

  TGraphErrors grpeaks(npeaks,gx,gy,0,gey);
  grpeaks.SetTitle("SiPM Gain from Amplitude Distribution");
  grpeaks.GetYaxis()->SetTitle("Amplitude/Volts");
  
  grpeaks.GetYaxis()->SetTitleOffset(1.4);
  grpeaks.GetXaxis()->SetTitle("Peak N");
  grpeaks.SetMarkerColor(4);
  grpeaks.SetMarkerStyle(20);
  grpeaks.SetFillColor(0);

  // Fit
  TF1 fit("fit","[0] + [1]*x",0.5,npeaks+1);
  fit.SetParName(1,"Gain");
  fit.SetParName(0, "Pedestal");
  grpeaks.Fit(fit, "QR");
  gStyle.SetOptFit();

  gains.push_back( 1000*fit->GetParameter(1) ); //Converted to mV 

  return grpeaks;
}



}
