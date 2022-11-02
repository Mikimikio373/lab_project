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
using std::string;


int GrainMatching_calc()
{
    string basepath = "../GrainMatching_CutSise2_VY1";
    string csvpath_s = basepath + "/fitdata.csv";
    char* csvpath = csvpath_s.c_str();
    string pdfname = "/GrainMatching_calc.pdf";
    string pdfpathS = basepath + pdfname + "[";
    string pdfpath = basepath + pdfname;
    string pdfpathE = basepath + pdfname + "]";
    ofstream fitdata(csvpath);
    fitdata << "sigmaX,meanX,sigmaY,meanY" << endl;

    TCanvas * c1 = new TCanvas("c1");
    c1->cd();
    char* fname = pdfpathS.c_str();
    c1->Print(fname,"pdf");  //Open pdf file
    fname = pdfpath.c_str();

    for (int a = 0; a < 1; a++)
    {
        for (int b = a + 1; b < 95; b++)
        {
            std::ostringstream ss_a, ss_b;
            ss_a << std::setw(3) << std::setfill('0') << a;
            string s_a(ss_a.str());
            ss_b << std::setw(3) << std::setfill('0') << b;
            string s_b(ss_b.str());

            string filepath = basepath + "/dist_" + s_a + "_" + s_b + "_flg0.csv";
            char* file = filepath.c_str();
            TTree *t = new TTree("t", "tree using ReadFile()");
            t->ReadFile(file , "distX/D:distY/D:flag/I");

            TF1 *func = new TF1("func","gaus", -2, 2);
            c1->Divide(2, 1);
            c1->cd(1);
            // string s_titleX = "distX " + s_a + " vs " + s_b;
            // char* titleX = s_titleX.c_str();
            TH1D * distX = new TH1D("X", "distX", 100, -2, 2);
            gStyle->SetOptStat("e");
            t->Draw("distX >> h1");
            h1->Fit("func");
            gStyle->SetOptFit(1111);
            fitdata << (float)func->GetParameter(2) << "," << (float)func->GetParameter(1) << ",";

            c1->cd(2);
            // string s_titleY = "distY " + s_a + " vs " + s_b;
            // char* titleY = s_titleY.c_str();
            TH1D * distY = new TH1D("Y", "distY", 100, -2, 2);
            gStyle->SetOptStat("e");
            t->Draw("distY >> h2");
            h2->Fit("func");
            gStyle->SetOptFit(1111);
            fitdata << (float)func->GetParameter(2) << "," << (float)func->GetParameter(1) << endl;

            c1->Print(fname);
            c1->Clear();

            
        }
    }
    fname = pdfpathE.c_str();
    c1->Print(fname, "pdf");
}