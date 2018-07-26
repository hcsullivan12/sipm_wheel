// 
// File: FileReader.cxx
//
// Author: Hunter Sullivan
//
// Discription: Structure to read the input waveform files.
//

#include "FileReader.h"

namespace wheel {

FileReader::FileReader()
{}

FileReader::~FileReader()
{}

void FileReader::ReadFiles(SiPMToHitCandVecMap& sipmToHitCandVecMap, const SiPMToFilesMap& map, const wheel::Configuration& config)
{
  // Loop over the sipms
  unsigned channel = 1;
  for (const auto& sipm : map)
  {
    // Create a temp hitVec
    HitCandidateVec hitCandidateVec;
    // Loop over the files for this sipm
    for (const auto& file : sipm.second)
    {
      // Now read this file
      ReadFile(hitCandidateVec, file, channel, config);
    }
    // Add this data to the hitVecMap
    sipmToHitCandVecMap.emplace(channel, hitCandidateVec);
    channel++;
  }
}

void FileReader::ReadFile(HitCandidateVec& hitCandidateVec, const std::string& filename, const unsigned& channel, const wheel::Configuration& config)
{
  // Open the file
  std::ifstream file(filename.c_str());
  if (!file.is_open()) {
    std::cout << "Cannot open file: " << filename << std::endl;
    return;
  }

  std::string word = "";
  std::vector<float> signal;

  // Skip first text
  while (word != "Time,Ampl") {
    file >> word;
  }

  int counter = 0;
  int push = 0;
  while(!file.eof()) 
  {
    // Data starts
    std::string yTemp;
    std::string xTemp;

    std::getline(file, xTemp, ',');
    std::getline(file, yTemp);

    counter++;

    signal.push_back( -1*atof(yTemp.c_str()) );
  }
 
  WaveformAlg waveformAlg; 
  waveformAlg.SmoothWaveform(signal, config);
  std::vector<float> waveform;
  counter = 0;
  for(const auto& amp : signal)
  {
    if (counter % config.resolution != 0) 
    {
      counter++;
      continue;
    }
    counter++;
    waveform.push_back(amp);
  }

  // Let's do the hit finding now
  HitCandidateVec hitCandVec;
  waveformAlg.FindHitCandidates(waveform, 0, channel, hitCandVec, config);

  // Only allow to store a few
  if (config.saveGraphs && waveforms.size() < 10)
  {
    TGraph g(waveform.size());
    g.SetNameTitle(filename.c_str(), filename.c_str());
    unsigned sample = 1;
    for (const auto& amp : waveform)
    {
      g.SetPoint(sample, sample - 1, amp);
      sample++;
    }
    waveforms.push_back(g);
  }

  std::cout << "Found: " << hitCandVec.size() << " hits. " << std::endl;
  // Append
  hitCandidateVec.insert(hitCandidateVec.end(), hitCandVec.begin(), hitCandVec.end());

  if (config.saveGraphs && waveforms.size() < 10) MakeTheMarkers(hitCandVec);

  //fits.push_back(FitTheHits(waveform, hitCandVec));

  /*for (const auto& hit : hitCandVec) 
  {
    std::cout << "\nHIT...\n";
    std::cout << "Start tick: " << hit.startTick  << std::endl
	      << "Stop  tick: " << hit.stopTick   << std::endl
	      << "Centertick: " << hit.hitCenter  << std::endl
	      << "Hit height: " << hit.hitHeight  << std::endl;
  } */
}

void FileReader::MakeTheMarkers(const HitCandidateVec& hitCandVec)
{
  std::vector<TMarker> mks;
  for (const auto& hit : hitCandVec)
  {
    TMarker m(hit.hitCenter, hit.hitHeight, 8);
    m.SetMarkerColor(2);
    m.SetMarkerSize(1.5);
    mks.push_back(m);
  } 
  markers.push_back(mks);
}
}
