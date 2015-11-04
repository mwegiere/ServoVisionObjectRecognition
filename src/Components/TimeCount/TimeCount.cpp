/*!
 * \file
 * \brief
 * \author Maciej Wegierek
 */

#include <memory>
#include <string>

#include "TimeCount.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace TimeCount {

TimeCount::TimeCount(const std::string & name) :
		Base::Component(name)  {

}

TimeCount::~TimeCount() {
}

void TimeCount::prepareInterface() {
	// Register data streams, events and event handlers HERE!
    registerStream("time", &in_time_nsec);
	// Register handlers
    h_onNewData.setup(boost::bind(&TimeCount::onNewData, this));
    registerHandler("onNewData", &h_onNewData);
    addDependency("onNewData", &in_time_nsec);

}

bool TimeCount::onInit() {

	return true;
}

bool TimeCount::onFinish() {
	return true;
}

bool TimeCount::onStop() {
	return true;
}

bool TimeCount::onStart() {
	return true;
}

void TimeCount::onNewData() {
    int time_nsec = in_time_nsec.read();
    CLOG(LINFO) << "TIME COUNT: " << time_nsec;
}



} //: namespace TimeCount
} //: namespace Processors
