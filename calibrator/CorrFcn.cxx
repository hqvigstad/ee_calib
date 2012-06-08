/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#include "CorrFcn.h"
#include <vector>
#include <math.h>
#include <iostream>
using namespace std;

CorrFcn::CorrFcn(int nSamples, double* peakPos, double* amp, double* peakError, double* ampWidth)
: peak(peakPos, peakPos+nSamples),
  peakErr(peakError, peakError+nSamples),
  amp(amp, amp+nSamples),
  ampWidth(ampWidth, ampWidth+nSamples),
  errorDef(1.)
{
//   for(unsigned int i = 0; i<(unsigned int)nSamples; ++i)
//     cout << peakPos[i] << endl;
}

CorrFcn::CorrFcn(const CorrFcn& other)
: peak(other.peak), peakErr(other.peakErr), amp(other.amp), ampWidth(other.ampWidth), errorDef(other.errorDef)
{;}

/* This function that represents the power function to be minimized, i.e. the X^2
 * @p parameters of the 
 */
inline double CorrFcn::operator()(const std::vector<double>& p) const
{
  double chiSquare = 0.;
  for(unsigned int i = 0; i < peak.size(); ++i) {
    const double factor = f(amp[i],p);
    const double diffWeighted = (factor*factor*peak[i] - I(amp[i])) / (factor*factor*peakErr[i]);
    chiSquare += diffWeighted*diffWeighted;
  }
  return chiSquare;
}

/* Energy correction factor, E=f*A, f=f(A;p).
 */
inline double CorrFcn::f(const double& A, const std::vector<double>& p) const
{
  return (p[0]+p[1]*exp(-A*p[2]))
	*(1.+p[3]*exp(-A*p[4]))
	*(1.+p[6]/(A*A+p[5]))
	;
}


