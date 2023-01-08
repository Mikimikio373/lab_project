// 2022/12/02
// minami completely revised for root ver.6

#include <iostream>
#include <stdio.h>

#include <TROOT.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>
#include <TH2.h>
#include <TCut.h>
#include <TMath.h>

using std::cout;
using std::endl;

void logon();

void PlotLk(std::string rootfile){
  logon();

  TFile *f = new TFile(rootfile.c_str());
  TTree * nt = (TTree*)f->Get("nt");

  std::string fname = "PlotLk_" + rootfile + ".pdf";

  TCanvas * c1 = new TCanvas("c1");
  c1->cd();
  fname = "PlotLk_" + rootfile + ".pdf[";
  c1->Print(fname.c_str(),"pdf");  //Open pdf file
  fname = "PlotLk_" + rootfile + ".pdf";

  TH1D *xtemp = new TH1D("xtemp","xtemp",100,1,1);
  nt->Draw("x0>>xtemp","","goff");
  double MeanX = xtemp->GetMean();
  TH1D *ytemp = new TH1D("ytemp","ytemp",100,1,1);
  nt->Draw("y0>>ytemp","","goff");
  double MeanY = ytemp->GetMean();

  //
  //1. position
  //
  //y0:x0
  gStyle->SetOptStat("e");
  c1->SetRightMargin(0.17);
  TH2F * position = new TH2F("position","position",135,MeanX-67500,MeanX+67500,100,MeanY-50000,MeanY+50000);
  position->SetTitle("y0:x0");
  position->GetXaxis()->SetTitle("X[#mum]");
  position->GetYaxis()->SetTitle("Y[#mum]");
  position->GetZaxis()->SetTitle("/mm^{2}");
  nt->Draw("y0:x0 >> position","","colz");

  c1->Print(fname.c_str());
  c1->Clear();
  //projection
  gStyle->SetOptStat("e");
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.17);
  position->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  position->SetFillColor(kBlue);
  position->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.17);
  position->SetFillColor(kRed);
  position->ProjectionX()->Draw();
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();
  delete position;

  //y1:x1
  gStyle->SetOptStat("e");
  nt->Draw("x1>>xtemp","","goff");
  MeanX = xtemp->GetMean();
  nt->Draw("y1>>ytemp","","goff");
  MeanY = ytemp->GetMean();
  position = new TH2F("position","position",135,MeanX-67500,MeanX+67500,100,MeanY-50000,MeanY+50000);
  position->SetTitle("y1:x1");
  position->GetXaxis()->SetTitle("X[#mum]");
  position->GetYaxis()->SetTitle("Y[#mum]");
  position->GetZaxis()->SetTitle("/mm^{2}");
  nt->Draw("y1:x1 >> position","","colz");
  c1->Print(fname.c_str());
  c1->Clear();
  
  //projection
  gStyle->SetOptStat("e");
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.17);
  position->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  position->SetFillColor(kBlue);
  position->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.17);
  position->SetFillColor(kRed);
  position->ProjectionX()->Draw();
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();

  //
  //2. angle
  //
  //ay0:ax0
  gStyle->SetOptStat("e");
  c1->Clear();
  c1->SetRightMargin(0.2);
  c1->SetLeftMargin(0.2);
  TH2F * angle = new TH2F("angle","angle",400,-2.0,2.0,400,-2.0,2.0);
  angle->SetTitle("ay0:ax0");
  angle->GetXaxis()->SetTitle("ax");
  angle->GetYaxis()->SetTitle("ay");
  angle->GetZaxis()->SetTitle("/(0.01rad)^{2}");
  nt->Draw("ay0:ax0 >> angle","","colz");
  c1->Print(fname.c_str());
  c1->Clear();

  //projection
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.2);
  c1->GetPad(1)->SetLeftMargin(0.2);
  angle->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  c1->GetPad(2)->SetRightMargin(0.3);
  angle->SetFillColor(kBlue);
  angle->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.2);
  c1->GetPad(3)->SetLeftMargin(0.2);
  angle->SetFillColor(kRed);
  angle->ProjectionX()->Draw("bar");
  c1->Update();

  c1->cd(4);
  TH1F * htan = new TH1F("htan",";#sqrt{ax0^{2}+ay0^{2}};/0.01",300,0.,3.);
  nt->Draw("sqrt(ax0^2+ay0^2) >>htan");
  c1->Update();
  c1->Print(fname.c_str());
  delete angle;
  delete htan;

  //ay1:ax1
  gStyle->SetOptStat("e");
  c1->Clear();
  c1->SetRightMargin(0.2);
  c1->SetLeftMargin(0.2);
  angle = new TH2F("angle","angle",400,-2.0,2.0,400,-2.0,2.0);
  angle->SetTitle("ay1:ax1");
  angle->GetXaxis()->SetTitle("ax");
  angle->GetYaxis()->SetTitle("ay");
  angle->GetZaxis()->SetTitle("/(0.01rad)^{2}");
  nt->Draw("ay1:ax1 >> angle","","colz");
  c1->Print(fname.c_str());
  c1->Clear();

  //projection
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.2);
  c1->GetPad(1)->SetLeftMargin(0.2);
  angle->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  c1->GetPad(2)->SetRightMargin(0.3);
  angle->SetFillColor(kBlue);
  angle->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.2);
  c1->GetPad(3)->SetLeftMargin(0.2);
  angle->SetFillColor(kRed);
  angle->ProjectionX()->Draw("bar");
  c1->Update();

  c1->cd(4);
  htan = new TH1F("htan",";#sqrt{ax1^{2}+ay1^{2}};/0.01",300,0.,3.);
  nt->Draw("sqrt(ax1^2+ay1^2) >>htan");
  c1->Update();
  c1->Print(fname.c_str());
  c1->Clear();

  //
  //position distance
  //
  gStyle->SetOptStat("e");
  TH2D * dxy = new TH2D("dxy","dxy",200,-50,50,200,-50,50);
  dxy->SetTitle("position distance");
  dxy->GetXaxis()->SetTitle("dx");
  dxy->GetYaxis()->SetTitle("dy");
  nt->Draw("dy:dx >> dxy", "", "colz");
  c1->Print(fname.c_str());
  c1->Clear();

  //projection
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.17);
  dxy->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  dxy->SetFillColor(kBlue);
  dxy->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.17);
  dxy->SetFillColor(kRed);
  dxy->ProjectionX()->Draw();
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();

  c1->Divide(2,1);
  c1->cd(1);
