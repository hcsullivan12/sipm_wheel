#ifndef DATAREADER_HH
#define DATAREADER_HH

#include <vector>
#include <algorithm>

class DataReader {
public:
  DataReader(double, std::vector<double>, std::vector<double>);
  void ReadData(std::string[4] );
  void Read(unsigned, double, double, double, std::string );
  void Fit();

  double GetAttenuationLength();
  std::vector<double> GetSiPMCounts();
  std::vector<double> GetSiPMCountErrors();

private:
  void Analyze();
  void Find(unsigned);
  void ComputeCounts(unsigned );
  std::pair<double, double> ComputeNoiseParameters(unsigned );

  std::vector< std::map<double, double> > signals_test;
  std::vector<std::vector<double> > signals;
  double sipmBias;
  std::vector<double> sipmGains;
  std::vector<double> sipmGainErrors;
  std::vector<double> sipmCounts;
  std::vector<double> sipmCountErrors;
  std::vector<std::pair<double, double> > hits;
  double widthDisc = 30;
  TFile *f = new TFile("/home/hunter/projects/wheel/histo.root", "RECREATE");

};
#endif

DataReader::DataReader(double bias, std::vector<double> gains, std::vector<double> errors) {
  sipmBias       = bias;
  sipmGains      = gains;
  sipmGainErrors = errors;
}

void DataReader::Read(unsigned n_chan, double x_start, double x_stop,
                      double x_inc, std::string sipm_data_dir) {

  /// Loop over positions
  double iter = x_start;
  while (iter <= x_stop) {
    for (unsigned chan = 1; chan <= n_chan; chan++) {
      /// Open this file
      std::string filename = sipm_data_dir + "F" + std::to_string(chan) + std::to_string(x_start) + "cm_ch"
                             + std::to_string(chan) + "00000.txt";
      std::ifstream file(filename.c_str());
      /// Open the file
      std::ifstream file(sipm_data_files[i].c_str());
      if (!file) {
        std::cout << "Cannot open " << filename << std::endl;
      } else {
        ///Read file and make histogram
        std::string word = "";

        ///Skip first text
        while (word != "Time,Ampl") {
          file >> word;
        }
        std::string valueTemp;
        std::string weightTemp;
        double minValue = 0;
        double maxValue = 0;
        double value;
        int weight;
        std::multimap<double, int> data;

        while(std::getline(file, valueTemp, ',')) {

          std::getline(file, weightTemp);

          value  = std::stod(valueTemp);
          weight = std::stoi(weightTemp);

          data.insert(std::make_pair(value, weight));

          if(value < minValue) {
            minValue = value;
          }

          if(value > maxValue) {
            maxValue = value;
          }

          //std::cout << value << "   " << weight << std::endl;
        }

        /// Make histo
        std::string name = std::to_string(chan) + std::string(x_start) + "cm_ch" + std::to_string(chan);
        TH1D *h = new TH1D(name.c_str(), name.c_str(), data.size(), minValue, maxValue);

        int bin = 1;
        for (auto it = data.begin(); it != data.end(); it++) {
          h->SetBinContent(bin, it->second);
          bin++;
        }
        h->Draw();
      }
    }

    iter = iter + x_inc;
  }
}

void DataReader::ReadData(std::string sipm_data_files[4]) {

  /// Clear signal holder
  signals.clear();

  /// Loop over the files
  for (unsigned i = 0; i < 4; i++) {
    /// Open the file
    std::ifstream file(sipm_data_files[i].c_str());
    if (!file) {
      std::cout << "Cannot open " << sipm_data_files[i] << std::endl;
    } else {
      ///Read file
      std::string word = "";
      std::vector<double> signal_temp;
      //std::cout << "Reading Filename: " << filename << "\n" << std::endl;
      ///Skip first text
      while (word != "Time,Ampl") {
        file >> word;
      }
      while(!file.eof()) {
        ///Data starts
        std::string vTemp;
        std::string tTemp;

        std::getline(file, tTemp, ',');
        std::getline(file, vTemp);

        double v = abs(atof(vTemp.c_str()));
        double t = atof(tTemp.c_str());

        //std::pair<double, double> p;
        //p = std::make_pair(t, v);
        signal_temp.push_back(v);
      }

      signals.push_back(signal_temp);
      file.close();
    }
  }

  /// Pre allocate
  sipmCounts.resize(signals.size());
  sipmCountErrors.resize(signals.size());

  TCanvas *master = new TCanvas("master", "All Waveforms", 1000, 1000);
  master->Divide(1,4);
  for (unsigned sipm = 0; sipm < signals.size(); sipm++) {
    master->cd(sipm + 1);
    /// Make a histogram for this sipm
    std::vector<double> this_sipm = signals.at(sipm);
    unsigned nBins = this_sipm.size();

    //std::cout << "nBins = " << nBins << std::endl;
    std::string name1 = "SiPM " + std::to_string(sipm);
    TH1D *h = new TH1D(name1.c_str(), name1.c_str(), nBins, 0, nBins );
    for (int sample = 0; sample < this_sipm.size(); sample++) {
      //	std::cout << this_sipm.at(sample) << std::endl;
      h->SetBinContent( sample + 1, this_sipm.at(sample) );
    }

    gStyle->SetOptStat(0);
    h->Draw(); //apl

    hits.clear();
    Find(sipm);

    std::string name2 = "SiPM " + std::to_string(sipm) + "hits";
    TH1D *dataHisto = new TH1D(name2.c_str(), name2.c_str(), nBins, 0, nBins);
    for (int hit = 0; hit < hits.size(); hit++) {
      dataHisto->SetBinContent( hits.at(hit).first + 1, hits.at(hit).second );
    }
    dataHisto->SetMarkerStyle(7);
    dataHisto->SetMarkerSize(1);
    dataHisto->SetMarkerColor(2);
    dataHisto->Draw("same P");

  }
  //Analyze();
};

