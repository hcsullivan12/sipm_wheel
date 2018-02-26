std::string amplitudeFile = "/home/hunter/Desktop/characterize_sipm/M1amp200000.txt";
std::string areaFile      = "/home/hunter/Desktop/characterize_sipm/M2area200000.txt";
std::string display       = "/home/hunter/Desktop/characterize_sipm/Measuredisplay00000.txt";

///Fitting parameters
double ampThreshold  = 0.004;
double areaThreshold = 0.01;
double ampSig  = 5;
double areaSig = 5;
double ampFitRange = 0.002;

std::pair ReadFile(std::string );
void MakeHistograms(std::pair< std::vector<double>, std::vector<double> > , std::string );
TGraphErrors* FitGain(TH1D* , std::string );

void CharacterizeSiPMs()
{
	///Amplitude Distribution
	auto ampData = ReadFile(amplitudeFile);
	auto ampDist = MakeHistograms(ampData, "amplitude");
        TCanvas *c1  = new TCanvas("c1", "c1", 800, 800);
        ampDist->Draw();
        auto ampPeaks   = FitGain(ampDist, "amplitude");
        TCanvas *c2  = new TCanvas("c2", "c2", 800, 800);
        ampPeaks->Draw("AP*");


	///Area Distribution
	auto areaData = ReadFile(areaFile);
	auto areaDist = MakeHistograms(areaData, "area");
	TCanvas *c3  = new TCanvas("c3", "c3", 800, 800);
        areaDist->Draw();
        auto areaPeaks   = FitGain(areaDist, "area");
        TCanvas *c4  = new TCanvas("c4", "c4", 800, 800);
        areaPeaks->Draw("AP*");
}

std::pair ReadFile(std::string filename)
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

		std::pair< std::vector<double>, std::vector<double> p;
		p = std::make_pair(xVec, yVec);

		return p;
	}
}

TH1D* MakeHistograms(std::pair< std::vector<double>, std::vector<double> > dataSet, std::string type) 
{

	///Find min and max in data set
	double xMin = 0;
	double xMax = 0;
	for (auto it = dataSet.first.begin(); it != dataSet.first.end(); it++) {
		if (it > xMax) xMax = it;
		if (it < xMin) xMin = it; 
	}

	///Make histogram
        //TCanvas *c1 = new TCanvas("c1", "c1", 800, 800);
        TH1D *dist = new TH1D("dist", "dist", dataSet.first.size(), xMin, xMax);

        auto it = 0;
        ///Fill
        while (it < dataSet.first.size()) {
        	dist->SetBinContent(it + 1, dataSet.second.at(it));
                it++;
        }

        dist->SetTitle("SiPM Response in Dark Box");
	if (type == "amplitude") {
		dist->GetXaxis()->SetTitle("Amplitude/Volts");
	} elif (type == "area") {
		dist->GetXaxis()->SetTitle("Area/Weber");
	}

        return dist;
}

TGraphErrors* FitGain(TH1D *hs, std::string type)
{

	TGraphErrors* grpeaks;

	Float_t gain=0;
	TSpectrum *s = new TSpectrum();
	Int_t nfound = s->Search(hs, 5, "", 0.004);
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
       			if(peaks[p] < peaks[p+1]) {
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
		std::cout << y[j] << "\n";
     	}
   
   	int point = 1;
   	for (int g = 0; g < npeaks; g++) {
     
     		TF1 *gfit = new TF1("gfit", "gaus", y[g] - ampFitRange/2, y[g] + ampFitRange/2);
     
     		hs->Fit(gfit,"QR+");

     		//if( (abs(y[g] - gfit->GetParameter(1)) ) < 25 ) {
       			gx[point - 1] = point;
	       		gy[point - 1] = gfit->GetParameter(1);
	       		gey[point - 1] = gfit->GetParError(1);
			std::cout << gey[point - 1] << "\n";
	       		point++;
	       		// cout<<"good"<<endl;
	       		//getchar();
	     	//}     
	}
   
   	grpeaks = new TGraphErrors(npeaks,gx,gy,0,gey);
	
   	//for (int p = 0; p < npeaks ; p++){grpeaks->SetPoint(p,p,0); grpeaks->SetPointError(p,p,0);}

   	//for(int p = 0; p < gg; p++){ grpeaks->SetPoint(p,gx[p],gy[p]); grpeaks->SetPointError(p,0,gey[p]);}

   
   	grpeaks->SetTitle("SiPM Gain");
   	grpeaks->GetXaxis()->SetTitle("Peak N");
   	grpeaks->GetYaxis()->SetTitle("Absolute Amplitude/Volts");
   	grpeaks->SetMarkerColor(4);
   	grpeaks->SetMarkerStyle(20);
   	grpeaks->SetFillColor(0);
   
   	//Fit
   	//TF1 *fit = new TF1("fit","[0] + [1]*x",0.5,gg-0.5);
   	fit = new TF1("fit", "[0] + [1]*x", 0.5, npeaks);
   
   	fit->SetParName(1,"Gain");
   	fit->SetParName(0, "Pedestal");
   	grpeaks->Fit(fit, "QR"); 
	gStyle->SetOptFit();

  
	return grpeaks;
}

