/*!
 * \file
 * \brief 
 * \author Maciej Wegierek
 */

#ifndef TIMECOUNT_HPP_
#define TIMECOUNT_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"



namespace Processors {
namespace TimeCount {

/*!
 * \class TimeCount
 * \brief TimeCount processor class.
 *
 * 
 */
class TimeCount: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	TimeCount(const std::string & name = "TimeCount");

	/*!
	 * Destructor
	 */
	virtual ~TimeCount();

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
    Base::DataStreamIn<int> in_time_nsec;

	// Output data streams

	// Handlers

	// Properties

    // Handlers
    Base::EventHandler2 h_onNewData;

    void onNewData();

};

} //: namespace TimeCount
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("TimeCount", Processors::TimeCount::TimeCount)

#endif /* TIMECOUNT_HPP_ */
