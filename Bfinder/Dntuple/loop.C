using namespace std;

#include "loop.h"
#include "format.h"
#include "Dntuple.h"
//here I change the skim option to true.
//I also chane REAL option to true.
Bool_t istest = true;
int loop(TString infile="", TString outfile="", Bool_t REAL=false, Bool_t isPbPb=false, Int_t startEntries=0, Int_t endEntries=-1, Bool_t skim=true, Bool_t gskim=true, Bool_t checkMatching=true, Bool_t iseos=false, Bool_t SkimHLTtree=true)
{
  if(istest)
    {
      //infile="/scratch/hammer/x/xiao147/deltaplusplus_crab_whole.root";
	 // outfile="/scratch/hammer/x/xiao147/MC_privatesample_reconeff_test_result/deltaplusplus_test.root";
	  //infile="/scratch/hammer/x/xiao147/kstar892crab_whole.root";
	  //outfile="/scratch/hammer/x/xiao147/MC_privatesample_reconeff_test_result/kstar892.root";
	 //infile="/scratch/hammer/x/xiao147/lambda1520crab/lambda1520_10.root";
	  //infile="/scratch/hammer/x/xiao147/lambda1520crab_whole.root";
	  //outfile="/scratch/hammer/x/xiao147/MC_privatesample_reconeff_test_result/lambda1520_test.root";
      //infile="/scratch/hammer/x/xiao147/pkpi_crab_32.root";
      infile="/home/xiao147/private/newchannel_lambda_CtoproduceDntuple/CMSSW_7_5_8_patch3/src/test/finder_test.root";
	  outfile="test.root";
      //outfile="test_lambda1520.root";
	  REAL=false;
      isPbPb=false;
      skim=true;
      checkMatching=true;
      iseos=false;
    }
  cout<<endl;
  if(REAL) cout<<"--- Processing - REAL DATA";
  else cout<<"--- Processing - MC";
  if(isPbPb) cout<<" - PbPb";
  else cout<<" - pp";
  cout<<endl;

  TString ifname;
  if(iseos) ifname = Form("root://eoscms.cern.ch//eos/cms%s",infile.Data());
  else ifname = infile;
  if (!TFile::Open(ifname))   { cout << " fail to open file" << endl; return 0;}
  TFile* f = TFile::Open(ifname);
  TTree* root = (TTree*)f->Get("Dfinder/root");  
  TTree* hltroot = (TTree*)f->Get("hltanalysis/HltTree");
  TTree* skimroot = (TTree*)f->Get("skimanalysis/HltTree");
  TTree* hiroot = (TTree*)f->Get("hiEvtAnalyzer/HiTree");

  DntupleBranches     *Dntuple = new DntupleBranches;
  EvtInfoBranches     *EvtInfo = new EvtInfoBranches;
  VtxInfoBranches     *VtxInfo = new VtxInfoBranches;
  TrackInfoBranches   *TrackInfo = new TrackInfoBranches;
  DInfoBranches       *DInfo = new DInfoBranches;
  GenInfoBranches     *GenInfo = new GenInfoBranches;

  if(SkimHLTtree) SetHlttreestatus(hltroot, isPbPb);
  setHltTreeBranch(hltroot);
  setHiTreeBranch(hiroot);

  EvtInfo->setbranchadd(root);
  VtxInfo->setbranchadd(root);
  TrackInfo->setbranchadd(root);
  DInfo->setbranchadd(root);
  GenInfo->setbranchadd(root);

  Long64_t nentries = root->GetEntries();
  if(endEntries>nentries || endEntries == -1) endEntries = nentries;
  TFile* outf = TFile::Open(Form("%s", outfile.Data()),"recreate");

//  int isDchannel[14];
  int isDchannel[16];
  isDchannel[0] = 0; //D0(k+pi-)
  isDchannel[1] = 0; //D0(k-pi+)
  isDchannel[2] = 0; //D*(D0(k-pi+)pi+)
  isDchannel[3] = 0; //D*(D0(k+pi-)pi-)
  isDchannel[4] = 0; //D*(D0(k-pi-pi+pi+)pi+)
  isDchannel[5] = 0; //D*(D0(k+pi+pi-pi-)pi-)
  isDchannel[6] = 0; 
  isDchannel[7] = 0; 
  isDchannel[8] = 0; 
  isDchannel[9] = 0; 
  isDchannel[10] = 0; 
  isDchannel[11] = 0;
  isDchannel[12] = 0; //B+(D0(k-pi+)pi+)
  isDchannel[13] = 0; //B-(D0(k-pi+)pi-)
  isDchannel[14] = 1; //lambdaC(p+k-pi+)
  isDchannel[15] = 1; //lambdaCbar(pbar-k+pi-)
  cout<<"--- Building trees"<<endl;
//  bool detailMode = true;
  bool detailMode=true;
  bool D0kpimode = true;
  TTree* ntD1 = new TTree("ntDkpi","");           Dntuple->buildDBranch(ntD1,D0kpimode,detailMode);
  D0kpimode = false;
  TTree* ntD2 = new TTree("ntDkpipi","");         Dntuple->buildDBranch(ntD2,D0kpimode,detailMode);
  TTree* ntD3 = new TTree("ntDkpipipi","");       Dntuple->buildDBranch(ntD3,D0kpimode,detailMode);
  TTree* ntD4 = new TTree("ntDPhikkpi","");       Dntuple->buildDBranch(ntD4,D0kpimode,detailMode);
  TTree* ntD5 = new TTree("ntDD0kpipi","");       Dntuple->buildDBranch(ntD5,D0kpimode,detailMode);
  TTree* ntD6 = new TTree("ntDD0kpipipipi","");   Dntuple->buildDBranch(ntD6,D0kpimode,detailMode);
  TTree* ntD7 = new TTree("ntBptoD0pi","");       Dntuple->buildDBranch(ntD7,D0kpimode,detailMode);
  TTree* ntD8 = new TTree("ntlambdaCtopkpi","");  Dntuple->buildDBranch(ntD8,D0kpimode,detailMode);
  TTree* ntGen = new TTree("ntGen","");           Dntuple->buildGenBranch(ntGen);
  TTree* ntHlt = hltroot->CloneTree(0);
  ntHlt->SetName("ntHlt");
  TTree* ntSkim = skimroot->CloneTree(0);
  ntSkim->SetName("ntSkim");
  TTree* ntHi = hiroot->CloneTree(0);
  ntHi->SetName("ntHi");
  cout<<"--- Building trees finished"<<endl;

  cout<<"--- Check the number of events for three trees"<<endl;
  cout<<root->GetEntries()<<" "<<hltroot->GetEntries()<<" "<<hiroot->GetEntries();
  cout<<" "<<skimroot->GetEntries()<<endl;
  cout<<endl;
  cout<<"--- Processing events"<<endl;
  for(int i=startEntries;i<endEntries;i++)
    {
      root->GetEntry(i);
//cout<<"entry"<<root->GetEntry(i)<<endl;
      hltroot->GetEntry(i);
      skimroot->GetEntry(i);
      hiroot->GetEntry(i);
      if(i%1000==0) cout<<setw(7)<<i<<" / "<<endEntries<<endl;
//here I add one line like that in lambdaC to lambda pi channel to remove all the Dsize=0
//cout<<"RunNo"<<EvtInfo->RunNo<<endl;
  //    if(DInfo->size==0)continue;//here I command this out, if run data, then I have to keep this filter.
//cout<<"Dsize2"<<DInfo->size<<endl;
//till this line, everything is fine.
//cout<<"RunNo"<<EvtInfo->RunNo<<endl;
//cout<<"PVxErr"<<EvtInfo->PVxE;
      if(checkMatching)
        {
          if(((int)Df_HLT_Event!=EvtInfo->EvtNo||(int)Df_HLT_Run!=EvtInfo->RunNo||(int)Df_HLT_LumiBlock!=EvtInfo->LumiNo) || 
             ((int)Df_HiTree_Evt!=EvtInfo->EvtNo||(int)Df_HiTree_Run!=EvtInfo->RunNo||(int)Df_HiTree_Lumi!=EvtInfo->LumiNo))
            {
              cout<<"Error: not matched "<<i<<" | (Hlt,Dfr,Hi) | ";
              cout<<"EvtNo("<<Df_HLT_Event<<","<<EvtInfo->EvtNo<<","<<Df_HiTree_Evt<<") ";
              cout<<"RunNo("<<Df_HLT_Run<<","<<EvtInfo->RunNo<<","<<Df_HiTree_Run<<") ";
              cout<<"LumiNo("<<Df_HLT_LumiBlock<<","<<EvtInfo->LumiNo<<","<<Df_HiTree_Lumi<<")"<<endl;
              continue;
            }
        }
//cout<<"RunNo2"<<EvtInfo->RunNo<<endl;
//cout<<"Dsize2"<<DInfo->size<<endl;
//till this line, there seems that there is no problems.
//cout<<"PVxErr"<<EvtInfo->PVxE;

      ntHlt->Fill();
      ntSkim->Fill();
      ntHi->Fill();
      Dntuple->makeDNtuple(isDchannel, REAL, skim, EvtInfo, VtxInfo, TrackInfo, DInfo, GenInfo, ntD1, ntD2, ntD3, ntD4, ntD5, ntD6, ntD7, ntD8);
//      ntD8->ls();
      if(!REAL) Dntuple->fillDGenTree(ntGen, GenInfo, gskim);
    }
  outf->Write();
  cout<<"--- Writing finished"<<endl;
  outf->Close();

  cout<<"--- In/Output files"<<endl;
  cout<<ifname<<endl;
  cout<<outfile<<endl;
  cout<<endl;

  return 0;
}

int main(int argc, char *argv[])
{
  if(argc==3)
    {
      loop(argv[1], argv[2]);
    }
  else
    {
      std::cout << "Usage: mergeForest <input_collection> <output_file>" << std::endl;
      return 1;
    }
  
  return 0;
}

