#ifndef DetectorHit_hh
#define DetectorHit_hh

#include "G4ThreeVector.hh"
#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

class DetectorHit {

	public:
		DetectorHit(const G4Step*, int);
		DetectorHit(int id) 				{
									EDep	= 0;
									EDepP	= 0; 
									detID 	= id;	
									ToF	= -1;
								}
		DetectorHit(const DetectorHit& d);
		DetectorHit& operator = (const DetectorHit& d);
		~DetectorHit()	{;}
	
		void		AddStep(const G4Step*);
	
		void		AddEdep(double e)		{ EDep 		+= e; 	}	
		void		AddPEdep(double e)		{ EDepP 	+= e;	}
		void		SetToF(double t)		{ ToF 		= t;	}
		void		SetPosition(G4ThreeVector v)	{ position 	= v;	}
		void		SetDetHit(bool b=true)		{ detHit	= b;	}

		double		GetToF()		const	{ return ToF;		}

		int		GetDetID()		const 	{ return detID;		}
		double		GetEDep()		const	{ return EDep;		}
		double		GetEDepP()		const	{ return EDepP;		}
		G4ThreeVector	GetPosition()		const	{ return position;	}
		bool		IsDetHit()		const	{ return detHit;	}
	
	private:
		int		detID;
		bool		detHit;
		double		EDep;
		double		EDepP;
		double		ToF;
		G4ThreeVector	position;

};
#endif
