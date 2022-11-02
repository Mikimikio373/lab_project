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

void plot_viv(int npicture){
    char pdfs[20] = "plot_viv.pdf[";
    char pdf[20] = "plot_viv.pdf";
    char pdfe[20] = "plot_viv.pdf]";
    TCanvas * c1 = new TCanvas("c1");
    c1->cd();
    c1->Print(pdfs, "pdf");
    gStyle->SetOptStat("e");
    gStyle->SetOptFit(1111);
    gStyle->SetFuncColor(kBlack);

    char outfile[20] = "fit_vivcenter.csv";
    ofstream fitdata(outfile);
    fitdata << "line,num,dx,dy,sigmaX,sigmaY" << std::endl;

    for (int i = 1; i < npicture; i++){
        int entries;
        double meanX, meanY, sigmaX, sigmaY;
        char read_csv[20];
        sprintf(read_csv, "dist_000_%03d.csv", i);
       
        
        TTree * t = new TTree("t","t");
        t->ReadFile(read_csv, "dx:dy:flag");

        TH2D *dpos = new TH2D("dpos", "dpos", 100, -1., 1., 100, -1., 1.);
        t->Draw("dy:dx>>dpos", "flag==0&&fabs(dx)<=1&&fabs(dy)<=1", "colz");
        // c1->Print(pdf, "pdf");
        c1->Clear();

        TF1 * f1 = new TF1("f1","gaus(0)"); //fitting function
        f1->SetParName(0,"hight");
        f1->SetParName(1,"center");
        f1->SetParName(2,"sigma");

        c1->Divide(2,2);
        c1->cd(1);
        dpos->Draw("colz");
        c1->Update();

        c1->cd(2);
        dpos->SetFillColor(kBlue);
        dpos->ProjectionY()->Draw("bar");
        f1->SetParameter(0,dpos->ProjectionY()->GetMaximum());
        f1->SetParameter(1,dpos->ProjectionY()->GetMean());
        f1->SetParameter(2,dpos->ProjectionY()->GetRMS());
        dpos->ProjectionY()->Fit(f1,"Q","",-1,1);
        meanY = f1->GetParameter(1);
        sigmaY = f1->GetParameter(2);
        c1->Update();

        c1->cd(3);
        dpos->SetFillColor(kRed);
        dpos->ProjectionX()->Draw("bar");
        f1->SetParameter(0,dpos->ProjectionX()->GetMaximum());
        f1->SetParameter(1,dpos->ProjectionX()->GetMean());
        f1->SetParameter(2,dpos->ProjectionX()->GetRMS());
        dpos->ProjectionX()->Fit(f1,"Q","",-1,1);
        meanX = f1->GetParameter(1);
        sigmaX = f1->GetParameter(2);
        c1->Update();

        c1->Print(pdf, "pdf");
        c1->Clear();
        entries = dpos->GetEntries();
        fitdata << i << "," << entries << "," << meanX << "," << meanY << "," << sigmaX << "," << sigmaY << std::endl;

        char write_root[20];
        sprintf(write_root, "dist_000_%03d.root", i);

        delete t;
        delete dpos;
        delete f1;
    }
    c1->Print(pdfe, "pdf");
}