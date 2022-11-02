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


int GrainMatching_calc4aff()
{
    string basepath = "../GrainMatching_distX_100um";
    string csvpath_s = basepath + "/fitdata.csv";
    char* csvpath = csvpath_s.c_str();
    string pdfname = "/GrainMatching_calc4aff.pdf";
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

    int cut_pixel = 200;
    int hash_size = 400;
    int siftX = 4;
    int siftY = 4;
    int cut_side = 1;
    string savepath = "../GrainMatching_distX_100um";
    int minus = 20;
    std::ostringstream ss_minus, ss_hash_size, ss_siftX, ss_siftY;
    ss_minus << minus;
    ss_hash_size << hash_size;
    ss_siftX << (siftX + 1);
    ss_siftY << (siftY + 1);
    string s_minus = ss_minus.str();
    string s_hash_size = ss_hash_size.str();
    string s_siftX = ss_siftX.str();
    string s_siftY = ss_siftY.str();
    string hashpath = savepath + "/hashcsv_" + "minus" + s_minus + "_" + s_hash_size + "_" + s_siftX + "by" + s_siftY;


    int pixel_bottom = 1088;
    int pixel_right;
    double hash_top = 0;
    double hash_bottom = hash_size;
    double hash_left, hash_right;

    if (cut_side == 0)
    {
        pixel_right = 2048;
    }
    else
    {
        pixel_right = 2048 - cut_pixel;
    }
    double dist_x = (double)(pixel_right - hash_size) / siftX;
    double dist_y = (double)(pixel_bottom - hash_size) / siftY;

    while (hash_bottom <= pixel_bottom)
    {
        if (cut_side == 0)
        {
            hash_left = cut_pixel;
        }
        else
        {
            hash_left = 0;
        }
        hash_right = hash_left + hash_size;

        while (hash_right <= pixel_right)
        {
            std::ostringstream ss_top, ss_left;
            ss_top << (int)hash_top;
            ss_left << (int)hash_left;
            string s_top = ss_top.str();
            string s_left = ss_left.str();
            string filepath = hashpath + "/top_" + s_top + "left_" + s_left + ".csv";
            cout << filepath << endl;

            char* file = filepath.c_str();
            TTree *t = new TTree("t", "tree using ReadFile()");
            t->ReadFile(file , "distX/D:distY/D:flag/I");

            TF1 *func1 = new TF1("func1","gaus", -159.2, -157.2);
            TF1 *func2 = new TF1("func2","gaus", -1, 1);
            c1->Divide(2, 1);
            c1->cd(1);
            // string s_titleX = "distX " + s_a + " vs " + s_b;
            // char* titleX = s_titleX.c_str();
            TH1D * distX= new TH1D("distX", "distX", 30, -159.7, -156.7);
            gStyle->SetOptStat("e");
            t->Project("distX", "distX");
            distX->Draw();
            distX->Fit("func1");
            gStyle->SetOptFit(1111);
            fitdata << (float)func1->GetParameter(2) << "," << (float)func1->GetParameter(1) << ",";

            c1->cd(2);
            // string s_titleY = "distY " + s_a + " vs " + s_b;
            // char* titleY = s_titleY.c_str();
            TH1D * distY = new TH1D("distY", "distY", 30, -1.5, 1.5);
            gStyle->SetOptStat("e");
            t->Project("distY", "distY");
            distY->Draw()
            distY->Fit("func2");
            gStyle->SetOptFit(1111);
            fitdata << (float)func2->GetParameter(2) << "," << (float)func2->GetParameter(1) << endl;

            c1->Print(fname);
            c1->Clear();

            hash_left += dist_x;
            hash_right += dist_x;
        }
        hash_top += dist_y;
        hash_bottom += dist_y;
    }
    fname = pdfpathE.c_str();
    c1->Print(fname, "pdf");

}
