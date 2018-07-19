///
/// CharacterizeSiPMs.C
/// Written by Hunter Sullivan
///
/// This code will take histogram csv files outputted by the Lecroy oscilliscope
/// and compute the gain from the area and amplitude distributions. 

std::string data_dir = "/home/hunter/projects/wheel/data/sipms_to_characterize/sipm1";
const unsigned nFiles = 5;
std::string files[nFiles] = {"M1-72-6-00000.txt", /*"M1-72-8-00000.txt",*/ "M1-73-0-00000.txt", 
			     "M1-73-2-00000.txt", /*"M1-73-4-00000.txt", "M1-73-6-00000.txt",*/ 
			     "M1-73-8-00000.txt", "M1-74-0-00000.txt", /*"M1-74-2-00000.txt"*/};

/*std::string files[nFiles] = {"M2-72-8-00000.txt", "M2-73-0-00000.txt", "M2-73-2-00000.txt",
                             "M2-73-4-00000.txt", "M2-73-6-00000.txt", "M2-73-8-00000.txt", 
                             "M2-74-0-00000.txt", "M2-74-2-00000.txt"};*/

///Fitting parameters
double ampThreshold  = 0.05; //0.05
double areaThreshold = 0.01;
double ampSig        = 0.05; //0.01
double areaSig       = 0.1*TMath::Power(10, -9);
double ampFitRange   = 0.00020;
double areaFitRange  = 0.1*TMath::Power(10, -9);

TCanvas *master_amp_dist = new TCanvas("master_amp_dist", "All Amplitude Distributions", 1000, 1000);
TCanvas *master_gain_plot = new TCanvas("master_gain_plot", "All gains", 1000, 1000);

std::vector<double> gains;
double bias[nFiles];

std::pair< std::vector<double>, std::vector<double> > ReadFile(std::string );
void MakeHistograms(TH1D* , std::pair< std::vector<double>, std::vector<double> > , std::string );
TGraphErrors* FitGain(TH1D* , std::string );

