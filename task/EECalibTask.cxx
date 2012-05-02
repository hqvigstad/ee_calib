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

#include "EECalibTask.h"

#include "TChain.h"
#include "TLorentzVector.h"
#include "TList.h"
#include "TCanvas.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TMath.h"
#include "AliVEvent.h"
#include "AliESDEvent.h"
#include "AliMCEvent.h"
#include "AliESDVertex.h"
#include "AliESDCaloCluster.h"
#include "TRefArray.h"
#include "TArrayI.h"
#include "AliMCEvent.h"
#include "AliESDCaloCells.h"
//#include "AliPHOSGeometry.h"
//#include "TGeoManager.h"

#include <vector>
#include <iostream>
#include <fstream>
#include <stdexcept>

ClassImp(EECalibTask)

