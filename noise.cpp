#include <iostream>
#include <vector>
#include <exception>
#include <string.h>
#include <fstream>
#include <stdlib.h>
#include <cstdlib>
#include <time.h>
#include <sstream>
#include <algorithm>
#include "regions.h"

using namespace std;

string masterFile;

string getChromosome(string);
string readSequence(ifstream &);
vector<long> getCGPositions(string);

bool getNewFile(string);
bool readlong(long &);
bool readint(int &);

vector<Region*> findRegions(vector<long>, long, string);
vector<Region*> getRandomRegions(vector<Region*>, int, string);
vector<long> getCytosine(vector<long>, vector<Region*>, float);

void addNoise(vector<long>, vector<Region*>, string, string, float);
void createMethTable(vector<long>, vector<Region*>, string, string);


int main(int argc, char *argv[])
{
	int option;
	int percent;
	string choice;
	string line, filename;
	string chromosome_name;
	vector<long> Cpositions;
	vector<Region*> regions;
	vector<Region*> randomregions;
	long threshold;
	int no_randomregions;
	bool file_exists = false;

	if(argc < 2)
	{
		cout << "ERROR: Pass file name as a command line arguement" << endl;
		cout << "EXAMPLE: ./make_meth <filename> " << endl;

		exit(1); 
	}

	ifstream input_file(argv[1]);
	getline(input_file, line);
	chromosome_name = getChromosome(line);
	
	Cpositions = getCGPositions(readSequence(input_file));
	input_file.close();
	
	while(1)
	{
		cout << "--------MENU--------" << endl;
	     	cout << "1- Get CpG Regions" << endl;
	     	cout << "2- Get Random Regions" << endl;
	     	cout << "3- Create Meth Table" << endl;
		cout << "4- Add Noise" << endl;
		cout << "5- Exit" << endl;
	     	cout << "Enter choice- " ;
		getline(cin, choice);

		try
		{
			option = stoi(choice);
		}
		catch(exception &e)
		{
			cerr << "Invalid Choice!! Please enter a number. " << endl;
			cin.clear();
			cin.ignore();
			continue;
		}

		switch(option)
		{
			case 1:
				cout << "Enter the threshold number- " << endl;
				if(!readlong(threshold))
				{
					continue;
				}
			
				cout << "Enter a file name to store the regions in- " << endl;
				getline(cin, filename);
			
				file_exists = getNewFile(filename);
				while(file_exists)
				{
					cout << "Please enter a new name for the file- " << endl;
					getline(cin, filename);
					file_exists = getNewFile(filename);
				}
				masterFile = "Master_" + filename;
				regions = findRegions(Cpositions, threshold, filename);
			break;

			case 2:
				cout << "Enter the number of random regions- " << endl;
				if(!readint(no_randomregions))
				{
					continue;
				}
				cout << "Enter a file name to store the random regions in- " << endl;
				getline(cin, filename);

				file_exists = getNewFile(filename);
				while(file_exists)
				{
					cout << "Please enter a new name for the file- " << endl;
					getline(cin, filename);
					file_exists = getNewFile(filename);
				}
				
				randomregions = getRandomRegions(regions, no_randomregions, filename);
			break;

			case 3:
				cout << "Enter a file name to store the Meth Table in- " << endl;
				getline(cin, filename);

				file_exists = getNewFile(filename);
				while(file_exists)
				{
					cout << "Please enter a new name for the file- " << endl;
					getline(cin, filename);
					file_exists = getNewFile(filename);
				}

				createMethTable(Cpositions, randomregions, filename, chromosome_name);
			break;

			case 4:
				cout << "Enter a file name to store the Meth Table with noise- " << endl;
				getline(cin, filename);

				file_exists = getNewFile(filename);
				while(file_exists)
				{
					cout << "Please enter a new name for the file- " << endl;
					getline(cin, filename);
					file_exists = getNewFile(filename);
				}
				cout << "Please select the percentage of noise you wish to add-" << endl;
				cout << "1- 5% " << endl;
				cout << "2- 10% " << endl;
				cout << "3- 20% " << endl;
				cout << "Enter your choice- ";
				cin >> percent;
				switch(percent)
				{
					case 1:
						addNoise(Cpositions, randomregions, filename, chromosome_name, 0.05);
					break;

					case 2:
						addNoise(Cpositions, randomregions, filename, chromosome_name, 0.1);
					break;

					case 3:
						addNoise(Cpositions, randomregions, filename, chromosome_name, 0.2);
					break;
				}
			break;

			case 5: 
				exit(0);
			break;

			default:
				cout << "Enter a valid choice" << endl;
		}
	}
	return 0;
}

