#include <iostream>
#include <string>
#include "my_hmr.h"

using namespace std;

My_hmr :: My_hmr(long start, long end, string sequence, int sites)
{
	this->start = start;
	this-> end = end;
	this->sequence = sequence;
	this->sites = sites;
}

My_hmr :: ~My_hmr()
{

}

long My_hmr :: getStart()
{
	return this->start;
}

long My_hmr :: getEnd()
{
	return this->end;
}

int My_hmr :: getSites()
{
	return this->sites;
}
