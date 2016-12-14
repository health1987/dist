#include <vector>
#include "individual.h"
using namespace std;

#ifndef LOCUS_H
#define LOCUS_H

class Locus
{
public:
	//constructor
	Locus()
	{
		chr = 0;
		snpid = "";
		startpos = 0;
		endpos = 0;
	}

	//copy constructor
	Locus(const Locus& h1) { copy(h1); }	//implicit
	Locus & operator=(const Locus& h1)		//explicit
	{
		copy(h1);
    }

	int chr;			// chromosome num
	string snpid;		// snp id rs12222

	double startpos;	// start position
	double endpos;		// end position




	void copy(const Locus& h1)
	{
		chr = h1.chr;
		snpid = h1.snpid;

		startpos = h1.startpos;
		endpos = h1.endpos;
	}

	//void calcFreq(const Locus &, const Individual &);	//calculate frequency of snp

};

#endif
