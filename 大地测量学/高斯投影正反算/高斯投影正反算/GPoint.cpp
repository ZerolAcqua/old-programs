#include "GPoint.h"
#include "math.h"


GPoint::GPoint()
{

}
GPoint::GPoint(char* ID, double* cood)
{
	this->msID = string(ID);
	memcpy(this->mdCood, cood, 2 * sizeof(double));
}



void GPoint::SetPointIdCood(const char* ID, double* cood)
{
	this->msID = string(ID);
	memcpy(this->mdCood, cood, 2 * sizeof(double));
}

double GPoint::GetDist(GPoint point)
{
	return sqrt(pow(this->mdCood[0] - point.mdCood[0], 2)
		+ pow(this->mdCood[1] - point.mdCood[1], 2));
}

bool GPoint::operator==(GPoint tmp)
{
	return this->msID == tmp.msID;
}