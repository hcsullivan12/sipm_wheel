//
// File: main.cxx
//
// Author: Hunter Sullivan
//
// Decription: Analysis code for sipmwheel
//             This analyzes waveforms seen by each sipm for each "trigger"
// 
// Future work: Implent this into the live event display.
//

// Includes
#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <experimental/filesystem>
#include <map>
#include <list>
#include "TFile.h"
#include "TCanvas.h"
#include "Utilities.h"
#include "FileReader.h"
#include "Analyzer.h"
#include "Characterizer.h"

// Prototypes
void PrintTheFiles(const wheel::SiPMToFilesMap& sipmToFilesMap);
void PrintTheFiles(const wheel::SiPMToBiasTriggerMap& sipmToBiasTriggerMap);
void GetTheCharacterizationFiles(wheel::SiPMToBiasTriggerMap& map, const wheel::Configuration& config);
void GetTheFiles(wheel::SiPMToFilesMap& map, const wheel::Configuration& config);
void ReadConfigFile(wheel::Configuration& config);
void SaveWaveforms(std::vector<TGraph>& graphs, std::vector<std::vector<TMarker>>& markers, const wheel::Configuration& config);
void Characterize(const wheel::Configuration& myConfig);
void Reco(const wheel::Configuration& myConfig);
void RecordBiases(wheel::Configuration& config, const std::string& value);
void RecordGains(std::map<unsigned, float>& map, const wheel::Configuration& config, const std::string& value);
void FillSiPMInfo(wheel::SiPMInfoMap& sipmInfoMap, const wheel::Configuration& config);
void SaveCharacterizationPlots(std::multimap<unsigned, std::vector<TH1D>>         ampDists,
                               std::multimap<unsigned, std::vector<TGraphErrors>> ampPeaks,
                               const wheel::Configuration&                        config);

int main(int argc, char **argv)
{
  // First check to make sure the path to data is given
  if (argc < 2)
  {
    std::cerr << "\nUsage: " << argv[0] << " pathToConfigFile\n" << std::endl;
    exit(1);
  }

  // First set the configuration
  std::cout << "\nReading configuration file... " << std::endl;
  wheel::Configuration myConfig;
  myConfig.pathToConfig = argv[1];
  ReadConfigFile(myConfig);

  //const std::string process = argv[2];
  if      (myConfig.process == "characterize") Characterize(myConfig);
  else if (myConfig.process == "reco")         Reco(myConfig);
  else    { std::cout << "Error. Must choose characterize or reco.\n" << myConfig.process << std::endl; exit(1); }

  return 0;
}

void Reco(const wheel::Configuration& myConfig)
{
  // First we need to get the files to read data from
  std::cout << "Getting the files from: " << myConfig.pathToData << std::endl;
  wheel::SiPMToFilesMap sipmToFilesMap;
  GetTheFiles(sipmToFilesMap, myConfig);

  // Option to print the collected files
  if (myConfig.printFiles) PrintTheFiles(sipmToFilesMap);

  // Now read the files
  std::cout << "Reading the files... " << std::endl;
  wheel::SiPMToTriggerMap sipmToTriggerMap;
  wheel::FileReader fr;
  fr.ReadFiles(sipmToTriggerMap, sipmToFilesMap, myConfig);
 
  // Option to output graphs
  if (myConfig.saveWaveforms) SaveWaveforms(fr.GetGraphs(), fr.GetMarkers(), myConfig);

  // Start the main work horse after filling sipm info
  wheel::SiPMInfoMap sipmInfoMap;
  FillSiPMInfo(sipmInfoMap, myConfig);
  wheel::Analyzer analyzer;
  analyzer.RunReco(sipmToTriggerMap, sipmInfoMap, myConfig);
}

void Characterize(const wheel::Configuration& myConfig)
{
  // First we need to get the files to read data from
  std::cout << "Getting the files from: " << myConfig.pathToData << std::endl;
  wheel::SiPMToBiasTriggerMap sipmToBiasTriggerMap;
  GetTheCharacterizationFiles(sipmToBiasTriggerMap, myConfig);

  // Option to print the collected files
  if (myConfig.printFiles) PrintTheFiles(sipmToBiasTriggerMap);

  // Now read the files
  std::cout << "Reading the files... " << std::endl;
  wheel::SiPMToTriggerMap sipmToTriggerMap;
  wheel::FileReader fr;
  fr.ReadFiles(sipmToTriggerMap, sipmToBiasTriggerMap, myConfig);
 
  // Option to output graphs
  if (myConfig.saveWaveforms) SaveWaveforms(fr.GetGraphs(), fr.GetMarkers(), myConfig);

  wheel::Characterizer ch;
  wheel::SiPMInfoMap sipmInfoMap;
  ch.Characterize(sipmInfoMap, sipmToTriggerMap, myConfig);

  // Save the plots
  SaveCharacterizationPlots(ch.GetAmpDists(), ch.GetAmpPeaks(), myConfig);
}

