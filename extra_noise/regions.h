/*
Class that takes information from make_meth.cpp and creates regions
*/

#ifndef REGIONS_H
#define REGIONS_H
#include <iostream>
#include <string>

using namespace std;

class Region
{
	private:
		long start;
		long end;
		int siteCount;
		string hypo;

	public:
		Region(long, long, int);
		~Region();
		void print();
		int no_of_sites();
		long getStart();
		long getEnd();
		void setHypo(string);
		string getHypo();
};
#endif
