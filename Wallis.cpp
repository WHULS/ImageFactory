
#include "Wallis.h"

#define gridWIndowSize 20
#define filterWindowSize 71
 
Wallis::Wallis(Mat _SourceImg, float _B_Value, float _C_Value, float _meanValue, float _sigmaValue)
:SourceImg(_SourceImg), B_Value(_B_Value), C_Value(_C_Value),meanValue(_meanValue),sigmaValue(_sigmaValue){}

Wallis::~Wallis()
{}

Mat Wallis::ReadMask(Mat SourceImg, int bRow, int bCol, int winRow, int winCol)
{
	Mat Mask (SourceImg,Rect(bCol, bRow, winCol, winRow));
	return Mask;
}

int	Wallis::InterplotWallisParameter( float *grid, int gridRow, int gridCol,float *value, int x, int y )
{
  	int      grid_r,grid_c;
	float    X,Y,L,Z00,Z10,Z01,Z11;
  	grid_r = y/gridWIndowSize;
  	grid_c = x/gridWIndowSize; 
	if (grid_r<0 || grid_c<0 || grid_r>=gridRow-1 || grid_c>=gridCol-1) 
	{
    	if (grid_r<=0)         grid_r = 0;
		if (grid_c<=0)         grid_c = 0;
		if (grid_r>=gridRow-1) grid_r = gridRow-1;
		if (grid_c>=gridCol-1) grid_c = gridCol-1;
		x = grid_c*gridWIndowSize;
		y = grid_r*gridWIndowSize;
  	}
  	Z00 = *(grid+grid_r*gridCol+grid_c);
  	Z10 = *(grid+grid_r*gridCol+grid_c+1);
  	Z01 = *(grid+(grid_r+1)*gridCol+grid_c);
  	Z11 = *(grid+(grid_r+1)*gridCol+grid_c+1);
  	X = float( x-grid_c*gridWIndowSize );
  	Y = float( y-grid_r*gridWIndowSize );
  	L = float( gridWIndowSize );
  	*value = (1-X/L)*(1-Y/L)*Z00 + X/L*(1-Y/L)*Z10 + Y/L*(1-X/L)*Z01 + X*Y/L/L*Z11;
	return 1;
}

int Wallis::CalWallisParameter(Mat Mask, int winRow,int winCol, float *r0, float *r1)
{
	float mean0(0), sigma0(0);
	for (int row = 0; row < winRow; ++row)
	{
		for (int col = 0; col < winCol; ++col)
		{
			mean0 += Mask.at<uchar>(row, col);
			sigma0 += Mask.at<uchar>(row, col)*Mask.at<uchar>(row,col);
		}
	}
	
	int sum   = winRow*winCol;
	float mean, sigma;
    mean  = mean0/float(sum);
    sigma = sigma0/float(sum-mean*mean);
	if (sigma<0) sigma = 0;
	sigma = float( sqrt(sigma) );
	
    if (sigma==0.0f)
	{
		*r1 = 1.0;
		*r0 = B_Value*meanValue+(1.0f-B_Value- *r1)*mean;
	}
	else
	{
		*r1 = C_Value*sigmaValue/(C_Value*sigma+(1.0f-C_Value)*sigmaValue);
		*r0 = B_Value*meanValue+(1.0f-B_Value- *r1)*mean;
	}
	return 1;
}

void Wallis::Wallisfilter(Mat Result) 
{      
	int heights = SourceImg.rows;
	int widths = SourceImg.cols;
	int	  x,y,br,bc,gridRow,gridCol;
	float r0,r1,rc,gf,rmean0,rsigma0,rmean,rsigma,*gridR0,*gridR1;
	gridRow = (heights-filterWindowSize)/gridWIndowSize;
	gridCol = (widths-filterWindowSize)/gridWIndowSize;
	gridR0 = (float *)calloc((gridRow+1)*(gridCol+1), sizeof(float));
	gridR1 = (float *)calloc((gridRow+1)*(gridCol+1), sizeof(float));
	rmean0 = rsigma0 = 0.0;
	Mat Mask;
	for( int row=0; row<gridRow; ++row )
	{
		br = row*gridWIndowSize;
		for (int col=0; col<gridCol; ++col) 
		{
			bc = col*gridWIndowSize;
			 Mask=ReadMask(SourceImg,br, bc, filterWindowSize, filterWindowSize);
			CalWallisParameter(Mask, filterWindowSize, filterWindowSize, &r0, &r1);
			*(gridR0+row*gridCol+col) = r0;
			*(gridR1+row*gridCol+col) = r1;
			gf = (float)Mask.at<uchar>((filterWindowSize + 1) / 2, (filterWindowSize + 1) / 2);
			rc = gf*r1+r0;
			rmean0 += rc;
			rsigma0 += rc*rc;
		}
	}
	rc = (float)(gridRow*gridCol);
	rmean = rmean0/rc;
	rsigma = rsigma0/rc-rmean*rmean;
	if (rsigma<0) rsigma = 0;
	rsigma = float( sqrt(rsigma) );
	for( int row=0;row<heights; ++row ) 
	{
		y = row-filterWindowSize/2;
		for( int col=0; col<widths; ++col)
		{
			x = col-filterWindowSize/2;
			InterplotWallisParameter(gridR0, gridRow, gridCol, &r0, x, y);
			InterplotWallisParameter(gridR1, gridRow, gridCol, &r1, x, y);
			gf = (float)Result.at<uchar>(row,col);
			if (gf<=3.0 || gf>=252.0) continue;
			rc = (gf*r1+r0-rmean)*sigmaValue/rsigma+meanValue;
			if (rc>=256) rc = 255.0;
			else if (rc<0) rc = 0.0;
			Result.at<uchar>(row,col) = (unsigned char)rc;
		}
	}
}