void DataReader::Find(unsigned sipm)
{
  // Get the sipm parameters
  std::vector<double> signal = signals.at(sipm);
  double gain                = sipmGains.at(sipm);
  double gain_error          = sipmGainErrors.at(sipm);
  unsigned counts            = 0;

  // Compute the baseline parameters
  std::pair<double, double> noiseParams = ComputeNoiseParameters(sipm);

  // Define all the thresholds
  unsigned nMissed     = 0;
  double noiseBaseline = noiseParams.first;
  double thrPosLead    = noiseParams.first + 2 * noiseParams.second;
  double thrPosPeak    = noiseParams.first + 3 * noiseParams.second;
  double thrPosTrail   = noiseParams.first + 2 * noiseParams.second;

  std::cout << "noiseBaseline = " << noiseBaseline << "   thrPosLead = " << thrPosLead << "   thrPosPeak = " << thrPosPeak << std::endl;
  // Find the first potential hit
  auto min_max = std::minmax_element( signal.begin(), signal.end() );
  unsigned posPeakSample = static_cast<int>( min_max.second - signal.begin() );
  //std::cout << "MaxSample " << posPeakSample << std::endl;
  double posPeakValue = signal.at(posPeakSample);
  //std::cout << "posPeakValue = : " << posPeakValue << std::endl;
  // Check if peak is above threshold
  while (posPeakValue >= thrPosPeak) {

    std::cout << "In while\n";
    // Initialize parameters
    int firstSample           = 0;
    int zeroCrossSample       = 0;
    unsigned lastSample       = 0;
    bool foundFirstSample     = false;
    bool foundLastSample      = false;
    // Loop to find the first/last sample of the pulse using constant fraction discrimination.
    // Search until first and last sample are found
    int sampleOffset             = 1;
    double localMinRising        = posPeakValue;
    double localMinRisingSample  = posPeakSample;
    double localMinFalling       = posPeakValue;
    double localMinFallingSample = posPeakSample;

    while ( !(foundFirstSample && foundLastSample) ) {
      // First sample. Only look if we haven't found it yet.
      if (!foundFirstSample) {
        firstSample = posPeakSample - sampleOffset;
        // If we've gone outside sample range, we've missed the hit, exit
        if (firstSample < 0) {
          break;
        }
        if (signal.at(firstSample) < localMinRising) {
          localMinRising       = signal.at(firstSample);
          localMinRisingSample = firstSample;
        }
        // Check whether we've crossed the constant fraction threshold.
        if ((firstSample >= 0) && (signal.at(firstSample) < thrPosLead)) {
          foundFirstSample = true;
        }
        /*// Or if we're looking at another pulse
        if (firstSample > 2) {
        if (   signal.at(firstSample - 1) > signal.at(firstSample)
        && signal.at(firstSample - 2) > signal.at(firstSample)
        && signal.at(firstSample - 3) > signal.at(firstSample) ) {
                foundFirstSample = true;
        }
        }*/
      }
      // Last sample. Only look if we haven't found it yet.
      if (!foundLastSample) {
        lastSample = posPeakSample + sampleOffset;
        // If we've gone outside sample range, we've missed the hit, exit
        if (lastSample >= signal.size()) {
          break;
        }
        if (signal.at(lastSample) < localMinFalling) {
          localMinFalling       = signal.at(lastSample);
          localMinFallingSample = lastSample;
        }
        // Check whether we've crossed the constant fraction threshold.
        if (signal.at(lastSample) < thrPosTrail) {
          foundLastSample = true;
        }
        /*// Or if we're looking at another pulse
        if (lastSample < (signal.size() - 3) ) {
        if (   signal.at(lastSample + 1) > signal.at(firstSample)
        && signal.at(lastSample + 2) > signal.at(firstSample)
        && signal.at(lastSample + 3) > signal.at(firstSample) ) {
                foundFirstSample = true;
        }
        }*/
      }
      sampleOffset++;
    }
    //double width = abs(lastSample - firstSample);

    // If we detected both the rising and the falling edge, this is a hit.
    if (foundFirstSample && foundLastSample) {
      unsigned nPE = static_cast<unsigned>( posPeakValue/(gain*sipmBias) );
      counts = counts + nPE;
      hits.push_back( std::make_pair(posPeakSample, posPeakValue) );

      if (firstSample < 0) {
        firstSample = 0;
      }
      if (lastSample >= signal.size()) {
        lastSample = signal.size() - 1;
      }
      // Replace this hit with baseline
      for (unsigned sample = localMinRising; sample <= localMinFalling; ++sample) {
        signal.at(sample) = noiseBaseline;
      }
      // Find the next potential hit
      auto min_max = std::minmax_element( signal.begin(), signal.end() ); //returns pair iterator to min/max
      posPeakSample = static_cast<int>( min_max.second - signal.begin() );
      posPeakValue = signal.at(posPeakSample);
      std::cout << "posPeakValue = " << posPeakValue << std::endl;

    } else {
      ++nMissed;
      if (firstSample < 0) {
        firstSample = 0;
      }
      if (lastSample >= signal.size()) {
        lastSample = signal.size() - 1;
      }
      // Replace this hit with baseline
      for (unsigned sample = firstSample; sample <= lastSample; ++sample) {
        signal.at(sample) = noiseBaseline;
      }
      // Find the next potential hit
      auto min_max = std::minmax_element( signal.begin(), signal.end() ); //returns pair iterator to min/max
      posPeakSample = static_cast<int>( min_max.second - signal.begin() );
      posPeakValue = signal.at(posPeakSample);
      std::cout << "posPeakValue = " << posPeakValue << std::endl;
    }
    /*if (firstSample < 0) {
            firstSample = 0;
    }
    if (lastSample >= signal.size()) {
        lastSample = signal.size() - 1;
    }
    // Replace this hit with baseline
    for (unsigned sample = localMinRising; sample <= localMinFalling; ++sample) {
    signal.at() = noiseBaseline;
    }
    // Find the next potential hit
    auto min_max = std::minmax_element( signal.begin(), signal.end() ); //returns pair iterator to min/max
    posPeakSample = static_cast<int>( min_max.second - signal.begin() );
    posPeakValue = signal.at(posPeakSample);*/
    //std::cout << "posPeakValue = " << posPeakValue << std::endl;
  }
  // Store counts for this sipm
  sipmCounts.at(sipm) = counts;
}

