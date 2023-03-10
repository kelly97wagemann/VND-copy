/****************************************************************************/
// Eclipse SUMO, Simulation of Urban MObility; see https://eclipse.org/sumo
// Copyright (C) 2001-2017 German Aerospace Center (DLR) and others.
/****************************************************************************/
//
//   This program and the accompanying materials
//   are made available under the terms of the Eclipse Public License v2.0
//   which accompanies this distribution, and is available at
//   http://www.eclipse.org/legal/epl-v20.html
//
/****************************************************************************/
/// @file    MSCFModel_Krauss.cpp
/// @author  Tobias Mayer
/// @author  Daniel Krajzewicz
/// @author  Jakob Erdmann
/// @author  Michael Behrisch
/// @author  Laura Bieker
/// @date    Mon, 04 Aug 2009
/// @version $Id$
///
// Krauss car-following model, with acceleration decrease and faster start
/****************************************************************************/


// ===========================================================================
// included modules
// ===========================================================================
#ifdef _MSC_VER
#include <windows_config.h>
#else
#include <config.h>
#endif

#include <microsim/MSVehicle.h>
#include <microsim/MSLane.h>
#include <microsim/MSGlobals.h>
#include "MSCFModel_BAM.h"
#include <microsim/lcmodels/MSAbstractLaneChangeModel.h>
#include <utils/common/RandHelper.h>



// ===========================================================================
// DEBUG constants
// ===========================================================================
//#define DEBUG_MOVE_HELPER
//#define DEBUG_COND (true)


// ===========================================================================
// method definitions
// ===========================================================================
MSCFModel_BAM::MSCFModel_BAM(const MSVehicleType* vtype, double accel, double decel,
                                   double emergencyDecel, double apparentDecel,
                                   double dawdle, double headwayTime) :
    MSCFModel_KraussOrig1(vtype, accel, decel, emergencyDecel, apparentDecel, dawdle, headwayTime) {
}


MSCFModel_BAM::~MSCFModel_BAM() {}


double
MSCFModel_BAM::moveHelper(MSVehicle* const veh, double vPos) const {
    const double oldV = veh->getSpeed(); // save old v for optional acceleration computation
    const double vSafe = MIN2(vPos, veh->processNextStop(vPos)); // process stops
    const double vMin = minNextSpeed(oldV, veh);

    // aMax: Maximal admissible acceleration until the next action step, such that the vehicle's maximal
    // desired speed on the current lane will not be exceeded.
    double aMax = (veh->getLane()->getVehicleMaxSpeed(veh) - oldV) / veh->getActionStepLengthSecs();

    // do not exceed max decel even if it is unsafe
    double vMax = MAX2(vMin, MIN3(oldV + ACCEL2SPEED(aMax), maxNextSpeed(oldV, veh), vSafe));
#ifdef _DEBUG
    //if (vMin > vMax) {
    //    WRITE_WARNING("Maximum speed of vehicle '" + veh->getID() + "' is lower than the minimum speed (min: " + toString(vMin) + ", max: " + toString(vMax) + ").");
    //}
#endif

    const double sigma = (veh->passingMinor()
                          ? veh->getVehicleType().getParameter().getJMParam(SUMO_ATTR_JM_SIGMA_MINOR, myDawdle)
                          : myDawdle);
    const double vDawdle = MAX2(vMin, dawdle2(vMax, sigma));

    double vNext = veh->getLaneChangeModel().patchSpeed(vMin, vDawdle, vMax, *this);

    assert(vNext >= vMin);
    assert(vNext <= vMax);

#ifdef DEBUG_MOVE_HELPER
    if DEBUG_COND {
    std::cout << "\nMOVE_HELPER\n"
    << "veh '" << veh->getID() << "' vMin=" << vMin
        << " vMax=" << vMax << " vDawdle=" << vDawdle
        << " vSafe" << vSafe << " vNext=" << vNext << " vPos=" << vPos << " veh->getSpeed()=" << oldV
        << "\n";
    }
#endif

    return vNext;
}



double
MSCFModel_BAM::stopSpeed(const MSVehicle* const veh, const double speed, double gap) const {
    // NOTE: This allows return of smaller values than minNextSpeed().
    // Only relevant for the ballistic update: We give the argument headway=veh->getActionStepLengthSecs(), to assure that
    // the stopping position is approached with a uniform deceleration also for tau!=veh->getActionStepLengthSecs().
    return MIN2(maximumSafeStopSpeed(gap, speed, false, veh->getActionStepLengthSecs()), maxNextSpeed(speed, veh));
}


double
MSCFModel_BAM::followSpeed(const MSVehicle* const veh, double speed, double gap, double predSpeed, double predMaxDecel) const {
    const double vsafe = maximumSafeFollowSpeed(gap, speed, predSpeed, predMaxDecel);
    const double vmin = minNextSpeed(speed);
    const double vmax = maxNextSpeed(speed, veh);
    if (MSGlobals::gSemiImplicitEulerUpdate) {
        return MIN2(vsafe, vmax);
    } else {
        // ballistic
        // XXX: the euler variant can break as strong as it wishes immediately! The ballistic cannot, refs. #2575.
        return MAX2(MIN2(vsafe, vmax), vmin);
    }
}


double
MSCFModel_BAM::dawdle2(double speed, double sigma) const {
    if (!MSGlobals::gSemiImplicitEulerUpdate) {
        // in case of the ballistic update, negative speeds indicate
        // a desired stop before the completion of the next timestep.
        // We do not allow dawdling to overwrite this indication
        if (speed < 0) {
            return speed;
        }
    }
    // generate random number out of [0,1)
    const double random = RandHelper::rand();
    // Dawdle.
    if (speed < myAccel) {
        // we should not prevent vehicles from driving just due to dawdling
        //  if someone is starting, he should definitely start
        // (but what about slow-to-start?)!!!
        speed -= ACCEL2SPEED(sigma * speed * random);
    } else {
        speed -= ACCEL2SPEED(sigma * myAccel * random);
    }
    return MAX2(0., speed);
}


MSCFModel*
MSCFModel_BAM::duplicate(const MSVehicleType* vtype) const {
    return new MSCFModel_BAM(vtype, myAccel, myDecel, myEmergencyDecel, myApparentDecel, myDawdle, myHeadwayTime);
}


/****************************************************************************/
