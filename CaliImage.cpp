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
	ControlPoints.clear();
	X = -1.0;
	Y = -1.0;
	Z = -1.0;
	Phi = -1.0;  
	Omega = -1.0;
	Kappa = -1.0;
	x_0 = -1.0;
	y_0 = -1.0;
	f = -1.0;  
}

bool CaliImage::isEmpty()
{
    if (ImagePath.isEmpty())
        return true;
    else
        return false;
}
