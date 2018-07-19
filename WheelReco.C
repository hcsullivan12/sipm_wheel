///
/// WheelReco.C
/// Written by Hunter Sullivan
///
/// This code will use an MLE for SiPM wheel reconstruction
/// by cycling through different radius, theta values

#include "DataReader.h"

/// Configuration file
std::string config_file = "/home/hunter/projects/wheel/data/sipms_to_characterize/sipm_gains.txt";

/// Data directory
std::string data_dir = "/home/hunter/projects/wheel/data/spring2018_1";

/// Data files
std::string sipm1 = data_dir + "/C1-73-5V-30usec-80percent-15msec-00016.txt";
std::string sipm2 = data_dir + "/C2-73-5V-30usec-80percent-15msec-00016.txt";
std::string sipm3 = data_dir + "/C3-73-5V-30usec-80percent-15msec-00016.txt";
std::string sipm4 = data_dir + "/C4-73-5V-30usec-80percent-15msec-00016.txt";
std::string sipm_data_files[4];
//sipm_data_files[0] = sipm1; sipm_data_files[1] = sipm2; sipm_data_files[2] = sipm3; sipm_data_files[3] = sipm4;

double x_start = ;
double x_stop = ;
double x_inc = ;


/// Bias
double bias = 73.5;

const double diskRadius        = 5.7; ///cm
const double incrementRadius   = 0.5; ///cm
const double incrementTheta    = 1.0; ///degrees
const double attenuationLength = 2.7;
const double upperBoundN       = 5000;
const int numberOfPositions    = 8;
const double beta              = 360/numberOfPositions;  ///Angle between each position, in degrees
//double data[numberOfPositions];
double data[numberOfPositions] = {1.16456, 0.4465, 0.231687, 0.147494, 0.122291, 0.125334, 0.172409, 0.483366};
//double data[numberOfPositions] = {0.172409, 0.483366, 1.16456, 0.4465, 0.231687, 0.147494, 0.122291, 0.125334};
const double gain = 0.005; /// V/p.e.

/// Intialize gain arrays
std::vector<double> gains;
std::vector<double> gains_error;

/// Initialize sipm data arrays
std::vector<double> sipm_counts;
std::vector<double> sipm_errors;

///Initialize the ml and container for mle parameters
double maxLikelihoodValue = 0;
std::multimap<std::string, double> maxLiklihoodParam;

///True values!
double ml;
double mleRadius;
double mleTheta;
double mleN0;

///Prototypes
void *handle(void *ptr);
double ComputeLikelihood(double r, double theta, int N0);
double ComputeLambda(double r, double theta, int N0, int m);
void MakePlots(std::multimap<std::string, double> parameters);

Double_t disk(Double_t* x, Double_t* par) 
{
        Double_t f = x[0]*x[0] + x[1]*x[1];
        return f;
}

