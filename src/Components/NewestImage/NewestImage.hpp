/*!
 * \file
 * \brief 
 * \author Mikołaj Kojdecki
 */

#ifndef NEWESTIMAGE_HPP_
#define NEWESTIMAGE_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"

#include <opencv2/opencv.hpp>


namespace Processors {
namespace NewestImage {

/*!
 * \class NewestImage
 * \brief NewestImage processor class.
 *
 * 
 */
class NewestImage: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	NewestImage(const std::string & name = "NewestImage");

	/*!
	 * Destructor
	 */
	virtual ~NewestImage();

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
	Base::DataStreamIn<cv::Mat, Base::DataStreamBuffer::Newest> in_img;

	// Output data streams
	Base::DataStreamOut<cv::Mat> out_img;

	// Handlers
	Base::EventHandler2 h_onImage;

	// Properties

	
	// Handlers
	void onImage();

};

} //: namespace NewestImage
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("NewestImage", Processors::NewestImage::NewestImage)

#endif /* NEWESTIMAGE_HPP_ */
