/*!
 * \file
 * \brief 
 * \author Maciej Wegierek
 */

#ifndef HOMOGMATRIXPUB_HPP_
#define HOMOGMATRIXPUB_HPP_

#include "Component_Aux.hpp"
#include "Component.hpp"
#include "DataStream.hpp"
#include "Property.hpp"
#include "EventHandler2.hpp"
#include "Types/HomogMatrix.hpp"
#include <opencv2/opencv.hpp>

#include <fstream>
namespace Processors {
namespace HomogMatrixPub {

/*!
 * \class HomogMatrixPub
 * \brief HomogMatrixPub processor class.
 *
 * 
 */
class HomogMatrixPub: public Base::Component {
public:
	/*!
	 * Constructor.
	 */
	HomogMatrixPub(const std::string & name = "HomogMatrixPub");

	/*!
	 * Destructor
	 */
	virtual ~HomogMatrixPub();

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
	Base::DataStreamIn<Types::HomogMatrix> in_matrix;

	// Output data streams
    Base::DataStreamOut<vector<float> > out_matrix;

	// Handlers
    Base::EventHandler2 h_onNewData;

	// Properties

	
	// Handlers
    void onNewData();

    std::fstream plik;

};

} //: namespace HomogMatrixPub
} //: namespace Processors

/*
 * Register processor component.
 */
REGISTER_COMPONENT("HomogMatrixPub", Processors::HomogMatrixPub::HomogMatrixPub)

#endif /* HOMOGMATRIXPUB_HPP_ */
