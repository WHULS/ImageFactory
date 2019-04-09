#include "CaliImage.h"

CaliImage::CaliImage()
{

}

CaliImage::~CaliImage()
{
    clear();
}

void CaliImage::clear()
{
    ImagePath.clear();
	Image.release();
	ControlPoints.clear();
	X = 0.0;
	Y = 0.0;
	Z = 0.0;
	Phi = 0.0;  
	Omega = 0.0;
	Kappa = 0.0;
	x_0 = 0.0;
	y_0 = 0.0;
	f = 0.0;  
}
