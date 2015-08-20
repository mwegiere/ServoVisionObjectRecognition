/*!
 * \file
 * \brief 
 * \author Maciej Wegierek
 */

#ifndef DRAWDIODS_HPP_
#define DRAWDIODS_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace DrawDiods {

/*!
 * \class DrawDiods
 * \brief DrawDiods processor class.
 *
 * 
 */
class DrawDiods: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	DrawDiods(const std::string & name = "DrawDiods");

	/*!
	 * Destructor
	 */
	virtual ~DrawDiods();

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
	Base::DataStreamIn<cv::Mat> in_img;
    Base::DataStreamIn <std::vector<cv::Point2f> > diodPoints;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers

	// Properties

	
	// Handlers
	void Process();



};

} //: namespace DrawDiods
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("DrawDiods", Processors::DrawDiods::DrawDiods)

#endif /* DRAWDIODS_HPP_ */
