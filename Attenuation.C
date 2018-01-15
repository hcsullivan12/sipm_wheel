void Attenuation() {

	TFile *f = new TFile("./Results/data.root", "r");

	TGraph *g1 = nullptr;
	TGraph *g2 = nullptr;
	TGraph *g3 = nullptr;
	TGraph *g4 = nullptr;
	TGraph *g5 = nullptr;
	TGraph *g6 = nullptr;
	TGraph *g7 = nullptr;
	TGraph *g8 = nullptr;

	f->GetObject("Graph;1", g1);
	f->GetObject("Graph;2", g2);
	f->GetObject("Graph;3", g3);
	f->GetObject("Graph;4", g4);
	f->GetObject("Graph;5", g5);
	f->GetObject("Graph;6", g6);
	f->GetObject("Graph;7", g7);
	f->GetObject("Graph;8", g8);

	g1->SetMarkerStyle(24);
	g1->SetMarkerColor(3);
	g1->GetYaxis()->SetRangeUser(0, 1.6);

	g2->SetMarkerStyle(25);
        g2->SetMarkerColor(2);

	g3->SetMarkerStyle(26);
        g3->SetMarkerColor(4);

	g4->SetMarkerStyle(30);
        g4->SetMarkerColor(7);

	g5->SetMarkerStyle(27);
        g5->SetMarkerColor(6);

	g6->SetMarkerStyle(kOpenCircle);
        g6->SetMarkerColor(1);

	g7->SetMarkerStyle(32);
        g7->SetMarkerColor(9);

	g8->SetMarkerStyle(2);
        g8->SetMarkerColor(kOrange);

/*	Double_t px1[8] = {g1->GetX()[0], g1->GetX()[1],
				    g1->GetX()[2], g1->GetX()[3],
				    g1->GetX()[4], g1->GetX()[5],
				    g1->GetX()[6], g1->GetX()[7]};
	Double_t dx1[8] = {0.25, 0.25, 0.25, 0.25,
			   0.25, 0.25, 0.25, 0.25};
	auto g1E = new TGraphErrors(10, px1, dx1);
	auto multiG = new TMultiGraph();
	multiG->Add(g1E);
	multiG->Draw("A");*/



	TCanvas *c1 = new TCanvas("c1", "c1", 800, 600);

	g1->SetTitle("Average SiPM Peaks for 8 Positions around Disk");
	g1->GetXaxis()->SetTitle("Distance from SiPMs (cm)");
	g1->GetYaxis()->SetTitle("Average Signal Peak");

	g1->Draw("AP");
	g2->RemovePoint(0);
	g2->Draw("P");
	g3->Draw("P");
	g4->RemovePoint(6);
	g4->Draw("P");
	g5->Draw("P");
	g6->RemovePoint(7);
	g6->Draw("P");
	g7->RemovePoint(7);
	g7->Draw("P");
	g8->Draw("P");

	/*fit = new TF1("fit", "expo", 0, 9);
	g1->Fit("fit");
	cout << "Parameter is: " << 1/fit->GetParameter(1) << endl;*/

	double mu [8];
	double sig [8];

	for (auto i = 0; i < 8; i++) {
		auto p1 = 0.0;
		auto p2 = 0.0;
		auto p3 = 0.0;
		auto p4 = 0.0;
		auto p5 = 0.0;
		auto p6 = 0.0;
		auto p7 = 0.0;
		auto p8 = 0.0;
		
		p1 = g1->GetY()[i];
		if (i != 0) {
			 p2 = g2->GetY()[i];
		}
		p3 = g3->GetY()[i];
		if (i != 6) {
			p4 = g4->GetY()[i];
		}
		p5 = g5->GetY()[i];
		if (i != 7) {
			p6 = g6->GetY()[i];
		}
		if (i != 7) {
			p7 = g7->GetY()[i];
		}
		p8 = g8->GetY()[i];

		double mean;
		double stdDev;
		if (i == 0) {
			mean = (p1 + p3 + p4 + p5 + p6 + p7 + p8)/7;
			cout << p1 << " " << p4 << " " << p3 << " " << p5 << " " << p6 << " " << p7 << " " << p8 << " " << endl;
			stdDev = std::sqrt(((p1 - mean)*(p1 - mean) + (p3 - mean)*(p3 - mean) + (p4 - mean)*(p4 - mean)
				+ (p5 - mean)*(p5 - mean) + (p6 - mean)*(p6 - mean) + (p7 - mean)*(p7 - mean) +
				(p8 - mean)*(p8 - mean))/6);
		} else {
			if (i == 6) {
				mean = (p1 + p2 + p3 + p5 + p6 + p7 + p8)/7;
				stdDev = std::sqrt(((p1 - mean)*(p1 - mean) + (p3 - mean)*(p3 - mean) + (p2 - mean)*(p2 - mean)
                                 + (p5 - mean)*(p5 - mean) + (p6 - mean)*(p6 - mean) + (p7 - mean)*(p7 - mean) +
                                (p8 - mean)*(p8 - mean))/6);
			} else {
				if (i == 7) {
					mean = (p1 + p2 + p3 + p4 + p5 + p8)/7;
					cout << "i=7 mean: " << mean << endl;
					stdDev = std::sqrt(((p1 - mean)*(p1 - mean) + (p2 - mean)*(p2 - mean) + (p4 - mean)*(p4 - mean)
                                		+ (p5 - mean)*(p5 - mean) + (p3 - mean)*(p3 - mean) +
                                		(p8 - mean)*(p8 - mean))/6);
				} else { 
					mean = (p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8)/8;
					cout << "i=" << i << " mean: " << mean << endl;
					stdDev = std::sqrt(((p1 - mean)*(p1 - mean) + (p2 - mean)*(p2 - mean) + 
						(p3 - mean)*(p3 - mean) + (p4 - mean)*(p4 - mean)
                               			 + (p5 - mean)*(p5 - mean) + (p6 - mean)*(p6 - mean) + (p7 - mean)*(p7 - mean) +
                               			 (p8 - mean)*(p8 - mean))/7);
	     			}
			}
		}
	
		mu[i] = mean;
		sig[i] = stdDev;
	}
	
	TCanvas *c2 = new TCanvas("c2", "c2", 800, 600);
	
	const Int_t n = 8;
	double x[n] = {1, 2, 3, 4, 5, 6, 7, 8};
	double y[n] = {mu[0], mu[1], mu[2], mu[3], mu[4], mu[5], mu[6], mu[7]};
	double ex[n] = {0, 0, 0, 0, 0, 0, 0, 0};
	double ey[n] = {sig[0], sig[1], sig[2], sig[3], sig[4], sig[5], sig[6], sig[7]};
	TGraphErrors *gr = new TGraphErrors(n, x, y, ex, ey);
	gr->SetTitle("SiPM Peaks Around Disk");
	gr->SetMarkerColor(1);
	gr->SetMarkerStyle(21);
	gr->GetXaxis()->SetTitle("Distance from SiPMs (cm)");
	gr->GetYaxis()->SetTitle("Average Signal Peak (V)");
	
	gr->Draw("AP");
	
	c2->Update();
	
	TF1 *fit = new TF1("fit", "[0]*TMath::Exp([1]*x)/x", 0.5, 9);
	gr->Fit("fit");
	cout << "Constant parameter 0 is: " << fit->GetParameter(0) << endl;
	cout << "Attenuation length is: " << -1/fit->GetParameter(1) << endl;
	//cout << "Constant parameter 2 is: " << fit->GetParameter(2) << endl;
	
	string value = to_string(1/fit->GetParameter(1));
	TLatex latex;
	latex.DrawLatex(6,1,"#mu = 2.4 cm");
	
	
	/*TText *t = new TText(6,1, "#varphi");
	t->SetTextAlign(22);
	t->SetTextColor(kRed+2);
	t->SetTextFont(43);
	t->SetTextSize(40);
	t->Draw();*/
	
}
