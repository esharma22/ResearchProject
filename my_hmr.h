#include <iostream>
#ifndef MY_HMR_H
#define MY_HMR_H

using namespace std;

class My_hmr
{
	private:
		static string chromosome;
		long start;
		long end;
		string sequence;
		int sites;

	public:
		My_hmr(long, long, string, int);
		~My_hmr();
		long getStart();
		long getEnd();
		int getSites();
};

#endif

