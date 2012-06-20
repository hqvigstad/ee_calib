/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#ifndef CORRFCN_H
#define CORRFCN_H

#include "TF1.h"
#include "TGraphErrors.h"

#include "Minuit2/FCNBase.h"
#include "Minuit2/MnUserParameters.h"

#include <vector>


/* This class represents the power function to minimized in EE Calib.
 *
 * The Idea of the calibration is to minimize variance from Ideal of pi0 -> gamma gamma IM,
 * using an dataset which approximate E_1 ~=E_2.
 * f = f(A;p)
 * E = f*A
 * M_A = AA*(1-cos(\phi))
 * M = E*E*(1-cos(\phi)) = A*f * A*f * (1-cos(\phi)) = f*f * A*A*(1-cos(\phi)) = f*f*M_A
 *
 * It is an representation of the following X^2:
 * X^2 = sum_A [{M_A*f^2 - I(A)} / s_A)]^2
 * 
 * Termonologi:
 * E - cluster energy, after Non-Linear Correction
 * A - Amplitude, i.e. Non-"Non-Linear Corrected" Cluster Energy.
 * p - Parameters, i.e. the parameters of the linear correction.
 * M_A - mean Mass of reconstructed  pi0, measured through peak position, calculated with Amplitude.
 * s_A - error of said measured mean.
 *
 * f(A;p) - Energy Correction factor, E = f*A
 * I(A) - Ideal peak position, mass of pi0 if pi0 distribtuion was flat.
 */
class CorrFcn : public ROOT::Minuit2::FCNBase
{
public:
  CorrFcn(int nSamples, double* peakPos, double* amp, double* peakError, double* ampWidth);
  CorrFcn(const CorrFcn& other);
  virtual ~CorrFcn() {;}

  double operator () (const std::vector<double>& par) const;
  double Up() const {return errorDef;}
  void SetErrorDef(double def) {errorDef = def;}

  virtual ROOT::Minuit2::MnUserParameters GetInitParameters();
  virtual double f(const double& A, const std::vector<double>& par) const;

  double I(const double& A) const {return 0.1349766 +(A-A);} // pi0 mass [PDG 2010]
  virtual TF1 Createf(const std::vector<double>& par) const;
  virtual TF1 CreateF(const std::vector< double >& par) const;
  TGraphErrors CreateIMGraph(const std::vector<double>& par) const;
  int GetNPoints() { return peak.size(); }
    
  
private:
  std::vector<double> peak; // Position of IM peak
  std::vector<double> peakErr; // Error of Position of IM peak
  std::vector<double> amp; // Amplitude
  std::vector<double> ampWidth; // Width of Amplitude
  double errorDef;
};

#endif // CORRFCN_H
