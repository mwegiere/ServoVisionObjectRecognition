/*
 *  Created on: 06-07-2015
 *      Author: Maciej Węgierek
 */

#ifndef DIODRECOGNITION_HPP_
#define DIODRECOGNITION_HPP_

#include "Component_Aux.hpp"
#include "Types/Drawable.hpp"
#include "Timer.hpp"
#include "Property.hpp"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "Types/Objects3D/GridPattern.hpp"
#include "Types/ImagePosition.hpp"

namespace Processors {
namespace DiodRecognition {

class DiodRecognition: public Base::Component
{
public:
	DiodRecognition(const std::string & name = "");
	virtual ~DiodRecognition();

	/*!
	 * Prepares communication interface.
	 */
	virtual void prepareInterface();


protected:
	/*!
	 * Method called when component is started
	 * \return true on success
	 */
	virtual bool onStart();

	/*!
	 * Method called when component is stopped
	 * \return true on success
	 */
	virtual bool onStop();

	/*!
	 * Method called when component is initialized
	 * \return true on success
	 */
	virtual bool onInit();

	/*!
	 * Method called when component is finished
	 * \return true on success
	 */
	virtual bool onFinish();

	/*!
	 * Method called when step is called
	 * \return true on success
	 */
	virtual bool onStep();


private:
	void onNewImage();
	void initGridPattern();
	void sizeCallback();

	// Input image stream.
	Base::DataStreamIn <cv::Mat> in_img;

	// Output grid pattern.
    Base::DataStreamOut <Types::Objects3D::Object3D> out_gridPattern;

	// Output image position.
	Base::DataStreamOut <Types::ImagePosition> out_imagePosition;

	//Output image
	Base::DataStreamOut <cv::Mat> out_img;

    // Output 0/1 image found/not found
    Base::DataStreamOut <int> found;

	// Output diod points
    Base::DataStreamOut <std::vector<cv::Point2f> > diodPoints;

	//Common::Timer timer;

    Types::Objects3D::Object3D gridPattern;
};

} // namespace DiodRecognition {

} // namespace Processors {

REGISTER_COMPONENT("DiodRecognition", Processors::DiodRecognition::DiodRecognition)

#endif /* DIODRECOGNITION_HPP_ */
