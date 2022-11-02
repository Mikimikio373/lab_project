void tree_using_readfile()
{
  TString ifn = "../GrainMatching_test/dist_000_001_flg0.csv";
  TTree *tree = new TTree("tree", "tree using ReadFile()");
  tree->ReadFile(ifn, "distX/D:distY/D:flag/I");
 
  TString ofn = "out.root";
  TFile *fout = new TFile(ofn, "recreate");
  tree->Write();
  fout->Close();
 
  return;
}