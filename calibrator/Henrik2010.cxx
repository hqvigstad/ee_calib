/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#include "TAxis.h"

#include <vector>
using namespace std;

#include "Henrik2010.h"
using namespace ROOT::Minuit2;

Henrik2010::Henrik2010(const CorrFcn& other)
: CorrFcn(other)
{
}

Henrik2010::~Henrik2010()
{
}

/* Energy correction factor, E=f*A, f=f(A;p).
 */
double Henrik2010::f(const double& A, const std::vector< double >& p) const
{
  return (p[0]+p[1]*exp(-A*p[2]))
	*(1.+p[3]*exp(-A*p[4]))
	*(1.+p[6]/(A*A+p[5]))
	;
}
MnUserParameters Henrik2010::GetInitParameters()
{

  double ipars[7]={1.051, 2.54e-003, -1.737e-002, 5.69e-002, 3.3e-001, 1.42e-001, 1.50e-002};
  vector<double> iparameters = vector<double>(ipars, ipars+7);
  double ipar_errs[7]={0.1051, 2.54e-004,-1.737e-003,5.69e-003,3.3e-002,1.42e-002,1.50e-003};
  vector<double> iparameter_errs = vector<double>(ipar_errs, ipar_errs+7);

  MnUserParameters rpar = MnUserParameters(iparameters, iparameter_errs);

  rpar.SetLowerLimit(5, 0.);
  return rpar;
}

TF1 Henrik2010::CreateF(const std::vector< double >& par) const
{
  TF1 func = TF1("f", "([0]+[1]*exp(-x*[2]))*(1.+[3]*exp(-x*[4]))*(1.+[6]/(x*x+[5]))", 0, 100) ;
  func.SetParameters(&par[0]);
  func.GetXaxis()->SetTitle("A [APD]");
  func.GetYaxis()->SetTitle("E [GeV]");
  return func;
}
