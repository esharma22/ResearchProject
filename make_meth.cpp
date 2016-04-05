#include <iostream>
#include <vector>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include "regions.h"
using namespace std;

int main(int argc, char *argv[])
{
	string line;
	string sequence = "";
	vector<long> Cpositions;
	vector<Region *> regions;

	getline(cin, line);
	while(getline(cin, line))			//Concatenating the lines in the file
	{
		sequence += line;
	}
	for(long i = 0; i < sequence.length(); i++)
	{
		char c = sequence.at(i);
		char g = '0';
		if( i != (sequence.length() - 1))
		{
			g = sequence.at(i + 1);
		}
		if((c == 'C') && (g == 'G'))		//Identifying the "CG" pairs in the sequence
		{
			Cpositions.push_back(i);
		}
	}


	//FINDING REGIONS WITHIN CG COUNTS
	long upperbound = atoi(argv[1]);
	long start = 0, end = 0;
	int count = 1;
	bool first = true;
	fstream fs_regions;
	fs_regions.open("newRegions.txt", ios::in | ios::out);
	if(!fs_regions.is_open())
	{
		cerr << "Can't open file." << endl;
		exit(1);
	}

	for(long j = 0; j < (Cpositions.size() - 1); j++)	//Defining regions within a threshold of 1000
	{
		if((Cpositions[j+1] - Cpositions[j]) < upperbound)
		{
			if(first)
			{
				start = Cpositions[j];
				first = false;
			}
			if(j == (Cpositions.size()-1))
			{
				Region *r = new Region(start, (j+1), count);
				regions.push_back(r);
				break;
			}
			count++;			//Counting no of "CG" occurences in one region
		}
		else
		{
			end = Cpositions[j];
			Region *r = new Region(start, end, count);
			regions.push_back(r);
			start = Cpositions[j + 1];
			end = 0;
			count = 1;
		}

		if(j == (Cpositions.size() - 1))
		{
			Region *r = new Region(start, j+1, count);
			regions.push_back(r);
		}
	}
	if(regions.size() == 0)
	{
		cerr << "ERROR: No regions found" << endl;
		return 0;
	}
	fs_regions << "START" << "\t" << "END" << "\t" << "SITES" << "\n";
	for(long k = 0; k < regions.size(); k++)
	{
		Region *r = regions[k];
		fs_regions << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
	}
	fs_regions.close();
	

	//SELECTING RANDOM REGIONS
	int num_random = atoi(argv[2]);
	fstream fs_random;
	fs_random.open("randomregions.txt", ios::in | ios::out);
	if(!fs_random.is_open())
	{
		cerr<< "Can't open file" << endl;
		exit(1);	
	}
	srand(time(NULL));
	vector<Region *> randomRegions;
	for(int i = 0; i < num_random;)
	{
		Region *r = regions[rand() % (regions.size() - 1)];
		if(r->no_of_sites() > 5)
		{					//Selecting 10 random regions with atleast 5 "CG" sites
			randomRegions.push_back(r);
			i++;
		}
	}
	for(int i = 0; i < randomRegions.size(); i++)
	{
		Region *r = randomRegions[i];
		fs_random << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
	}
	fs_random.close();



	//SETTING METHYLATION LEVEL TO EACH CG COUNT
	float random_meth[] = {0.0 , 0.1, 0.2};
	float regular_meth[] = {0.7, 0.8, 0.9, 1.0};

	for(long i = 0; i < Cpositions.size(); i++)
	{
		cout << "chrY" << "\t";
		long c =Cpositions[i];
		cout << c << "\t" << (c+1) << "\t";
		cout << "CpG:" << (10 + (rand() % 11)) << "\t";

		bool random = false;
		for(int j = 0; j < num_random; j++)
		{
			Region *r = randomRegions[j];
			if(c >= r->getStart() && c <= r->getEnd())
			{
				random = true;
				break;
			}
		}
		if(random)
		{
			int pos = rand() % 3;
			if(pos == 0)
			{
				cout << "0.0";
			}
			else
			{
				cout << random_meth[pos];
			}
		}
		else
		{
			int pos = rand() % 4;			
			cout << regular_meth[pos];
		}
		cout << "\t+" << endl;
	}

	return 0;
}
