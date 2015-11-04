/*!
 * \file
 * \brief
 * \author Maciej Węgierek
 */

#include <memory>
#include <string>

#include "ROSProxy.hpp"
#include "Common/Logger.hpp"

#include "serwo/SerwoInfo.h"

#include <boost/bind.hpp>

namespace Processors {
namespace ROSProxy {

ROSProxy::ROSProxy(const std::string & name) :
		Base::Component(name) , 
		topic_name("topic_name", boost::bind(&ROSProxy::onTopicNameChanged, this, _1, _2), "image_data") {
	registerProperty(topic_name);

}

ROSProxy::~ROSProxy() {
}

void ROSProxy::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_matrix", &in_matrix);
    registerStream("in_found", &in_found);
	registerStream("in_time_nsec_pocz", &in_time_nsec_pocz);
	registerStream("in_time_sec_pocz", &in_time_sec_pocz);
	registerStream("in_time_nsec_kon", &in_time_nsec_kon);
	registerStream("in_time_sec_kon", &in_time_sec_kon);

	// Register handlers
	h_onNewData.setup(boost::bind(&ROSProxy::onNewData, this));
	registerHandler("onNewData", &h_onNewData);
    addDependency("onNewData", &in_matrix);
    addDependency("onNewData", &in_found);
    addDependency("onNewData", &in_time_nsec_pocz);
    addDependency("onNewData", &in_time_sec_pocz);
    addDependency("onNewData", &in_time_nsec_kon);
    addDependency("onNewData", &in_time_sec_kon);

}

bool ROSProxy::onInit() {
	static char * tmp = NULL;
	static int tmpi;
	ros::init(tmpi, &tmp, "discode_irp6_checkers", ros::init_options::NoSigintHandler);
	nh = new ros::NodeHandle;
	pub = nh->advertise<serwo::SerwoInfo>(topic_name, 3);
	return true;
}

bool ROSProxy::onFinish() {
	delete nh;
	return true;
}

bool ROSProxy::onStop() {
	return true;
}

bool ROSProxy::onStart() {
	return true;
}

void ROSProxy::onNewData() {

	std::vector<float> matrix = in_matrix.read();
    int found = in_found.read();
    long int time_nsec_pocz = in_time_nsec_pocz.read();
    long int time_sec_pocz = in_time_sec_pocz.read();
    long int time_nsec_kon = in_time_nsec_kon.read();
    long int time_sec_kon = in_time_sec_kon.read();

	serwo::SerwoInfo msg;

	msg.matrix = matrix;
	msg.found = found;
    msg.out_time_nsec_pocz = time_nsec_pocz;
    msg.out_time_sec_pocz = time_sec_pocz;
    msg.out_time_nsec_kon = time_nsec_kon;
    msg.out_time_sec_kon = time_sec_kon;

	pub.publish(msg);
	ros::spinOnce();
}

void ROSProxy::onTopicNameChanged(const std::string & old_value, const std::string & new_value) {
}


} //: namespace ROSProxy
} //: namespace Processors
