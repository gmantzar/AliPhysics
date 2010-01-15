/**************************************************************************
* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
*                                                                        *
* Author: The ALICE Off-line Project.                                    *
* Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  *
**************************************************************************/
//
// Post analysis class
// Creating results and draw pictures
// Called in AliAnalysisTaskHFE::Terminate or in a macro
//
#ifndef ALIHFEPOSTANALYSIS_H
#define ALIHFEPOSTANALYSIS_H

#ifndef ROOT_THnSparse
#include <THnSparse.h>
#endif

class AliCFContainer;
class TList;
class AliHFEpostAnalysis : public TObject{
  public:
    AliHFEpostAnalysis();
    AliHFEpostAnalysis(const AliHFEpostAnalysis &ref);
    AliHFEpostAnalysis &operator=(const AliHFEpostAnalysis &ref);
    ~AliHFEpostAnalysis();

    Int_t SetResults(TList *input);
    void StoreOutput(const char *filename = "HFEresults.root");

    void DrawMCSignal2Background();
    void DrawEfficiency();
    void DrawPIDperformance();
  private:
    enum{
      kCFC,
      kPIDperf,
      kSigBackg
    };
    TList *fResults;                          // Container for output objects
    UChar_t fAnalysisObjects;                       // S
    AliCFContainer *fEfficiencyContainer;     // Task Results
    THnSparseF *fPIDperformance;              // PID Performance Studies
    THnSparseF *fSignalToBackgroundMC;        // Signal To Background Studies

    ClassDef(AliHFEpostAnalysis, 1)           // Result Creator class
};

#endif
