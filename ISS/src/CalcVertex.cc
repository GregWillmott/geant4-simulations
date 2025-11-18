#include "CalcVertex.hh"

// Holder class for reaction kinematics calculated in Kinematics.cc

CalcVertex::CalcVertex(const CalcVertex& v){

	recoilTheta	= v.recoilTheta;
	recoilKinE	= v.recoilKinE;

	ejecTheta	= v.ejecTheta;
	ejecKinE	= v.ejecKinE;

}
CalcVertex& CalcVertex::operator = (const CalcVertex& v){


	recoilTheta	= v.recoilTheta;
	recoilKinE	= v.recoilKinE;

	ejecTheta	= v.ejecTheta;
	ejecKinE	= v.ejecKinE;

	return *this;

}
