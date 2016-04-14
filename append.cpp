#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	ofstream trial("trial.txt", ios::in | ios::out | ios::app);
	trial << "This is the second sentence." << endl;

	trial.close();
	return 0;
}
