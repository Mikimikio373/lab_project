#include <TROOT.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TStyle.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
using std::string;

void plot_fitting(){
    string basepath = "GrainMatching_loop";
    string pdfname = "/plot_fitting.pdf";
	string pdfpathS = basepath + pdfname + "[";
	string pdfpath = basepath + pdfname;
	string pdfpathE = basepath + pdfname + "]";

    TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	char* fname = pdfpathS.c_str();
	c1->Print(fname, "pdf");
	fname = pdfpath.c_str();

    TTree * t = new TTree("t","t");
    string fitdatapath = basepath + "/fitdata_edit.csv";
    char* dataname = fitdatapath.c_str();
    t->ReadFile(dataname,"n:vx:vy:entries:px:sigmapx:py:sigmapy:dX:dY:dx:dy");

    Long64_t t_vxy = t->Draw("vx:vy","","goff");
    TGraph * vxy = new TGraph(t_vxy, t->GetV1(), t->GetV2());
    vxy->SetMarkerStyle(kCircle);
    vxy->SetMarkerSize(0.5);
    vxy->SetTitle("vx vy");
    vxy->GetXaxis()->SetTitle("vx (step)");
    vxy->GetYaxis()->SetTitle("vy (step)");
    vxy->Draw("AP");
    c1->Print(fname);
    c1->Clear();

    Long64_t t_stagexy = t->Draw("dX:dY","","goff");
    TGraph * stagexy = new TGraph(t_stagexy, t->GetV1(), t->GetV2());
    stagexy->SetMarkerStyle(kCircle);
    stagexy->SetMarkerSize(0.5);
    stagexy->SetMarkerColor(kOrange);
    stagexy->SetTitle("stage dX dY");
    stagexy->GetXaxis()->SetTitle("dX [mm]");
    stagexy->GetYaxis()->SetTitle("dY [mm]");
    stagexy->Draw("AP");
    c1->Print(fname);
    c1->Clear();

    Long64_t t_pxy = t->Draw("px:py","","goff");
    TGraph * pxy = new TGraph(t_pxy, t->GetV1(), t->GetV2());
    pxy->SetMarkerStyle(kCircle);
    pxy->SetMarkerSize(0.5);
    pxy->SetMarkerColor(kSpring);
    pxy->SetTitle("pixel dx dy");
    pxy->GetXaxis()->SetTitle("dx [pixel]");
    pxy->GetYaxis()->SetTitle("dy [pixel]");
    pxy->Draw("AP");
    c1->Print(fname);
    c1->Clear();

    Long64_t t_x = t->Draw("dX:px:dx","","goff");
    TGraphErrors * dxpx = new TGraphErrors(t_x,t->GetV1(),t->GetV2(),0,t->GetV3());
    dxpx->SetMarkerStyle(kCircle);
    dxpx->SetMarkerSize(1);
    dxpx->SetLineColor(kBlue);
    dxpx->SetMarkerColor(kBlue);
    dxpx->SetTitle("distance x (stage X vs pixel x)");
    dxpx->GetXaxis()->SetTitle("distance stage X [mm]");
    dxpx->GetYaxis()->SetTitle("distance pixel x [pixel]");
    dxpx->SetPoint(0,0,0.1);
    dxpx->Draw("AP");
    c1->Print(fname);
    c1->Clear();

    Long64_t t_y = t->Draw("dY:py:dy","","goff");
    TGraphErrors * dypy = new TGraphErrors(t_y,t->GetV1(),t->GetV2(),0,t->GetV3());
    dypy->SetMarkerStyle(kCircle);
    dypy->SetMarkerSize(1);
    dypy->SetLineColor(kRed);
    dypy->SetMarkerColor(kRed);
    dypy->SetTitle("distance y (stage Y vs pixel y)");
    dypy->GetXaxis()->SetTitle("distance stage Y [mm]");
    dypy->GetYaxis()->SetTitle("distance pixel y [pixel]");
    dypy->Draw("AP");
    dypy->SetPoint(0,0,0.1);
    c1->Print(fname);
    c1->Clear();


    fname = pdfpathE.c_str();
	c1->Print(fname, "pdf");

    t->SaveAs("GrainMatching_loop/fitdata.root");


}