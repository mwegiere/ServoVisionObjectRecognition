/*
 *  Created on: 06-07-2015
 *      Author: Maciej Węgierek
 */

#include "DiodRecognition.hpp"
#include <boost/bind.hpp>
#include <cmath>

namespace Processors {

namespace DiodRecognition {

DiodRecognition::DiodRecognition(const std::string & name) :
	Component(name)
{
}

DiodRecognition::~DiodRecognition()
{
}

void DiodRecognition::prepareInterface() {
	CLOG(LTRACE) << "DiodRecognition::prepareInterface\n";

	// Register data streams.
	registerStream("in_img", &in_img);
	registerStream("out_gridPattern", &out_gridPattern);
	registerStream("out_imagePosition", &out_imagePosition);
	registerStream("out_img", &out_img);
	registerStream("found", &found);
	registerStream("diodPoints", &diodPoints);

	// Register handler responsible for finding grid pattern.
	registerHandler("onNewImage", boost::bind(&DiodRecognition::onNewImage, this));
	addDependency("onNewImage", &in_img);
}

bool DiodRecognition::onFinish()
{
	return true;
}

bool DiodRecognition::onStop()
{
	return true;
}

bool DiodRecognition::onInit()
{
	initGridPattern();
	CLOG(LTRACE) << "component initialized\n";
	return true;
}

void DiodRecognition::initGridPattern() {

	vector <cv::Point3f> modelPoints;

	modelPoints.push_back(cv::Point3f(0.04813,0.00802, 0)); //B
        modelPoints.push_back(cv::Point3f(0.00943,0.0457, 0)); //G
        modelPoints.push_back(cv::Point3f(0.090827,0.04535, 0)); //R
	modelPoints.push_back(cv::Point3f(0.09066,0.004626, 0)); //W

	// Set model points.
	gridPattern.setModelPoints(modelPoints);
	CLOG(LWARNING) << gridPattern.getModelPoints();
}

void DiodRecognition::sizeCallback() {
	initGridPattern();
}

bool DiodRecognition::onStart()
{
	return true;
}

bool DiodRecognition::onStep()
{
	return true;
}

void DiodRecognition::onNewImage()
{
	CLOG(LTRACE) << "void DiodRecognition::onNewImage() begin\n";
	try {
		if(in_img.empty()){
			return;
		}
		// Retrieve image from the stream.
		cv::Mat image = in_img.read();//.clone();

        	float _found = 0.0;

		double sumaB = 0;
		double sumaG = 0;
		double sumaR = 0;

		//wsp maksymalnych składowych
		cv::Point2f wsp_maxB;
		cv::Point2f wsp_maxG;
		cv::Point2f wsp_maxR;
		cv::Point2f wsp_maxW;

		//maksymalne składowe
		int maxB = 0;
		double maxBminG = 255;
		double maxBminR = 255;

		int maxG = 0;
		double maxGminB = 255;
		double maxGminR = 255;

		int maxR = 0;
		double maxRminB = 255;
		double maxRminG = 255;

		int maxW = 0;
		
		int rows = image.size().height;
		int cols = image.size().width;
		
		uchar * ptr;
		for (int i = 3; i < rows; ++i) {
			ptr = image.ptr(i);
			for (int j = 3; j < cols; ++j) {
				int b = ptr[3*j];
				int g = ptr[3*j+1];
				int r = ptr[3*j+2];
			
				if (b - (r+g) > maxB){
					wsp_maxB.y = i;
					wsp_maxB.x = j;
					maxB = b - (r+g);
				}

				if (g- (r+b) > maxG ){
					wsp_maxG.y = i;
					wsp_maxG.x = j;
					maxG = g- (r+b);
				}

				if (r- (g+b) > maxR){
					wsp_maxR.y = i;
					wsp_maxR.x = j;
					maxR = r- (g+b);
				}
				
				if (r + g + b > maxW){
					wsp_maxW.y = i;
					wsp_maxW.x = j;
					maxW = r + g + b;
				}
			}
		}
        int prog = 50;
        std::vector<cv::Point2f> gridPoints;	
	cv::Point2f wsp_zero;
	wsp_zero.x = 0.0;
	wsp_zero.y = 0.0;

        if (maxB > prog && maxG > prog && maxR > prog && maxW > prog){
            _found = 1.0;
            gridPoints.push_back(wsp_maxB);
            gridPoints.push_back(wsp_maxG);
            gridPoints.push_back(wsp_maxR);
            gridPoints.push_back(wsp_maxW);
	    
            diodPoints.write(gridPoints);
	    gridPattern.setImagePoints(gridPoints); 
            out_gridPattern.write(gridPattern);
	    CLOG(LWARNING)<<"Diods found!!!\n\n\n";
        }

	else {
            CLOG(LWARNING) << "Diods NOT found!!!\n\n\n";
        }

        out_img.write(image);
	//0 lub 1 jesli plansza znaleziona lub nie znaleziona
	found.write(_found);

	} catch (const cv::Exception& e) {
		CLOG(LERROR) << e.what() << "\n";
	}

	CLOG(LTRACE) << "void DiodRecognition::onNewImage() end\n";
}

} // namespace DiodRecognition{
} // namespace Processors {