void CharacterizeSiPMs()
{
	///Set correct file names
	for(int i = 0; i < nFiles; i++){
		files[i] = data_dir + "/" + files[i];
	}
	
	master_amp_dist->Divide(4,4);
	master_gain_plot->Divide(4,4);

	///For each file, read it, make histogram, fit
	for(int file = 0; file < nFiles; file++) {
		
 		///Amplitude Distribution
		auto ampData = ReadFile(files[file]);
		TH1D* ampDist = new TH1D(); 
		MakeHistograms(ampDist, ampData, "amplitude");
	
		master_amp_dist->cd(file + 1);		

		gStyle->SetOptStat(0);
    		ampDist->Draw("apl");

        	auto ampPeaks = FitGain(ampDist, "amplitude");
	
		master_gain_plot->cd(file + 1);

		//std::string amp_peaks_plot = std::to_string(file);
        	//TCanvas *c2  = new TCanvas(amp_peaks_plot.c_str(), amp_peaks_plot.c_str(), 800, 800);
		ampPeaks->SetMarkerStyle(20);
		ampPeaks->SetMarkerColor(1);
		ampPeaks->Draw("AP");
		ampPeaks->GetXaxis()->SetLimits(0, 5);
		ampPeaks->SetMinimum(0);
		//ampPeaks->Draw("AP*");
		//master_gain_plot->Update();
	}

	///Set the bias voltages
	bias[0]  = 72.8;
        //bias[1]  = 73.0;
        bias[1]  = 73.2;
        bias[2]  = 73.4;
	//bias[4]  = 73.6;
	//bias[5]  = 73.8;
	bias[3]  = 74.0;
	bias[4]  = 74.2;
	//bias[5]  = 74.2;

	///Plot versus bias
	TCanvas *breakdown_plot  = new TCanvas("breakdown_plot", "breakdown_plot", 800, 800);
	TGraph *g1 = new TGraph(nFiles, bias, &gains[0]);
	g1->SetTitle("Breakdown of SiPM");
	g1->GetXaxis()->SetTitle("Bias (V)");
	g1->GetYaxis()->SetTitle("Gain (mV/p.e.)");
	g1->GetXaxis()->SetLimits(70,75);
	g1->SetMinimum(0);
	g1->SetMaximum( 1.1*(*max_element(gains.begin(), gains.end())) );
	g1->SetMarkerStyle(23);
	g1->Draw("AP");

	///Fit for bias plot
        TF1 *fit1 = new TF1("fit1", "[0] + [1]*x", 70, 74.5);

        fit1->SetParName(1,"Slope");
        fit1->SetParName(0, "YInt");
        g1->Fit(fit1, "QR");
	//g1->GetXaxis()->SetRangeUser(70, 75);
        gStyle->SetOptFit();

	double breakdown = -fit1->GetParameter(0)/fit1->GetParameter(1);
	
	std::cout << "\n\nBreakdown is... " << breakdown << "\n\n";

	///Plot versus overvoltage
	double bias_overvoltage[nFiles];
	for (int i = 0; i < nFiles; i++){
		bias_overvoltage[i] = bias[i] - breakdown;
	}
        TCanvas *overvoltage_plot  = new TCanvas("overvoltage_plot", "Overvoltage Plot", 800, 800);
        TGraph *g2 = new TGraph(nFiles, bias_overvoltage, &gains[0]);
        g2->SetTitle("SiPM Gain");
        g2->GetXaxis()->SetTitle("Over-voltage (#DeltaV)");
        g2->GetYaxis()->SetTitle("Gain (mV/p.e.)");
        g2->GetXaxis()->SetLimits(0,4);
	g2->SetMinimum(0);
	g2->SetMaximum(0.75);
	g2->SetMarkerStyle(23);
	g2->SetMarkerSize(2);
        g2->Draw("AP");

	///Fit for overvoltage plot
        TF1 *fit2 = new TF1("fit2", "[0] + [1]*x", 0, 4);

        fit2->SetParName(1,"Slope");
        fit2->SetParName(0, "YInt");
        g2->Fit(fit2, "QR");
        gStyle->SetOptFit();

	std::cout << fit2->GetParError(0) << "   " << fit2->GetParError(1) <<  std::endl;

	std::cout << "\n\nGain is... " << std::setprecision(3) << fit2->GetParameter(1) << " mV/p.e./O.V.\n\n";

	///Place the quantities on plot
	std::string bd = std::to_string(breakdown);
	//double breakdown_error = breakdown*std::sqrt( std::pow(fit2->GetParError(0)/fit1->GetParameter(0), 2) + std::pow( fit2->GetParError(1)/fit2->GetParameter(1), 2) );
	//std::string bd_e = std::to_string(breakdown_error);
	std::string text1 = "V_{BD} = " + bd.substr(0,4) + " V";
	TLatex *bd_label = new TLatex(1,0.4,text1.c_str());
	bd_label->SetTextSize(0.025);
	bd_label->Draw("same");
	
	std::string g = std::to_string(fit2->GetParameter(1));
	std::string e = std::to_string(fit2->GetParError(1));
	std::string text2 = "G = " + g.substr(0,5) + " #pm " + e.substr(0,5) + " mV/p.e./#DeltaV";
        TLatex *gain_label = new TLatex(1,0.35,text2.c_str());
        gain_label->SetTextSize(0.025);
        gain_label->Draw("same");

	/*///Area Distribution
	auto areaData = ReadFile(areaFile);
	TH1D* areaDist = new TH1D();
	MakeHistograms(areaDist, areaData, "area");
	
	TCanvas *c3  = new TCanvas("c3", "c3", 800, 800);
        areaDist->Draw();
        auto areaPeaks   = FitGain(areaDist, "area");

        areaPeaks->Draw("AP*");*/
	//gApplication->Terminate();
      
}

std::pair< std::vector<double>, std::vector<double> > ReadFile(std::string filename)
{
	double xMin = 0;
        double xMax = 0;

        std::ifstream file(filename.c_str());
        if (!file) {
                std::cout << "Cannot open " << filename << std::endl;
        } else {
                ///Read file
                std::string word = "";
                std::vector<double> xVec;
                std::vector<double> yVec;
                //double xMin = 0;
                //double xMax = 0;

                ///Skip first text
                while (word != "Time,Ampl") {
                        file >> word;
                }

                while(!file.eof()) {
                        ///Data starts
                        std::string yTemp;
                        std::string xTemp;

                        std::getline(file, xTemp, ',');
                        std::getline(file, yTemp);

                        double y = atof(yTemp.c_str());
                        double x = atof(xTemp.c_str());

                        if (x < xMin) xMin = x;
                        if (x > xMax) xMax = x;

                        xVec.push_back(x);
                        yVec.push_back(y);
                }

		std::pair< std::vector<double>, std::vector<double> > p;
		p = std::make_pair(xVec, yVec);

		return p;
	}
}

void MakeHistograms(TH1D *dist, std::pair< std::vector<double>, std::vector<double> > dataSet, std::string type) 
{

	///Find min and max in data set
	double xMin = 0;
	double xMax = 0;
	for (auto it = dataSet.first.begin(); it != dataSet.first.end(); it++) {
		if (*it > xMax) xMax = *it;
		if (*it < xMin) xMin = *it; 
	}

	///Make histogram
        //TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
        dist->SetBins(dataSet.first.size(), xMin, xMax);
        //TH1D dist("dist", "dist", dataSet.first.size(), xMin, xMax);

        auto it = 0;
        ///Fill
        while (it < dataSet.first.size()) {
        	dist->SetBinContent(it + 1, dataSet.second.at(it));
                it++;
        }

        dist->SetTitle("SiPM Response in Dark Box");
	if (type == "amplitude") {
		dist->GetXaxis()->SetTitle("Amplitude/Volts");
	} else {
		if (type == "area") {
			dist->GetXaxis()->SetTitle("Area/Weber");
		}
		else std::cout << "Error in type\n";
	}
}

