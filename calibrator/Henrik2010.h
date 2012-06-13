/* Copyright(c) 1998-1999, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice                               */

// Author: Henrik Qvigstad <henrik.qvigstad@cern.ch>
/* $Id$ */


#ifndef HENRIK2010_H
#define HENRIK2010_H

#include <CorrFcn.h>


class Henrik2010 : public CorrFcn
{

public:
    Henrik2010(const CorrFcn& other);
    virtual ~Henrik2010();

    double f(const double& A, const std::vector<double>& p) const;
    ROOT::Minuit2::MnUserParameters GetInitParameters();
    TF1 CreateF(const std::vector< double >& par) const;
};

#endif // HENRIK2010_H