//   c1->GetPad(1)->SetRightMargin(0.2);
//   c1->GetPad(1)->SetTopMargin(0.25);
  TH1D * dxy2 = new TH1D("dxy2", ";#sqrt{dax^{2}+day^{2}};/0.01", 100, 0, 50);
  nt->Draw("sqrt(dx^2+dy^2) >> dxy2", "sqrt(dx^2+dy^2)<50 && sqrt(ax0^2+ay0^2)<0.1");
  dxy2->SetTitle("dxy sqrt(ax0^2+ay0^2)<0.1");
  dxy2->SetMinimum(0);
  c1->Update();

  c1->cd(2);
//   c1->GetPad(2)->SetRightMargin(0.2);
//   c1->GetPad(2)->SetTopMargin(0.25);
  TH1D * dxy3 = new TH1D("dxy3", ";#sqrt{dax^{2}+day^{2}};/0.01", 100, 0, 50);
  nt->Draw("sqrt(dx^2+dy^2) >> dxy3", "sqrt(dx^2+dy^2)<50 && sqrt(ax0^2+ay0^2)<1.0 && sqrt(ax0^2+ay0^2)>0.9");
  dxy3->SetTitle("dxy sqrt(ax0^2+ay0^2)>0.9 sqrt(ax0^2+ay0^2)<1.0");
  dxy3->SetMinimum(0);
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();

  //
  //angle distance
  //
  gStyle->SetOptStat("e");
  TH2D * daxy = new TH2D("daxy","daxy",200,-0.5,0.5,200,-0.5,0.5);
  daxy->SetTitle("angle distance");
  daxy->GetXaxis()->SetTitle("dax");
  daxy->GetYaxis()->SetTitle("day");
  nt->Draw("ay0-ay1:ax0-ax1 >> daxy", "", "colz");
  c1->Print(fname.c_str());
  c1->Clear();

  //projection
  c1->Divide(2,2);
  c1->cd(1);
  c1->GetPad(1)->SetRightMargin(0.17);
  daxy->Draw("colz");
  c1->Update();

  c1->cd(2);
  gStyle->SetOptStat("eou");
  daxy->SetFillColor(kBlue);
  daxy->ProjectionY()->Draw("hbar");
  c1->RedrawAxis();

  c1->cd(3);
  c1->GetPad(3)->SetRightMargin(0.17);
  daxy->SetFillColor(kRed);
  daxy->ProjectionX()->Draw();
  c1->Update();

  c1->cd(4);
  TH1F * dangtan = new TH1F("dtan",";#sqrt{dax^{2}+day^{2}};/0.01",100,0.,1.0);
  nt->Draw("sqrt((ax0-ax1)^2+(ay0-ay1)^2) >>dangtan");
  c1->Update();
  c1->Print(fname.c_str());
  c1->Clear();

  c1->Divide(2,1);
  c1->cd(1);
