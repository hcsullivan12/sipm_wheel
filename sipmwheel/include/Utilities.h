// 
// File: Utilities.h
//
// Author: Hunter Sullivan
//
// Detail: Structure to hold configuration settings and defining some useful
//         containers.
//

#ifndef UTILITIES_H
#define UTILITIES_H

#include <vector>
#include <map>
#include <list>

namespace wheel {

struct HitCandidate
{
  size_t channel;
  size_t startTick;
  size_t stopTick;
  size_t maxTick;
  size_t minTick;
  float  hitCenter;
  float  hitHeight;
};

using HitCandidateVec = std::vector<HitCandidate>;
using SiPMToFilesMap  = std::map<unsigned, std::list<std::string>>;

struct Configuration {

  std::string pathToData;
  std::string outputPath;
  std::string pathToConfig;
  bool        printFiles;
  bool        baselineSubtract;
  bool        saveGraphs;
  unsigned    nSiPMs;
  unsigned    smaRange;
  unsigned    resolution;
  float       hitSigma;
  float       minimumHitAmp;
};
}

#endif
