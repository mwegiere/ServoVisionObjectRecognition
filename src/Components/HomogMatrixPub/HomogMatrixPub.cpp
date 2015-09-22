/*!
 * \file
 * \brief
 * \author Maciej Wegierek
 */

#include <memory>
#include <string>

#include "HomogMatrixPub.hpp"
#include "Common/Logger.hpp"

#include <boost/bind.hpp>
#include <vector>

#include <iostream>


namespace Processors {
namespace HomogMatrixPub {

HomogMatrixPub::HomogMatrixPub(const std::string & name) :
		Base::Component(name)  {

}

HomogMatrixPub::~HomogMatrixPub() {
}

void HomogMatrixPub::prepareInterface() {
	// Register data streams, events and event handlers HERE!
	registerStream("in_matrix", &in_matrix);
	registerStream("out_matrix", &out_matrix);
	// Register handlers
	h_onNewData.setup(boost::bind(&HomogMatrixPub::onNewData, this));
	registerHandler("onNewData", &h_onNewData);
	addDependency("onNewData", &in_matrix);


}

bool HomogMatrixPub::onInit() {

	return true;
}

bool HomogMatrixPub::onFinish() {

	return true;
}

bool HomogMatrixPub::onStop() {
	return true;
}

bool HomogMatrixPub::onStart() {
	return true;
}

void HomogMatrixPub::onNewData() {



    plik.open( "/home/mwegiere/DCL/ServoVisionObjectRecognition/src/Components/HomogMatrixPub/wyniki2.txt", std::ios::app | ios::out);
	Types::HomogMatrix homogMatrix;
    float tmp[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	if(!in_matrix.empty()){
		int k = 0;
		homogMatrix = in_matrix.read();
        cv::Matx44d dataMatrix = (cv::Matx44d)homogMatrix;
        for (int i=0; i<4; ++i)
            for (int j=0; j<4; ++j){
                tmp[k] = (float)dataMatrix(i,j);
                plik<<tmp[k];
                plik<<'\n';
                ++k;
			}
	}
    CLOG(LINFO) << "Tmp:\n" << tmp;
    std::vector<float> out_homogMatrix (tmp, tmp + sizeof(tmp) / sizeof(float) );
	out_matrix.write(out_homogMatrix);
    plik.close();

	
//class Base::DataStreamIn<Types::HomogMatrix>

}

} //: namespace HomogMatrixPub
} //: namespace Processors
