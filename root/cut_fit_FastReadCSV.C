#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <TROOT.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TStyle.h>

//最後の改行文字をnull文字に変える
void lntrim(char *str) {  
  char *p;  
  p = strchr(str, '\n');  
  if(p != NULL) {  
    *p = '\0';  
  }  
} 

int cut_fit_FastReadCSV()
{
    TTree * t = new TTree("t", "pixel distance");

    FILE * fp = fopen("sample.csv", "r");

    char buffer1[30];
    int flag;
    double dx, dy;
    t->Branch("dx",&dx,"dx/F");
    t->Branch("dy",&dy,"dy/F");
    t->Branch("flag",&flag,"flag/I");

    while(fgets(buffer1, 30, fp) != NULL) {
        char *ptr;
        // カンマを区切りに文字列を分割
        // 1回目
        ptr = strtok(buffer1, ",");
        dx = atof(ptr);
        
        // 2回目
        ptr = strtok(NULL, ",");
        dy = atof(ptr);

        // 3回目
        ptr = strtok(NULL, ",");
        lntrim(ptr);
        flag = atoi(ptr);
        
        t->Fill();
	}
 
	fclose(fp); // ファイルを閉じる

    TFile *fout = new TFile("sample.root", "recreate");
    t->Write();  
    fout->Close();

    std::cout << "end" << std::endl;
}