TGraphErrors* FitGain(TH1D *hs, std::string type)
{

	TGraphErrors* grpeaks;

	Float_t gain=0;
	TSpectrum *s = new TSpectrum(3);
	double threshold;
	if (type == "amplitude") {
		threshold = ampThreshold;
	} else {
		if (type == "area") {
			threshold = areaThreshold;
		} else std::cout << "Error of type.\n";
	}
	Int_t nfound = s->Search(hs, ampSig, "", threshold);
	Int_t npeaks = s->GetNPeaks();
	//printf("Found %d peaks to fit\n",nfound);
   	printf("Found %d peaks to fit\n",npeaks);
   
   	Double_t *peaks;
   	Double_t *altpeaks;
   	peaks = s->GetPositionX();
 	//  altpeaks = s->GetPositionY();

	Double_t x[npeaks], ex[npeaks];
        Double_t y[npeaks], ey[npeaks];

        Double_t gx[npeaks], gex[npeaks];
        Double_t gy[npeaks], gey[npeaks];

 
 	//Sort peaks
   	double temp;
   	int nchanges=0;
   	do{
     		nchanges=0;
     		for(int p = 0; p < nfound - 1; p++) {
       			if(peaks[p] > peaks[p+1]) {
	 	 		temp=peaks[p];
			 	peaks[p]=peaks[p+1];
	 			peaks[p+1]=temp;
	 			nchanges++;
       			}
       		//cout<<"iteraciones "<<nchanges<<endl;
     		}
     
   	}while(nchanges != 0);

   	for (int j = 0; j < npeaks; j++) {
     		//cout << "Peak "<<j<<" at = " << peaks[j] <<", with "<<altpeaks[j] <<" counts"<< endl;
     
     		x[j] =j+1;
     		y[j]=peaks[j];
		//std::cout << y[j] << "\n";
     	}
   
   	int point = 1;
	double fitRange;
	if (type == "amplitude") {
                fitRange = ampFitRange;
        } else {
                if (type == "area") {
                	fitRange = areaFitRange;
                       
                } else std::cout << "Error in type.\n";
        }


   	for (int g = 0; g < npeaks; g++) {
     
     		TF1 *gfit = new TF1("gfit", "gaus", y[g] - fitRange/2, y[g] + fitRange/2);
     
     		hs->Fit(gfit,"QR+");

     		//if( (abs(y[g] - gfit->GetParameter(1)) ) < 25 ) {
       			gx[point - 1] = point;
	       		gy[point - 1] = gfit->GetParameter(1);  //Mean
	       		gey[point - 1] = gfit->GetParError(1);
			//std::cout << gey[point - 1] << "\n";
	       		point++;
	       		// cout<<"good"<<endl;
	       		//getchar();
	     	//}     
	}
   
   	grpeaks = new TGraphErrors(npeaks,gx,gy,0,gey);
	
   	//for (int p = 0; p < npeaks ; p++){grpeaks->SetPoint(p,p,0); grpeaks->SetPointError(p,p,0);}

   	//for(int p = 0; p < gg; p++){ grpeaks->SetPoint(p,gx[p],gy[p]); grpeaks->SetPointError(p,0,gey[p]);}

   	if (type == "amplitude") {
		grpeaks->SetTitle("SiPM Gain from Amplitude Distribution");
		grpeaks->GetYaxis()->SetTitle("Amplitude/Volts");
	} else {
		if (type == "area") {
			grpeaks->SetTitle("SiPM Gain from Area Distribution");
			grpeaks->GetYaxis()->SetTitle("Area/Weber");
		} else std::cout << "Error in type.\n";
	}
	grpeaks->GetYaxis()->SetTitleOffset(1.4);
   	grpeaks->GetXaxis()->SetTitle("Peak N");
   	grpeaks->SetMarkerColor(4);
   	grpeaks->SetMarkerStyle(20);
   	grpeaks->SetFillColor(0);
   
   	//Fit
   	TF1 *fit = new TF1("fit","[0] + [1]*x",0.5,npeaks-0.5);
   	fit = new TF1("fit", "[0] + [1]*x", 0.5, npeaks + 1);
   
   	fit->SetParName(1,"Gain");
   	fit->SetParName(0, "Pedestal");
   	grpeaks->Fit(fit, "QR"); 
	gStyle->SetOptFit();

	/*std::pair<TGraphErrors, double> p;
	p.first = grpeaks;
	p.second = fit->GetParameter(1);*/
 
	gains.push_back( 1000*fit->GetParameter(1) ); //Converted to mV
 
	return grpeaks;
}

