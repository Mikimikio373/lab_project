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

int cut_fit4vib(int NPicture){
	string csvpath_s = "fitdata4vib.csv";
	char* csvpath = csvpath_s.c_str();
	string pdfname = "cut_fit4vib.pdf";
	string pdfpathS = pdfname + "[";
	string pdfpath = pdfname;
	string pdfpathE = pdfname + "]";
	ofstream fitdata(csvpath);
	fitdata << "n_picture,Entries,meanX,sigmaX,meanY,sigmaY" << endl;
	

	TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	char* fname = pdfpathS.c_str();
	c1->Print(fname, "pdf");
	fname = pdfpath.c_str();

	for (int n = 1; n < NPicture; n++)
	{

		std::ostringstream num_n;
		num_n << std::setw(3) << std::setfill('0') << n;
		string s_num_n(num_n.str());

		//ファイル読み込み
		string filepath_s = "dist_000_" + s_num_n + ".csv";
		char* filepath = filepath_s.c_str();
		TTree *t = new TTree("t", "dist");
		t->ReadFile(filepath, "distx/D:disty/D:flag/I");

		//全体像描写
		TH2D * dxy = new TH2D("dxy", "dxy", 200, -2048, 2048, 100, -1088, 1088);
		string title_s = "picture = 000 vs picture = " + s_num_n;
		char* title = title_s.c_str();
		gStyle->SetOptStat("e");
		dxy->SetTitle(title);
		dxy->GetXaxis()->SetTitle("distance x [pixel]");
		dxy->GetYaxis()->SetTitle("distance y [pixel]");
		t->Draw("disty:distx >> dxy", "flag==0", "colz");
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
		sprintf(cut1, "distx>%.2f && distx<%.2f && disty>%.2f && disty<%.2f && flag==0", left, right, bottom, top);
		cerr << cut1 << endl;
		TCut tcut1 = cut1;
		TH2D * dxy_cut1 = new TH2D("dxy_cut1", "dxy_cut1", 100, left, right, 100, bottom, top);
		title_s = title_s + " cut1";
		title = title_s.c_str();
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
		left = dxy_cut1->GetXaxis()->GetBinLowEdge(a) - 1.5;
		right = dxy_cut1->GetXaxis()->GetBinLowEdge(a) + 1.5;
		bottom = dxy_cut1->GetYaxis()->GetBinLowEdge(b) - 1.5;
		top = dxy_cut1->GetYaxis()->GetBinLowEdge(b) + 1.5;
		char cut2[100];
		sprintf(cut2, "distx>%.2f && distx<%.2f && disty>%.2f && disty<%.2f && flag==0", left, right, bottom, top);
		cerr << cut2 << endl;
		TCut tcut2 = cut2;
		TH2D * dxy_cut2 = new TH2D("dxy_cut2", "dxy_cut2", 100, left, right, 100, bottom, top);
		title_s = "picture = 000 vs picture = " + s_num_n + " cut_2";
		title = title_s.c_str();
		dxy_cut2->SetTitle(title);
		dxy_cut2->GetXaxis()->SetTitle("distance x [pixel]");
		dxy_cut2->GetYaxis()->SetTitle("distance y [pixel]");
		t->Draw("disty:distx >> dxy_cut2", tcut2, "colz");
		c1->Print(fname);
		c1->Clear();

		//さらに拡大、再描画
		m = dxy_cut2->GetMaximumBin();
		dxy_cut2->GetBinXYZ(m, a, b, c);
		width = dxy_cut2->GetXaxis()->GetBinWidth(0);
		height = dxy_cut2->GetYaxis()->GetBinWidth(0);
		left = dxy_cut2->GetXaxis()->GetBinLowEdge(a) - 0.5;
		right = dxy_cut2->GetXaxis()->GetBinLowEdge(a) + 0.5;
		bottom = dxy_cut2->GetYaxis()->GetBinLowEdge(b) - 0.5;
		top = dxy_cut2->GetYaxis()->GetBinLowEdge(b) + 0.5;
		char cut3[100];
		sprintf(cut3, "distx>%.2f && distx<%.2f && disty>%.2f && disty<%.2f && flag==0", -0.5, 0.5, -0.5, 0.5);
		cerr << cut3 << endl;
		TCut tcut3 = cut3;
		TH2D * dxy_cut3 = new TH2D("dxy_cut3", "dxy_cut3", 100, -0.5, 0.5, 100, -0.5, 0.5);
		title_s = "picture = 000 vs picture = " + s_num_n + " cut_3";
		title = title_s.c_str();
		dxy_cut3->SetTitle(title);
		dxy_cut3->GetXaxis()->SetTitle("distance x [pixel]");
		dxy_cut3->GetYaxis()->SetTitle("distance y [pixel]");
		t->Draw("disty:distx >> dxy_cut3", tcut3, "colz");
		c1->Print(fname);
		c1->Clear();

		//画面分割、フィッティング
		double sigmaX, sigmaY, meanX, meanY;
		int entries = 0;
		TF1 * func = new TF1("func", "gaus");
		func->SetLineColor(1);
		c1->Divide(2, 2);
		c1->cd(1);
		dxy_cut3->Draw("colz");
		entries = dxy_cut2->GetEntries();
		c1->Update();

		c1->cd(2);
		gStyle->SetOptFit(1111);
		dxy_cut3->SetFillColor(kBlue);
		dxy_cut3->ProjectionY()->Draw("bar");
		dxy_cut3->ProjectionY()->Fit("func");
		meanY = (float)func->GetParameter(1);
		sigmaY = (float)func->GetParameter(2);
		c1->Update();

		c1->cd(3);
		gStyle->SetOptFit(1111);
		dxy_cut3->SetFillColor(kRed);
		dxy_cut3->ProjectionX()->Draw("bar");
		dxy_cut3->ProjectionX()->Fit("func");
		meanX = (float)func->GetParameter(1);
		sigmaX = (float)func->GetParameter(2);
		c1->Update();
		c1->Print(fname);
		c1->Clear();

		fitdata << n << "," << entries << "," << meanX << "," << sigmaX << "," << meanY << "," << sigmaY << endl;
		
		string rootpath_s = "dist_000_" + s_num_n + ".root";
		char* rootpath = rootpath_s.c_str();
		t->SaveAs(rootpath);
		delete t;
		delete dxy;
		delete dxy_cut1;
		delete dxy_cut2;
		delete func;
	}
	fname = pdfpathE.c_str();
	c1->Print(fname, "pdf");
	
}
