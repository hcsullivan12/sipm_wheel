const std::string pathToData = "/home/hunter/projects/wheel/data/07_22_18";
const bool printFiles        = false;
const bool baselineSubtract  = true;
const bool drawGraphs        = true;
const unsigned nSiPMs        = 1;
const float hitSigma         = 3;     // hit threshold: sigma above noise
const float minimumHitAmp    = 0.01;  // absolute minimum hit threshold
const unsigned smaRange      = 100;     // number of points before and after centroid for sma
const unsigned resolution    = 50;

// SiPM, files
std::map<unsigned, std::list<std::string> > files;
std::vector<TGraph*> graphs;
std::vector<TF1*>    fits;
std::vector<std::vector<TMarker*>> markers;

#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

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
HitCandidateVec hitCandidateVec;

void ReadFiles();
void DrawGraphs();
void MakeTheMarkers(const HitCandidateVec& hitCandVec);
TF1* FitTheHits(const std::vector<float>& signal, const HitCandidateVec& hitCandVec);
void MakePlots();
void GetTheFiles();
void PrintTheFiles();
void ReadCurrentFile(const std::string& file, const unsigned& channel);
void SmoothWaveform(std::vector<float>& signal);
void FindHitCandidates(std::vector<float>& waveform,
                       size_t              roiStartTick,
                       size_t              channel,
		       HitCandidateVec&    hitCandVec);

void FindHitCandidates(std::vector<float>::const_iterator startItr,
                       std::vector<float>::const_iterator stopItr,
                       const std::pair<float, float>&     noiseParameters,
                       size_t                             roiStartTick,
		       HitCandidateVec&                   hitCandVec);
std::pair<float, float> ComputeNoise(std::vector<float>& signal);

void WaveformAnalysis()
{
  // First clear memory
  hitCandidateVec.clear();

  // First obtain all the files we want to read
  // This assumes they are stored accordingly
  std::cout << "\nGetting the files from: " << pathToData << std::endl;
  GetTheFiles();
  if (printFiles) PrintTheFiles();

  // Now read all the files
  std::cout << "Reading the files..." << std::endl;
  ReadFiles();

  if (drawGraphs)
  {
    std::cout << "Drawing graphs... " << std::endl;
    DrawGraphs();
  }

  // Make the amplitude distribution?
  std::cout << "Making amplitude distribution..." << std::endl;
  MakePlots();
}

void MakePlots()
{
  TCanvas *c = new TCanvas("AmpDist", "AmpDist", 500, 500);
  TH1S *ampDist = new TH1S("Amplitude Distribution", "Amplitude Distribution", 100, 0, 0.1);
	
  // Loop over the hits
  for (const auto& hit : hitCandidateVec)
  {
    ampDist->Fill( hit.hitHeight ); 
  }

  ampDist->GetXaxis()->SetTitle("Amplitude/V");
  ampDist->Draw();
}

void DrawGraphs()
{
  unsigned counter = 0;
  for (const auto& g : graphs)
  {
    // Set a safety net here
    if (counter > 1) return;
   
    TCanvas *c = new TCanvas(std::to_string(counter).c_str(), std::to_string(counter).c_str(), 500, 500);
    //g->GetXaxis()->SetRangeUser(1900, 2100);
    //g->Fit(fits[counter], "R+", "", 1988, 2100);
    g->Draw("");
    for (const auto& m : markers[counter])
    {
      m->Draw("same");
    }
    counter++;
  }
}

void PrintTheFiles()
{
  std::cout << "\nReading the following files: " << std::endl;
  for (const auto& sipmData : files)
  {
    std::cout << sipmData.second.size() << " files for SiPM " << sipmData.first << ":" << std::endl;
    for (const auto& file : sipmData.second)
    {
      std::cout << file << std::endl;
    }
    std::cout << std::endl;
  }
}

void GetTheFiles()
{
  DIR *dir;
  struct dirent *file;
  struct stat filestat;

  for (unsigned sipm = 1 ; sipm <= nSiPMs; sipm++)
  {
    std::string thisDir = pathToData + "/sipm" + std::to_string(sipm);
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
      return EXIT_FAILURE;
    }
    files.emplace(sipm, sipmData);
  }
}

void ReadFiles()
{
  // Loop over the sipms
  unsigned channel = 1;
  for (const auto& sipm : files)
  {
    // Loop over the files for this sipm
    for (const auto& file : sipm.second)
    {
      // Now read this file
      ReadCurrentFile(file, channel);
    }
    channel++;
  }
}

