/*!
 * \file
 * \brief 
 * \author Maciej Węgierek
 */

#ifndef ROSPROXY_HPP_
#define ROSPROXY_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include "ros/ros.h"

namespace Processors {
namespace ROSProxy {

/*!
 * \class ROSProxy
 * \brief ROSProxy processor class.
 *
 * Description TODO
 */
class ROSProxy: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	ROSProxy(const std::string & name = "ROSProxy");

	/*!
	 * Destructor
	 */
	virtual ~ROSProxy();

	/*!
	 * Prepare components interface (register streams and handlers).
	 * At this point, all properties are already initialized and loaded to 
	 * values set in config file.
	 */
	void prepareInterface();

protected:

	/*!
	 * Connects source to given device.
	 */
	bool onInit();

	/*!
	 * Disconnect source from device, closes streams, etc.
	 */
	bool onFinish();

	/*!
	 * Start component
	 */
	bool onStart();

	/*!
	 * Stop component
	 */
	bool onStop();


	// Input data streams
        Base::DataStreamIn<vector<double> > in_matrix;

        Base::DataStreamIn<int> in_found;

    	Base::DataStreamIn<long int> in_time_nsec_pocz;

    	Base::DataStreamIn<long int> in_time_sec_pocz;

    	Base::DataStreamIn<long int> in_time_nsec_kon;

   	 Base::DataStreamIn<long int> in_time_sec_kon;

	// Output data streams

	// Handlers
	Base::EventHandler2 h_onNewData;

	// Properties
	Base::Property<std::string> topic_name;

	// Handlers
	void onNewData();

	// Others
	void onTopicNameChanged(const std::string & old_value, const std::string & new_value);

	ros::Publisher pub;
	ros::NodeHandle * nh;
};

} //: namespace ROSProxy
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("ROSProxy", Processors::ROSProxy::ROSProxy)

#endif /* ROSPROXY_HPP_ */
