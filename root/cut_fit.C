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

int cut_fit(int shift_X, int shift_Y){
	// int shift_X = argv[1];
	// int shift_Y = argv[2]:
    string basepath = "GrainMatching_loop";
	string csvpath_s = basepath + "/fitdata.csv";
	char* csvpath = csvpath_s.c_str();
	string pdfname = "/cut_fit.pdf";
	string pdfpathS = basepath + pdfname + "[";
	string pdfpath = basepath + pdfname;
	string pdfpathE = basepath + pdfname + "]";
	// std::vector<std::vector<double>> data;
	ofstream fitdata(csvpath);
	fitdata << "VX,VY,Entries,meanX,sigmaX,meanY,sigmaY,entriesY" << endl;
	

	TCanvas * c1 = new TCanvas("c1");
	c1->cd();
	char* fname = pdfpathS.c_str();
	c1->Print(fname, "pdf");
	fname = pdfpath.c_str();

	for (int vx = 1; vx < shift_X; vx++)
	{
		for (int vy = 0; vy < shift_Y; vy++)
		{
			if (vx==1 && vy==0){continue;} //動かしていないものはスキップ

			std::ostringstream ss_vx, ss_vy;
			ss_vx << std::setw(4) << std::setfill('0') << vx;
			ss_vy << std::setw(4) << std::setfill('0') << vy;
			string s_vx(ss_vx.str());
			string s_vy(ss_vy.str());

			//ファイル読み込み
			string filepath_s = basepath + "/dist_00010000vs" + s_vx + s_vy + ".csv";
			char* filepath = filepath_s.c_str();
			TTree *t = new TTree("t", "dist");
			t->ReadFile(filepath, "distx/D:disty/D:flag/I");

			//全体像描写
			TH2D * dxy = new TH2D("dxy", "dxy", 200, -2048, 2048, 100, -1088, 1088);
			string title_s = "VX0001_VY0000 vs VX" + s_vx + "_VY"+ s_vy;
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
			sprintf(cut1, "distx>%f && distx<%f && disty>%f && disty<%f && flag==0", left, right, bottom, top);
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
			sprintf(cut2, "distx>%f && distx<%f && disty>%f && disty<%f && flag==0", left, right, bottom, top);
			cerr << cut2 << endl;
			TCut tcut2 = cut2;
			TH2D * dxy_cut2 = new TH2D("dxy_cut2", "dxy_cut2", 100, left, right, 100, bottom, top);
			title_s = "VX0001_VY0000 vs VX" + s_vx + "_VY"+ s_vy + " cut_2";
			title = title_s.c_str();
			dxy_cut2->SetTitle(title);
			dxy_cut2->GetXaxis()->SetTitle("distance x [pixel]");
			dxy_cut2->GetYaxis()->SetTitle("distance y [pixel]");
			t->Draw("disty:distx >> dxy_cut2", tcut2, "colz");
			c1->Print(fname);
			c1->Clear();

			//画面分割、フィッティング
			double sigmaX, sigmaY, meanX, meanY;
			int entries = 0;
			TF1 * func = new TF1("func", "gaus");
			func->SetLineColor(1);
			c1->Divide(2, 2);
			c1->cd(1);
			dxy_cut2->Draw("colz");
			entries = dxy_cut2->GetEntries();
			c1->Update();

			c1->cd(2);
			gStyle->SetOptFit(1111);
			dxy_cut2->SetFillColor(kBlue);
			dxy_cut2->ProjectionY()->Draw("bar");
			dxy_cut2->ProjectionY()->Fit("func");
			meanY = (float)func->GetParameter(1);
			sigmaY = (float)func->GetParameter(2);
			c1->Update();

			c1->cd(3);
			gStyle->SetOptFit(1111);
			dxy_cut2->SetFillColor(kRed);
			dxy_cut2->ProjectionX()->Draw("bar");
			dxy_cut2->ProjectionX()->Fit("func");
			meanX = (float)func->GetParameter(1);
			sigmaX = (float)func->GetParameter(2);
			c1->Update();
			c1->Print(fname);
			c1->Clear();

			fitdata << vx << "," << vy  << "," << entries << "," << meanX << "," << sigmaX << "," << meanY << "," << sigmaY << endl;

			// std::vector<double> input_data;
			// input_data.push_back((double)vx);
			// input_data.push_back((double)vy);
			// input_data.push_back(meanX);
			// input_data.push_back(sigmaX);
			// input_data.push_back(meanY);
			// input_data.push_back(sigmaY);
			// data.push_bask(input_data);
			delete t;
			delete dxy;
			delete dxy_cut1;
			delete dxy_cut2;
			delete func;
		}
	}
	fname = pdfpathE.c_str();
	c1->Print(fname, "pdf");
	// ofstream fitdata(csvpath);
	// fitdata << "VX,VY,meanX,sigmaX,meanY,sigmaY" << endl;
	// for (int i = 0; i < data.size(); i++)
	// {
	// 	for (int j = 0; j < data.at(i).size(); j++)
	// 	{
	// 		fitdata << data.at(i).at(j) << ",";
	// 	}
	// 	fitdata << endl;
	// }
	
}