void ReadCurrentFile(const std::string& filename, const unsigned& channel)
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
  
  SmoothWaveform(signal);
  std::vector<float> waveform;
  counter = 0;
  for(const auto& amp : signal)
  {
    if (counter % resolution != 0) 
    {
      counter++;
      continue;
    }
    counter++;
    waveform.push_back(amp);
  }

  // Let's do the hit finding now
  HitCandidateVec hitCandVec;
  FindHitCandidates(waveform, 0, channel, hitCandVec);

  TGraph *g = new TGraph(waveform.size());
  g->SetNameTitle(filename.c_str(), filename.c_str());
  unsigned sample = 1;
  for (const auto& amp : waveform)
  {
    g->SetPoint(sample, sample - 1, amp);
    sample++;
  }
  if (drawGraphs) graphs.push_back(g);

  std::cout << "Found: " << hitCandVec.size() << " hits. " << std::endl;
  // Append
  hitCandidateVec.insert(hitCandidateVec.end(), hitCandVec.begin(), hitCandVec.end());

  MakeTheMarkers(hitCandVec);

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

void MakeTheMarkers(const HitCandidateVec& hitCandVec)
{
  std::vector<TMarker*> mks;
  for (const auto& hit : hitCandVec)
  {
    TMarker *m = new TMarker(hit.hitCenter, hit.hitHeight, 8);
    m->SetMarkerColor(2);
    m->SetMarkerSize(1.5);
    mks.push_back(m);
  } 
  markers.push_back(mks);
}

TF1* FitTheHits(const std::vector<float>& signal, const HitCandidateVec& hitCandVec)
{
  // How many hits did we find?
  const unsigned nHits = hitCandVec.size();

  // Prepare fit
  std::string equation = "";
  float min            = std::numeric_limits<float>::max();
  float max            = 0;
  int parameter        = 0;
  for (const auto& hit : hitCandVec)
  {
    if (hit.startTick < min) min = hit.startTick;
    if (hit.stopTick  > max) max = hit.stopTick;

    if ( parameter == (nHits*4 - 4) ) 
    { 
      /*equation += "(x<" + std::to_string(hit.hitCenter) + ")*[" + std::to_string(parameter) + "]*(1-exp(-x/[" + std::to_string(parameter + 1) + "]))" + 
	          " + (x>" + std::to_string(hit.hitCenter) + ")*[" + std::to_string(parameter + 2)  + "]*exp(-x/[" + std::to_string(parameter + 3) + "])";*/
       equation += "[" + std::to_string(parameter) + "]*(1-[" + std::to_string(parameter + 1) + "]*exp(-1*(x-" + std::to_string(hit.hitCenter) + ")/[" + std::to_string(parameter + 2) + "]))" + 
	           "*exp(-1*(x-" + std::to_string(hit.hitCenter) + ")/[" + std::to_string(parameter + 3) + "])";
    }
    else 
    {
      /*"(x<" + std::to_string(hit.hitCenter) + ")*[" + std::to_string(parameter) + "]*(1-[" + std::to_string(parameter + 1) + "]*exp(-x/[" + std::to_string(parameter + 2) + "]))" + 
      " + (x>" + std::to_string(hit.hitCenter) + ")*exp(-x/[" + std::to_string(parameter + 3) + "]) + ";*/
      equation += "[" + std::to_string(parameter) + "]*(1-[" + std::to_string(parameter + 1) + "]*exp(-1*(x-" + std::to_string(hit.hitCenter) + ")/[" + std::to_string(parameter + 2) + "]))" + 
                  "*exp(-1*(x-" + std::to_string(hit.hitCenter) + ")/[" + std::to_string(parameter + 3) + "]) + ";

   }
    parameter = parameter + 4;
  }

  std::cout << min << "  " << max << std::endl;
  std::cout << equation << std::endl;

  // Initialize
  TF1 *fit = new TF1("Fit", equation.c_str(), min, max);
  parameter = 0;
  for (const auto& hit : hitCandVec)
  {
    const float amp      = hit.hitHeight;
    const float riseTime = hit.hitCenter - hit.startTick;
    const float fallTime = hit.stopTick  - hit.hitCenter;

    std::cout << amp << " " << riseTime << " " << fallTime << std::endl;

    fit->SetParameter(parameter,     amp);
    fit->SetParameter(parameter + 1, amp);
    fit->SetParameter(parameter + 2, 0.7*riseTime);
    fit->SetParameter(parameter + 3, 0.7*fallTime);
    fit->SetParLimits(parameter,     0.1*amp,      2.0*amp);
    fit->SetParLimits(parameter + 1, 0.1*amp,      2.0*amp);
    fit->SetParLimits(parameter + 2, 0.2*riseTime, 0.7*riseTime);
    fit->SetParLimits(parameter + 3, 0.2*fallTime, 0.7*fallTime);
    parameter = parameter + 4;
  }

  fit->Draw();

  return fit;
}

void SmoothWaveform(std::vector<float>& signal)
{
  // Going to perform a moving average here
  unsigned sample(1);
  for (auto& amp : signal)
  {
    // Ignore first few points
    if (sample <= smaRange) 
    {
      sample++;
      continue;
    }
    // Ignore last points
    if (sample >= signal.size() - smaRange) return;

    float leadSum(0), trailSum(0);
    for (unsigned offset = 1; offset <= smaRange; offset++)
    {
      leadSum  += signal[sample - offset];
      trailSum += signal[sample + offset];
    }
    float rangeAvg = (leadSum + trailSum + amp)/(2*smaRange + 1);

    // Reassign
    amp = rangeAvg;
    sample++;
  }
}

