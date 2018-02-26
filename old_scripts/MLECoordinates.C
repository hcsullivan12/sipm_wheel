///Config parameters for reconstruction
const double diskRadius = 5.7; ///cm
const double attenuationLength = 2.5; ///cm
const int    nPositions = 8;
const int    nSiPMs = 2;
const double gain = 1;
const std::string SiPMDataPath = "/home/hunter/Desktop/wheel/ZackRun/SiPM_Wheel_Zatt";
const double overVoltage = 2;

double getSiPMSignal(int position) {
	
	///First get the data from SiPMs
	std::ifstream inFile("./build/signalFile.txt");
	if (!inFile.is_open()) {
		std::cout << "Error. Could not open signal file\n";
		exit(1);
	}
	std::vector<double> maxHeights;
	string word;
	while (inFile >> word) {
		maxHeights.push_back(std::stod(word));
	}
	
	return maxHeights.at(position - 1);
}

void findIntersectionPoints(TF2* func1, TF2* func2) {
	
	///First find when func1 and func2 intersect the xy plane, then check if there is an intersection between func1 and func2
	double epsilon = 0.7;
	double theta;
	double error = 0.1;
	int counter = 0;
	std::vector<std::pair<double, double> > intersectionPoints; ///(epsilon, theta)
	
	std::cout << std::endl;
	
	
	
	while (epsilon < 0.8) {
		theta = -10;
		
		
		//if (counter%10 == 0) std::cout << "Epsilon = " << counter/100.0 << std::endl;
		
		while (theta <= 10) {
			///Evaluate func1 and func2 at epsilon, theta
			double temp1 = func1->Eval(epsilon, theta);
			double temp2 = func2->Eval(epsilon, theta);
			std::cout << "Func1 at epsilon = " << epsilon << "  theta = " << theta << "  ---> " << temp1 << std::endl;
			std::cout << "Func2 at epsilon = " << epsilon << "  theta = " << theta << "  ---> " << temp2 << std::endl;
			std::cout << std::endl;
			
			/*if (temp1 < error && temp2 < error) {
				///Found simultaneous zero of func1 and func2 and intersection
				std::cout << "FOUND INTERSECTION\n";
				std::cout << "Func1 at epsilon = " << epsilon << "  theta = " << theta << "  ---> " << temp1 << std::endl;
				std::cout << "Func2 at epsilon = " << epsilon << "  theta = " << theta << "  ---> " << temp2 << std::endl;
				std::cout << std::endl;
				intersectionPoints.push_back(std::make_pair(epsilon, theta));
			}*/
			theta = theta + 1;
		}
		
		
		epsilon = epsilon + 0.01;
		//counter++;
		
	}
	
	std::cout << "\nThe intersection points (epsilon, theta) are approximately... " << std::endl;
	for (auto it = intersectionPoints.begin(); it != intersectionPoints.end(); it++) {
		std::cout << "(" << it->first << ", " << it->second << ")" << std::endl;
	}
}

Double_t N_0Function(Double_t* x, Double_t* par) {
	Double_t sum1 = 0;
	Double_t sum2 = 0;
	
	for (int n = 1; n <= nPositions; n++) {
		Double_t term = getSiPMSignal(n)*100;
		sum1 = sum1 + term;
	}
	
	for (int n = 1; n <= nPositions; n++) {
		Double_t term = TMath::Exp(-(diskRadius/attenuationLength)*TMath::Sqrt(1 + x[0]*x[0] - 2*x[0]*TMath::Cos((n-1)*TMath::Pi()/4 - x[1])));
		sum2 = sum2 + term;
	}
	Double_t N_0 = sum1/sum2;
	return N_0;
}