string getChromosome(string line)
{
	stringstream ss(line);
	string name;
	int count = 0;

	while(count < 4)
	{
		getline(ss, name, ':');
		count ++;
	}
	name = "chr" + name;
	return name;
}

string readSequence(ifstream &input_file)
{
	string line;
	string sequence = "";
	
	while(getline(input_file, line))
	{
		sequence += line;
	}
	return sequence;
}

vector<long> getCGPositions(string sequence)
{
	vector<long> Cpositions;

	for(unsigned long i = 0; i < sequence.length(); i++)
	{
		char c = sequence.at(i);
		char g = '0';
		if(i != (sequence.length()) - 1)
		{
			g = sequence.at(i + 1);
		}
		if((c == 'C') && (g == 'G'))
		{
			Cpositions.push_back(i);
		}
	}
	return Cpositions;
}

bool getNewFile(string filename)
{
	ifstream ifile(filename.c_str());

	if(ifile)
	{
		cout << "File of the same name already exists." << endl;
		ifile.close();
		return true;
	}
	else
	{
		ifile.close();
		return false;
	}
}

bool readlong(long &threshold)
{
	string input;

	getline(cin, input);
	try
	{
		threshold = stol(input);
	}	
	catch(exception &e)
	{
		cerr << "Invalid input. Please enter a number only." << endl;
		cin.clear();
		cin.ignore();
		return false;
	}
	return true;
}

bool readint(int &randomregions)
{
	string input;
	
	getline(cin, input);
	try
	{
		randomregions = stol(input);
	}
	catch(exception &e)
	{
		cerr << "Invalid input. Please enter a number only." << endl;
		cin.clear();
		cin.ignore();
		return false;
	}
	return true;
}

vector<Region*> findRegions(vector<long> Cpositions, long threshold, string filename)
{
	long start = 0, end = 0;
	int count = 1;
	bool first = true;
	vector<Region*> regions;

	ofstream fs_regions(filename);
	ofstream master_file(masterFile, ios::in | ios::out | ios::app);	

	if(Cpositions.size() == 0)
	{
		cerr << "ERROR. No CG positions available." << endl;
		return regions;
	}
	for(unsigned long i = 0; i < (Cpositions.size() - 1); i++)
	{
		if((Cpositions[i+1] - Cpositions[i]) < threshold)
		{
			if(first)
			{
				start = Cpositions[i];
				first = false;
			}
			if(i == (Cpositions.size()-1))
			{
				Region *r = new Region(start, (i+1), count);
				regions.push_back(r);
				break;
			}
			count++;
		}
		else
		{
			end = Cpositions[i];
			Region *r = new Region(start, (end+1), count);
			regions.push_back(r);
			start = Cpositions[i + 1];
			end = 0;
			count = 1;
		}

		if(i == (Cpositions.size() - 1))
		{
			Region *r = new Region(start, (i+1), count);
			regions.push_back(r);
		}
	}
	if(regions.size() == 0)
	{
		cerr << "No regions found in this threshold" << endl;
		fs_regions.close();
		return regions;
	}
	
	master_file << "REGIONS TABLE" << "\t" << "No  of regions found- " << regions.size() << endl;
	master_file << "---------------------------------------------------------" << endl;
	fs_regions << "START" << "\t" << "END" << "\t" << "SITES" << endl;
	fs_regions << "---------------------------------------------------------" << endl;
	master_file << "START" << "\t" << "END" << "\t" << "SITES" << endl;
	master_file << "---------------------------------------------------------" << endl; 
	for(unsigned long j = 0; j < regions.size(); j++)
	{
		Region *r = regions[j];
		fs_regions << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
		master_file << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
	}
	master_file << "---------------------------------------------------------" << endl;
	master_file << "---------------------------------------------------------\n" << endl;
	fs_regions.close();
	master_file.close();
	return regions;
}


