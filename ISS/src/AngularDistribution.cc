#include "AngularDistribution.hh"

AngularDistribution::AngularDistribution(){


	bDist		= false;
	messenger	= new AngularDistributionMessenger(this);

}	
AngularDistribution::~AngularDistribution(){

	if(gDist)
		delete	gDist;
	if(messenger)
		delete	messenger;

}

AngularDistribution::AngularDistribution(const AngularDistribution& v){
			
	gDist		= (TGraph*)v.gDist->Clone();
	hDist		= (TH1F*)v.hDist->Clone();
	bDist		= v.bDist;	
	messenger	= v.messenger;

}
AngularDistribution& AngularDistribution::operator = (const AngularDistribution& v){
		
			
	gDist		= (TGraph*)v.gDist->Clone();
	hDist		= (TH1F*)v.hDist->Clone();
	bDist		= v.bDist;
	messenger	= v.messenger;

	return *this;

}



void AngularDistribution::ReadAngularDistribution(G4String s){


	int thr		=	G4Threading::G4GetThreadId();	
	if(thr == 0)
		std::cout	<< "Reading distribution from file: " << s << std::endl;

	double	xmin = -1;
	double	xmax = -1;

	double	tmpTheta;
	double	tmpCS;

	gDist	= new TGraph();

	char	chr[32];
	sprintf(chr,"graph_%i",thr);
	gDist->SetName(chr);

	int	counter = 0;
	std::string str;
	std::ifstream	infile(s);
	if(infile.is_open()){
		while(std::getline(infile, str)){ // more reliable than eof()			
			if(str[0] == '#') continue; // get past comment lines
			infile >> tmpTheta >> tmpCS; // get the data
			if(tmpTheta < xmin || xmin < 0)
				xmin = tmpTheta;
			if(tmpTheta > xmax)
				xmax = tmpTheta;

			double	sinTheta = sin(tmpTheta * D2R);
			gDist->SetPoint(counter,tmpTheta,tmpCS * sinTheta);

			counter++;
				
		}
	}
	infile.close();

	sprintf(chr,"dist_%i",thr);
	hDist	= new TH1F(chr,chr,counter * 100,xmin,xmax);
	for(int i=1;i<=hDist->GetNbinsX();i++){
		//std::cout	<< i << "\t" << hDist->GetBinCenter(i) << "\t" << gDist->Eval(hDist->GetBinCenter(i)) << std::endl;
		hDist->SetBinContent(i,gDist->Eval(hDist->GetBinCenter(i)));
	}
	bDist	= true;

}