void WheelReco(/*std::string outputFilename*/) 
{
	
	clock_t start = clock();

	sipm_data_files[0] = sipm1; sipm_data_files[1] = sipm2; sipm_data_files[2] = sipm3; sipm_data_files[3] = sipm4;

	// Read configuration file
	std::ifstream gain_file(config_file.c_str());
	if (!gain_file) {
		std::cout << "Cannot open " << config_file << std::endl;
	} else {
		///Ignore first line
		std::string line;
		std::getline(gain_file, line);

		///Read the gains
		while (!gain_file.eof()){
			std::string g;
			std::string e;
			std::getline(gain_file, g, ' ');
			std::getline(gain_file, e);

			gains.push_back( atof(g.c_str()) );
			gains_error.push_back( atof(e.c_str()) );
		}
	}

	/*/// Make sure number of sipms = number of positions
	if (gains.size() != numberOfPositions) {
		std::cout << "Error! Number of sipms does not equal number of positions." << std::endl;
		gApplication->Terminate();		
	}*/
	
	/// Read the data, sipm_counts stores how many p.e. each sipm saw
	DataReader dr(bias, gains, gains_error);
	dr.Read(numberOfPositions, x_start, x_stop, x_inc);
	//dr.ReadData(sipm_data_files);
	sipm_counts = dr.GetSiPMCounts();
	//sipm_errors = dr.GetSiPMCountErrors();

	/*///Initialize data array
	for (int i = 0; i < numberOfPositions; i++) {
		data[i] = 0;
	}*/
	
	
	///Find max in data and store photons
	double Nmax = 0;
	for (int i = 0; i < numberOfPositions; i++) {
		data[i] = static_cast<unsigned>(data[i]/gain);
		if (data[i] > Nmax) Nmax = data[i];
	}
	
	///N0 set to lower threshold to decrease computation time
	int N0 = Nmax;
	
	while ( N0 <= upperBoundN ) {
		
		///Create threads
	        TThread *t0 = new TThread("t0", handle, (void*) N0);
		N0++;
        	TThread *t1 = new TThread("t1", handle, (void*) N0);
		N0++;
		TThread *t2 = new TThread("t2", handle, (void*) N0);
		N0++;
                TThread *t3 = new TThread("t3", handle, (void*) N0);
		N0++;
		TThread *t4 = new TThread("t4", handle, (void*) N0);
		N0++;
		TThread *t5 = new TThread("t5", handle, (void*) N0);
                N0++;
                TThread *t6 = new TThread("t6", handle, (void*) N0);
                N0++;


		t0->Run();
		t1->Run();
		t2->Run();
		t3->Run();
                t4->Run();
		t5->Run();
                t6->Run();

		t0->Join();
		t1->Join();
		t2->Join();
                t3->Join();
		t4->Join();
		t5->Join();
                t6->Join();
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

void *handle(void *ptr)
{

	long N0 = (long) ptr;

	///Variables used for incrementing
        ///Start with r = 0, theta = 0, (N0 is what's passed) and increment by the constants defined above
        double radius = 0;
        double theta  = 0;
	
	TThread::Lock();
	//std::cout << "N0: " << N0 << std::endl;
	TThread::UnLock();
        while ( radius <= diskRadius ) {
        	//std::cout << "radius: " << radius << std::endl;
                while ( theta < 360 ) {
                	//std::cout << "N0: " << N0 << "   radius: " << radius << "   theta: " << theta << std::endl;
                        double likelihood = ComputeLikelihood( radius, theta, N0 );
			
			TThread::Lock();
                        if ( likelihood > maxLikelihoodValue ) {
                        	maxLikelihoodValue = likelihood;
                                ///Store temporary maximum
                                ml        = maxLikelihoodValue;
                                mleRadius = radius;
                                mleTheta  = theta;
                                mleN0     = N0;
                        }
			TThread::UnLock();

 	                theta = theta + incrementTheta;
                }
                theta = 0;
                radius = radius + incrementRadius;
        }
        //radius = 0;
        //theta = 0;

	return 0;
}

double ComputeLikelihood(double r, double theta, int N0 ) 
{
	
	///Sum over terms ----> k_m*ln(lambda_m) - lambda_m
	double sum = 0;
	
	for (int m = 1; m <= numberOfPositions; m++) {
		double lambda_m = ComputeLambda( r, theta, N0, m );
		double term = data[ m - 1 ]*log( lambda_m ) - lambda_m;
		
		sum = sum + term;
	}
	
	return sum;
}

double ComputeLambda(double r, double theta, int N0, int m) 
{
	
	///lambda_m = N0e^( -r_m/attenuationLength )
	///Convert angle to radians
	double angleRad = ( (m - 1)*beta - theta )*( TMath::Pi()/180 ); 
	double r_mSquared = r*r + diskRadius*diskRadius - 2*r*diskRadius*TMath::Cos( angleRad );
	if (r_mSquared < 0) {
		std::cout << "Error! Square root of negative number!\n";
		return 0;
	}
	double r_m = TMath::Sqrt( r_mSquared );
	double lambda_m = N0*TMath::Exp( -r_m/attenuationLength );//(r_m);
	
	return lambda_m;
}

void MakePlots(std::multimap<std::string, double> parameters) 
{
	
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
	double x = parameters.find("radius")->second*TMath::Cos(parameters.find("theta")->second*(TMath::Pi()/180) );
	double y = parameters.find("radius")->second*TMath::Sin(parameters.find("theta")->second*(TMath::Pi()/180) );
	TMarker *point = new TMarker(x, y, 3);
	point->SetMarkerColor(4);
	point->SetMarkerSize(2);
	fDisk->Draw();
	point->Draw("same");

	///Draw y = 0
        TF1* line0 = new TF1("line0", "0", -diskRadius, diskRadius);
        line0->SetLineStyle(3);
        line0->SetLineColor(1);
        line0->SetLineWidth(3);
        line0->Draw("same");

        ///Draw y = x
        TF1* line1 = new TF1("line1", "x", -(sqrt(2)/2)*diskRadius, (sqrt(2)/2)*diskRadius);
        line1->SetLineStyle(3);
        line1->SetLineColor(1);
        line1->SetLineWidth(3);
        line1->Draw("same");

        ///Draw y = -x
        TF1* line2 = new TF1("line2", "-x", -(sqrt(2)/2)*diskRadius, (sqrt(2)/2)*diskRadius);
        line2->SetLineStyle(3);
        line2->SetLineColor(1);
        line2->SetLineWidth(3);
        line2->Draw("same");

        ///Draw x = 0
        TF1* line3 = new TF1("line3", "100000*x", -0.00001*diskRadius, 0.00001*diskRadius);
        line3->SetLineStyle(3);
        line3->SetLineColor(1);
        line3->SetLineWidth(3);
        line3->Draw("same");
}
