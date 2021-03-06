///This will use the MLE for reconstruction of SiPM Wheel 
///by cycling through different radius, theta values

const double diskRadius        = 5.7; ///cm
const double incrementRadius   = 0.2; ///cm
const double incrementTheta    = 1; ///degrees
const double attenuationLength = 2.7;
const double upperBoundN       = 2500;
const int numberOfPositions    = 8;
const double beta              = 360/numberOfPositions;  ///Angle between each position, in degrees
//double data[numberOfPositions];
double data[numberOfPositions] = {1.16456, 0.4465, 0.231687, 0.147494, 0.122291, 0.125334, 0.172409, 0.483366};
//double data[numberOfPositions] = {0.172409, 0.483366, 1.16456, 0.4465, 0.231687, 0.147494, 0.122291, 0.125334};
const double gain = 0.005; /// V/p.e.

double ComputeLikelihood(double r, double theta, int N0);
double ComputeLambda(double r, double theta, int N0, int m);
void MakePlots(std::multimap<std::string, double> parameters);
Double_t disk(Double_t* x, Double_t* par) {
        Double_t f = x[0]*x[0] + x[1]*x[1];
        return f;
}

void MLEReco(/*std::string outputFilename*/) {
	
	clock_t start = clock();

	/*///Initialize data array
	for (int i = 0; i < numberOfPositions; i++) {
		data[i] = 0;
	}
	
	std::string SiPMDataPath = "/home/hunter/Desktop/wheel/ZackRun/SiPM_Wheel_Zatt";
		
	//std::multimap<unsigned, double> SiPMMap;
	
	for (int position = 0; position < numberOfPositions; position++) {
		
		///Define filename
		std::string filename = SiPMDataPath + "/Channel_" + "1" + "/M" + "1" + " Pos" + to_string(position) + "_00000.txt";
		
		///Open file
		std::ifstream file(filename.c_str());
		if (!file) {
			std::cout << "Cannot open " << filename << std::endl;
		} else {
			///Read file
			std::string word = "";
			std::vector<double > time;
			std::vector<double > voltage;
			double max = 0;
			
			std::cout << "Reading Filename: " << filename << "\n" << std::endl;
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
				
				if (v > max) {
					max = v;
				}
			}
			
			///Store max in map
			//SiPMMap.insert(std::pair<unsigned, double>(position, max) );
			data[position] = max;
		}
		
		file.close();
	}*/
	
	///First need to read in data and store
	
	
	///Maybe will need the gains
	
	
	///Find max in data and store photons
	double Nmax = 0;
	for (int i = 0; i < numberOfPositions; i++) {
		data[i] = static_cast<unsigned>(data[i]/gain);
		if (data[i] > Nmax) Nmax = data[i];
	}
	
	///Variables used for incrementing
	///Start with r = 0, theta = 0, N0 = Nmax and increment by the constants defined above
	double radius = 0;
	double theta  = 0;
	int N0 = Nmax;
	
	double maxLikelihoodValue = 0;
	std::multimap<std::string, double> maxLiklihoodParam;
	
	///True values!
	double ml;
	double mleRadius;
	double mleTheta;
	double mleN0;
	
	while ( N0 <= upperBoundN ) {
		std::cout << "N0: " << N0 << std::endl;
		while ( radius <= diskRadius ) {
			//std::cout << "radius: " << radius << std::endl;
			while ( theta < 360 ) {
				//std::cout << "N0: " << N0 << "   radius: " << radius << "   theta: " << theta << std::endl;
				double likelihood = ComputeLikelihood( radius, theta, N0 );
				if ( likelihood > maxLikelihoodValue ) {
					maxLikelihoodValue = likelihood;
					
					///Store temporary maximum
					ml        = maxLikelihoodValue;
					mleRadius = radius;
					mleTheta  = theta;
					mleN0     = N0;
				}
				theta = theta + incrementTheta;
			}
			theta = 0;
			radius = radius + incrementRadius;
		}
		radius = 0;
		theta = 0;
		N0++;
	}
	
	maxLiklihoodParam.insert(std::pair<std::string, double>("likelihood", ml) );
	maxLiklihoodParam.insert(std::pair<std::string, double>("radius", mleRadius) );
	maxLiklihoodParam.insert(std::pair<std::string, double>("theta", mleTheta) );
	maxLiklihoodParam.insert(std::pair<std::string, double>("N0", mleN0) );
	
	std::cout << "Maximum likelihood: " << maxLiklihoodParam.find("likelihood")->second << std::endl;
	std::cout << "Radius = " << maxLiklihoodParam.find("radius")->second << "cm    Theta = " << maxLiklihoodParam.find("theta")->second << " deg     N0: " << maxLiklihoodParam.find("N0")->second << "\n\n";
	
	MakePlots(maxLiklihoodParam);

	clock_t end = clock();

	double duration = (end - start) / (double) CLOCKS_PER_SEC;
	std::cout << "Run time of " << duration << std::endl;
}

double ComputeLikelihood(double r, double theta, int N0 ) {
	
	///Sum over terms ----> k_m*ln(lambda_m) - lambda_m
	double sum = 0;
	
	for (int m = 1; m <= numberOfPositions; m++) {
		double lambda_m = ComputeLambda( r, theta, N0, m );
		double term = data[ m - 1 ]*log( lambda_m ) - lambda_m;
		
		sum = sum + term;
	}
	
	return sum;
}

double ComputeLambda(double r, double theta, int N0, int m) {
	
	///lambda_m = N0e^( -r_m/attenuationLength )
	///Convert angle to radians
	double angleRad = ( (m - 1)*beta - theta )*( TMath::Pi()/180 ); 
	double r_mSquared = r*r + diskRadius*diskRadius - 2*r*diskRadius*TMath::Cos( angleRad );
	if (r_mSquared < 0) {
		std::cout << "Error! Square root of negative number!\n";
		return 0;
	}
	double r_m = TMath::Sqrt( r_mSquared );
	double lambda_m = N0*TMath::Exp( -r_m/attenuationLength )/(r_m*r_m);
	
	return lambda_m;
}

void MakePlots(std::multimap<std::string, double> parameters) {
	
	///Get the counts lambda_m using the mlestimates for r, theta, and N0
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
	fDisk->Draw();
}