vector<Region*> getRandomRegions(vector<Region*> regions, int no_randomregions, string filename)
{
	long no_regions;
	int index = 0;
	vector<Region*> randomregions;
	
	no_regions = regions.size();
	unsigned int region_array[no_regions];
	memset(region_array, 0, sizeof(region_array));
	ofstream fs_random(filename);
	ofstream master_file(masterFile, ios::in | ios::out | ios::app);
	
	if(regions.size() == 0)
	{
		cerr << "ERROR. No regions are available to choose from." << endl;
		return randomregions;
	}
	srand(time(NULL));
	for(int i = 0; i < no_randomregions;)
	{
		index = rand() % regions.size();
		if(region_array[index] == 0)
		{
			Region *r = regions[index];
			if(r->no_of_sites() > 5)
			{
				randomregions.push_back(r);
				region_array[index] = 1;
				i++;
			}
			if((no_regions < no_randomregions) && (i == no_regions))
			{
				break;
			}
		}
	
	}

	for(unsigned int i = 0; i < randomregions.size(); i++)
	{
		Region *ir = randomregions[i];
		for(unsigned int j = 0; j < randomregions.size(); j++)
		{
			Region *jr = randomregions[j];
			if(ir->getStart() < jr->getStart())
			{
				swap(*randomregions[i], *randomregions[j]);
			}
		}
	}

	master_file << "RANDOM REGIONS" <<  "\t" << "No of random regions- " << randomregions.size() << endl;
	master_file << "--------------------------------------------------------" << endl;
	master_file << "START" << "\t" << "END" << "\t" << "SITES" << endl;
	master_file << "--------------------------------------------------------" << endl;
	for(unsigned int j = 0; j < randomregions.size(); j++)
	{
		Region *r = randomregions[j];
		fs_random << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
		master_file << r->getStart() << "\t" << r->getEnd() << "\t" << r->no_of_sites() << "\n";
	}
	master_file << "--------------------------------------------------------" << endl;
	master_file << "--------------------------------------------------------\n" << endl;
	fs_random.close();
	master_file.close();
	return randomregions;
}


void createMethTable(vector<long> Cpositions, vector<Region*> randomregions, string filename, string chromosome_name)
{
	ofstream methTable(filename);
	ofstream master_file(masterFile, ios::in | ios::out | ios::app);
	float random_meth[] = {0.0, 0.1, 0.2};
	float regular_meth[] = {0.7, 0.8, 0.9, 1.0};

	master_file << "METHYLATION TABLE" << endl;
	master_file << "--------------------------------------------------------" << endl;
	for(unsigned long i = 0; i < Cpositions.size(); i++)
	{
		methTable << chromosome_name << "\t";
		master_file << chromosome_name << "\t";
		long c  = Cpositions[i];
		methTable << c << "\t" << (c+1) << "\t";
		methTable << "CpG:" << (10 + (rand() % 11)) << "\t";
		master_file << c << "\t" << (c+1) << "\t";
		master_file << "CpG:" << (10 + (rand() % 11)) << "\t";

		bool random = false;
		for(unsigned int j = 0; j < randomregions.size(); j++)
		{
			Region *r = randomregions[j];
			if(c >= r->getStart() && c <= r->getEnd())
			{
				random = true;
				break;
			}
		}
		if(random)
		{
			int pos  = rand() % 3;
			if(pos == 0)
			{
				methTable << "0.0";
				master_file << "0.0";
			}
			else
			{
				methTable << random_meth[pos];
				master_file << random_meth[pos];
			}
		}
		else
		{
	 		int pos = rand() % 4;
			methTable << regular_meth[pos];
			master_file << regular_meth[pos];
		}
		methTable << "\t+" << "\n";
		master_file << "\t+" << "\n";
	}
	methTable.close();
	master_file.close();
}


