void SetupTrackCutsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD);
void SetupPairCutsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD);
void InitHistogramsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD);
void InitCFDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD);

AliESDtrackCuts *SetupESDtrackCutsDieleData(Int_t cutDefinition);

TString namesDieleData=("baseMixzVert30Bins");


TObjArray *arrNamesDieleData=namesDieleData.Tokenize(";");

const Int_t nDie=arrNamesDieleData->GetEntries();

AliDielectron* ConfigCCbar_mk_pp(Int_t cutDefinition, Bool_t isAOD=kFALSE)
{
  //
  // Setup the instance of AliDielectron
  //
  
  // create the actual framework object
  TString name=Form("%02d",cutDefinition);
  if (cutDefinition<arrNamesDieleData->GetEntriesFast()){
    name=arrNamesDieleData->At(cutDefinition)->GetName();
  }
  AliDielectron *diele = new AliDielectron(Form("%s",name.Data()),
                             Form("Track cuts: %s",name.Data()));

  // cut setup
  SetupTrackCutsDieleData(diele, cutDefinition, isAOD);
//  SetupPairCutsDieleData(diele, cutDefinition, isAOD);

  //
  // histogram setup
  // only if an AliDielectronHistos object is attached to the
  // dielelectron framework histograms will be filled
  //
  InitHistogramsDieleData(diele, cutDefinition, isAOD);
  InitCFDieleData(diele, cutDefinition, isAOD);

  // mixing
  AliDielectronMixingHandler *mix=new AliDielectronMixingHandler;
  mix->AddVariable(AliDielectronVarManager::kZvPrim,30,-10.,10.);
//  if(cutDefinition == 1)mix->SetMixUncomplete(kFALSE);
  mix->SetDepth(100);
  mix->SetMixType(AliDielectronMixingHandler::kAll);
  diele->SetMixingHandler(mix);

  
//  old code
//  if(cutDefinition == 2)mix->AddVariable(AliDielectronVarManager::kZvPrim,20,-10.,10.);  
//  if(cutDefinition==1)mix->AddVariable(AliDielectronVarManager::kRefMult,"1,35,44,50,57,62,68,73,79,85,91,97,104,111,119,129,140,155,176,212,600");// 20 binssame integral per bin
//  if(cutDefinition==2)mix->AddVariable(AliDielectronVarManager::kVZEROchMult,"1,2,3,4,5,6,7,8,9,10,12,120");
//  if(cutDefinition==3){mix->SetDepth(20); mix->SetMixUncomplete(kFALSE);}
//  if(cutDefinition != 3)mix->SetDepth(100);  
  
  
  AliDielectronTrackRotator *rot=new AliDielectronTrackRotator;
  rot->SetConeAnglePhi(TMath::Pi());
  rot->SetIterations(20);
  //  if(cutDefinition = 0)diele->SetTrackRotator(rot);
  
  return diele;
}


//______________________________________________________________________________________
void SetupTrackCutsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD)
{
  //
  // Setup the track cuts
  //
  
  AliDielectronCutGroup* cuts = new AliDielectronCutGroup("cuts","cuts",AliDielectronCutGroup::kCompAND);
  diele->GetTrackFilter().AddCuts(cuts);
  
    AliDielectronTrackCuts *trackCuts=new AliDielectronTrackCuts("trackCuts","trackCuts");
    trackCuts->SetRequireTPCRefit(kTRUE);
    trackCuts->SetRequireITSRefit(kTRUE);
    trackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);
    trackCuts->SetMinNCrossedRowsOverFindable(0.7);
    //    diele->GetTrackFilter().AddCuts(trackCuts);
    cuts->AddCut(trackCuts);
  
  //Pt cut ----------------------------------------------------------
  AliDielectronVarCuts *pt = new AliDielectronVarCuts("ptCut","pt cut");
  pt->AddCut(AliDielectronVarManager::kPt,0.8,1e30);
  pt->AddCut(AliDielectronVarManager::kKinkIndex0,0.);
  pt->AddCut(AliDielectronVarManager::kNclsTPC,50.,160.);
  pt->AddCut(AliDielectronVarManager::kEta,-0.9,0.9);
  pt->AddCut(AliDielectronVarManager::kTPCchi2Cl,0.,4.);
  pt->AddCut(AliDielectronVarManager::kImpactParXY,-1.5,1.5);
  pt->AddCut(AliDielectronVarManager::kImpactParZ,-3.,3.);
  pt->AddCut(AliDielectronVarManager::kTPCnSigmaEle,-3.,4.);
//  diele->GetTrackFilter().AddCuts(pt);
  cuts->AddCut(pt);

  
}//SetupTrackCutsDieleData

