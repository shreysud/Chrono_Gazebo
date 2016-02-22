// =============================================================================
// PROJECT CHRONO - http://projectchrono.org
//
// Copyright (c) 2014 projectchrono.org
// All right reserved.
//
// Use of this source code is governed by a BSD-style license that can be found
// in the LICENSE file at the top level of the distribution and at
// http://projectchrono.org/license-chrono.txt.
//
// =============================================================================
// Authors: Asher Elmquist
// =============================================================================
//
//gcVehicle defines a vehicle based on chrono_vehicle and gazebo relying on the
//dynamics and definitions provided by chrono and the world geometry, sensors,
//and visuals within gazebo
//
// =============================================================================

#include "GcVehicleBuilder.hh"

#include <boost/bind/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <chrono_vehicle/ChVehicleModelData.h>
#include <chrono_vehicle/driver/ChPathFollowerDriver.h>
#include <chrono_vehicle/powertrain/SimplePowertrain.h>
#include <chrono_vehicle/wheeled_vehicle/tire/RigidTire.h>
#include <chrono_vehicle/wheeled_vehicle/vehicle/WheeledVehicle.h>
#include <core/ChSmartpointers.h>
#include <gazebo/physics/PhysicsTypes.hh>
#include <gazebo/sensors/SensorManager.hh>
#include <gazebo/sensors/SensorTypes.hh>
#include <gazebo/physics/World.hh>
#include <ros/callback_queue.h>
#include <ros/node_handle.h>
#include <ros/subscribe_options.h>
#include <std_msgs/Float64.h>
#include <iostream>
#include <vector>

#include "GcVehicle.hh"

GcVehicleBuilder::GcVehicleBuilder(physics::WorldPtr world, ChSystem *chsys,
		ChSharedPtr<vehicle::RigidTerrain> terrain, const double stepSize) :
		world(world), chsys(chsys), terrain(terrain), stepSize(stepSize) {
}

boost::shared_ptr<GcVehicle> GcVehicleBuilder::buildGcVehicle() {

	const std::string id = std::to_string(vehId);

	auto veh = ChSharedPtr<vehicle::WheeledVehicle>(
			new vehicle::WheeledVehicle(chsys, vehicleFile));
	veh->Initialize(coordsys);
	const int numWheels = 2 * veh->GetNumberAxles();

	auto driver = ChSharedPtr<vehicle::ChPathFollowerDriver>(
			new vehicle::ChPathFollowerDriver(*veh, steerFile, speedFile, path,
					std::string("my_path"), 0.0));

	auto powertrain = ChSharedPtr<vehicle::SimplePowertrain>(
			new vehicle::SimplePowertrain(powertrainFile));
	powertrain->Initialize();

	auto tires = std::vector<ChSharedPtr<vehicle::RigidTire>>(numWheels);

	for (int i = 0; i < numWheels; i++) {
		//create the tires from the tire file
		tires[i] = ChSharedPtr<vehicle::RigidTire>(
				new vehicle::RigidTire(tireFile));
		tires[i]->Initialize(veh->GetWheelBody(i));
	}

	physics::ModelPtr gazeboVehicle;
	std::vector<physics::ModelPtr> gazeboWheels(numWheels);
	sensors::RaySensorPtr raySensor;

	if ((gazeboVehicle = world->GetModel("vehicle" + id)) == NULL) {
		std::cerr << "COULD NOT FIND GAZEBO MODEL: vehicle" + id + '\n';
	}
	for (int i = 1; i < numWheels; i++) {
		physics::ModelPtr wheelPtr;
		const std::string wheelName = "wheel" + id + "_" + std::to_string(i);
		if ((wheelPtr = world->GetModel(wheelName)) != NULL) {
			gazeboWheels.push_back(wheelPtr);
		} else {
			std::cerr << "COULD NOT FIND GAZEBO MODEL: " + wheelName + '\n';
		}
	}
	const std::string sensorName = world->GetName() + "::vehicle" + id
			+ "::chassis" + id + "::laser";
	if ((raySensor = boost::dynamic_pointer_cast<sensors::RaySensor>(
			sensors::SensorManager::Instance()->GetSensor(sensorName))) == NULL) {
		std::cerr << "COULD NOT FIND LASER SENSOR " + id + '\n';
	}

	auto gcVeh = boost::shared_ptr<GcVehicle>(
			new GcVehicle(vehId, terrain, veh, powertrain, tires, driver,
					maxSpeed, raySensor, gazeboVehicle, gazeboWheels,
					stepSize));

	auto opt = ros::SubscribeOptions::create<std_msgs::Float64>(
			"/track_point" + std::to_string(vehId), 1,
			boost::bind(&GcVehicle::updateDriver, gcVeh, _1), ros::VoidPtr(),
			queue);
	lastSub = handle->subscribe(opt);

	vehId++;

	return gcVeh;
}

ros::Subscriber &GcVehicleBuilder::getLastRosSubscriber() {
	return lastSub;
}

void GcVehicleBuilder::setVehicleFile(const std::string &vehicleFile) {
	this->vehicleFile = vehicle::GetDataFile(vehicleFile);
}

void GcVehicleBuilder::setPowertrainFile(const std::string &powertrainFile) {
	this->powertrainFile = vehicle::GetDataFile(powertrainFile);
}

void GcVehicleBuilder::setTireFile(const std::string &tireFile) {
	this->tireFile = vehicle::GetDataFile(tireFile);
}

void GcVehicleBuilder::setSteerCtrlFile(const std::string &steerFile) {
	this->steerFile = vehicle::GetDataFile(steerFile);
}

void GcVehicleBuilder::setSpeedCtrlFile(const std::string &speedFile) {
	this->speedFile = vehicle::GetDataFile(speedFile);
}

void GcVehicleBuilder::setInitCoordsys(const ChCoordsys<> &coordsys) {
	this->coordsys = coordsys;
}

void GcVehicleBuilder::setMaxSpeed(const double maxSpeed) {
	this->maxSpeed = maxSpeed;
}

void GcVehicleBuilder::setPath(ChBezierCurve *const path) {
	this->path = path;
}

void GcVehicleBuilder::setNodeHandler(ros::NodeHandle *const handle) {
	this->handle = handle;
}

void GcVehicleBuilder::setCallbackQueue(ros::CallbackQueue *const queue) {
	this->queue = queue;
}