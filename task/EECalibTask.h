#ifndef ETATASK_H
#define ETATASK_H
/*
Eta Prime Analysis for the ALICE Experiment.
Copyright (C) 2011 Henrik Qvigstad <henrik.qvigstad@cern.ch>

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation
version 2.1 of the License.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

class TList;
class AliVEvent;
class TRefArray;
class AliVVertex;
class AliVCaloCells;
class EtaHandler;
class TH1F;
class TH1;

#include <string>


#include "AliAnalysisTaskSE.h"

class EECalibTask : public AliAnalysisTaskSE
{
 public:
  EECalibTask(const char* name = "EECalibTask");
  virtual ~EECalibTask();

  virtual void UserCreateOutputObjects();
  virtual Bool_t UserNotify();
  virtual void UserExec(Option_t * );
  virtual void Terminate(Option_t *);

 protected:
  EECalibTask(const EECalibTask&);//Not Implemented
  EECalibTask& operator=(const EECalibTask& );//Not Implemented

  TTree* fOutput;

  ClassDef(EECalibTask, 1);
};

#endif
