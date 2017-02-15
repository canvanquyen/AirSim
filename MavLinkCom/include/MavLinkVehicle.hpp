// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#ifndef MavLinkCom_MavLinkCom_hpp
#define MavLinkCom_MavLinkCom_hpp

#include "VehicleState.hpp"
#include <memory>
#include <string>
#include <vector>
#include "MavLinkNode.hpp"
#include "AsyncResult.hpp"

namespace mavlinkcom_impl {
	class MavLinkVehicleImpl;
}

namespace mavlinkcom {

	// This class represents a MavLinkNode that can be controlled, hence a "vehicle" of some sort.
	// It also keeps certain state about the vehicle position so you can query it any time.
	class MavLinkVehicle : public MavLinkNode {
	public:
		MavLinkVehicle(int local_system_id, int local_component_id);

		// Send command to arm or disarm the drone.  Drone will not fly until it is armed successfully.
		// It returns false if the command is rejected.
		AsyncResult<bool> armDisarm(bool arm);
		AsyncResult<bool> takeoff(float altitude = 2.5, float pitch = 0, float yaw = 0);
		AsyncResult<bool> land(float yaw, float lat = 0, float lon = 0, float altitude = 0);
		AsyncResult<bool> returnToHome();
		AsyncResult<bool> loiter();

		void setStabilizedFlightMode();
		void setHomePosition(float lat = 0, float lon = 0, float alt = 0);
		void setAutoMode();
		AsyncResult<MavLinkHomePosition> waitForHomePosition();
		AsyncResult<bool> allowFlightControlOverUsb();

		// request OFFBOARD control.  
		void requestControl();
		// release OFFBOARD control
		void releaseControl();
		// return true if we still have offboard control (can lose this if user flips the switch).
		bool hasOffboardControl();

		// send this to keep offboard control but do no movement.
		void offboardIdle();
		// offboard control methods.
		bool isLocalControlSupported();
		void moveToLocalPosition(float x, float y, float z, bool isYaw, float yawOrRate);
		void moveToGlobalPosition(float lat, float lon, float alt, bool isYaw, float yawOrRate);
		void moveByLocalVelocity(float vx, float vy, float vz, bool isYaw, float yawOrRate);
		void moveByLocalVelocityWithAltHold(float vx, float vy, float z, bool isYaw, float yawOrRate);

		// low level control, only use this one if you really know what you are doing!!
		bool isAttitudeControlSupported();

		// Move drone by directly controlling the attitude of the drone (units are degrees).
		// If the rollRate, pitchRate and yawRate are all zero then you will get the default rates provided by the drone.
		void moveByAttitude(float roll, float pitch, float yaw, float rollRate, float pitchRate, float yawRate, float thrust);
		 
		uint32_t getTimeStamp();
		int getVehicleStateVersion();
		const VehicleState& getVehicleState();

	public:
		//needed for piml pattern
		MavLinkVehicle();
		~MavLinkVehicle();
		//MavLinkVehicle(MavLinkVehicle&&);
	};
}

#endif
