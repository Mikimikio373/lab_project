#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <TROOT.h>
#include <TFile.h>
#include <TF1.h>
#include <TH1.h>
#include <TCanvas.h>
#include <TTree.h>
#include <TStyle.h>

bool find_vec(const std::vector<int>& input, int a){
    for (int i = 0; i < input.size(); i++)
    {
        if(input[i]==a){return true;}
    }
    return false;
}

void out_lk_id(const char *ref, const char* comp, const char* output1, const char* output2) {
    std::cout << "Read as ref: " << ref << std::endl;
    TFile *f1 = new TFile(ref);
    TTree *nt_ref = (TTree*)f1->Get("nt");
    const int entries_r = nt_ref->GetEntries();
    std::cout << "original entries:" << entries_r << std::endl;
    int id;
    nt_ref->SetBranchAddress("id0",&id);
    std::vector<int> id_ref;
    for (int i = 0; i < entries_r; i++)
    {
        nt_ref->GetEntry(i);
        id_ref.push_back(id);
    }
    std::sort(id_ref.begin(), id_ref.end());
    id_ref.erase(std::unique(id_ref.begin(), id_ref.end()), id_ref.end()); //unique
    std::cout << "unique entries:" << id_ref.size() << std::endl;
    

    std::cout << "Read as comparison: " << comp << std::endl;
    TFile *f2 = new TFile(comp);
    TTree *nt_comp = (TTree*)f2->Get("nt");
    const int entries_c = nt_comp->GetEntries();
    std::cout << "original entries" << entries_c << std::endl;
    nt_comp->SetBranchAddress("id0",&id);
    std::vector<int> id_comp;
    for (int i = 0; i < entries_c; i++)
    {
        nt_comp->GetEntry(i);
        id_comp.push_back(id);
    }
    std::sort(id_comp.begin(), id_comp.end());
    id_comp.erase(std::unique(id_comp.begin(), id_comp.end()), id_comp.end()); //unique
    std::cout << "unique entries" << id_comp.size() << std::endl;
    std::vector<int> id_and, id_nor;
    
    std::set_intersection(id_ref.begin(), id_ref.end(), id_comp.begin(), id_comp.end(), std::back_inserter(id_and)); //id_refとid_compの関集合
    std::set_difference(id_ref.begin(), id_ref.end(), id_and.begin(), id_and.end(), std::back_inserter(id_nor)); //id_refとid_andの差集合
    std::cout << "id_and entries:" << id_and.size() << std::endl;
    std::cout << "id_nor entries:" << id_nor.size() << std::endl;

    TFile *ref_and = new TFile(output1, "recreate");
    TTree *nt1 = (TTree*)nt_ref->CloneTree(0);
    for (int i = 0; i < entries_r; i++)
    {
        nt_ref->GetEntry(i);
        if (find_vec(id_and, id)){nt1->Fill();}
    }
    nt1->Print();
    nt1->Write();
    // nt->Reset();
    delete ref_and;

    TFile *ref_nor = new TFile(output2, "recreate");
    TTree *nt2 = (TTree*)nt_ref->CloneTree(0);
    for (int i = 0; i < entries_r; i++)
    {
        nt_ref->GetEntry(i);
        if (find_vec(id_nor, id)){nt2->Fill();}
    }
    nt2->Print();
    nt2->Write();
}