Double_t f1(Double_t* x, Double_t* par) {
	
	///First Calculate N_0
	Double_t sum = 0;
	for (int n = 1; n <= nPositions; n++) {
		Double_t term = TMath::Exp(-(diskRadius/attenuationLength)*TMath::Sqrt(1 + x[0]*x[0] - 2*x[0]*TMath::Cos((n-1)*TMath::Pi()/4 - x[1])));
		sum = sum + term;
	}
	Double_t N_0 = 1/sum;
	
	///Now the sum
	sum = 0;
	for (int n = 1; n <= nPositions; n++) {
		Double_t r_n = TMath::Sqrt(1 + x[0]*x[0] - 2*x[0]*TMath::Cos((n - 1)*TMath::Pi()/4 - x[1])); 
		Double_t factor = N_0*TMath::Exp(-(diskRadius/attenuationLength)*r_n) - getSiPMSignal(n)*100;
		Double_t term = factor*(1/r_n)*(x[0] - TMath::Cos((n - 1)*TMath::Pi()/4 - x[1])); 
		sum = sum + term;
	}
	
	return sum;
	
}

Double_t f2(Double_t* x, Double_t* par) {
	
	///First Calculate N_0
	Double_t sum = 0;
	for (int n = 1; n <= nPositions; n++) {
		Double_t term = TMath::Exp(-(diskRadius/attenuationLength)*TMath::Sqrt(1 + x[0]*x[0] - 2*x[0]*TMath::Cos((n-1)*TMath::Pi()/4 - x[1])));
		sum = sum + term;
	}
	Double_t N_0 = 1/sum;
	
	///Now the sum
	sum = 0;
	for (int n = 1; n <= nPositions; n++) {
		Double_t r_n = TMath::Sqrt(1 + x[0]*x[0] - 2*x[0]*TMath::Cos((n - 1)*TMath::Pi()/4 - x[1])); 
		Double_t factor = N_0*TMath::Exp(-(diskRadius/attenuationLength)*r_n) - getSiPMSignal(n)*100;
		Double_t term = factor*(1/r_n)*x[0]*diskRadius*TMath::Sin((n - 1)*TMath::Pi()/4 - x[1]); 
		sum = sum + term;
	}
	
	return sum;
	
}

