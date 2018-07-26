// 
// File: FileReader.h
//
// Author: Hunter Sullivan
//
// Discription: Structure to read the input waveform files.
//

#ifndef FILEREADER_H
#define FILEREADER_H

#include "Utilities.h"
#include "WaveformAlg.h"
#include <fstream>
#include <iostream>
#include "TGraph.h"
#include "TMarker.h"

namespace wheel {

class FileReader {

public:
  FileReader();
  ~FileReader();
  
  void ReadFiles(SiPMToHitCandVecMap& sipmToHitCandVecMap, const SiPMToFilesMap& map, const wheel::Configuration& config);

  std::vector<TGraph>&               GetGraphs()  { return waveforms; }
  std::vector<std::vector<TMarker>>& GetMarkers() { return markers; }
  
private:

  void ReadFile(HitCandidateVec& hitCandidateVec, const std::string& filename, const unsigned& channel, const wheel::Configuration& config);
  void MakeTheMarkers(const HitCandidateVec& hitCandVec);

  std::vector<TGraph>               waveforms;
  std::vector<std::vector<TMarker>> markers; 

};
}

#endif