void FillSiPMInfo(wheel::SiPMInfoMap& sipmInfoMap, const wheel::Configuration& config)
{
  // Loop over sipms
  for (unsigned sipm = 1; sipm <= config.nSiPMs; sipm++)
  {
    wheel::SiPMInfo sipmInfo;
    sipmInfo.gain      = config.gains.find(sipm)->second;
    sipmInfo.breakdown = config.breakdowns.find(sipm)->second; 

    sipmInfoMap.emplace(sipm, sipmInfo);
  }
}

void ReadConfigFile(wheel::Configuration& config)
{
  // Open the file
  std::ifstream file(config.pathToConfig.c_str());
  if (!file.is_open()) {
    std::cout << "Cannot open file: " << config.pathToConfig << std::endl;
    exit(1);
  }
  
  std::string header, value;
  while(std::getline(file, header, '=')) 
  {
    std::getline(file, value);

    if      (header == "pathToData")              config.pathToData = value;
    else if (header == "outputPath")              config.outputPath = value;
    else if (header == "printFiles")              value == "true" ? config.printFiles       = true : config.printFiles       = false;
    else if (header == "baselineSubtract")        value == "true" ? config.baselineSubtract = true : config.baselineSubtract = false;
    else if (header == "saveWaveforms")           value == "true" ? config.saveWaveforms    = true : config.saveWaveforms    = false;
    else if (header == "nSiPMs")                  config.nSiPMs          = std::stoi(value);
    else if (header == "smaRange")                config.smaRange        = std::stoi(value);
    else if (header == "hitFinderSearch")         config.hitFinderSearch = std::stoi(value);
    else if (header == "resolution")              config.resolution      = std::stoi(value);
    else if (header == "hitSigma")                config.hitSigma        = std::stof(value);
    else if (header == "minimumHitAmp")           config.minimumHitAmp   = std::stof(value);
    else if (header == "process")                 value == "characterize" ? config.process  = "characterize" : config.process = "reco"; // default to reco
    else if (header == "characterizeAmpThr")      config.characterizeAmpThr      = std::stof(value);
    else if (header == "characterizeAmpSig")      config.characterizeAmpSig      = std::stof(value);
    else if (header == "characterizeAmpFitRange") config.characterizeAmpFitRange = std::stof(value);
    else if (header == "characterizeOutputFile")  config.characterizeOutputFile  = value;
    else if (header == "nBiases")                 config.nBiases                 = std::stoi(value);
    else if (header == "biases")                  RecordBiases(config, value); 
    else if (header == "gains")                   RecordGains(config.gains, config, value);
    else if (header == "breakdowns")              RecordGains(config.breakdowns, config, value);
    else if (header == "thetaBinSize")            config.thetaBinSize  = std::stoi(value);
    else if (header == "radiusBinSize")           config.radiusBinSize = std::stoi(value);
    else if (header == "attenuationLengthBinSize") config.attenuationLengthBinSize = std::stoi(value);
    else if (header == "diskRadius")              config.diskRadius = std::stof(value);
    else    { std::cout << "Cannot identify " << header << std::endl; exit(1); }
  }
  
  // Place a series of safety nets here;
  if (config.nBiases != config.biases.size())     { std::cout << "Error. Number of biases does not match in config.\n" << std::endl; exit(1); }
  if (config.nSiPMs  != config.gains.size())      { std::cout << "Error. Number of gains does not match number of SiPMs\n." << std::endl; exit(1); }
  if (config.nSiPMs  != config.breakdowns.size()) { std::cout << "Error. Number of breakdowns does not match number of SiPMs\n." << std::endl; exit(1); }
}

void RecordGains(std::map<unsigned, float>& map, const wheel::Configuration& config, const std::string& value)
{
  // Make sure n sipms has been defined
  if (!config.nSiPMs) { std::cout << "Error. Please specify number of sipms before listing gains and breakdowns.\n" << std::endl; exit(1); }
  
  std::stringstream linestream(value);
  std::string gain;
  unsigned sipm = 1;
  while(std::getline(linestream, gain, ',')) { map.emplace(sipm, std::stof(gain)); sipm++; }
}


