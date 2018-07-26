const double diskRadius = 5.7; ///cm
const double cornerLength = sqrt(2)*diskRadius;
const double attenuationLength = 2.5; ///cm
const int    nPositions = 8;
const int    nSiPMs = 1;

//const double data[nPositions] = {1.16456, 0.4465, 0.231687, 0.147494, 0.122291, 0.125334, 0.172409, 0.483366};
//const double data[nPositions] = {1.0, 1.1, 0.4, 1.01, 0.2, 1.06, 0.4, 1.05};

/*const double I1 = 1.16456;
const double I3 = 0.231687;
const double I5 = 0.122291;
const double I7 = 0.172409;*/

Double_t disk(Double_t* x, Double_t* par) {
	Double_t f = x[0]*x[0] + x[1]*x[1];
	return f; 
}

Double_t radius5Function(Double_t* x, Double_t* par) {
	///Changed to funtion of epsilon *(units of diskRadius)
	Double_t f = par[0]*x[0]*x[0] + par[1]*x[0]*(1/diskRadius) + par[2]*(1/diskRadius)*(1/diskRadius);
	return f; 
}

Double_t cosPhi_posFunction(Double_t* x, Double_t* par) {
	///Changed to funtion of epsilon *(units of diskRadius)
	Double_t f = par[0]*x[0]*x[0] + par[1]*x[0] + par[2];
	return f; 
}

