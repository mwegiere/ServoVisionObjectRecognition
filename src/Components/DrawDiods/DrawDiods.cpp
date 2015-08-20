/*!
 * \file
 * \brief
 * \author Maciej Wegierek
 */

#include <memory>
#include <string>

#include "DrawDiods.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>

namespace Processors {
namespace DrawDiods {

DrawDiods::DrawDiods(const std::string & name) :
		Base::Component(name)  {

}

DrawDiods::~DrawDiods() {
}

void DrawDiods::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_img", &in_img);
    registerStream("diodPoints", &diodPoints);
	registerStream("out_img", &out_img);
	// Register handlers
	registerHandler("Process", boost::bind(&DrawDiods::Process, this));
    addDependency("Process", &diodPoints);
	addDependency("Process", &in_img);

}

bool DrawDiods::onInit() {

	return true;
}

bool DrawDiods::onFinish() {
	return true;
}

bool DrawDiods::onStop() {
	return true;
}

bool DrawDiods::onStart() {
	return true;
}

void DrawDiods::Process() {

    std::vector<cv::Point2f> points = diodPoints.read();
    cv::Mat _I = in_img.read();
    cv::Mat I = _I.clone();

    CV_Assert(I.depth() != sizeof(uchar));
    cv::Mat res(I.rows, I.cols, CV_8UC3);
    cv::Mat_<cv::Vec3b> _R = I;
    circle(_R, cv::Point((int)points.at(0).x,(int)points.at(0).y), 3, cv::Scalar(255,0,0,0));
    circle(_R, cv::Point((int)points.at(1).x,(int)points.at(1).y), 3, cv::Scalar(0,255,0,0));
    circle(_R, cv::Point((int)points.at(2).x,(int)points.at(2).y), 3, cv::Scalar(0,0,255,0));
    circle(_R, cv::Point((int)points.at(3).x,(int)points.at(3).y), 3, cv::Scalar(255,255,255,0));

    res = _R;
    out_img.write(I);

}


} //: namespace DrawDiods
} //: namespace Processors
