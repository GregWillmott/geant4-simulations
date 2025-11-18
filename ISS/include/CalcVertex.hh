#ifndef CalcVertex_h
#define CalcVertex_h

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <sstream>

class CalcVertex {

	public:
		CalcVertex()	{;}
		CalcVertex(double hT, double hKE, double eT, double eKE){
			recoilTheta	= hT;
			recoilKinE	= hKE;
			ejecTheta	= eT;
			ejecKinE	= eKE;
		};
		~CalcVertex()	{;}
		CalcVertex(const CalcVertex& );
		CalcVertex& operator = (const CalcVertex&);

		void	Clear(){

			recoilTheta 	= 0;
			recoilKinE	= 0;

			ejecTheta	= 0;
			ejecKinE	= 0;

		};

		double	GetRecoilTheta()	{ return recoilTheta;	}
		double	GetRecoilKinE()		{ return recoilKinE;	}
		double	GetEjecTheta()		{ return ejecTheta;	}
		double	GetEjecKinE()		{ return ejecKinE;	}

		void	SetRecoilTheta(double v)	{ recoilTheta 	= v;	}
		void	SetRecoilKinE(double v)		{ recoilKinE	= v;	}
		void	SetEjecTheta(double v)		{ ejecTheta	= v;	}
		void	SetEjecKinE(double v)		{ ejecKinE	= v;	}

	private:

		double	recoilTheta;
		double	recoilKinE;

		double	ejecTheta;
		double	ejecKinE;

		
};

#endif
