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

	modelPoints.push_back(cv::Point3f(0.055,0.006, 0)); //B
	modelPoints.push_back(cv::Point3f(0.009,0.045, 0)); //G
	modelPoints.push_back(cv::Point3f(0.091,0.044, 0)); //R
	modelPoints.push_back(cv::Point3f(0.091,0.004, 0)); //W

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

double moment(double p, double q, cv::Mat& _I, cv::Vec3b color, int minRows, int maxRows, int minCols, int maxCols) {
    cv::Mat_<cv::Vec3b> I = _I;
    double m = 0.0;
    for (int i = minRows; i <= maxRows; ++i)
        for (int j = minCols; j <= maxCols; ++j) {
            if (I(i, j) != color && i >= minRows && i<= maxRows && j>=minCols && j<=maxCols)
                m += (pow((double) i, p)) * (pow((double) j, q));
        }
    return m;
}

cv::Vec3b pobierzKolor(cv::Mat& image, int i, int j){
	cv::Mat_<cv::Vec3b> _I = image;
	if (i > 0 && i< _I.rows && j > 0 && j < _I.cols){
		double b = (_I(i-1 ,j)[0] + _I(i ,j)[0] + _I(i ,j+1)[0] + _I(i ,j-1)[0] + _I(i ,j+1)[0])/5;
		double g = (_I(i -1 ,j)[1] + _I(i ,j)[1] + _I(i ,j+1)[1] + _I(i ,j-1)[1] + _I(i ,j+1)[2])/5;
		double r = (_I(i -1 ,j)[2] + _I(i ,j)[2] + _I(i ,j+1)[2] + _I(i ,j-1)[2] + _I(i ,j+1)[2])/5;
		return cv::Vec3b(b,g,r);
	}
		
	else
		return cv::Vec3b(_I(i,j)[0], _I(i,j)[1], _I(i,j)[2]);
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

		int _found = 0;

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

		//cv::Mat_<cv::Vec3b> _I = image;
		//cv::Mat_<cv::Vec3b> _IWhietOnly = image.clone();
		
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
		
		
		
		//zamalowanie na czarno obszarów B,G,R
		/*for (int by = wsp_maxB.y - 10; by < wsp_maxB.y + 10; ++by){
			for (int bx = wsp_maxB.x - 20; bx < wsp_maxB.x + 20; ++bx){
				_IWhietOnly(by,bx)[0] = 0;
				_IWhietOnly(by,bx)[1] = 0;
				_IWhietOnly(by,bx)[2] = 0;
			}
		}
		
		for (int gy = wsp_maxG.y - 20; gy < wsp_maxG.y + 20; ++gy){
			for (int gx = wsp_maxG.x - 20; gx < wsp_maxG.x + 20; ++gx){
				_IWhietOnly(gy,gx)[0] = 0;
				_IWhietOnly(gy,gx)[1] = 0;
				_IWhietOnly(gy,gx)[2] = 0;
			}
		}
		
		for (int ry = wsp_maxR.y - 20; ry < wsp_maxR.y + 20; +ry){
			for (int rx = wsp_maxR.x - 20; rx < wsp_maxR.x + 20; ++rx){
				_IWhietOnly(ry,rx)[0] = 0;
				_IWhietOnly(ry,rx)[1] = 0;
				_IWhietOnly(ry,rx)[2] = 0;
			}
		}
		
		for (int i = 3; i < _I.rows; ++i) {
				for (int j = 3; j < _I.cols; ++j) {

			if (_IWhietOnly(i,j)[0] + _IWhietOnly(i,j)[1] + _IWhietOnly(i,j)[2] > maxW){
				wsp_maxW.y = i;
				wsp_maxW.x = j;
				maxW = _IWhietOnly(i,j)[0] + _IWhietOnly(i,j)[1] + _IWhietOnly(i,j)[2];
			}
						
				}
		}*/
		/*for (int i = 3; i < _I.rows; ++i) {
				for (int j = 3; j < _I.cols; ++j) {

			bool inB = i > wsp_maxB.y - 20 && i < wsp_maxB.y + 20 && j > wsp_maxB.x && j < wsp_maxB.x;
			bool inG = i > wsp_maxG.y - 20 && i < wsp_maxG.y + 20 && j > wsp_maxG.x && j < wsp_maxG.x;
			bool inR = i > wsp_maxR.y - 20 && i < wsp_maxR.y + 20 && j > wsp_maxR.x && j < wsp_maxR.x;
			if (_I(i,j)[0] + _I(i,j)[1] + _I(i,j)[2] > maxW && i){
				wsp_maxW.y = i;
				wsp_maxW.x = j;
				maxW = _I(i,j)[0] + _I(i,j)[1] + _I(i,j)[2];
			}
						
				}
		}*/

		/*LOG(LWARNING)<<wsp_maxR.x<<"\n";
		CLOG(LWARNING)<<wsp_maxR.y<<"\n";
		
		CLOG(LWARNING)<<wsp_maxG.x<<"\n";
		CLOG(LWARNING)<<wsp_maxG.y<<"\n";
		
		CLOG(LWARNING)<<wsp_maxB.x<<"\n";
		CLOG(LWARNING)<<wsp_maxB.y<<"\n";
		
		CLOG(LWARNING)<<wsp_maxW.x<<"\n";
		CLOG(LWARNING)<<wsp_maxW.y<<"\n";*/

		//dla wyznaczenia obszaru policzę minimalną odległośc między punktami
		/*double xRG = abs(wsp_maxR.x - wsp_maxG.x);
		double xRB = abs(wsp_maxR.x - wsp_maxB.x);
		double xGB = abs(wsp_maxG.x - wsp_maxB.x);
		double yRG = abs(wsp_maxR.y - wsp_maxG.y);
		double yRB = abs(wsp_maxR.y - wsp_maxB.y);
		double yGB = abs(wsp_maxG.y - wsp_maxB.y);

		double RG = sqrt(pow(wsp_maxR.x - wsp_maxG.x,2) + pow(wsp_maxR.y - wsp_maxG.y,2));
		double RB = sqrt(pow(wsp_maxR.x - wsp_maxB.x,2) + pow(wsp_maxR.y - wsp_maxB.y,2));
		double GB = sqrt(pow(wsp_maxG.x - wsp_maxB.x,2) + pow(wsp_maxG.y - wsp_maxB.y,2));

		double _min = min(RG, RB);
		_min = min(_min, GB);

		//minimalna odległość między diodami
		double _minDiod = _min;
*/
		//poprawki
		//dla kolorów R, G, B rozpatrzę obszar w ich danym otoczeniu i onliczę moment centralny dla zbioru niezerowych pikseli
		//otoczenie nie może zachaczac o inne diody - ustaliłem minimalną odległość między diodami
		//otoczenie nie może też wychodzić poza obrazek

/*
		//dla każdej diody musze rozpatrzeć
		//odległość od innych diod
		//odległość od górnej, dolnej, lewej i prawej krawędzi obrazka
		//dla B
		double _minB = min(_minB, (double)wsp_maxB.x);
		_minB = min(_minB, _I.rows - (double)wsp_maxB.x);
		_minB = min(_minB, (double)wsp_maxB.y);
		_minB = min(_minB, _I.cols - (double)wsp_maxB.y);

		//dla G
		double _minG = min(_minG, (double)wsp_maxG.x);
		_minG = min(_minG, _I.rows - (double)wsp_maxG.x);
		_minG = min(_minG, (double)wsp_maxG.y);
		_minG = min(_minG, _I.cols - (double)wsp_maxG.y);

		//dla R
		double _minR = min(_minR, (double)wsp_maxR.x);
		_minR = min(_minR, _I.rows - (double)wsp_maxR.x);
		_minR = min(_minR, (double)wsp_maxR.y);
		_minR = min(_minR, _I.cols - (double)wsp_maxR.y);

		_min = min(_min,_minB);
		_min = min(_min,_minG);
		_min = min(_min,_minR);

		//wielkość otoczenia wokoło każdej diody w którym będę liczył centrum elementów niezerowych
		double wielkoscOtoczenia = _min/3;
*/
		//liczenie momentu dla diody czerwonej w celu poprawy znalezonego punktu
		/*if (_minDiod > 10){
			
			//std:cout<<"aa"<<std::endl;

		    double m00 = moment(0, 0, _I, cv::Vec3b(0,0,0), wsp_maxB.y - wielkoscOtoczenia , wsp_maxB.y + wielkoscOtoczenia, wsp_maxB.x - wielkoscOtoczenia , wsp_maxB.x + wielkoscOtoczenia);
		    double m10 = moment(1, 0, _I, cv::Vec3b(0,0,0), wsp_maxB.y - wielkoscOtoczenia , wsp_maxB.y + wielkoscOtoczenia, wsp_maxB.x - wielkoscOtoczenia , wsp_maxB.x + wielkoscOtoczenia);
		    double m01 = moment(0, 1, _I, cv::Vec3b(0,0,0), wsp_maxB.y - wielkoscOtoczenia , wsp_maxB.y + wielkoscOtoczenia, wsp_maxB.x - wielkoscOtoczenia , wsp_maxB.x + wielkoscOtoczenia);
		    wsp_maxB.y = m10/m00;
		    wsp_maxB.x = m01/m00;

		    m00 = moment(0, 0, _I, cv::Vec3b(0,0,0), wsp_maxG.y - wielkoscOtoczenia , wsp_maxG.y + wielkoscOtoczenia, wsp_maxG.x - wielkoscOtoczenia , wsp_maxG.x + wielkoscOtoczenia);
		    m10 = moment(1, 0, _I, cv::Vec3b(0,0,0), wsp_maxG.y - wielkoscOtoczenia , wsp_maxG.y + wielkoscOtoczenia, wsp_maxG.x - wielkoscOtoczenia , wsp_maxG.x + wielkoscOtoczenia);
		    m01 = moment(0, 1, _I, cv::Vec3b(0,0,0), wsp_maxG.y - wielkoscOtoczenia , wsp_maxG.y + wielkoscOtoczenia, wsp_maxG.x - wielkoscOtoczenia , wsp_maxG.x + wielkoscOtoczenia);
		    wsp_maxG.y = m10/m00;
		    wsp_maxG.x = m01/m00;

		    m00 = moment(0, 0, _I, cv::Vec3b(0,0,0), wsp_maxR.y - wielkoscOtoczenia , wsp_maxR.y + wielkoscOtoczenia, wsp_maxR.x - wielkoscOtoczenia , wsp_maxR.x + wielkoscOtoczenia);
		    m10 = moment(1, 0, _I, cv::Vec3b(0,0,0), wsp_maxR.y - wielkoscOtoczenia , wsp_maxR.y + wielkoscOtoczenia, wsp_maxR.x - wielkoscOtoczenia , wsp_maxR.x + wielkoscOtoczenia);
		    m01 = moment(0, 1, _I, cv::Vec3b(0,0,0), wsp_maxR.y - wielkoscOtoczenia , wsp_maxR.y + wielkoscOtoczenia, wsp_maxR.x - wielkoscOtoczenia , wsp_maxR.x + wielkoscOtoczenia);
		    wsp_maxR.y = m10/m00;
		    wsp_maxR.x = m01/m00;
		}*/
		
		std::vector<cv::Point2f> gridPoints;
		gridPoints.push_back(wsp_maxB);
		gridPoints.push_back(wsp_maxG);
		gridPoints.push_back(wsp_maxR);
		gridPoints.push_back(wsp_maxW);

		//_minDiod > 20 oznacza, że jedna z diod nie została odnaleziona jako diwe różne
		//if(maxB != 0 and maxG != 0 and maxR != 0 and maxW != 0 and _minDiod > 20.0)
			_found = 1;

		if(_found==1){
		    CLOG(LWARNING)<<"Diods found!!!\n\n\n";
		    diodPoints.write(gridPoints);
		    gridPattern.setImagePoints(gridPoints);
		    out_gridPattern.write(gridPattern);

			} else {
				CLOG(LWARNING) << "Diods NOT found!!!\n\n\n";
			}
			out_img.write(image);
		//0 lub 1 jesli plansza znalalzeniona lub nie znaleziona
		found.write(_found);

	} catch (const cv::Exception& e) {
		CLOG(LERROR) << e.what() << "\n";
	}

	CLOG(LTRACE) << "void DiodRecognition::onNewImage() end\n";
}

} // namespace DiodRecognition{
} // namespace Processors {
