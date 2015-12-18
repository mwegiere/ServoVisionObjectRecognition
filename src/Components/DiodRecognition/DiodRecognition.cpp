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
			
                if (b - (r+g) > maxB + 4){
					wsp_maxB.y = i;
                    wsp_maxB.x = j;
					maxB = b - (r+g);
				}

                if (g- (r+b) > maxG + 4){
					wsp_maxG.y = i;
					wsp_maxG.x = j;
					maxG = g- (r+b);
				}

                if (r- (g+b) > maxR + 4){
					wsp_maxR.y = i;
					wsp_maxR.x = j;
					maxR = r- (g+b);
				}
				
                if (r + g + b > maxW +4){
					wsp_maxW.y = i;
					wsp_maxW.x = j;
					maxW = r + g + b;
				}
			}
        }
        //std::cout<<maxB<<std::endl;
        int licznik = 0;
        float suma_i = 0.0;
        float suma_j = 0.0;
        int i = 0;
        int j = 0;
        if (wsp_maxB.y - 3 > 0 && wsp_maxB.y + 3 < rows && wsp_maxB.x - 3 > 0 && wsp_maxB.x + 3 < cols){
            for (i = wsp_maxB.y - 3; i < wsp_maxB.y + 3; ++i) {
                ptr = image.ptr(i);
                for (j = wsp_maxB.x - 3; j < wsp_maxB.x + 3; ++j) {
                    if (ptr[3*j] > 0 || ptr[3*j+1] > 0 || ptr[3*j+2] > 0){
                        licznik += ptr[3*j];
                        suma_i += i*ptr[3*j];
                        suma_j += j*ptr[3*j];
                    }
                }
            }
            wsp_maxB.y = (int)(suma_i/licznik);
            wsp_maxB.x = (int)(suma_j/licznik);
        }

        licznik = 0;
        suma_i = 0.0;
        suma_j = 0.0;
        if (wsp_maxG.y - 3 > 0 && wsp_maxG.y + 3 < rows && wsp_maxG.x - 3 > 0 && wsp_maxG.x + 3 < cols){
            for (i = wsp_maxG.y - 3; i < wsp_maxG.y + 3; ++i) {
                ptr = image.ptr(i);
                for (j = wsp_maxG.x - 3; j < wsp_maxG.x + 3; ++j) {
                    if (ptr[3*j] > 0 && ptr[3*j+1] > 0 && ptr[3*j+2] > 0){
                        licznik += ptr[3*j+1];
                        suma_i += i*ptr[3*j+1];
                        suma_j += j*ptr[3*j+1];
                    }
                }
            }
            wsp_maxG.y = (int)suma_i/licznik;
            wsp_maxG.x = (int)suma_j/licznik;
        }


        licznik = 0;
        suma_i = 0.0;
        suma_j = 0.0;
        if (wsp_maxR.y - 3 > 0 && wsp_maxR.y + 3 < rows && wsp_maxR.x - 3 > 0 && wsp_maxR.x + 3 < cols){
            for (i = wsp_maxR.y - 3; i < wsp_maxR.y + 3; ++i) {
                ptr = image.ptr(i);
                for (j = wsp_maxR.x - 3; j < wsp_maxR.x + 3; ++j) {
                    if (ptr[3*j] > 0 && ptr[3*j+1] > 0 && ptr[3*j+2] > 0){
                        licznik += ptr[3*j+1];
                        suma_i += i*ptr[3*j+1];
                        suma_j += j*ptr[3*j+1];
                    }
                }
            }
            wsp_maxR.y = (int)suma_i/licznik;
            wsp_maxR.x = (int)suma_j/licznik;
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
            //std::cout<<wsp_maxB<<std::endl;
            //std::cout<<wsp_maxG<<std::endl;
            //std::cout<<wsp_maxR<<std::endl;
            //std::cout<<wsp_maxW<<std::endl;
            //std::cout<<"---------"<<std::endl;
	    
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