void RecordBiases(wheel::Configuration& config, const std::string& value)
{
  // Make sure n biases has been defined
  if (!config.nBiases) { std::cout << "Error. Please specify number of biases before listing biases.\n" << std::endl; exit(1); }
  
  std::stringstream linestream(value);
  std::string bias;
  while(std::getline(linestream, bias, ',')) config.biases.insert(std::stof(bias));
}

void GetTheCharacterizationFiles(wheel::SiPMToBiasTriggerMap& map, const wheel::Configuration& config)
{
  namespace stdfs = std::experimental::filesystem;

  for (unsigned sipm = 1; sipm <= config.nSiPMs; sipm++)
  {
    // What is the directory
    std::string sipmDir = config.pathToData + "/sipm" + std::to_string(sipm);
    std::set<std::string> sipmData;
    // Create a temp map for storing these
    std::map<float, std::set<std::string>> tempMap; 
    
    // Loop over the bias directories
    for(const auto& biasDirIter : stdfs::directory_iterator(sipmDir.c_str()))
    {
      // First get the bias
      std::string bias;
      std::string path = biasDirIter.path();
      for (std::string::iterator ch = path.end(); *ch != '/'; ch--) bias = *ch + bias;
      const float thisBias = std::stof(bias);

      // Loop over files?
      std::set<std::string> files;
      for (const auto& fileIter : stdfs::directory_iterator(biasDirIter.path().c_str()))
      {
        // Store these into our set
        files.insert(fileIter.path());
      }
      tempMap.emplace(thisBias, files);
    }

    // Insert into our map
    map.emplace(sipm, tempMap);
  }
}

void GetTheFiles(wheel::SiPMToFilesMap& map, const wheel::Configuration& config)
{
  namespace stdfs = std::experimental::filesystem;

  for (unsigned sipm = 1; sipm <= config.nSiPMs; sipm++)
  {
    // What is the directory
    std::string sipmDir = config.pathToData + "/sipm" + std::to_string(sipm);
    std::set<std::string> sipmData;
    // Create a temp map for storing these
    std::map<float, std::set<std::string>> tempMap; 
    // 
    // Loop over the bias directories
    for(const auto& biasDirIter : stdfs::directory_iterator(sipmDir.c_str()))
    {
      // First get the bias
      std::string bias;
      std::string path = biasDirIter.path();
      for (std::string::iterator ch = path.end(); *ch != '/'; ch--) bias = *ch + bias;
      const float thisBias = std::stof(bias);

      // Loop over files?
      std::set<std::string> files;
      for (const auto& fileIter : stdfs::directory_iterator(biasDirIter.path().c_str()))
      {
        // Store these into our set
        files.insert(fileIter.path());
      }
      tempMap.emplace(thisBias, files);
    }
    // Insert into our map
    map.emplace(sipm, tempMap);
  }
}

void PrintTheFiles(const wheel::SiPMToFilesMap& map)
{
  std::cout << "\nReading the following files: " << std::endl;
  for (const auto& sipmData : map)
  {
    std::cout << sipmData.second.size() << " files for SiPM " << sipmData.first << ":" << std::endl;
    for (const auto& file : sipmData.second)
    {
      std::cout << file << std::endl;
    }
    std::cout << std::endl;
  }
}

void PrintTheFiles(const wheel::SiPMToBiasTriggerMap& map)
{
  std::cout << "\nReading the following files: " << std::endl;
  for (const auto& sipmData : map)
  {
    std::cout << sipmData.second.size() << " biases for SiPM " << sipmData.first << ":" << std::endl;
    for (const auto& bias : sipmData.second)
    {
      std::cout << "Bias = " << bias.first << std::endl;
      for (const auto& file : bias.second) std::cout << file << std::endl;
    }
    std::cout << std::endl;
  }
}