//   c1->GetPad(1)->SetRightMargin(0.2);
//   c1->GetPad(1)->SetTopMargin(0.25);
  TH1D * daxy2 = new TH1D("daxy2", ";#sqrt{dax^{2}+day^{2}};/0.01", 100, 0, 0.3);
  nt->Draw("sqrt((ax0-ax1)^2+(ay0-ay1)^2) >> daxy2", "sqrt((ax0-ax1)^2+(ay0-ay1)^2)<0.3 && sqrt(ax0^2+ay0^2)<0.1");
  daxy2->SetTitle("daxy sqrt(ax0^2+ay0^2)<0.1");
  daxy2->SetMinimum(0);
  c1->Update();

  c1->cd(2);
//   c1->GetPad(2)->SetRightMargin(0.2);
//   c1->GetPad(2)->SetTopMargin(0.25);
  TH1D * daxy3 = new TH1D("daxy3", ";#sqrt{dax^{2}+day^{2}};/0.01", 100, 0, 0.3);
  nt->Draw("sqrt((ax0-ax1)^2+(ay0-ay1)^2) >> daxy3", "sqrt((ax0-ax1)^2+(ay0-ay1)^2)<0.3 && sqrt(ax0^2+ay0^2)<1.0 && sqrt(ax0^2+ay0^2)>0.9");
  daxy3->SetTitle("daxy sqrt(ax0^2+ay0^2)>0.9 sqrt(ax0^2+ay0^2)<1.0");
  daxy3->SetMinimum(0);
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();

  cout << "test0" << endl;

  //
  //ph
  //

  TH1D * hph0[9];
  TH1D * hph00[9];
  TH1D * hph01[9];
  TH1D * hph1[9];
  TH1D * hph10[9];
  TH1D * hph11[9];
  TH1D * hvph0[9];
  TH1D * hvph00[9];
  TH1D * hvph01[9];
  TH1D * hvph1[9];
  TH1D * hvph10[9];
  TH1D * hvph11[9];

  char angcut[100];
  double angcutmin,angcutmax;

  for(int i=0;i<4;i++){
    //angle range
    angcutmin = (double)0.5*i;
    angcutmax = (double)0.5*i+0.1;

    sprintf(angcut,"ax^2+ay^2>=%.1f^2 && ax^2+ay^2<%.1f^2",angcutmin,angcutmax);
    TCut cut2 = angcut;
    cout << angcut << endl;

    hph0[i] = new TH1D(Form("ph0_%d",i),cut2,32,0.5,32.5);
    hph0[i]->GetXaxis()->SetTitle("PH0");
    hph00[i] = new TH1D(Form("ph00_%d",i),cut2,16,0.5,16.5);
    hph00[i]->GetXaxis()->SetTitle("PH00");
    hph01[i] = new TH1D(Form("ph01_%d",i),cut2,16,0.5,16.5);
    hph01[i]->GetXaxis()->SetTitle("PH01");

    hvph0[i] = new TH1D(Form("vph0_%d",i),cut2,600,0,600);
    hvph0[i]->GetXaxis()->SetTitle("VolPH0");
    hvph00[i] = new TH1D(Form("vph00_%d",i),cut2,300,0,300);
    hvph00[i]->GetXaxis()->SetTitle("VolPH00");
    hvph01[i] = new TH1D(Form("vph01_%d",i),cut2,300,0,300);
    hvph01[i]->GetXaxis()->SetTitle("VolPH01");

    hph1[i] = new TH1D(Form("ph1_%d",i),cut2,32,0.5,32.5);
    hph1[i]->GetXaxis()->SetTitle("PH1");
    hph10[i] = new TH1D(Form("ph10_%d",i),cut2,16,0.5,16.5);
    hph10[i]->GetXaxis()->SetTitle("PH10");
    hph11[i] = new TH1D(Form("ph11_%d",i),cut2,16,0.5,16.5);
    hph11[i]->GetXaxis()->SetTitle("PH11");

    hvph1[i] = new TH1D(Form("vph1_%d",i),cut2,600,0,600);
    hvph1[i]->GetXaxis()->SetTitle("VolPH1");
    hvph10[i] = new TH1D(Form("vph10_%d",i),cut2,300,0,300);
    hvph10[i]->GetXaxis()->SetTitle("VolPH10");
    hvph11[i] = new TH1D(Form("vph11_%d",i),cut2,300,0,300);
    hvph11[i]->GetXaxis()->SetTitle("VolPH11");
  }

  //Fill
  float ax0,ay0,ph0,ph1, ph00, ph01, ph10, ph11;
  nt->SetBranchStatus("*",0);
  nt->SetBranchStatus("ax0",1);
  nt->SetBranchStatus("ay0",1);
  nt->SetBranchStatus("ph0",1);
  nt->SetBranchStatus("ph00",1);
  nt->SetBranchStatus("ph01",1);
  nt->SetBranchStatus("ph1",1);
  nt->SetBranchStatus("ph10",1);
  nt->SetBranchStatus("ph11",1);
  nt->SetBranchAddress("ax0",&ax0);
  nt->SetBranchAddress("ay0",&ay0);
  nt->SetBranchAddress("ph0",&ph0);
  nt->SetBranchAddress("ph00",&ph00);
  nt->SetBranchAddress("ph01",&ph01);
  nt->SetBranchAddress("ph1",&ph1);
  nt->SetBranchAddress("ph10",&ph10);
  nt->SetBranchAddress("ph11",&ph11);

  int entries = nt->GetEntries();

  for(int i=0;i<entries;i++){
    if(i%100000==0)cout << i  << "/" << entries << "\r";
    nt->GetEntry(i);

    //cut2
    double tan = TMath::Sqrt(ax0*ax0 + ay0*ay0);
    for(int j=0;j<4;j++){
      angcutmin = 0.5*j;
      angcutmax = 0.5*j+0.1;

      if(angcutmin<=tan&&tan<angcutmax){
        hph0[j]->Fill( ph0/10000);
        hph00[j]->Fill( ph00/10000 );
        hph01[j]->Fill( ph01/10000 );
        hph1[j]->Fill( ph1/10000);
        hph10[j]->Fill( ph10/10000 );
        hph11[j]->Fill( ph11/10000 );
        hvph0[j]->Fill( int(ph0)%10000);
        hvph00[j]->Fill( int(ph00)%10000 );
        hvph01[j]->Fill( int(ph01)%10000 );
        hvph1[j]->Fill( int(ph1)%10000);
        hvph10[j]->Fill( int(ph10)%10000 );
        hvph11[j]->Fill( int(ph11)%10000 );        
        break;
      }else{ continue;}
    }

  }
  cout << entries  << "/" << entries << endl;


  //Draw
  for(int i=0;i<4;i++){
    //ph0
    c1->Divide(3,2);
    gStyle->SetOptStat("em");
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.4);
    gStyle->SetStatW(0.3);
    gStyle->SetStatH(0.1);

    c1->cd(1);
    hph0[i]->Draw();

    c1->cd(2);
    hph00[i]->Draw();

    c1->cd(3);
    hph01[i]->Draw();

    c1->Update();

    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    gStyle->SetStatW(0.3);
    gStyle->SetStatH(0.1);

    c1->cd(4);
    gStyle->SetOptStat("emo");
    hvph0[i]->Draw();

    c1->cd(5);
    hvph00[i]->Draw();

    c1->cd(6);
    hvph01[i]->Draw();

    c1->Update();
    c1->Print(fname.c_str());
    c1->Clear();

    //ph1
    c1->Divide(3,2);
    gStyle->SetOptStat("em");
    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.4);
    gStyle->SetStatW(0.3);
    gStyle->SetStatH(0.1);

    c1->cd(1);
    hph1[i]->Draw();

    c1->cd(2);
    hph10[i]->Draw();

    c1->cd(3);
    hph11[i]->Draw();

    c1->Update();

    gStyle->SetStatY(0.9);
    gStyle->SetStatX(0.9);
    gStyle->SetStatW(0.3);
    gStyle->SetStatH(0.1);

    c1->cd(4);
    gStyle->SetOptStat("emo");
    hvph1[i]->Draw();

    c1->cd(5);
    hvph10[i]->Draw();

    c1->cd(6);
    hvph11[i]->Draw();

    c1->Update();
    c1->Print(fname.c_str());
    c1->Clear();

    delete hph0[i];
    delete hph00[i];
    delete hph01[i];
    delete hph1[i];
    delete hph10[i];
    delete hph11[i];
    delete hvph0[i];
    delete hvph00[i];
    delete hvph01[i];
    delete hvph1[i];
    delete hvph10[i];
    delete hvph11[i];
  }

  nt->SetBranchStatus("*",1);

  //
  //volsum vs tan
  //
  //0
  c1->Divide(2,1);
  gStyle->SetOptStat("eou");
  c1->GetPad(1)->SetRightMargin(0.2);
  c1->GetPad(1)->SetTopMargin(0.25);
  TH2D * hvph0vs0 = new TH2D("hvph0vs0",";#sqrt{ax^{2}+ay^{2}};VolSum",200,0,2.0,200,0,200);
  
  c1->cd(1);
  nt->Draw("ph0%10000:sqrt(ax0^2+ay0^2) >> hvph0vs0", "", "colz");
  c1->Update();

  c1->cd(2);
  c1->GetPad(2)->SetLogz();
  c1->GetPad(2)->SetRightMargin(0.2);
  c1->GetPad(2)->SetTopMargin(0.25);
  nt->Draw("ph0%10000:sqrt(ax0^2+ay0^2) >> hvph0vs0","","colz");
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();
  
  //1
  c1->Divide(2,1);
  gStyle->SetOptStat("eou");
  c1->GetPad(1)->SetRightMargin(0.2);
  c1->GetPad(1)->SetTopMargin(0.25);
  TH2D * hvph1vs0 = new TH2D("hvph1vs0",";#sqrt{ax^{2}+ay^{2}};VolSum",200,0,2.0,200,0,200);
  
  c1->cd(1);
  nt->Draw("ph1%10000:sqrt(ax0^2+ay0^2) >> hvph1vs0", "", "colz");
  c1->Update();

  c1->cd(2);
  c1->GetPad(2)->SetLogz();
  c1->GetPad(2)->SetRightMargin(0.2);
  c1->GetPad(2)->SetTopMargin(0.25);
  nt->Draw("ph1%10000:sqrt(ax0^2+ay0^2) >> hvph1vs0","","colz");
  c1->Update();

  c1->Print(fname.c_str());
  c1->Clear();




  //close pdf
  fname = "PlotLk_" + rootfile + ".pdf]";

  c1->Print(fname.c_str());
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