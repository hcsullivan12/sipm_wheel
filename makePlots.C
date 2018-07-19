void makePlots() {

   TCanvas *overvoltage_plot = new TCanvas("overvoltage_plot", "Overvoltage Plot",61,24,1859,1056);
   overvoltage_plot->Range(-0.5,-0.09375001,4.5,0.84375);
   overvoltage_plot->SetFillColor(0);
   overvoltage_plot->SetBorderMode(0);
   overvoltage_plot->SetBorderSize(2);
   overvoltage_plot->SetFrameBorderMode(0);
   overvoltage_plot->SetFrameBorderMode(0);

   Double_t Graph0_fx1[5] = {
   0.7865322,
   1.186532,
   1.386532,
   1.986532,
   2.186532};
   Double_t Graph0_fy1[5] = {
   0.2269156,
   0.2699164,
   0.3368669,
   0.5021671,
   0.5651131};
   TGraph *graph1 = new TGraph(5,Graph0_fx1,Graph0_fy1);
   graph1->SetName("Graph1");
   graph1->SetTitle("SiPM Gain");
   graph1->SetMarkerColor(1);
   graph1->SetMarkerStyle(23);
   graph1->SetMarkerSize(2);
   graph1->SetMinimum(0);
   graph1->SetMaximum(.75);
   graph1->GetXaxis()->SetLimits(0,4);
   graph1->GetXaxis()->SetTitle("Over-voltage (#DeltaV)");
   graph1->GetYaxis()->SetTitle("Gain (mV/p.e.)");

   Double_t Graph0_fx4[6] = {
   0.936547,
   1.536547,
   1.736547,
   1.936547,
   2.136547,
   2.336547};
   Double_t Graph0_fy4[6] = {
   0.2344894,
   0.3171896,
   0.3652346,
   0.4205365,
   0.4929735,
   0.5401111};
   TGraph *graph2 = new TGraph(6,Graph0_fx4,Graph0_fy4);
   graph2->SetName("Graph2");
   graph2->SetTitle("SiPM Gain");
   graph2->SetMarkerColor(2);
   graph2->SetMarkerStyle(21);
   graph2->SetMarkerSize(2);

  Double_t Graph0_fx7[6] = {
   0.7471518,
   1.347152,
   1.547152,
   1.947152,
   2.147152,
   2.147152};
   Double_t Graph0_fy7[6] = {
   0.1880867,
   0.3646651,
   0.440375,
   0.5022719,
   0.5455511,
   0.6002692};
   TGraph *graph3 = new TGraph(6,Graph0_fx7,Graph0_fy7);
   graph3->SetName("Graph3");
   graph3->SetTitle("SiPM Gain");
   graph3->SetMarkerColor(3);
   graph3->SetMarkerStyle(20);
   graph3->SetMarkerSize(2);

  Double_t Graph0_fx10[7] = {
   0.9596193,
   1.159619,
   1.359619,
   1.559619,
   1.759619,
   1.959619,
   2.159619};
   Double_t Graph0_fy10[7] = {
   0.2181666,
   0.3382162,
   0.3675505,
   0.3845514,
   0.439335,
   0.4853337,
   0.5765723};
   TGraph *graph4 = new TGraph(7,Graph0_fx10,Graph0_fy10);
   graph4->SetName("Graph4");
   graph4->SetTitle("SiPM Gain");
   graph4->SetMarkerColor(4);
   graph4->SetMarkerStyle(29);
   graph4->SetMarkerSize(2);

   TLegend *legend = new TLegend(0.1,0.7,0.48,0.9);
   legend->SetHeader("SiPMs","C"); 
   legend->AddEntry(graph1,"SiPM 1","p");
   legend->AddEntry(graph2,"SiPM 2","p");
   legend->AddEntry(graph3,"SiPM 3","p");
   legend->AddEntry(graph4,"SiPM 4","p");

   graph1->Draw("AP");
   graph2->Draw("same P");
   graph3->Draw("same P");
   graph4->Draw("same P");
   legend->Draw("same");

}