void SaveCharacterizationPlots(std::multimap<unsigned, std::vector<TH1D>>         ampDists, 
                               std::multimap<unsigned, std::vector<TGraphErrors>> ampPeaks, 
                               const wheel::Configuration&                        config)
{
  // Canvases for out histos
  TCanvas masterAmpDist("masterAmpDist",  "All Amplitude Distributions", 1000, 1000);
  TCanvas masterGainPlot("masterGainPlot", "All Gains", 1000, 1000); 

  // Divide the canvases
  masterAmpDist.Divide(config.nBiases,config.nSiPMs);
  masterGainPlot.Divide(config.nBiases,config.nSiPMs);

  // Loop over sipms
  unsigned counter(1);
  for (unsigned sipm = 1; sipm <= config.nSiPMs; sipm++)
  {
    // Get the dists for this sipm
    //std::vector<TH1D>& dists = ampDists.find(sipm)->second;
    // Draw the amp dists and gain plot
    for (auto& dist : ampDists.find(sipm)->second)
    {
      // Amplitude dist
      masterAmpDist.cd(counter);
      gStyle->SetOptStat(0);
      dist.Draw("apl");      
      counter++;
    }
    counter = 1;
    for (auto& peaks : ampPeaks.find(sipm)->second)
    {
      // Gain
      masterGainPlot.cd(counter);
      peaks.SetMarkerStyle(20);
      peaks.SetMarkerColor(1);
      peaks.Draw("AP");
      counter++;
      peaks.GetXaxis()->SetLimits(0, 5);
      peaks.SetMinimum(0);      
    }
  }

  TFile f(config.characterizeOutputFile.c_str(), "RECREATE");
  masterAmpDist.Write();
  masterGainPlot.Write();
  f.Close();
};

void SaveWaveforms(std::vector<TGraph>& graphs, std::vector<std::vector<TMarker>>& markers, const wheel::Configuration& config)
{
  std::cout << "Outputing file... " << std::endl;
  // Create a file to write to
  TFile f(config.outputPath.c_str(), "RECREATE");
  
  unsigned counter = 0;
  for (auto& g : graphs)
  {
    // Set a safety net here
    if (counter >= 10 || counter > markers.size()) break;

    TCanvas c(std::to_string(counter).c_str(), std::to_string(counter).c_str(), 500, 500);
    //g->GetXaxis()->SetRangeUser(1900, 2100);
    //g->Fit(fits[counter], "R+", "", 1988, 2100);
    g.Draw("");
    for (auto& m : markers[counter])
    {
      m.Draw("same");
    }
    // Write this to file
    c.Write();
    counter++;
  }
 
  f.Close();;
}
/*
void MakeRecoPlots()
{
  // Get the counts lambda_m using the mlestimates for r, theta, and N0
  unsigned prediction[numberOfPositions];
  for (int m = 1; m <= numberOfPositions; m++) {
    double lambda_m = ComputeLambda( parameters.find("radius")->second, parameters.find("theta")->second, parameters.find("N0")->second, m );
    prediction[m - 1] = static_cast<unsigned>(lambda_m);
  }

  TH1D *pred = new TH1D("pred", "pred", numberOfPositions, 0, numberOfPositions);
  for (int posBin = 1; posBin <= numberOfPositions; posBin++) {
    std::cout << "Pred Bin " << posBin << " :  " << prediction[posBin - 1] << std::endl;
    pred->SetBinContent( posBin, prediction[posBin - 1] );
  }

  TCanvas *c1 = new TCanvas("c1", "c1", 1000, 1000);
  pred->SetFillStyle(3001);
  pred->SetFillColor(kRed);
  pred->SetLineWidth(3);
  pred->SetLineColor(kRed);
  pred->GetXaxis()->SetTitle("SiPM Position");
  pred->GetYaxis()->SetTitle("p.e");
  pred->SetTitle("Estimator for SiPM Wheel");
  gStyle->SetOptStat(0);
  pred->Draw();

  TH1D *dataHisto = new TH1D("dataHisto", "dataHisto", numberOfPositions, 0, numberOfPositions);
  for (int posBin = 1; posBin <= numberOfPositions; posBin++) {
    std::cout << "Data Bin " << posBin << " :  " << data[posBin - 1] << std::endl;
    dataHisto->SetBinContent( posBin, data[posBin - 1] );
  }
  dataHisto->SetMarkerStyle(21);
  dataHisto->SetMarkerSize(2);
  dataHisto->Draw("same P");

  TLegend *leg = new TLegend(0.1,0.6,0.3,0.7);
  leg->AddEntry(pred, "Estimator", "f");
  leg->AddEntry(dataHisto, "Data", "p");
  leg->Draw("same");

  TF2* fDisk = new TF2("fDisk", disk, -2*diskRadius, 2*diskRadius, -2*diskRadius, 2*diskRadius, 0);
  TCanvas *c2 = new TCanvas("c2", "c2", 1000, 1000);
  fDisk->SetContour(1);
  fDisk->SetContourLevel(0, diskRadius*diskRadius);
  fDisk->SetLineColor(1);
  fDisk->SetLineWidth(4);
  fDisk->GetXaxis()->SetTitle("x/cm");
  fDisk->GetYaxis()->SetTitle("y/cm");
  fDisk->SetTitle("Reconstructed Position of Point Source");
}*/

