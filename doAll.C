{
    gROOT->ProcessLine(".L CMS3/CMS3.cc+");
    gROOT->ProcessLine(".L utils/utils.C+");
    gROOT->ProcessLine(".L /home/users/bemarsh/Software/dataMCplotMaker/dataMCplotMaker.cc+");
    gROOT->ProcessLine(".L ScanChain.C+");
    
    TChain *ch = new TChain("t"); 
    
    ch->Add("/nfs-7/userdata/stopRun2/StopBabies_V728_met30_ge1l_ge2j_Phys14/ttbar.root");
    ch->Add("/nfs-7/userdata/stopRun2/StopBabies_V728_met30_ge1l_ge2j_Phys14/WJets.root");
    ch->Add("/nfs-7/userdata/stopRun2/StopBabies_V728_met30_ge1l_ge2j_Phys14/ttzjets.root");
    ch->Add("/nfs-7/userdata/stopRun2/StopBabies_V728_met30_ge1l_ge2j_Phys14/t_tW.root");
    ch->Add("/nfs-7/userdata/stopRun2/StopBabies_V728_met30_ge1l_ge2j_Phys14/tbar_tW.root");
    
    ScanChain(ch); 
}