std::pair<double, double> DataReader::ComputeNoiseParameters(unsigned sipm) {

  std::vector<double> signal = signals.at(sipm);

  // Find min and max of this signal for number of bins
  auto min_max = std::minmax_element( signal.begin(), signal.end() );
  //int minSample = static_cast<int>( min_max.first - signal.begin() );
  int maxSample = static_cast<int>( min_max.second - signal.begin() );
  std::cout << "MaxSample = " << maxSample << "   vec size = " << signal.size() << std::endl;

  //int min = signal.at(minSample);
  auto max = signal.at(maxSample);

  // unsigned nBins = static_cast<unsigned>( 10000*(max - min) );

  // Make a histogram to compute noise parameters
  std::string name = "ampDist" + std::to_string(sipm);
  TH1S amplitudeDist(name.c_str(), name.c_str(), 10000, -max, max);
  for (unsigned sample = 0; sample < signal.size(); ++sample) {
    amplitudeDist.Fill( signal.at(sample) );
  }

  //TFile *f = new TFile("/home/hunter/projects/wheel/histo.root", "RECREATE");
  amplitudeDist.Write();


  // Fit the amplitude distribution
  TF1 gauss("gauss", "gaus");
  amplitudeDist.GetXaxis()->SetRangeUser(amplitudeDist.GetMean() - 1 * amplitudeDist.GetStdDev(), amplitudeDist.GetMean() + 1 * amplitudeDist.GetStdDev());
  amplitudeDist.Fit(&gauss, "QN");
  return std::pair<double, double>(gauss.GetParameter(1), gauss.GetParameter(2));
}

std::vector<double> DataReader::GetSiPMCounts() {
  return sipmCounts;
}
