//
// File: Analyzer.cxx
//
// Author: Hunter Sullivan
//
// Description: Structure to run reconstruction on sipm data.
//

#include "Analyzer.h"
#include "TMath.h"

namespace wheel {

Analyzer::Analyzer()
{}

Analyzer::~Analyzer()
{}

void Analyzer::Handle(const unsigned& N0)
{
  //long N0 = (long) ptr;

  // Variables used for incrementing
  // Start with r = 0, theta = 0, (N0 is what's passed) and increment by the constants defined above
  float radius = 0;
  float theta  = 0;
  float attenuationLength = 5.0;
  unsigned counter = 1;

  //TThread::Lock();
  std::cout << "N0: " << N0 << std::endl;
  while (attenuationLength <= 40)
  {
    while ( radius <= p_diskRadius ) 
    {
      //std::cout << "radius: " << radius << std::endl;
      while ( theta < 360 ) 
      {
        //std::cout << "N0: " << N0 << "   radius: " << radius << "   theta: " << theta << std::endl;
        float likelihood = ComputeLikelihood( radius, theta, N0, attenuationLength );

        //TThread::Lock();
        std::vector<float> temp = {radius, theta, likelihood};
        p_accumulatorMap.emplace_back(std::make_pair(N0, temp));
        counter++;
        //TThread::UnLock();
        theta = theta + p_thetaBinSize;
      }
      theta = 0;
      radius = radius + p_radiusBinSize;
    }
    attenuationLength = attenuationLength + p_attenuationLengthBinSize;
  }
}

void Analyzer::RunReco(SiPMToTriggerMap& sipmToTriggerMap, const SiPMInfoMap& sipmInfoMap, const Configuration& config)
{
  // Initialize
  p_thetaBinSize  = config.thetaBinSize;
  p_radiusBinSize = config.radiusBinSize;
  p_diskRadius    = config.diskRadius;
  p_attenuationLengthBinSize = config.attenuationLengthBinSize;
  p_beta          = 360/config.nSiPMs;
 
  // Loop over different triggers
  for (unsigned trigger = 1; trigger <= sipmToTriggerMap.find(1)->second.size(); trigger++)
  {
    WheelReco(sipmToTriggerMap, sipmInfoMap, trigger);
  }

}

void Analyzer::WheelReco(SiPMToTriggerMap& sipmToTriggerMap, const SiPMInfoMap& sipmInfoMap, const unsigned& trigger)
{
   clock_t start = clock();

  //sipm_errors = dr.GetSiPMCountErrors(); 

  // First to cound number of photons in each hit
  // also N0 set to lower threshold to decrease computation time
  // and clear temp data vec for this trigger
  p_data.clear();
  int N0 = CountPhotons(sipmToTriggerMap, sipmInfoMap, trigger);

  while ( N0 <= 1000 ) 
  {
    /*// Create threads
    TThread t0("t0", handle, (void*) N0);
    N0++;
    TThread t1("t1", handle, (void*) N0);
    N0++;
    TThread t2("t2", handle, (void*) N0);
    N0++;
    TThread t3("t3", handle, (void*) N0);
    N0++;
    TThread t4("t4", handle, (void*) N0);
    N0++;
    TThread t5("t5", handle, (void*) N0);
    N0++;
    TThread t6("t6", handle, (void*) N0);
    N0++;

    t0.Run();
    t1.Run();
    t2.Run();
    t3.Run();
    t4.Run();
    t5.Run();
    t6.Run();

    t0.Join();
    t1.Join();
    t2.Join();
    t3.Join();
    t4.Join();
    t5.Join();
    t6.Join();*/
    Handle(N0);
    N0++;
  }

  // Now find the maximum
  float max(0);
  unsigned ID(0);
  for (unsigned index = 0; index < p_accumulatorMap.size(); index++)
  {
    if (p_accumulatorMap[index].second[3] > max) { max = p_accumulatorMap[index].second[3]; ID = index; } 
  }
 
  std::cout << "Maximum likelihood: " << p_accumulatorMap[ID].second[3] << std::endl
            << "Radius = "            << p_accumulatorMap[ID].second[1] << " cm\n"
            << "Theta  = "            << p_accumulatorMap[ID].second[2] << " deg\n"
            << "N0: "                 << p_accumulatorMap[ID].first     << "\n";
 
  clock_t end = clock();
  double duration = (end - start) / (double) CLOCKS_PER_SEC;
  std::cout << "Run time of " << duration << std::endl;
}

unsigned Analyzer::CountPhotons(SiPMToTriggerMap& sipmToTriggerMap, const SiPMInfoMap& sipmInfoMap, const unsigned& trigger)
{
  // Get an average count to get a starting place for reco
  unsigned avg(0);
  unsigned hitCounter(0);
  
  // Loop over all the hits and set the n of photons
  for (auto& sipm : sipmToTriggerMap)
  {
    // Get the gain and bdown for this sipm
    const float& thisGain = sipmInfoMap.find(sipm.first)->second.gain;
    const float& thisBD   = sipmInfoMap.find(sipm.first)->second.breakdown;
    unsigned max(0);
    for (auto& hit : sipm.second[trigger])
    {
      hit.nPhotons = std::round(hit.hitHeight/( thisGain*( hit.bias - thisBD ) ));
      if (hit.nPhotons > max) max = hit.nPhotons;
      avg += hit.nPhotons;
      hitCounter++;
    }
    // Store max in the data vec 
    p_data.emplace(sipm.first, max);
  }

  return std::round(avg/hitCounter);
}

float Analyzer::ComputeLikelihood(const float& r, const float& theta, const unsigned& N0, const float& attenuationLength)
{
  // Sum over terms ----> k_m*ln(lambda_m) - lambda_m
  float sum = 0;
  for (int m = 1; m <= p_nSiPMs; m++) {
    float lambda_m = ComputeLambda( r, theta, N0, m, attenuationLength );
    float term = p_data.find(m)->second*log( lambda_m ) - lambda_m;
    sum = sum + term;
  }
  return sum;
}

float Analyzer::ComputeLambda(const float& r, const float& theta, const unsigned& N0, const unsigned& m, const float& attenuationLength)
{
  ///lambda_m = N0e^( -r_m/attenuationLength )
  // Convert angle to radians 
  const double angleRad   = ( (m - 1)*p_beta - theta )*( TMath::Pi()/180 );
  const double r_mSquared = r*r + p_diskRadius*p_diskRadius - 2*r*p_diskRadius*TMath::Cos( angleRad );
  if (r_mSquared < 0) {
    std::cout << "Error! Square root of negative number!\n";
    return 0;
  }
  const double r_m = TMath::Sqrt( r_mSquared );
  const double lambda_m = N0*TMath::Exp( -r_m/attenuationLength );//(r_m);

  return lambda_m;
}
}
