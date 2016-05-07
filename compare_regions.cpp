#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <sstream>
#include <exception>
#include <cmath>
#include "regions.h"
#include "my_hmr.h"

using namespace std;

bool getNewFile(string);
vector<Region*> getrandomregions(string filename);
vector<Region*> gethmr_regions(string filename);
void compare_regions(vector<Region*>, vector<Region*>);

int main()
{
	int option;
	string choice;
	string filename;
	vector<Region*> randomregions;
	vector<Region*> hmr_regions;
	
	while(1)
	{
		cout << "\n----------MENU----------" << endl;
		cout << "1- Random Regions" << endl;
		cout << "2- HMR Regions" << endl;
		cout << "3- Analysis" << endl;
		cout << "4- Exit" << endl;
		cout << "Enter your choice- ";
		getline(cin, choice);

		try
		{
			option = stoi(choice);
		}
		catch(exception &e)
		{
			cerr << "Invalid choice! Please enter a number." << endl;
			cin.clear();
			cin.ignore();
			continue;
		}

		switch(option)
		{
			case 1:
				cout << "Enter the name of the file with random regions" << endl;
				getline(cin, filename);
	
				if(getNewFile(filename))
				{
					randomregions = getrandomregions(filename);
				}
				else
				{
					cerr << "No such file found." << endl;
				}
			break;

			case 2:
				cout << "Enter the name of the HMR file" << endl;
				getline(cin, filename);

				if(getNewFile(filename))
				{
					hmr_regions = gethmr_regions(filename);
				}
				else
				{
					cerr << "No such file found." << endl;
				}
			break;

			case 3:
				cout << "Analysis" << endl;
				compare_regions(randomregions, hmr_regions);
			break;

			case 4:
				exit(0);
			break;

			default:
				cout << "Enter a valid choice" << endl;
		}
	}
	return 0;
}


bool getNewFile(string filename)
{
	ifstream ifile(filename.c_str());
	if(ifile)
	{
		ifile.close();
		return true;
	}
	return false;

}


vector<Region*> getrandomregions(string filename)
{
	string line;
	vector<Region*> randomregions;
	ifstream regions(filename);
	
	while(getline(regions, line))
	{
		stringstream ss(line);
		string s, e, ns, seq;
		getline(ss, s, '\t');
		getline(ss, e, '\t');
		getline(ss, ns);

		long start, end;
		int sites;
		
		start = stol(s);
		end = stol(e);
		sites = stoi(ns);
		Region *r = new Region(start, end, sites);
		randomregions.push_back(r);
	}
	regions.close();

	return randomregions;
}

vector<Region*> gethmr_regions(string filename)
{
	string line;
	vector<Region*> hmr_regions;
	ifstream hmr_file(filename);	

	while(getline(hmr_file, line))
	{
		//cout << line << endl;
		stringstream ss(line);
		string ig, s, e, seq, ns;
		getline(ss, ig, '\t');
		getline(ss, s, '\t');
		getline(ss, e, '\t');
		getline(ss, seq, '\t');
		getline(ss, ns, '\t');
		getline(ss, ig, '\t'); 

		long start, end;
		int sites;

		start = stol(s);
		end = stol(e);
		sites = stoi(ns);

		Region *r = new Region(start, end, sites);
		r->setHypo(seq);

		hmr_regions.push_back(r); 
	}

	hmr_file.close();

	return hmr_regions;
}

