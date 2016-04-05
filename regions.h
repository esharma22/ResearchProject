/*
Class that takes information from make_meth.cpp and creates regions
*/
class Region
{
	private:
		long start;
		long end;
		int siteCount;

	public:
		Region(long, long, int);
		~Region();
		void print();
		int no_of_sites();
		long getStart();
		long getEnd();
};