void mleCoordinates() {	
	
	///
	///Read in SiPM data
	///
	
	///Data multimap (SiPM number, (position, data)
	std::multimap<unsigned, std::multimap<unsigned, double> > Data; 
	
	std::cout << std::endl;
	
	///
	///Read in SiPM gains
	///
	
	std::multimap<unsigned, double> SiPMGains;
	std::string SiPMGainFilename = SiPMDataPath + "/GainConfig.txt";
	std::ifstream gainFile(SiPMGainFilename.c_str());
	if (!gainFile) {
		std::cout << "Cannot open " << SiPMGainFilename << std::endl;
	} else {
		///Read file
		while(!gainFile.eof()) {
			std::string number;
			std::string gain;
			std::getline(gainFile, number, ',');
			std::getline(gainFile, gain);
			
			unsigned SiPMNumber = atoi(number.c_str());
			double SiPMGain = atof(gain.c_str());
			//std::cout << "Inserting in gain\n";
			SiPMGains.insert(std::pair<unsigned, double>(SiPMNumber, SiPMGain) );
		}
	}
	
	for (auto it = SiPMGains.begin(); it != SiPMGains.end(); it++) {
		std::cout << "\nObtained gain of SiPM " << it->first << std::endl;
	}
	
	///
	///Print out the gain values
	///
	
    ///Loop through SiPMs
	for (auto it1 = SiPMGains.begin(); it1 != SiPMGains.end(); it1++) {
		std::cout << "SiPM " << it1->first << " gain = " << it1->second << std::endl;		
		std::cout << std::endl;
	}

	
	///
	///Read from SiPM files
	///
	
	for (int SiPM = 1; SiPM <= nSiPMs; SiPM++) {
		
		std::multimap<unsigned, double> CurrentSiPMMap;
		
		for (int position = 0; position < nPositions; position++) {
			
			///Define filename
			std::string filename = SiPMDataPath + "/Channel_" + to_string(SiPM) + "/M" + to_string(SiPM) + " Pos" + to_string(position) + "_00000.txt";
			
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
				//double t1 = -100000;
				
				std::cout << "Reading Filename: " << filename << "\n" << std::endl;
				///Skip first text
				while (word != "Time,Ampl") {
					file >> word;
				}
				
				while(!file.eof()) {
					///Data starts
					std::string vTemp;
					std::string tTemp;
					
					std:getline(file, tTemp, ',');
					std::getline(file, vTemp);
					
					double v = abs(atof(vTemp.c_str()));
					double t = atof(tTemp.c_str());
					
					//std::cout << v << "    " << t << std::endl;
			
					if (v > max) {
						max = v;
					}
		
					//t1 = t1/1000;
					if(v != 0) {
						time.push_back(t);
						voltage.push_back(v);
					}
		
					//t1++;
				
				}
				
				///Store max in map
				CurrentSiPMMap.insert(std::pair<unsigned, double>(position, max) );
			}
			
			file.close();
		}
		
		///Store SiPMMap in Data
		Data.insert(std::pair<unsigned, std::multimap<unsigned, double> >(SiPM, CurrentSiPMMap) );
	}
	
	std::cout << "\n";
	
	for (auto it = Data.begin(); it != Data.end(); it++) {
		std::cout << "Recived data from SiPM " << it->first << "\n" << std::endl;
	}
	
	
	TF2 *func1 = new TF2("func1", f1, 0, 1, -180, 180, 0);
	TF2 *func2 = new TF2("func2", f2, 0, 1, -180, 180, 0);
	
	TF2 *func3 = new TF2("func3", f1, 0, 1, -180, 180, 0);
	TF2 *func4 = new TF2("func4", f2, 0, 1, -180, 180, 0);
	
	TF2 *func5 = new TF2("func5", N_0Function, 0, 1, -180, 180, 0);

	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);
	func1->SetTitle("f_{1}(#epsilon, #theta)");
	func1->GetXaxis()->SetTitle("#epsilon/Units of Disk Radius");
	func1->GetYaxis()->SetTitle("#theta/Degrees");
	func1->GetXaxis()->CenterTitle();
	func1->GetYaxis()->CenterTitle();
	func1->GetXaxis()->SetTitleOffset(2);
	func1->GetYaxis()->SetTitleOffset(2);
	func1->Draw("surf1");
	
	TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
	func2->SetTitle("f_{2}(#epsilon, #theta)");
	func2->GetXaxis()->SetTitle("#epsilon/Units of Disk Radius");
	func2->GetYaxis()->SetTitle("#theta/Degrees");
	func2->GetXaxis()->CenterTitle();
	func2->GetYaxis()->CenterTitle();
	func2->GetXaxis()->SetTitleOffset(2);
	func2->GetYaxis()->SetTitleOffset(2);
	func2->Draw("surf1");
	
	TCanvas *c3 = new TCanvas("c3", "c3", 800, 600);
	func3->SetTitle("Contours for F_{1, 2}(#epsilon, #theta) = 0");
	func3->GetXaxis()->SetTitle("#epsilon/Units of Disk Radius");
	func3->GetYaxis()->SetTitle("#theta/Degrees");
	//func3->GetXaxis()->CenterTitle();
	//func3->GetYaxis()->CenterTitle();
	func3->GetXaxis()->SetTitleOffset(1.4);
	func3->GetYaxis()->SetTitleOffset(1.4);
	//double levels[1] = {1.0}; 
	func3->SetContour(1);
	func3->SetContourLevel(0, 0);
	func3->SetLineColor(4);
	func3->SetLineWidth(2);
	func3->Draw();
	
	func4->SetContour(1);
	func4->SetContourLevel(0, 0);
	func4->SetLineColor(3);
	func4->SetLineWidth(2);
	func4->Draw("same");
	
	auto leg = new TLegend(0.1,0.7,0.48,0.9);
	//legend->SetHeader("","C"); // option "C" allows to center the header
	leg->AddEntry("func3","f_{1}(#epsilon, #theta)","l");
	leg->AddEntry("func4","f_{2}(#epsilon, #theta)","l");
	leg->Draw();
	
	TCanvas *c4 = new TCanvas("c4", "c4", 800, 600);
	func5->SetTitle("N_{0}(#epsilon, #theta)");
	func5->GetXaxis()->SetTitle("#epsilon/Units of Disk Radius");
	func5->GetYaxis()->SetTitle("#theta/Degrees");
	func5->GetXaxis()->SetTitleOffset(2);
	func5->GetYaxis()->SetTitleOffset(2);
	func5->Draw("surf1");
	
	findIntersectionPoints(func1, func2);
	
}