void compare_regions(vector<Region*> randomregions, vector<Region*> hmr_regions)
{
	vector<Region*> overlap;
	vector<Region*> missed;
	bool full_overlap = false;
	bool rrtrue;
	int diff = 0;

	if(randomregions.size() < hmr_regions.size())
	{
		rrtrue = true;
	}
	else
	{
		rrtrue = false;
	}
	
	if(rrtrue ==  false)
	{
		int i = 0, j = 0;
		for(;i < hmr_regions.size();)
		{
			//Region *hmr = hmr_regions[i];
			//Region *region = randomregions[j];
			Region *region = randomregions[i];
			Region *hmr = hmr_regions[j];
			if((hmr->getStart() == region->getStart()) && (hmr->getEnd() == region->getEnd()))
			{
				//overlap.push_back(hmr);
				i++;
				j++;	
			}
			else if((hmr->getStart() == region->getStart())&& (hmr->getEnd() != region->getEnd()))
			{
				Region *tempHMR = hmr_regions[j + 1];
				Region *tempRR = randomregions[i + 1];
				if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					//if(hmr->getEnd() < region->getEnd())
					//{
					//	//overlap.push_back(hmr);
					//}
					//else
					//{
					//	missed.push_back(region);
					//}
					//overlap.push_back(hmr);
					i++;
					j++;
				}
				else if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() != tempHMR->getEnd()))
				{
					i++;
					j++;
				}
				else if((tempRR->getStart() != tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					i++;
					j++;
				}
				else
				{
					missed.push_back(region);
					i++;
				}

			}
			else if((hmr->getStart() != region->getStart())&& (hmr->getEnd() == region->getEnd()))
			{
				Region *tempHMR = hmr_regions[j + 1];
				Region *tempRR = randomregions[i + 1];
				if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					//if(hmr->getEnd() < region->getEnd())
					//{
					//	//overlap.push_back(hmr);
					//}
					//else
					//{
					//	missed.push_back(region);
					//}
					//overlap.push_back(hmr);
					i++;
					j++;
				}
				else if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() != tempHMR->getEnd()))
				{
					i++;
					j++;
				}
				else if((tempRR->getStart() != tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					i++;
					j++;
				}
				else
				{
					missed.push_back(region);
					i++;
				}
			}
			else
			{
				missed.push_back(region);
				i++;
			}
		}
	}
	else
	{
		int i = 0, j = 0;
		bool first = true;
		for(; i < randomregions.size();)
		{
			Region *region = randomregions[i];
			Region *hmr = hmr_regions[j];
			if((region->getStart() == hmr->getStart()) && (region->getEnd() == hmr->getEnd()))
			{
				//overlap.push_back(hmr);
				i++;
				j++;
			}
			else if((region->getStart() == hmr->getStart()) && (region->getEnd() != hmr->getEnd()))
			{
				Region *tempHMR = hmr_regions[j + 1];
				Region *tempRR = randomregions[i + 1];
				if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					if(hmr->getEnd() < region->getEnd())
					{
						//overlap.push_back(hmr);
					}
					else
					{
						missed.push_back(region);
					}
					i++;
					j++;
				}
				else if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() != tempHMR->getEnd()))
				{
					if(hmr->getEnd() < region->getEnd())
					{
						//overlap.push_back(hmr);
					}
					i++;
					j++;
				}
				else if((tempRR->getStart() != tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					overlap.push_back(hmr);
					i++;
					j++;
				}
				else
				{
					int diff = j;
					tempHMR = hmr_regions[diff];
					while(1)
					{
						if(region->getEnd() == tempHMR->getEnd())
						{
							break;
						}
						diff++;
						tempHMR = hmr_regions[diff];
					}
					//overlap.push_back(hmr);
					//overlap.push_back(tempHMR);
					//i++;
					//j+=2;
					while(j <= diff)
					{
						tempHMR = hmr_regions[j];
						//overlap.push_back(tempHMR);
						j++;
					}
					i++;
				}
				
			}
			else if((region->getStart() != hmr->getStart()) && (region->getEnd() == hmr->getEnd()))	
			{
				Region *tempHMR = hmr_regions[j + 1];
				Region *tempRR = randomregions[i + 1];
				if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					if(hmr->getEnd() < region->getEnd())
					{
						//overlap.push_back(hmr);
					}
					else
					{
						missed.push_back(region);
					}
					i++;
					j++;
				}
				else if((tempRR->getStart() == tempHMR->getStart()) && (tempRR->getEnd() != tempHMR->getEnd()))
				{
					if(hmr->getEnd() < region->getEnd())
					{
						//overlap.push_back(hmr);
					}
					i++;
					j++;
				}
				else if((tempRR->getStart() != tempHMR->getStart()) && (tempRR->getEnd() == tempHMR->getEnd()))
				{
					//overlap.push_back(hmr);
					i++;
					j++;
				}
				else
				{
					int diff = j;
					tempHMR = hmr_regions[diff];
					while(1)
					{
						if(region->getEnd() == tempHMR->getEnd())
						{
							break;
						}
						diff++;
						tempHMR = hmr_regions[diff];
					}
					//overlap.push_back(hmr);
					//overlap.push_back(tempHMR);
					//i++;
					//j+=2;
					while(j <= diff)
					{
						tempHMR = hmr_regions[j];
						//overlap.push_back(tempHMR);
						j++;
					}
					i++;
	
				}
				
			}
			else
			{
				missed.push_back(region);
				i++;
			}
		}
	}		

	/*cout << "--------------------------------------------------------------------------------------" << endl;
	cout << "\n1- Generated HMRs that overalap with HMR regions-" << overlap.size() << endl;
	cout << "--------------------------------------------------------------------------------------" << endl;
	cout << "\n2- HMR regions that overlap with generated HMRs- " << overlap.size() << endl;
	cout << "--------------------------------------------------------------------------------------" << endl;
	for(int i = 0; i < overlap.size(); i++)
	{
		cout << overlap[i]->getStart() << " " << overlap[i]->getEnd() << " " << overlap[i]->no_of_sites() << endl;
	}*/
	cout << "--------------------------------------------------------------------------------------" << endl;
	cout << "\n3- Generated HMRs that do not overalp with HMR regions-" << missed.size() << endl;
	cout << "--------------------------------------------------------------------------------------" << endl;
	for(int i = 0; i < missed.size(); i++)
	{
		cout << missed[i]->getStart() << " " << missed[i]->getEnd() << " " << missed[i]->no_of_sites() << endl;
	}
	/*int missedHMR = 0;
	missedHMR = hmr_regions.size() - overlap.size();
	cout << "--------------------------------------------------------------------------------------" << endl;
	cout << "\n4- HMR regions that do not overlap with generated HMRs- " << abs(missedHMR) << endl;*/
}