//______________________________________________________________________________________
void SetupPairCutsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD)
{
  // Setup the pair cuts
  AliDielectronVarCuts *mycut = new AliDielectronVarCuts("CutEMCAL","cut for EMCal");
  mycut->AddCut(AliDielectronVarManager::kEMCALnSigmaEle,-2.,3.);
  mycut->AddCut(AliDielectronVarManager::kEMCALE,3.5,100.);
//  mycut->AddCut(AliDielectronVarManager::kEMCALEoverP,0.75,1.25);//  

  AliDielectronPairLegCuts *varpair=new AliDielectronPairLegCuts();
  varpair->GetLeg1Filter().AddCuts(mycut);
  varpair->GetLeg2Filter().AddCuts(mycut);
  
  if(cutDefinition == 1){
    varpair->SetCutType(AliDielectronPairLegCuts::kAnyLeg);
    diele->GetPairFilter().AddCuts(varpair);
  } 


}//SetupPairCutsDieleData

//______________________________________________________________________________________
void InitHistogramsDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD)
{
  //
  // Initialise the histograms
  //
  
  //Setup histogram Manager
  AliDielectronHistos *histos=new AliDielectronHistos(diele->GetName(),diele->GetTitle());
  
  //Initialise histogram classes
  histos->SetReservedWords("Track;Pair");
  
  //Track classes
  //to fill also track info from 2nd event loop until 2
  for (Int_t i=0; i<2; ++i){
    histos->AddClass(Form("Track_%s",AliDielectron::TrackClassName(i)));
  }
  
  //Pair classes
  // to fill also mixed event histograms loop until 10

   for (Int_t i=0; i<3; ++i){
  	histos->AddClass(Form("Pair_%s",AliDielectron::PairClassName(i)));

  }
  
  //legs from pair
  for (Int_t i=0; i<3; ++i){
    histos->AddClass(Form("Track_Legs_%s",AliDielectron::PairClassName(i)));
  }
  //track rotation
  //histos->AddClass(Form("Pair_%s",PairClassName(AliDielectron::kEv1PMRot)));
  //histos->AddClass(Form("Track_Legs_%s",PairClassName(AliDielectron::kEv1PMRot)));
  

    //add histograms to event class

    histos->AddClass("Event");
    histos->UserHistogram("Event","VtxZ","Vertex Z;Z[cm]",500,-40.,40.,AliDielectronVarManager::kZvPrim);
    histos->UserHistogram("Event","VtxYxVtxZ","Vertexyz;Z[cm];Y[cm]",500,-40.,40.,400,-0.5,0.5,AliDielectronVarManager::kZvPrim,AliDielectronVarManager::kYvPrim);
    histos->UserHistogram("Event","VtxXxVtxZ","Vertexxz;Z[cm];X[cm]",500,-40.,40.,400,-0.5,0.5,AliDielectronVarManager::kZvPrim,AliDielectronVarManager::kXvPrim);
    histos->UserHistogram("Event","VtxYxVtxX","Vertexxz;Z[cm];X[cm]",400,-0.5,0.5,400,-0.5,0.5,AliDielectronVarManager::kXvPrim,AliDielectronVarManager::kYvPrim);
    histos->UserHistogram("Event","MultV0A","MultV0A;multiplicity",1000,0.,1000.,AliDielectronVarManager::kMultV0A);
    histos->UserHistogram("Event","MultV0C","MultV0C;multiplicity",1000,0.,1000.,AliDielectronVarManager::kMultV0C);
    histos->UserHistogram("Event","MultV0","MultV0;multiplicity",1000,0.,1000.,AliDielectronVarManager::kMultV0);
    histos->UserHistogram("Event","RefMult","RefMult;multiplicity",1000,0.,1000.,AliDielectronVarManager::kRefMult);   
    histos->UserHistogram("Event","RefMultTPConly","RefMultTPConly;multiplicity",1000,0.,1000.,AliDielectronVarManager::kRefMultTPConly);    
    histos->UserHistogram("Event","VZEROchMult","VZEROchMult;multiplicity",1000,0.,1000.,AliDielectronVarManager::kVZEROchMult);     
    histos->UserHistogram("Event","MixingBin","kMixingBin;",100,0.,100.,AliDielectronVarManager::kMixingBin);     
    
  
  
  
  //add histograms to Track classes
  histos->UserHistogram("Track","Pt","Pt;Pt [GeV];#tracks",250,0,50.,AliDielectronVarManager::kPt,kTRUE);
  histos->UserHistogram("Track","TPCnCls","Number of Clusters TPC;TPC number clusters;#tracks",160,-0.5,159.5,AliDielectronVarManager::kNclsTPC,kTRUE);
  histos->UserHistogram("Track","TPCchi2Cl","Chi-2/Clusters TPC;Chi2/ncls number clusters;#tracks",100,0,10,AliDielectronVarManager::kTPCchi2Cl,kTRUE);
  histos->UserHistogram("Track","TPCnFCls","Number of findable Clusters TPC;Number of findable Clusters TPC;#tracks",160,-0.5,159.5,AliDielectronVarManager::kNFclsTPC,kTRUE);
  histos->UserHistogram("Track","TPCnFClsfCross","fraction crossed rows/findable;fraction crossed rows/findable;#tracks",200,0.,2.,AliDielectronVarManager::kNFclsTPCfCross,kTRUE);
  histos->UserHistogram("Track","TPCnFClsr","Number of findable Clusters(crossed rows) TPC;Number of findable crossed rows TPC;#tracks",160,-0.5,159.5,AliDielectronVarManager::kNFclsTPCr,kTRUE);
  histos->UserHistogram("Track","TPCnFClsrFrac","Number of found/findable Clusters TPC;Number of found/findable Clusters TPC;#tracks",200,0.,2.,AliDielectronVarManager::kNFclsTPCrFrac,kTRUE);
  histos->UserHistogram("Track","TPCnFClsTPCfCross","Fraction of findable Clusters/Cr.rows TPC;Fraction of findable Clusters/Cr.rows TPC;#tracks",200,0.,2.,AliDielectronVarManager::kNFclsTPCfCross,kTRUE);  
  histos->UserHistogram("Track","TPCsignalN","Number of points for TPC Signal;TPC Npoints dEdx;#tracks",160,-0.5,159.5,AliDielectronVarManager::kTPCsignalN,kTRUE);    
  
  histos->UserHistogram("Track","dXY","dXY;dXY [cm];#tracks",200,-1.5,1.5,AliDielectronVarManager::kImpactParXY,kTRUE);
  histos->UserHistogram("Track","dZ","dZ;dZ [cm];#tracks",200,-4.,4.,AliDielectronVarManager::kImpactParZ,kTRUE);
  
  histos->UserHistogram("Track","Eta_Phi","Eta vs Phi; Eta; Phi;#tracks",
                        100,-1.2,1.2,144,0,6.285,AliDielectronVarManager::kEta,AliDielectronVarManager::kPhi,kTRUE);

  histos->UserHistogram("Track","dEdx_P","dEdx;P [GeV];TPC signal (arb units);#tracks",
                        200,0.2,20.,800,20.,200.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","dEdx_Pt","dEdx;Pt [GeV];TPC signal (arb units);#tracks",
                        200,0.2,20.,800,20.,200.,AliDielectronVarManager::kPt,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","TPCnSigmaEle_P","TPCnSigmaEle;P [GeV];TPCnSigmaEle;#tracks",
                        200,0.2,20.,800,-12.,12.,AliDielectronVarManager::kPIn,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);
  
  histos->UserHistogram("Track","TPCnSigmaEle_Pt","TPCnSigmaEle;Pt [GeV];TPCnSigmaEle;#tracks",
                        200,0.2,20.,800,-12.,12.,AliDielectronVarManager::kPt,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);

  histos->UserHistogram("Track","TPCnSigmaEle_Phi","TPCnSigmaEle;#phi [rad];TPCnSigmaEle;#tracks",
                        200,0.,2*TMath::Pi(),800,-12.,12.,AliDielectronVarManager::kPhi,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);
  
  histos->UserHistogram("Track","TPCnSigmaEle_Eta","TPCnSigmaEle;#eta;TPCnSigmaEle;#tracks",
                        200,-1.,1.,800,-12.,12.,AliDielectronVarManager::kEta,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);
    

  histos->UserHistogram("Track","dEdx_Phi","dEdx vs phi;#phi [rad];TPC signal (arb units);#tracks",
                        200,0.,2*TMath::Pi(),800,20.,200.,AliDielectronVarManager::kPhi,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","dEdx_Eta","dEdx vs eta;#eta;TPC signal (arb units);#tracks",
                        200,-1.,1.,800,20.,200.,AliDielectronVarManager::kEta,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","dEdx_nSigmaEMCal","dEdx vs nSigmaEMCal;NsigmaEmcal;TPC signal (arb units);NSigmaEMCAL",
                        200,-5.,5.,800,20.,200.,AliDielectronVarManager::kEMCALnSigmaEle,AliDielectronVarManager::kTPCsignal,kTRUE);

    histos->UserHistogram("Track","nSigmaTPC_nSigmaEMCal","nSigmaTPC vs nSigmaEMCal;NsigmaEmcal;nSigmaTPC;NSigmaEMCAL",
                        200,-5.,5.,200,-5.,5.,AliDielectronVarManager::kEMCALnSigmaEle,AliDielectronVarManager::kTPCnSigmaEle,kTRUE);
    
  histos->UserHistogram("Track","dEdx_TPCnSigmaEle","dEdx vs TPCnSigmaEle;TPC signal electrons(arbunits);TPC number of sigmas Electrons;TPC signal (a.u.);#tracks",
                        100,-10.,10.,800,20.,200.,AliDielectronVarManager::kTPCnSigmaEle,AliDielectronVarManager::kTPCsignal,kTRUE);

  histos->UserHistogram("Track","dEdx_EoverP","dEdx;EoverP;TPC signal (arbunits);E/P",100,0.,5.,800,20.,200.,AliDielectronVarManager::kEMCALEoverP,AliDielectronVarManager::kTPCsignal,kTRUE);
  
  histos->UserHistogram("Track","nSigmaEMCal_EoverP","NsigmaEmcal;EoverP;NSigmaEMCAL;E/P",100,0.,5.,200,-5.,5.,AliDielectronVarManager::kEMCALEoverP,AliDielectronVarManager::kEMCALnSigmaEle,kTRUE);
  histos->UserHistogram("Track","Pt_EoverP","Pt;EoverP;Pt;E/P",100,0.,5.,100,0.,10.,AliDielectronVarManager::kEMCALEoverP,AliDielectronVarManager::kPt,kTRUE);
 
  histos->UserHistogram("Track","EMCal_E","EmcalE;Cluster Energy [GeV];#Clusters",200,0.,40.,AliDielectronVarManager::kEMCALE,kTRUE);

  histos->UserHistogram("Track","ITS_FirstCls","ITS First Layer;ITS First Layer;#Entries",6,0.,6.,AliDielectronVarManager::kITSLayerFirstCls,kTRUE);
 
  //add histograms to Pair classes
  histos->UserHistogram("Pair","InvMass","Inv.Mass;Inv. Mass [GeV];#pairs",
                        251,-.01,5.01,AliDielectronVarManager::kM);
  histos->UserHistogram("Pair","InvMass2D","Inv.Mass;Pt [GeV]; Inv. Mass [GeV]",
                        20,0.,20.,251,-.01,5.01,AliDielectronVarManager::kPt,AliDielectronVarManager::kM);
  
  
     histos->UserHistogram("Pair","InvMasslongVarBin","Inv.Mass;Inv. Mass [GeV];#pairs",
        "0. , 0.025, 0.05 , 0.075 ,0.1 ,0.15 , 0.2 , 0.25 , 0.3 , 
        0.4 ,  0.5 , 0.6, 0.65 , 0.688 , 0.725, 0.75, 0.775, 0.8 , 0.85 ,
         0.95,  0.975 , 1.0 , 1.025 , 1.05, 1.125 , 1.25 , 1.5 , 1.75 , 2.0 , 
        2.25, 2.5 , 2.75 , 2.85, 2.95,3.05, 3.1 , 3.15 , 
        3.3 , 3.4 ,3.5, 3.6, 3.7,3.8,3.9, 4.0,4.5, 5.0,6.0,7.0,8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0",AliDielectronVarManager::kM);
     
     
     
    histos->UserHistogram("Pair","InvMasslong","Inv.Mass;Inv. Mass [GeV];#pairs",
                        301,-.02,15.02,AliDielectronVarManager::kM);
    
  histos->UserHistogram("Pair","Rapidity","Rapidity;Rapidity;#pairs",
                        50,-1.,1.,AliDielectronVarManager::kY);
  histos->UserHistogram("Pair","OpeningAngle","Opening angle;angle",
                        50,0.,3.15,AliDielectronVarManager::kOpeningAngle);

  histos->UserHistogram("Pair","PseudoProperTime","Pseudoproper decay length; pseudoproper-decay-length[#mum];Entries/40#mum",
                          150,-0.3.,0.3,AliDielectronVarManager::kPseudoProperTime);
  
 histos->UserHistogram("Pair","Chi2/NDF","#Chi^{2}/NDF;#Chi^{2}/NDF",
                        100, 0., 20., AliDielectronVarManager::kChi2NDF);
  
  
  diele->SetHistogramManager(histos);
}


void InitCFDieleData(AliDielectron *diele, Int_t cutDefinition, Bool_t isAOD)
{
  //
  // Setupd the CF Manager if needed
  //
  
  AliDielectronCF *cf=new AliDielectronCF(diele->GetName(),diele->GetTitle());
  
  //pair variables
  cf->AddVariable(AliDielectronVarManager::kPt,"1.0,2.0,3.0,4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0,15.0,16.0,17.0,18.0,19.0,20.0");
//  cf->AddVariable(AliDielectronVarManager::kM,750,0.,15.);//also try variable bi sizes later...
  cf->AddVariable(AliDielectronVarManager::kM,"0.05,0.1,0.2,0.3,0.4,0.5,0.6,0.7,0.8,0.9,1.0,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,1.9,2.0,2.2,2.4,2.6,2.8,3.0,3.5,4.0, 4.5, 5.0, 5.5, 6.0, 6.5, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0,15.0");
  
  cf->AddVariable(AliDielectronVarManager::kPairType,12,0,12);
  cf->AddVariable(AliDielectronVarManager::kEta,20,-1.,1.);
  cf->AddVariable(AliDielectronVarManager::kY,20,-1.,1.);
  cf->AddVariable(AliDielectronVarManager::kPhi,64,-64.*0.1,64.*0.1);
  cf->AddVariable(AliDielectronVarManager::kOpeningAngle,64,0.,3.2);
  
  //global leg variables
  cf->AddVariable(AliDielectronVarManager::kPt,"0.,0.5,0.75,0.9,1.0, 1.1, 1.2, 1.3, 1.4, 1.5, 2.0, 3.0, 4.0, 8.0,9.0,10.0,11.0,12.0,13.0,14.0,15.0,16.0,17.0,18.0,19.0,20.0,30.0,50.0,100.0",kTRUE);
  cf->AddVariable(AliDielectronVarManager::kEta,44,-1.2,1.2,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kPhi,64,-64.*0.1,64.*0.1,kTRUE);

  //ITS
  cf->AddVariable(AliDielectronVarManager::kNclsITS,6,0.,6.,kTRUE);
  
  //TPC
  cf->AddVariable(AliDielectronVarManager::kNclsSTPC,20,0.,1.,kTRUE);//shared cluster
  cf->AddVariable(AliDielectronVarManager::kNclsTPC,"65, 70, 75, 80, 85, 90, 95, 100, 120, 160",kTRUE);
  cf->AddVariable(AliDielectronVarManager::kTPCsignalN,80,0.,160.,kTRUE);   
  cf->AddVariable(AliDielectronVarManager::kTPCnSigmaEle,80,-4.,4.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kTPCnSigmaPio,8,1.,4.5,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kTPCnSigmaPro,8,0.,4.,kTRUE);

/*  
  //EMCal
  cf->AddVariable(AliDielectronVarManager::kEMCALE,20,0.,20.,kTRUE); 
  cf->AddVariable(AliDielectronVarManager::kEMCALnSigmaEle,50,-5.,5.,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kEMCALNCells,100,0,50,kTRUE);
  cf->AddVariable(AliDielectronVarManager::kEMCALEoverP,"0.6,0.7,0.8,0.9,1.1,1.2,1.3,1.4,1.8,2.0,4.0",kTRUE);
*/  
//  cf->AddVariable(AliDielectronVarManager::kMixingBin,100,0.,100.);
//  cf->AddVariable(AliDielectronVarManager::kZvPrim,20,-20.,20.);

  diele->SetCFManagerPair(cf);
  
}



/*


//______________________________________________________________________________________
AliESDtrackCuts *SetupESDtrackCutsDieleData(Int_t cutDefinition)
{
  //
  // Setup default AliESDtrackCuts
  //
  AliESDtrackCuts *esdTrackCuts = new AliESDtrackCuts;

  // basic track quality cuts  (basicQ)
  esdTrackCuts->SetMaxDCAToVertexZ(3.0);
  esdTrackCuts->SetMaxDCAToVertexXY(1.0);

  esdTrackCuts->SetEtaRange( -0.9 , 0.9 );

  esdTrackCuts->SetAcceptKinkDaughters(kFALSE);
  esdTrackCuts->SetRequireITSRefit(kTRUE);
  esdTrackCuts->SetRequireTPCRefit(kTRUE);

  esdTrackCuts->SetPtRange(.8,1e30);

  esdTrackCuts->SetMinNClustersTPC(70);
  esdTrackCuts->SetMaxChi2PerClusterTPC(4);

  // default SPD any
  esdTrackCuts->SetClusterRequirementITS(AliESDtrackCuts::kSPD,AliESDtrackCuts::kAny);

  return esdTrackCuts;
}

 */
