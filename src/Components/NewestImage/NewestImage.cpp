/*!
 * \file
 * \brief
 * \author Mikołaj Kojdecki
 */

#include <memory>
#include <string>

#include "NewestImage.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace NewestImage {

NewestImage::NewestImage(const std::string & name) :
		Base::Component(name)  {

}

NewestImage::~NewestImage() {
}

void NewestImage::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
	registerStream("out_img", &out_img);
	// Register handlers
	h_onImage.setup(boost::bind(&NewestImage::onImage, this));
	registerHandler("onImage", &h_onImage);
	addDependency("onImage", &in_img);

}

bool NewestImage::onInit() {

	return true;
}

bool NewestImage::onFinish() {
	return true;
}

bool NewestImage::onStop() {
	return true;
}

bool NewestImage::onStart() {
	return true;
}

void NewestImage::onImage() {
	out_img.write(in_img.read());
}



} //: namespace NewestImage
} //: namespace Processors
