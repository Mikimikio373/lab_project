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

void logon();

int cut_fit_csv(int shift_X, int shift_Y){

	logon();

    string basepath = "GrainMatching_loop";
	string csvpath_s = basepath + "/fitdata.csv";
	char* csvpath = csvpath_s.c_str();
	string pdfname = "/cut_fit.pdf";
	string pdfpathS = basepath + pdfname + "[";
	string pdfpath = basepath + pdfname;
	string pdfpathE = basepath + pdfname + "]";
	ofstream fitdata(csvpath);
	fitdata << "VX,VY,Entries,meanX,sigmaX,meanY,sigmaY" << endl;
	

	TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	char* fname = pdfpathS.c_str();
	c1->Print(fname, "pdf");
	fname = pdfpath.c_str();
	cerr << shift_X << "," << shift_Y << endl;

	for (int vx = 0; vx < shift_X; vx++)
	{
		for (int vy = 0; vy < shift_Y; vy++)
		{
			if (vx==0 && vy==0){continue;} //動かしていないものはスキップ

			cerr << vx << "," << vy << endl;
			std::ostringstream ss_vx, ss_vy;
			ss_vx << std::setw(4) << std::setfill('0') << vx;
			ss_vy << std::setw(4) << std::setfill('0') << vy;
			string s_vx(ss_vx.str());
			string s_vy(ss_vy.str());
			cerr << s_vx << "," << s_vy << endl;

			//ファイル読み込み
			string filepath_s = basepath + "/dist_00000000vs" + s_vx + s_vy + ".csv";
			cerr << filepath_s << endl;
			char* filepath = filepath_s.c_str();
			TTree *t = new TTree("t","t");
			t->ReadFile(filepath, "distx/D:disty/D:flag/I");
			

			//全体像描写
			TH2D * dxy = new TH2D("dxy", "dxy", 200, -2048, 2048, 100, -1088, 1088);
			string title_s = "VX0001_VY0000 vs VX" + s_vx + "_VY"+ s_vy;
			char* title = title_s.c_str();
			gStyle->SetOptStat("e");
			dxy->SetTitle(title);
			dxy->GetXaxis()->SetTitle("distance x [pixel]");
			dxy->GetYaxis()->SetTitle("distance y [pixel]");
			t->Draw("disty:distx>>dxy", "flag==0", "colz");
			c1->Print(fname);
			c1->Clear();

			//最大値の場所取得、再描写
			int m, a, b, c;
			m = dxy->GetMaximumBin();
			dxy->GetBinXYZ(m, a, b, c);
			// cerr << m << a << b << c << endl;
			double width, height, left, right, top, bottom;
			width = dxy->GetXaxis()->GetBinWidth(0);
			height = dxy->GetYaxis()->GetBinWidth(0);
			left = dxy->GetXaxis()->GetBinLowEdge(a) - width;
			right = dxy->GetXaxis()->GetBinLowEdge(a) + 2 * width;
			bottom = dxy->GetYaxis()->GetBinLowEdge(b) - height;
			top = dxy->GetYaxis()->GetBinLowEdge(b) + 2 * height;
			char cut1[100];
			sprintf(cut1, "distx>%f && distx<%f && disty>%f && disty<%f && flag==0", left, right, bottom, top);
			cerr << cut1 << endl;
			TCut tcut1 = cut1;
			TH2D * dxy_cut1 = new TH2D("dxy_cut1", "dxy_cut1", 100, left, right, 100, bottom, top);
			gStyle->SetOptStat("e");
			dxy_cut1->SetTitle(title);
			dxy_cut1->GetXaxis()->SetTitle("distance x [pixel]");
			dxy_cut1->GetYaxis()->SetTitle("distance y [pixel]");
			t->Draw("disty:distx >> dxy_cut1", tcut1, "colz");
			c1->Print(fname);
			c1->Clear();

			//さらに拡大して、再描画
			m = dxy_cut1->GetMaximumBin();
			dxy_cut1->GetBinXYZ(m, a, b, c);
			width = dxy_cut1->GetXaxis()->GetBinWidth(0);
			height = dxy_cut1->GetYaxis()->GetBinWidth(0);
			left = dxy_cut1->GetXaxis()->GetBinLowEdge(a) - 5;
			right = dxy_cut1->GetXaxis()->GetBinLowEdge(a) + 5;
			bottom = dxy_cut1->GetYaxis()->GetBinLowEdge(b) - 5;
			top = dxy_cut1->GetYaxis()->GetBinLowEdge(b) + 5;
			char cut2[100];
			sprintf(cut2, "distx>%f && distx<%f && disty>%f && disty<%f && flag==0", left, right, bottom, top);
			cerr << cut2 << endl;
			TCut tcut2 = cut2;
			TH2D * dxy_cut2 = new TH2D("dxy_cut2", "dxy_cut2", 100, left, right, 100, bottom, top);
			dxy_cut2->SetTitle(title);
			dxy_cut2->GetXaxis()->SetTitle("distance x [pixel]");
			dxy_cut2->GetYaxis()->SetTitle("distance y [pixel]");
			t->Draw("disty:distx >> dxy_cut2", tcut2, "colz");
			c1->Print(fname);
			c1->Clear();

			//meanの取得
			double meanX_tmp = dxy_cut2->ProjectionX()->GetMean();
			double meanY_tmp = dxy_cut2->ProjectionY()->GetMean();
			left = meanX_tmp - 2.5;
			right = meanX_tmp + 2.5;
			bottom = meanY_tmp - 2.5;
			top = meanY_tmp + 2.5;

			char cut3[100];
			sprintf(cut3, "distx>%f && distx<%f && disty>%f && disty<%f && flag==0", left, right, bottom, top);
			cerr << cut3 << endl;
			TCut tcut3 = cut3;

			TF1 * f1 = new TF1("f1","gaus(0)+pol0(3)"); //fitting function
			f1->SetParName(0,"hight");
			f1->SetParName(1,"center");
			f1->SetParName(2,"sigma");
			f1->SetParName(3,"constant");

			double sigmaX, sigmaY, meanX, meanY;
			int entries;

			//ターゲット描写
			TH2D * dxy_cut3 = new TH2D("dxy_cut3", "dxy_cut3", 70, left, right, 70, bottom, top);
			dxy_cut3->SetTitle(title);
			dxy_cut3->GetXaxis()->SetTitle("distance x [pixel]");
			dxy_cut3->GetYaxis()->SetTitle("distance y [pixel]");
			t->Draw("disty:distx >> dxy_cut3", tcut2, "colz");
			entries = dxy_cut3->GetEntries();
			c1->Clear();

			//plot & fit
			gStyle->SetOptFit(1111);
			c1->Divide(2,2);
			c1->SetRightMargin(0.2);
			c1->SetLeftMargin(0.2);
			c1->cd(1);
			dxy_cut3->Draw("colz");
			c1->Update();

			c1->cd(2);
			gStyle->SetOptStat("eou");
			c1->GetPad(2)->SetRightMargin(0.3);
			dxy_cut3->SetFillColor(kBlue);
			dxy_cut3->ProjectionY()->Draw("bar");
			f1->SetParameter(0,dxy_cut3->ProjectionY()->GetMaximum());
			f1->SetParameter(1,dxy_cut3->ProjectionY()->GetMean());
			f1->SetParameter(2,dxy_cut3->ProjectionY()->GetRMS());
			dxy_cut3->ProjectionY()->Fit(f1,"Q","",bottom,top);
			meanY = f1->GetParameter(1);
			sigmaY = f1->GetParameter(2);
			c1->Update();

			c1->cd(3);
			c1->GetPad(3)->SetRightMargin(0.2);
			c1->GetPad(3)->SetLeftMargin(0.2);
			dxy_cut3->SetFillColor(kRed);
			dxy_cut3->ProjectionX()->Draw("bar");
			f1->SetParameter(0,dxy_cut3->ProjectionX()->GetMaximum());
			f1->SetParameter(1,dxy_cut3->ProjectionX()->GetMean());
			f1->SetParameter(2,dxy_cut3->ProjectionX()->GetRMS());
			dxy_cut3->ProjectionX()->Fit(f1,"Q","",left,right);
			meanX = f1->GetParameter(1);
			sigmaX = f1->GetParameter(2);
			c1->Update();

			c1->Print(fname);
			c1->Clear();

			fitdata << vx << "," << vy  << "," << entries << "," << meanX << "," << sigmaX << "," << meanY << "," << sigmaY << endl;

			string rootfilepath_s = basepath + "/dist_00000000vs" + s_vx + s_vy + ".root";
			char* rootfilepath = rootfilepath_s.c_str();
			t->SaveAs(rootfilepath);
			delete t;
			// delete f;
			delete dxy;
			delete dxy_cut1;
			delete dxy_cut2;
			delete dxy_cut3;
			delete f1;
		}
	}

	fname = pdfpathE.c_str();
	c1->Print(fname, "pdf");
}

void logon()
{
	gROOT->SetStyle("Plain");
	gStyle->SetTitleBorderSize(0);
	gStyle->SetFrameFillColor(0);
	gStyle->SetCanvasColor(0);
	gStyle->SetPadBorderSize(0);
	gStyle->SetTitleAlign(22);

	gStyle->SetTitleX(0.5);
	gStyle->SetTitleY(0.95);
	gStyle->SetPadTickX(1);
	gStyle->SetPadTickY(1);
	Int_t fontid=22;
	gStyle->SetStatFont(fontid);
	gStyle->SetLabelFont(fontid, "XYZ");
	gStyle->SetLabelFont(fontid, "");
	gStyle->SetTitleFont(fontid, "XYZ");
	gStyle->SetTitleFont(fontid, "");
	gStyle->SetTitleOffset(1.2, "XYZ");
	gStyle->SetTextFont(fontid);
	gStyle->SetFuncWidth(2);
	gStyle->SetLegendBorderSize(0);
	gStyle->SetPalette(1);

	gStyle->SetGridColor(17);
	gStyle->SetPadGridX(1);
	gStyle->SetPadGridY(1);
	gStyle->SetMarkerColor(2);
	gStyle->SetHistFillColor(5);
}