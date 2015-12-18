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
    //input streams
    registerStream("in_matrix", &in_matrix);
	registerStream("in_found", &in_found);
    registerStream("in_time_nsec_pocz", &in_time_nsec_pocz);
    registerStream("in_time_sec_pocz", &in_time_sec_pocz);

    //output streams
    registerStream("out_matrix", &out_matrix);
    registerStream("out_found", &out_found);
    registerStream("out_time_nsec_pocz", &out_time_nsec_pocz);
    registerStream("out_time_sec_pocz", &out_time_sec_pocz);
    registerStream("out_time_nsec_kon", &out_time_nsec_kon);
    registerStream("out_time_sec_kon", &out_time_sec_kon);

	// Register handlers
	h_onNewData.setup(boost::bind(&HomogMatrixPub::onNewData, this));
	registerHandler("onNewData", &h_onNewData);
	addDependency("onNewData", &in_matrix);
    addDependency("onNewData", &in_found);
    //addDependency("onNewData", &in_time_nsec_pocz);
    //addDependency("onNewData", &in_time_sec_pocz);

    //registerProperty(trigger_delay);

    std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
    std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
    std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;
    std::cout<<"@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@"<<std::endl;


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

    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    std::cout<<"!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<std::endl;
    int found = in_found.read();
    out_found.write(found);

    plik.open( "/home/mwegiere/DCL/ServoVisionObjectRecognition/src/Components/HomogMatrixPub/T_DC.txt", std::ios::app | ios::out);
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
        //std::cout<<tmp[k]<<std::endl;
        //std::cout<<"aaaaaaaaa"<<std::endl;
			}
    }
    struct timespec currentTime;
    currentTime.tv_nsec = 0;
    currentTime.tv_sec = 0;
    if(clock_gettime(CLOCK_REALTIME, &currentTime) == -1){
        LOG(LFATAL) << "clock_gettime() failed. " << strerror(errno);
    }

    //long int time_nsec_pocz = in_time_nsec_pocz.read();
    //long int time_sec_pocz = in_time_sec_pocz.read();

    /*registerStream("out_time_nsec_pocz", &out_time_nsec_pocz);
    registerStream("out_time_sec_pocz", &out_time_sec_pocz);
    registerStream("out_time_nsec_kon", &out_time_nsec_kon);
    registerStream("out_time_sec_kon", &out_time_sec_kon);*/

    //uwzględnienie czasu triggera
    //long int trigger_nsec = 25000000;
    //time_nsec_pocz += trigger_nsec;


    //out_time_nsec_pocz.write(time_nsec_pocz);
    //out_time_sec_pocz.write(time_sec_pocz);
    //out_time_nsec_kon.write(currentTime.tv_nsec);
    //out_time_sec_kon.write(currentTime.tv_sec);
    long int sec = static_cast<long int>(currentTime.tv_sec);
    std::vector<float> out_homogMatrix (tmp, tmp + sizeof(tmp) / sizeof(float) );
    out_matrix.write(out_homogMatrix);
    plik.close();

    //CLOG(LINFO) << "time_nsec_pocz: " << time_nsec_pocz;
    //CLOG(LINFO) << "time_sec_pocz: " << time_sec_pocz;
    //CLOG(LINFO) << "time_nsec_kon: " << currentTime.tv_nsec;
    //CLOG(LINFO) << "time_sec_kon: " << sec;

}

} //: namespace HomogMatrixPub
} //: namespace Processors