void GeometricReco() {
		
	///Data arrays
	std::multimap<unsigned, std::multimap<unsigned, double> > Data; //(SiPM number, (position, point) 
	
	std::string SiPMDataPath = "/home/hunter/Desktop/wheel/ZackRun/SiPM_Wheel_Zatt";
	
	///Read from SiPM files
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
					
					std::getline(file, tTemp, ',');
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
	
	std::cout << "\n\n";
	
	for (auto it = Data.begin(); it != Data.end(); it++) {
		std::cout << "\nRecived data from SiPM " << it->first << std::endl;
	}
	
	std::cout << "\n\n";
	
	///Define reconstructed points vector (r, theta)
	std::vector<std::pair<double, double> > recoPoints; 
	
	///Print out the max values
	///Loop through SiPMs
	for (auto it1 = Data.begin(); it1 != Data.end(); it1++) {
		std::cout << "SiPM " << it1->first << " data...\n";
		///Loop through SiPM multimap
		for (auto it2 = it1->second.begin(); it2 != it1->second.end(); it2++) {
			std::cout << "Position " << it2->first << " max: " << it2->second << std::endl;
			
		}
		
		std::cout << std::endl;
		
	}
	
	///Loop through each SiPM
	for (auto it = Data.begin(); it != Data.end(); it++) {
		
		for (int pos = 0; pos < nPositions; pos++) {
		
			std::cout << "\nPosition " << pos << " results...\n";
			
			///Get data from SiPMs, and compute ratios assuming exponential decay
			int posTop = fmod(pos + (nPositions/4), nPositions);
			std::cout << "PosTop: " << posTop << std::endl;
			int posOpp = fmod(pos + (nPositions/2), nPositions);
			std::cout << "PosOpp: " << posOpp << std::endl;
			int posBot = fmod(pos + (3*nPositions/4), nPositions);
			std::cout << "PosBot: " << posBot << std::endl;
			double beta = (TMath::Pi()/4)*pos ;
			std::cout << "beta: " << beta*(180/TMath::Pi()) << " degrees" << std::endl;
			
			///Grab data corresponding to current SiPM and current position
			double I_pos = it->second.find(pos)->second;
			double I_Opp = it->second.find(posOpp)->second;
			double I_Top = it->second.find(posTop)->second;
			double I_Bot = it->second.find(posBot)->second;
			
			const double g_posOpp = log(I_pos/I_Opp);
			const double g_posTop = log(I_pos/I_Top);
			
			///Compute constants in derived equation
			const double gamma_posOpp = 4*diskRadius*diskRadius - attenuationLength*attenuationLength*g_posOpp*g_posOpp;
			const double gamma_posTop = cornerLength*cornerLength - attenuationLength*attenuationLength*g_posTop*g_posTop;
			
			///Calculate constants for finding cosPhi roots
			const double rho   = sqrt(2)*(gamma_posTop/gamma_posOpp)*attenuationLength*g_posOpp - sqrt(2)*attenuationLength*g_posTop; 
			const double omega =  2*sqrt(2)*diskRadius*(gamma_posTop/gamma_posOpp) - cornerLength;    
			
			const double a = omega*omega + cornerLength*cornerLength;
			const double b = 2*rho*omega;
			const double c = rho*rho - cornerLength*cornerLength;
			
			///Calculate cosPhi
			const double discriminant = b*b - 4*a*c;
			
			//std::cout << "\nDiscriminant: " << discriminant << std::endl;
			if (discriminant >= 0) {
				
				///cosPhi has at least one root
				///Quadratic formula
				const double root1 = (-b + sqrt(discriminant) )/(2*a);
				const double root2 = (-b - sqrt(discriminant) )/(2*a);
				
				///Draw cosPhi function
				//TCanvas *c1 = new TCanvas("c1", "c1", 1000, 800);
				TF1* cosPhi_pos = new TF1("cosPhi_pos", cosPhi_posFunction, -1, 1, 3);
				///Set constants
				cosPhi_pos->SetParameter(0, a);
				cosPhi_pos->SetParameter(1, b);
				cosPhi_pos->SetParameter(2, c);
				
				///If both roots lie outside boundaries (0 and 1), no real solution
				if ((root1 < 0 && root2 > 1) || (root2 < 0 && root1 > 1)) {
					std::cout << "ERROR. No solution for position!\n\n";
				} else {
					///If both roots lie inside boundaries, no unique solution
					if (root1 >= 0 && root1 <= 1 && root2 >= 0 && root2 <= 1 && root1 != root2) {
						std::cout << "ERROR. No unique solution!\n\n";
					} else {
						///Found physical root (either one or repeated roots)
						double cosPhi_posTemp;
						
						if (root1 >= 0 && root1 <= 1) {
							cosPhi_posTemp = root1;
						} else {
							cosPhi_posTemp = root2;
						}
						///Got cosPhi_pos!!
						const double cosPhi_pos = cosPhi_posTemp;
						//std::cout << "cosPhi_i: " << cosPhi_i << "\n";
						
						///Calculate r_pos
						const double r_pos = gamma_posOpp*(1/(4*diskRadius*cosPhi_pos + 2*attenuationLength*g_posOpp));
						
						///Calculate r
						const double r = sqrt(diskRadius*diskRadius + r_pos*r_pos - 2*r_pos*diskRadius*cosPhi_pos);
						//std::cout << "r: " << r/diskRadius << " diskRadii\n\n";
						
						///Calculate coordinates relative to line to position
						double xPrime = diskRadius - r_pos*cosPhi_pos;
						double yPrime;
						///Check sign of yPrime
						if (I_Top >= I_Bot) {
							yPrime = r_pos*sqrt(1 - cosPhi_pos*cosPhi_pos);
						} else {
							yPrime = -r_pos*sqrt(1 - cosPhi_pos*cosPhi_pos);
						}
						
						///Rotate to true coordinate system
						const double x = xPrime*TMath::Cos(beta) - yPrime*TMath::Sin(beta);
						const double y = xPrime*TMath::Sin(beta) + yPrime*TMath::Cos(beta);
						//std::cout << "X: " << x << std::endl;
						//std::cout << "Y: " << y << std::endl;
						
						//std::cout << "r again: " << sqrt(x*x + y*y)/diskRadius << " diskRadii" << std::endl;
						
						///Find theta
						double thetaTemp = TMath::ATan(abs(y)/abs(x));
						if (x >= 0 && y >= 0) thetaTemp = thetaTemp;                 ///1st quadrant
						if (x <  0 && y >= 0) thetaTemp = TMath::Pi() - thetaTemp;   ///2nd quadrant
						if (x <  0 && y <  0) thetaTemp = TMath::Pi() + thetaTemp;   ///3rd quadrant
						if (x >= 0 && y <  0) thetaTemp = 2*TMath::Pi() - thetaTemp; ///4th quadrant
						
						const double theta = thetaTemp;
						
						//if (foundTheta) {
							//const double theta = thetaTemp;
							recoPoints.push_back(std::make_pair(r, theta));
							std::cout << "r: " << r/diskRadius << " diskRadii\n";
							std::cout << "Theta: " << theta*(180/TMath::Pi()) << " degrees" << std::endl;
						//}
					}
				}
			} else {
				std::cout << "\nERROR. Could not find roots for r5!\n";
			}
		}
		
	}
	
	TF2* diskF = new TF2("diskF", disk, -2*diskRadius, 2*diskRadius, -2*diskRadius, 2*diskRadius, 0);
	TCanvas *c2 = new TCanvas("c2", "c2", 1500, 1500);
	diskF->SetContour(1);
	diskF->SetContourLevel(0, diskRadius*diskRadius);
	diskF->SetLineColor(1);
	diskF->SetLineWidth(4);
	diskF->GetXaxis()->SetTitle("x/cm");
	diskF->GetYaxis()->SetTitle("y/cm");
	diskF->SetTitle("Reconstructed Position of Point Source");
	diskF->Draw();
		
	int colors[recoPoints.size()];
	for (int i = 0; i < recoPoints.size(); i++) {
		colors[i] = i + 1;
	}
	int i = 0;
	///Draw reconstructed points
	for (auto it = recoPoints.begin(); it != recoPoints.end(); it++) {
		double r = it->first;
		double theta = it->second;
		
		TMarker* point = new TMarker(r*TMath::Cos(theta), r*TMath::Sin(theta), 3);
		point->SetMarkerColor(1);
		point->SetMarkerSize(2);
		point->Draw();
		
		i++;
		
	}
		
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
	
	///List values at positions
	/*for (int pos = 0; pos < nPositions; pos++){
		double x = diskRadius*TMath::Cos(pos*TMath::Pi()/4);
		double y = diskRadius*TMath::Sin(pos*TMath::Pi()/4);
		std::string value = to_string(data[pos]) + " V";
		
		TText *label = new TText();
		//label->SetNDC();
		label->SetTextFont(1);
		label->SetTextColor(1);
		label->SetTextSize(0.03);
		//label->SetTextAlign(22);
		//label->SetTextAngle(0);
		label->DrawText(x + 0.1*x, y + 0.1*y, value.c_str());
	}*/

	std::cout << "\n\n";

	std::cout << "Number of reconstructed points: " << recoPoints.size() << "\n" << std::endl;

	///Compute average position
	double rSum = 0;
	//double rSum2 = 0;
	int n = 1;
	double thetaSum = 0;
	//double thetaSum2 = 0;
	
	for (auto it = recoPoints.begin(); it != recoPoints.end(); it++) {
		rSum = rSum + it->first;
		thetaSum = thetaSum + it->second;
		n++;
	}
	
	const double rAvg     = rSum/recoPoints.size();
	const double thetaAvg = thetaSum/recoPoints.size();
	//const double rAvg2     = (2*rSum2)/recoPoints.size();
	//const double thetaAvg2 = (2*thetaSum2)/recoPoints.size();
	const double X        = rAvg*TMath::Cos(thetaAvg);
	const double Y        = rAvg*TMath::Sin(thetaAvg);
        std::cout << rAvg << " " << rSum << std::endl;	
	std::cout << "Average position is: \n";
	std::cout << "R (SiPM 1)     --> " << rAvg/diskRadius << " diskRadii" << std::endl;
	std::cout << "Theta (SiPM 1) --> " << thetaAvg*(180/TMath::Pi()) << " degrees" << std::endl;
	//std::cout << "R (SiPM 2)     --> " << rAvg2/diskRadius << " diskRadii" << std::endl;
	//std::cout << "Theta (SiPM 2) --> " << thetaAvg2*(180/TMath::Pi()) << " degrees" << std::endl;
	
	/*///Compute standard deviation in (X, Y)
	double devSum = 0;
	for (auto it = recoPoints.begin(); it != recoPoints.end(); it++) {
		devSum = devSum + it->first
	}*/
	
	std::cout << "\n\n";
	
	///Draw average point
	TMarker* avgPoint = new TMarker(X, Y, 8);
	avgPoint->SetMarkerColor(2);
	avgPoint->SetMarkerSize(2);
	avgPoint->Draw("same");
	
	///Draw true point
	TMarker* truePoint = new TMarker(0.71*diskRadius*TMath::Cos(249*(TMath::Pi()/180)), 0.71*diskRadius*TMath::Sin(249*(TMath::Pi()/180)), 5);
	truePoint->SetMarkerColor(6);
	truePoint->SetMarkerSize(3);
	truePoint->Draw("same");
	
	std::cout << "R was measured to be: 0.71 diskradii\n";
	std::cout << "Theta was measured to be: 249 degrees\n\n";
	
	///Draw legend
	TLegend* leg = new TLegend(0.1,0.7,0.48,0.9);
	leg->AddEntry(truePoint, "True Point", "p");
	leg->AddEntry(avgPoint, "Average Point", "p");
	leg->Draw();
	 
}