void FindHitCandidates(std::vector<float>& waveform,
                       size_t              roiStartTick,
                       size_t              channel,
		       HitCandidateVec&    hitCandVec)
{
  // Find noise parameters first
  const auto noiseParameters = ComputeNoise(waveform);

  // Use the recursive version to find the candidate hits
  FindHitCandidates(waveform.begin(), waveform.end(), noiseParameters, roiStartTick, hitCandVec);
  
  return;
}

void FindHitCandidates(std::vector<float>::const_iterator startItr,
                       std::vector<float>::const_iterator stopItr,
                       const std::pair<float, float>&     noiseParameters,
                       size_t                             roiStartTick,
		       HitCandidateVec&                   hitCandVec)
{
  // Need a minimum number of ticks to do any work here
  if (std::distance(startItr,stopItr) > 4)
  {
    // Find the highest peak in the range given
    auto maxItr = std::max_element(startItr, stopItr);

    float maxValue = *maxItr;
    int   maxTime  = std::distance(startItr,maxItr);

    float hitThreshold;
    if (baselineSubtract) hitThreshold = minimumHitAmp;
    else hitThreshold = std::max( noiseParameters.first + hitSigma*noiseParameters.second, minimumHitAmp ); 

    //std::cout << noiseParameters.first + hitSigma*noiseParameters.second << "  " << minimumHitAmp << "  " << maxValue << std::endl;

    if (maxValue > hitThreshold)
    {
      // backwards to find first bin for this candidate hit
      auto firstItr = std::distance(startItr,maxItr) > 2 ? maxItr - 1 : startItr;

      while(firstItr != startItr)
      {
        // Check for pathology where waveform goes too negative
        if (*firstItr < -hitThreshold) break;

        // Check both sides of firstItr and look for min/inflection point
        if (*firstItr < *(firstItr+1) && *firstItr <= *(firstItr-1)) break;

        firstItr--;
      }

      int firstTime = std::distance(startItr,firstItr);

      // Recursive call to find all candidate hits earlier than this peak
      FindHitCandidates(startItr, firstItr + 1, noiseParameters, roiStartTick, hitCandVec);

      // forwards to find last bin for this candidate hit
      auto lastItr = std::distance(maxItr,stopItr) > 2 ? maxItr + 1 : stopItr - 1;

      while(lastItr != stopItr - 1)
      {
        // Check for pathology where waveform goes too negative
        if (*lastItr < -hitThreshold) break;

        // Check both sides of firstItr and look for min/inflection point
        if (*lastItr <= *(lastItr+1) && *lastItr < *(lastItr-1)) break;

        lastItr++;
      }

      int lastTime = std::distance(startItr,lastItr);

      // Now save this candidate's start and max time info
      HitCandidate hitCandidate;
      hitCandidate.startTick     = roiStartTick + firstTime;
      hitCandidate.stopTick      = roiStartTick + lastTime;
      hitCandidate.maxTick       = roiStartTick + firstTime;
      hitCandidate.minTick       = roiStartTick + lastTime;
      hitCandidate.hitCenter     = roiStartTick + maxTime;
      hitCandidate.hitHeight     = maxValue;

      hitCandVec.push_back(hitCandidate);

      // Recursive call to find all candidate hits later than this peak
      FindHitCandidates(lastItr + 1, stopItr, noiseParameters, roiStartTick + std::distance(startItr,lastItr + 1), hitCandVec);
    }
  }

  return;
}

std::pair<float, float> ComputeNoise(std::vector<float>& signal)
{
  // Find min and max of this signal for number of bins
  auto min_max = std::minmax_element( signal.begin(), signal.end() );
  int minSample = static_cast<int>( min_max.first - signal.begin() );
  int maxSample = static_cast<int>( min_max.second - signal.begin() );

  float min = signal.at(minSample);
  float max = signal.at(maxSample);

  unsigned nBins = static_cast<unsigned>(std::abs(max - min)*10000);

  // Make a histogram to compute mean noise
  TH1S amplitudeDist("amplitudeDist", "amplitudeDist", nBins, min, max);
  for (unsigned sample = 0; sample < signal.size(); ++sample) {
    amplitudeDist.Fill( signal.at(sample) );
  }

  // Fit the amplitude distribution
  TF1 gauss("gauss", "gaus");
  amplitudeDist.GetXaxis()->SetRangeUser(amplitudeDist.GetMean() - 3 * amplitudeDist.GetStdDev(), amplitudeDist.GetMean() + 3 * amplitudeDist.GetStdDev());
  amplitudeDist.Fit(&gauss, "QN");

  if (baselineSubtract)
  {
    for (auto& amp : signal) amp  = amp - gauss.GetParameter(1);
  }

  return std::pair<double, double>(gauss.GetParameter(1), gauss.GetParameter(2));
}
