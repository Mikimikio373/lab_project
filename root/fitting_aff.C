void fitting_aff(){

    // string csvpath_s = basepath + "fitdata_aff.csv";
	// char* csvpath = csvpath_s.c_str();
    // ofstream fitdata(csvpath);

    string csvpath_s = "aff_fitdata.csv";
	char* csvpath = csvpath_s.c_str();
    ofstream fitdata(csvpath);
	fitdata << "dxdX,dxdY,dydX,dydY" << endl;
    char fname[100];
    sprintf(fname,"fitting_aff.pdf");

    TCanvas * c1 = new TCanvas("c1");
    c1->cd();
    sprintf(fname,"fitting_aff.pdf[");
    c1->Print(fname,"pdf");  //Open pdf file
    sprintf(fname,"fitting_aff.pdf");

    gStyle->SetOptFit(1111);
    TF1 *f1 = new TF1("f1", "[0]*x");
    TGraphErrors *gr1 = new TGraphErrors("dxdX.csv", "%lg,%lg,%lg,%lg", "");
    gr1->SetTitle("dx = a' * dX");
    gr1->GetXaxis()->SetTitle("dX [mm]");
    gr1->GetYaxis()->SetTitle("dx [pixel]");
    gr1->Draw("ap");
    gr1->Fit("f1");
    float tilt1;
    tilt1 = (float)f1->GetParameter(0);

    c1->Print(fname);
    c1->Clear();

    TF1 *f2 = new TF1("f2", "[0]*x");
    TGraphErrors *gr2 = new TGraphErrors("dxdY.csv", "%lg,%lg,%lg,%lg", "");
    gr2->SetTitle("dx = b' * dY");
    gr2->GetXaxis()->SetTitle("dY [mm]");
    gr2->GetYaxis()->SetTitle("dx [pixel]");
    gr2->Draw("ap");
    gr2->Fit("f2");
    float tilt2;
    tilt2 = (float)f2->GetParameter(0);

    c1->Print(fname);
    c1->Clear();

    TF1 *f3 = new TF1("f3", "[0]*x");
    TGraphErrors *gr3 = new TGraphErrors("dydX.csv", "%lg,%lg,%lg,%lg", "");
    gr3->SetTitle("dy = c' * dX");
    gr3->GetXaxis()->SetTitle("dX [mm]");
    gr3->GetYaxis()->SetTitle("dx [pixel]");
    gr3->Draw("ap");
    gr3->Fit("f3");
    float tilt3;
    tilt3 = (float)f3->GetParameter(0);
    
    c1->Print(fname);
    c1->Clear();

    TF1 *f4 = new TF1("f4", "[0]*x");
    TGraphErrors *gr4 = new TGraphErrors("dydY.csv", "%lg,%lg,%lg,%lg", "");
    gr4->SetTitle("dy = d' * dY");
    gr4->GetXaxis()->SetTitle("dY [mm]");
    gr4->GetYaxis()->SetTitle("dy [pixel]");
    gr4->Draw("ap");
    gr4->Fit("f4");
    float tilt4;
    tilt4 = (float)f4->GetParameter(0);
    
    c1->Print(fname);
    c1->Clear();

    fitdata << tilt1 << "," << tilt2 << "," << tilt3 << "," << tilt4 << endl;
    

    //close pdf
    sprintf(fname,"fitting_aff.pdf]");

    c1->Print(fname);
}