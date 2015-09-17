// Usage:
// > root -b doAll.C

// C++
#include <iostream>
#include <vector>

// ROOT
#include "TBenchmark.h"
#include "TChain.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTreeCache.h"

// CMS3
#include "CMS3/CMS3.h"

// utils
#include "utils/utils.h"

// dataMCplotMaker
#include "/home/users/bemarsh/Software/dataMCplotMaker/dataMCplotMaker.h"

#define N_BKG_TYPES 4
#define N_SIGNALS 3
#define N_BKG_CAT 3

using namespace std;
using namespace tas;

int ScanChain( TChain* chain, bool fast = true, int nEvents = -1, string skimFilePrefix = "test") {

    // Benchmark
    TBenchmark *bmark = new TBenchmark();
    bmark->Start("benchmark");

    // Loop over events to Analyze
    unsigned int nEventsTotal = 0;
    unsigned int nEventsChain = chain->GetEntries();
    if( nEvents >= 0 ) nEventsChain = nEvents;
    TObjArray *listOfFiles = chain->GetListOfFiles();
    TIter fileIter(listOfFiles);
    TFile *currentFile = 0;

    vector<string> bkg_names_vec;
    bkg_names_vec.push_back("ttbar");
    bkg_names_vec.push_back("WJets");
    bkg_names_vec.push_back("ttZ");
    bkg_names_vec.push_back("tW");

    vector<TH1F*> ht_ngoodjets_vec;
    vector<TH1F*> ht_ngoodbtags_vec;
    vector<TH1F*> ht_met_vec;
    vector<TH1F*> ht_mt_vec;
    vector<TH1F*> ht_mt2w_vec;
    vector<TH1F*> ht_lep1_pt_vec;
    vector<TH1F*> ht_jet1_pt_vec;
    vector<TH1F*> ht_btag1_pt_vec;
    vector<TH1F*> ht_count_vec;
    vector<TH1F*> ht_ngenlep_vec;

    string name;
    for(int i=0; i<N_BKG_TYPES; i++){
        name = string("ht_ngoodjets_")+(char)('0'+i);
        ht_ngoodjets_vec.push_back(new TH1F(name.c_str(), name.c_str(), 9, -0.5, 8.5));
        name = string("ht_ngoodbtags_")+(char)('0'+i);
        ht_ngoodbtags_vec.push_back(new TH1F(name.c_str(), name.c_str(), 5, -0.5, 4.5));
        name = string("ht_met_")+(char)('0'+i);
        ht_met_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("ht_mt_")+(char)('0'+i);
        ht_mt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("ht_mt2w_")+(char)('0'+i);
        ht_mt2w_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("ht_lep1_pt_")+(char)('0'+i);
        ht_lep1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("ht_jet1_pt_")+(char)('0'+i);
        ht_jet1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("ht_btag1_pt_")+(char)('0'+i);
        ht_btag1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("ht_count_")+(char)('0'+i);
        ht_count_vec.push_back(new TH1F(name.c_str(), name.c_str(), 1, 0, 2));
        name = string("ht_ngenlep_")+(char)('0'+i);
        ht_ngenlep_vec.push_back(new TH1F(name.c_str(), name.c_str(), 3, -0.5, 2.5));
    }

    vector<string> bkg_cat_vec;
    bkg_cat_vec.push_back("0 lep");
    bkg_cat_vec.push_back("1 lep");
    bkg_cat_vec.push_back("2 lep");

    vector<TH1F*> hc_ngoodjets_vec;
    vector<TH1F*> hc_ngoodbtags_vec;
    vector<TH1F*> hc_met_vec;
    vector<TH1F*> hc_mt_vec;
    vector<TH1F*> hc_mt2w_vec;
    vector<TH1F*> hc_lep1_pt_vec;
    vector<TH1F*> hc_jet1_pt_vec;
    vector<TH1F*> hc_btag1_pt_vec;
    vector<TH1F*> hc_count_vec;

    for(int i=0; i<N_BKG_CAT; i++){
        name = string("hc_ngoodjets_")+(char)('0'+i);
        hc_ngoodjets_vec.push_back(new TH1F(name.c_str(), name.c_str(), 9, -0.5, 8.5));
        name = string("hc_ngoodbtags_")+(char)('0'+i);
        hc_ngoodbtags_vec.push_back(new TH1F(name.c_str(), name.c_str(), 5, -0.5, 4.5));
        name = string("hc_met_")+(char)('0'+i);
        hc_met_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hc_mt_")+(char)('0'+i);
        hc_mt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hc_mt2w_")+(char)('0'+i);
        hc_mt2w_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hc_lep1_pt_")+(char)('0'+i);
        hc_lep1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("hc_jet1_pt_")+(char)('0'+i);
        hc_jet1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hc_btag1_pt_")+(char)('0'+i);
        hc_btag1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("hc_count_")+(char)('0'+i);
        hc_count_vec.push_back(new TH1F(name.c_str(), name.c_str(), 1, 0, 2));
    }

    vector<string> sig_name_vec;
    sig_name_vec.push_back("425/230/222.5");
    sig_name_vec.push_back("650/330/325");
    sig_name_vec.push_back("850/105/100");

    vector<TH1F*> hs_ngoodjets_vec;
    vector<TH1F*> hs_ngoodbtags_vec;
    vector<TH1F*> hs_met_vec;
    vector<TH1F*> hs_mt_vec;
    vector<TH1F*> hs_mt2w_vec;
    vector<TH1F*> hs_lep1_pt_vec;
    vector<TH1F*> hs_jet1_pt_vec;
    vector<TH1F*> hs_btag1_pt_vec;
    vector<TH1F*> hs_count_vec;
    vector<TH1F*> hs_decay_vec;

    for(int i=0; i<N_SIGNALS; i++){
        name = string("hs_ngoodjets_")+(char)('0'+i);
        hs_ngoodjets_vec.push_back(new TH1F(name.c_str(), name.c_str(), 9, -0.5, 8.5));
        name = string("hs_ngoodbtags_")+(char)('0'+i);
        hs_ngoodbtags_vec.push_back(new TH1F(name.c_str(), name.c_str(), 5, -0.5, 4.5));
        name = string("hs_met_")+(char)('0'+i);
        hs_met_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hs_mt_")+(char)('0'+i);
        hs_mt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hs_mt2w_")+(char)('0'+i);
        hs_mt2w_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hs_lep1_pt_")+(char)('0'+i);
        hs_lep1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("hs_jet1_pt_")+(char)('0'+i);
        hs_jet1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 500));
        name = string("hs_btag1_pt_")+(char)('0'+i);
        hs_btag1_pt_vec.push_back(new TH1F(name.c_str(), name.c_str(), 60, 0, 400));
        name = string("hs_count_")+(char)('0'+i);
        hs_count_vec.push_back(new TH1F(name.c_str(), name.c_str(), 1, 0, 2));
        name = string("hs_decay_")+(char)('0'+i);
        hs_decay_vec.push_back(new TH1F(name.c_str(), name.c_str(), 4, -0.5, 3.5));
    }


    // File Loop
    while ( (currentFile = (TFile*)fileIter.Next()) ) {

        // Get File Content
        TFile *file = new TFile( currentFile->GetTitle() );
        TTree *tree = (TTree*)file->Get("t");
        if(fast) TTreeCache::SetLearnEntries(10);
        if(fast) tree->SetCacheSize(128*1024*1024);
        cms3.Init(tree);
    
        int fileType = getFileType(currentFile->GetTitle());

        // Loop over Events in current file
        if( nEventsTotal >= nEventsChain ) continue;
        unsigned int nEventsTree = tree->GetEntriesFast();
        for( unsigned int event = 0; event < nEventsTree; ++event) {
    
            // Get Event Content
            if( nEventsTotal >= nEventsChain ) continue;
            if(fast) tree->LoadTree(event);
            cms3.GetEntry(event);
            ++nEventsTotal;

            // Progress
            CMS3::progress( nEventsTotal, nEventsChain );

            double scale = scale1fb();
            double isSignal = (fileType>=4);
            int bkgType = getBkgType();

            // if(fileType==4)
            //     scale = 0.0104253;
            // if(fileType==5)
            //     scale = 0.0009576;
            // if(fileType==6)
            //     scale = 0.0001751;

            if(isSignal)
                hs_decay_vec[fileType-N_BKG_TYPES]->Fill(getStopDecayMode(), scale);

 
            if(isSignal && getStopDecayMode()!=1)
                continue;
            
            if(bkgType<0 || bkgType>2)
                continue;
    
            double metcut = 200, mtcut = 150, mt2wcut = 200;
            int jcut=2, bcut=1;

            double met = pfmet();
            double mt = mt_met_lep();
            double mt2w = MT2W();
            double lep1pt = lep1_pt();
            int njets = ngoodjets();
            int nbtags = ngoodbtags();
            double jet1pt = ak4pfjets_pt()[0];
            double btag1pt = ak4pfjets_leadbtag_p4().Pt();

            if(ngoodleps()!=1 || nvetoleps()!=1 || !PassTrackVeto() || !PassTauVeto())
                continue;

            if(met > metcut && mt > mtcut && mt2w > mt2wcut && nbtags >= bcut){
                if(!isSignal){
                    ht_ngoodjets_vec[fileType]->Fill(njets, scale);
                    hc_ngoodjets_vec[bkgType]->Fill(njets, scale);
                }else{
                    hs_ngoodjets_vec[fileType-N_BKG_TYPES]->Fill(njets,scale);
                }
            }

            if(njets<jcut)
                continue;
            
            if(met > metcut && mt > mtcut && mt2w > mt2wcut){
                if(!isSignal){
                    ht_ngoodbtags_vec[fileType]->Fill(nbtags, scale);
                    hc_ngoodbtags_vec[bkgType]->Fill(nbtags, scale);
                }else{
                    hs_ngoodbtags_vec[fileType-N_BKG_TYPES]->Fill(nbtags,scale);
                }
            }

            if(nbtags<bcut)
                continue;

            if(mt > mtcut && mt2w > mt2wcut){
                if(!isSignal){
                    ht_met_vec[fileType]->Fill(met, scale);
                    hc_met_vec[bkgType]->Fill(met, scale);
                }else{
                    hs_met_vec[fileType-N_BKG_TYPES]->Fill(met, scale);
                }
            }

            if(met < metcut)
                continue;

            if(mt2w > mt2wcut){
                if(!isSignal){
                    ht_mt_vec[fileType]->Fill(mt, scale);
                    hc_mt_vec[bkgType]->Fill(mt, scale);
                }else{
                    hs_mt_vec[fileType-N_BKG_TYPES]->Fill(mt, scale);
                }
            }

            if(mt < mtcut)
                continue;

            if(!isSignal){
                ht_mt2w_vec[fileType]->Fill(mt2w, scale);
                hc_mt2w_vec[bkgType]->Fill(mt2w, scale);
            }else{
                hs_mt2w_vec[fileType-N_BKG_TYPES]->Fill(mt2w, scale);
            }                

            if(mt2w<mt2wcut)
                continue;

            if(!isSignal){
                ht_lep1_pt_vec[fileType]->Fill(lep1pt, scale);
                ht_jet1_pt_vec[fileType]->Fill(jet1pt, scale);
                ht_btag1_pt_vec[fileType]->Fill(btag1pt, scale);
                ht_count_vec[fileType]->Fill(1,scale);
                ht_ngenlep_vec[fileType]->Fill(bkgType,scale);
                
                hc_lep1_pt_vec[bkgType]->Fill(lep1pt, scale);
                hc_jet1_pt_vec[bkgType]->Fill(jet1pt, scale);
                hc_btag1_pt_vec[bkgType]->Fill(btag1pt, scale);
                hc_count_vec[bkgType]->Fill(1,scale);
            }else{
                hs_lep1_pt_vec[fileType-N_BKG_TYPES]->Fill(lep1pt, scale);
                hs_jet1_pt_vec[fileType-N_BKG_TYPES]->Fill(jet1pt, scale);
                hs_btag1_pt_vec[fileType-N_BKG_TYPES]->Fill(btag1pt, scale);
                hs_count_vec[fileType-N_BKG_TYPES]->Fill(1,scale);
            }


            if(fileType==3)
                cout << gents_status().size();


        }
  
        // Clean Up
        delete tree;
        file->Close();
        delete file;
    }

    if ( nEventsChain != nEventsTotal ) {
        cout << Form( "ERROR: number of events from files (%d) is not equal to total number of events (%d)", nEventsChain, nEventsTotal ) << endl;
    }

    TFile out("histos.root","RECREATE");
    for(int i=0; i<N_BKG_TYPES; i++){
        ht_ngoodjets_vec[i]->Write();
        ht_ngoodbtags_vec[i]->Write();
        ht_met_vec[i]->Write();
        ht_mt_vec[i]->Write();
        ht_mt2w_vec[i]->Write();
        ht_lep1_pt_vec[i]->Write();
        ht_jet1_pt_vec[i]->Write();
        ht_btag1_pt_vec[i]->Write();
        ht_count_vec[i]->Write();
        ht_ngenlep_vec[i]->Write();
    }
    for(int i=0; i<N_BKG_CAT; i++){
        hc_ngoodjets_vec[i]->Write();
        hc_ngoodbtags_vec[i]->Write();
        hc_met_vec[i]->Write();
        hc_mt_vec[i]->Write();
        hc_mt2w_vec[i]->Write();
        hc_lep1_pt_vec[i]->Write();
        hc_jet1_pt_vec[i]->Write();
        hc_btag1_pt_vec[i]->Write();
        hc_count_vec[i]->Write();
    }
    for(int i=0; i<N_SIGNALS; i++){
        hs_ngoodjets_vec[i]->Write();
        hs_ngoodbtags_vec[i]->Write();
        hs_met_vec[i]->Write();
        hs_mt_vec[i]->Write();
        hs_mt2w_vec[i]->Write();
        hs_lep1_pt_vec[i]->Write();
        hs_jet1_pt_vec[i]->Write();
        hs_btag1_pt_vec[i]->Write();
        hs_count_vec[i]->Write();
        hs_decay_vec[i]->Write();
    }

    out.Close();

    TH1F *null = new TH1F("","",1,0,1);
    vector<Color_t> colors;
    colors.push_back(kAzure+7);                                                
    colors.push_back(kRed-7);
    colors.push_back(kSpring-5);
    colors.push_back(kOrange-2);
    colors.push_back(kBlack);
    colors.push_back(kBlack);
    colors.push_back(kBlack);
    //colors.push_back(kViolet-3);

    string comopt = " --lumi 1.0 --blackSignals --legendTextSize 0.025 --outOfFrame";
    
    dataMCplotMaker(null, ht_met_vec, bkg_names_vec, "met", "", "--outputName plots/t_met.pdf --xAxisLabel met"+comopt, hs_met_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_mt_vec, bkg_names_vec, "M_{T}", "", "--outputName plots/t_mt.pdf --xAxisLabel M_T"+comopt, hs_mt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_mt2w_vec, bkg_names_vec, "MT2W", "", "--outputName plots/t_mt2w.pdf --xAxisLabel MT2W"+comopt, hs_mt2w_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_lep1_pt_vec, bkg_names_vec, "lep1 p_{T}", "", "--outputName plots/t_lep1_pt.pdf --xAxisLabel p_{T}"+comopt, hs_lep1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_jet1_pt_vec, bkg_names_vec, "jet1 p_{T}", "", "--outputName plots/t_jet1_pt.pdf--xAxisLabel p_{T}"+comopt, hs_jet1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_btag1_pt_vec, bkg_names_vec, "btag1 p_{T}", "", "--outputName plots/t_btag1_pt.pdf --xAxisLabel p_{T}"+comopt, hs_btag1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_ngoodjets_vec, bkg_names_vec, "ngoodjets", "", "--outputName plots/t_ngoodjets.pdf --nDivisions 9 --xAxisLabel n --noXaxisUnit"+comopt, hs_ngoodjets_vec, sig_name_vec, colors);
    dataMCplotMaker(null, ht_ngoodbtags_vec, bkg_names_vec, "ngoodbtags", "", "--outputName plots/t_ngoodbtags.pdf --nDivisions 5 --xAxisLabel n --noXaxisUnit"+comopt, hs_ngoodbtags_vec, sig_name_vec, colors);
  
    colors.pop_back(); colors.pop_back(); colors.pop_back(); colors.pop_back();
    colors.push_back(kBlack);
    colors.push_back(kBlack);
    colors.push_back(kBlack);
    
    dataMCplotMaker(null, hc_met_vec, bkg_cat_vec, "met", "", "--outputName plots/c_met.pdf --xAxisLabel met"+comopt, hs_met_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_mt_vec, bkg_cat_vec, "M_{T}", "", "--outputName plots/c_mt.pdf --xAxisLabel M_T"+comopt, hs_mt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_mt2w_vec, bkg_cat_vec, "MT2W", "", "--outputName plots/c_mt2w.pdf --xAxisLabel MT2W"+comopt, hs_mt2w_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_lep1_pt_vec, bkg_cat_vec, "lep1 p_{T}", "", "--outputName plots/c_lep1_pt.pdf --xAxisLabel p_{T}"+comopt, hs_lep1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_jet1_pt_vec, bkg_cat_vec, "jet1 p_{T}", "", "--outputName plots/c_jet1_pt.pdf --xAxisLabel p_{T}"+comopt, hs_jet1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_btag1_pt_vec, bkg_cat_vec, "btag1 p_{T}", "", "--outputName plots/c_btag1_pt.pdf --xAxisLabel p_{T}"+comopt, hs_btag1_pt_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_ngoodjets_vec, bkg_cat_vec, "ngoodjets", "", "--outputName plots/c_ngoodjets.pdf --nDivisions 9 --xAxisLabel n --noXaxisUnit"+comopt, hs_ngoodjets_vec, sig_name_vec, colors);
    dataMCplotMaker(null, hc_ngoodbtags_vec, bkg_cat_vec, "ngoodbtags", "", "--outputName plots/c_ngoodbtags.pdf --nDivisions 5 --xAxisLabel n --noXaxisUnit"+comopt, hs_ngoodbtags_vec, sig_name_vec, colors);
  
    // return
    bmark->Stop("benchmark");
    cout << endl;
    cout << nEventsTotal << " Events Processed" << endl;
    cout << "------------------------------" << endl;
    cout << "CPU  Time:	" << Form( "%.01f", bmark->GetCpuTime("benchmark")  ) << endl;
    cout << "Real Time:	" << Form( "%.01f", bmark->GetRealTime("benchmark") ) << endl;
    cout << endl;
    delete bmark;
    return 0;
}
