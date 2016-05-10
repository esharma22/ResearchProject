#include <iostream>
#include <string>
#include <vector>
#include "regions.h"
using namespace std;

//Constructor
Region::Region(long start, long end, int siteCount)
{
	this->start = start;
	this->end = end;
	this->siteCount = siteCount;
}

//Destructor
Region::~Region()
{
}

//To display the regions
void Region::print()
{
	cout<< start<<"	"<< end<< "	"<< siteCount<< endl;
}

//Gives number of sites in a region
int Region::no_of_sites()
{
	return siteCount;
}

long Region::getStart()
{
	return this->start;
}

long Region::getEnd()
{
	return this->end;
}

void Region::setHypo(string seq)
{
	this->hypo = seq;
}

string Region::getHypo()
{
	return this->hypo;
}