/*
	Getting RR for adding noise
*/
vector<long> getCytosine(vector<long> Cpositions, vector<Region*> randomregions, float percent)
{
	long totalCpG = 0;
	int noise;
	vector<long> allrandomCs;
	vector<long> CforNoise;

	for(unsigned int i = 0; i < randomregions.size(); i++)
	{
		Region *r = randomregions[i];
		totalCpG = totalCpG + r->no_of_sites();
	}
	noise = totalCpG * percent;
	cout << "Noise- " << noise << endl;
	for(unsigned long i = 0; i < Cpositions.size(); i++)
	{
		long c = Cpositions[i];
		//bool random = false;
		for(unsigned int j = 0; j < randomregions.size(); j++)
		{
			Region *r = randomregions[j];
			if(c >= r->getStart() && c <= r->getEnd())
			{
				//random = true;
				allrandomCs.push_back(c);
			}
		}
	}
	while(noise > 0)
	{
		int pos = rand() % allrandomCs.size();
		CforNoise.push_back(allrandomCs[pos]);
		noise--;
	}
	return CforNoise;
}

/*
	Adding noise to RR
*/

void addNoise(vector<long> Cpositions, vector<Region*> randomregions, string filename, string chromosome_name, float percent)
{
	ofstream methTable(filename);
	ofstream master_file(masterFile, ios::in | ios::out | ios::app);
	float random_meth[] = {0.0, 0.1, 0.2};
	float regular_meth[] = {0.7, 0.8, 0.9, 1.0};
	vector<long> CforNoise;
	vector<long>::iterator it;

	CforNoise = getCytosine(Cpositions, randomregions, percent);
	master_file << "METHYLATION TABLE" << endl;
	master_file << "--------------------------------------------------------" << endl;
	for(unsigned long i = 0; i < Cpositions.size(); i++)
	{
		methTable << chromosome_name << "\t";
		master_file << chromosome_name << "\t";
		long c  = Cpositions[i];
		methTable << c << "\t" << (c+1) << "\t";
		methTable << "CpG:" << (10 + (rand() % 11)) << "\t";
		master_file << c << "\t" << (c+1) << "\t";
		master_file << "CpG:" << (10 + (rand() % 11)) << "\t";

		bool random = false;
		for(unsigned int j = 0; j < randomregions.size(); j++)
		{
			Region *r = randomregions[j];
			if(c >= r->getStart() && c <= r->getEnd())
			{
				random = true;
				break;
			}
		}
		if(random)
		{
			it = find(CforNoise.begin(), CforNoise.end(), c);
			if(it != CforNoise.end())
			{
				methTable << "0.8";
				master_file << "0.8";
			}
			else
			{ 
				int pos = rand() % 3;
				if(pos == 0)
				{
					methTable << "0.0";
					master_file << "0.0";
				}
				else
				{
					methTable << random_meth[pos];
					master_file << random_meth[pos];
				}
			}
		}
		else
		{
	 		int pos = rand() % 4;
			methTable << regular_meth[pos];
			master_file << regular_meth[pos];
		}
		methTable << "\t+" << "\n";
		master_file << "\t+" << "\n";
	}
	methTable.close();
	master_file.close();
}
