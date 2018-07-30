#include "DataReader.h"
#include <vector>

DataReader::DataReader(double sipmBias, std::vector<double> sipmGains, std::vector<double> sipmGainErrors) {
  sipmBias       = bias;
  sipmGains      = gains;
  sipmGainErrors = errors;
}

void DataReader::ReadData(std::string sipm_data_files) {

  /// Clear signal holder
  signals.clear();

  /// Loop over the files
  for (unsigned i = 0; i < sipm_data_files.size(); i++) {
    /// Open the file
    std::ifstream file(sipm_data_files[i].c_str());
    if (!gain_file) {
      std::cout << "Cannot open " << config_file << std::endl;
    } else {
      ///Read file
      std::string word = "";
      std::vector<std::pair<double, double> > signal_temp;
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

  //Find();
  //Analyze();
};

void DataReader::Find() {
  /// Loop over sipms and find peaks
  for (unsigned sipm = 0; sipm < signals.size(); sipm++) {
    /// Make a histogram for this sipm
    std::vector<double> this_sipm = signals.at(sipm);
    double this_gain              = sipmGains.at(sipm);
    double this_gain_error        = sipmGainErrors.at(sipm);
    unsigned nBins = this_sipm.size();
    TH1D h("h", "h", nBins, this_sipm.at(0), this_sipm.at(nBins - 1) );

    for (int sample = 0; sample < this_sipm.size(); sample++) {
      h.SetBinContent( sample, this_sipm.at(sample) );
    }

    /// Find peaks
    TSpectrum *s = new TSpectrum();
    int nfound = s->Search(h, ampSig, "", threshold);
    int npeaks = s->GetNPeaks();

    Double_t *peak_samples;
    Double_t *peaks;
    peak_samples = s->GetPositionX();
    peaks = s->GetPositionY();

    unsigned counts = 0;
    for (int peak = 0; peak < nPeaks; peak++) {
      unsigned nPE = peaks[peak]/(this_gain*sipmBias);
      counts = counts + nPE;
    }
    sipmcounts.at(sipm) = counts;
  }
}

void DataReader::Analyze() {
  /// Loop over the sipms
  for (unsigned sipm = 0; sipm < data.size(); sipm++) {
    /// Get the counts
    unsigned counts = ComputeCounts(sipm);

    sipmCounts.push_back(counts);
  }
}

unsigned DataReader::ComputeCounts(unsigned sipm) {
  /// Get the information for this sipm
  double gain             = sipmGains.at(sipm);
  double gain_error       = sipmGainErrors(sipm);
  auto this_sipm_signal   = signals.at(sipm);
  unsigned counts = 0;

  std::pair<double, double> noiseParam = ComputeNoiseParameters();
  /// Define thresholds
  double ampThresh = abs(2.0*noiseParam.first);

  /// Loop over the signal
  unsigned sample = 0;
  while(sample < this_sipm_signal.size()) {
    double voltage = this_sipm_signal.at(sample);
    if (voltage > ampThresh) {
      /// Found hit, need to find amplitude
      sample++;
      double next_voltage = this_sipm_signal.at(sample);
      while (next_voltage > voltage) {
        next_voltage =
      }
      /// Found peak
      counts = counts + voltage/(gain*sipmBias);

    }
    sample++;
  }

  /// Found event
  while
  return counts;
}

std::pair<double, double> DataReader::ComputeNoiseParameters(unsigned sipm) {
  /// Get the signal for this sipm
  auto signal = signals.at(sipm);

  // Find min and max of this signal for number of bins
  auto min_max = std::minmax_element( signal.begin(), signal.end() );
  int minSample = static_cast<int>( min_max.first - signal.begin() );
  int maxSample = static_cast<int>( min_max.second - signal.begin() );

  int min = signal.at(minSample);
  int max = signal.at(maxSample);

  unsigned nBins = static_cast<unsigned>(max - min);

  // Make a histogram to compute noise parameters
  TH1S amplitudeDist("amplitudeDist", "amplitudeDist", nBins, min, max);
  for (unsigned sample = 0; sample < signal.size(); ++sample) {
    amplitudeDist.Fill( signal.at(sample) );
  }
  // Fit the amplitude distribution
  TF1 gauss("gauss", "gaus");
  amplitudeDist.GetXaxis()->SetRangeUser(amplitudeDist.GetMean() - 1 * amplitudeDist.GetStdDev(), amplitudeDist.GetMean() + 1 * amplitudeDist.GetStdDev());
  amplitudeDist.Fit(&gauss, "QN");
  return std::pair<double, double>(gauss.GetParameter(1), gauss.GetParameter(2));
}

double DataReader::GetSiPMCounts() {
  return sipmCounts;
}

double DataReader::GetSiPMCountErrors() {
  return sipmCountErrors;
}
