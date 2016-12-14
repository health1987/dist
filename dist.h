#include <vector>
#include <string>

#include "individual.h"
#include "locus.h"

using namespace std;

#ifndef DIST_H
#define DIST_H

class Dist
{
public:
	// read tped and tfam file
	void readTransposedData(string);

	void calcFreq(vector<Locus*> & , vector<Individual*> & );
	void calcDist();
	void getDist();

	void outPut(string);
	string insertDelitmer(unsigned int);

	vector<Locus*> SNP;				// 
	vector<Individual*> SAMPLE;		// read from tfam
	vector<double> DISTANCE;		// total distance of pairwise
	vector<double> DISTANCE_HET;	// het distance of pairwise
	vector<double> DISTANCE_HOM;	// homo distance of pairwise
	vector<double> VAILDSNP;			// vail snp include: AT vs. NN, but NOT AA vs. NN

	vector<unsigned long> A_CONTENT;// allele A number
	vector<unsigned long> C_CONTENT;// above
	vector<unsigned long> G_CONTENT;// above
	vector<unsigned long> T_CONTENT;// above
};

#endif // !DIST_H