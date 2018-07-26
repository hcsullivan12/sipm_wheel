// 
// File: Characterizer.h
//
// Author: Hunter Sullivan
//
// Discription: Structure to characterize sipms.
//

#ifndef CHARACTERIZER_H
#define CHARACTERIZER_H

#include "Utilities.h"
#include <iostream>

namespace wheel {

class Characterizer {

public:
  Characterizer();
  ~Characterizer();
  
  void CharacterizeSiPMs(const SiPMToHitCandVec& sipmToHitCandVec, const Configuration& config);
  
private:


};
}

#endif
