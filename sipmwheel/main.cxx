//
// File: main.cxx
//
// Author: Hunter Sullivan
//
// Decription: Analysis code for sipmwheel
//

// Includes
#include <iostream>
#include <fstream>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <map>
#include <list>
#include "TFile.h"
#include "TCanvas.h"
#include "Utilities.h"
#include "FileReader.h"
#include "Characterizer.h"

// Prototypes
void PrintTheFiles(const wheel::SiPMToFilesMap& sipmToFilesMap);
void GetTheFiles(wheel::SiPMToFilesMap& map, const wheel::Configuration& config);
void ReadConfigFile(wheel::Configuration& config);
void SaveGraphs(std::vector<TGraph>& graphs, std::vector<std::vector<TMarker>>& markers, const wheel::Configuration& config);
void Characterize(const wheel::Configuration& myConfig);
void Reco(const wheel::Configuration& myConfig);
void RecordBiases(wheel::Configuration& config, const std::string& value);

int main(int argc, char **argv)
{
  // First check to make sure the path to data is given
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " pathToConfigFile " << " characterize/reco  " <<std::endl;
    exit(1);
  }

  // First set the configuration
  std::cout << "\nReading configuration file... " << std::endl;
  wheel::Configuration myConfig;
  myConfig.pathToConfig = argv[1];
  ReadConfigFile(myConfig);

  if      (argv[2] == "characterize") Characterize(myConfig);
  else if (argv[2] == "reco")         Reco(myConfig);
  else    { std::cout << "Error. Must choose characterize or reco." << std::endl; exit(1); }

  return 0;
}

void Characterize(const wheel::Configurtion& myConfig)
{
  // First we need to get the files to read data from
  std::cout << "Getting the files from: " << myConfig.pathToData << std::endl;
  wheel::SiPMToFilesMap sipmToFilesMap;
  GetTheFiles(sipmToFilesMap, myConfig);

  // Option to print the collected files
  if (myConfig.printFiles) PrintTheFiles(sipmToFilesMap);

  // Now read the files
  std::cout << "Reading the files... " << std::endl;
  wheel::SiPMToHitCandVecMap sipmToHitCandVecMap;
  wheel::FileReader fr;
  fr.ReadFiles(sipmToHitCandVecMap, sipmToFilesMap, myConfig);

  // Characterize
  Characterizer ch;
  ch.Characterize(sipmToHitCandVec, myConfig);

  // Option to output graphs
  if (myConfig.saveGraphs) SaveGraphs(fr.GetGraphs(), fr.GetMarkers(), myConfig);

  return 0;
}

void Reco(const wheel::Configuration& myConfig)
{

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
    else if (header == "saveGraphs")              value == "true" ? config.saveGraphs       = true : config.saveGraphs       = false;
    else if (header == "nSiPMs")                  config.nSiPMs        = std::stoi(value);
    else if (header == "smaRange")                config.smaRange      = std::stoi(value);
    else if (header == "resolution")              config.resolution    = std::stoi(value);
    else if (header == "hitSigma")                config.hitSigma      = std::stof(value);
    else if (header == "minimumHitAmp")           config.minimumHitAmp = std::stof(value);
    else if (header == "characterizeAmpThr")      config.characterizeAmpThr      = std::stof(value);
    else if (header == "characterizeAmpSig")      config.characterizeAmpSig      = std::stof(value);
    else if (header == "characterizeAmpFitRange") config.characterizeAmpFitRange = std::stof(value);
    else if (header == "characterizeNBiases")     config.characerizeNBiases      = std::stoi(value);
    else if (header == "characterizeBiases")      RecordBiases(config, value); 
    else    { std::cout << "Cannot identify " << header << std::endl; exit(1); }
  }
}

void RecordBiases(wheel::Configuration& config, const std::string& value)
{
  // Make sure n biases has been defined
  if (!config.characterizeNBiases) { std::cout << "Error. Please specify number of biases before listing biases." << std::endl; exit(1); }
  
  std::stringsteam linestream(value);

  for (unsigned b = 1; b <= config.characterizeNBiases; b++)
  {
    if (b == config.characterizeNBiases) 
    {
      std::getline(linestream, bias);
      config.characterizeBiases.emplace_back(std::stof(bias));
      break;
    }
    std::getline(linestream, bias, ",");
    config.characterizeBiases.emplace_back(std::stof(bias));
  }
}

void GetTheFilesForCharacerization(wheel::SiPMToFilesMap& map, const wheel::Configuration& config)
{
  DIR *dir;
  struct dirent *file;
  struct stat filestat;

  for (unsigned sipm = 1 ; sipm <= config.nSiPMs; sipm++)
  {
    std::string thisDir = config.pathToData + "/sipm" + std::to_string(sipm);
    std::list<std::string> sipmData;
    if ((dir = opendir (thisDir.c_str())) != NULL)
    {
      // Print all the files and directories within directory
      while ((file = readdir (dir)) != NULL) {
        std::string filepath = thisDir + "/" + file->d_name;
        // If the file is a directory skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;
        // Store into our file list
        sipmData.emplace_back(filepath);
      }
      closedir (dir);
    } else
    {
      // Could not open directory
      std::string error = "Could not open directory: " + thisDir;
      perror (error.c_str());
      exit(1);
    }
    map.emplace(sipm, sipmData);
  }
}


void GetTheFiles(wheel::SiPMToFilesMap& map, const wheel::Configuration& config)
{
  DIR *dir;
  struct dirent *file;
  struct stat filestat;

  for (unsigned sipm = 1 ; sipm <= config.nSiPMs; sipm++)
  {
    std::string thisDir = config.pathToData + "/sipm" + std::to_string(sipm);
    std::list<std::string> sipmData;
    if ((dir = opendir (thisDir.c_str())) != NULL)
    {
      // Print all the files and directories within directory
      while ((file = readdir (dir)) != NULL) {
        std::string filepath = thisDir + "/" + file->d_name;
        // If the file is a directory skip it
        if (stat( filepath.c_str(), &filestat )) continue;
        if (S_ISDIR( filestat.st_mode ))         continue;
        // Store into our file list
        sipmData.emplace_back(filepath);
      }
      closedir (dir);
    } else
    {
      // Could not open directory
      std::string error = "Could not open directory: " + thisDir;
      perror (error.c_str());
      exit(1);
    }
    map.emplace(sipm, sipmData);
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

void SaveGraphs(std::vector<TGraph>& graphs, std::vector<std::vector<TMarker>>& markers, const wheel::Configuration& config)
{
  std::cout << "Outputing file... " << std::endl;
  // Create a file to write to
  TFile f(config.outputPath.c_str(), "RECREATE");
  
  unsigned counter = 0;
  for (auto& g : graphs)
  {
    // Set a safety net here
    if (counter > 1) return;

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

