#include <vector>
using namespace std;

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

class Individual
{
public:
	Individual()
	{
		one.resize(0);
		two.resize(0);
	}

	///////////////
	// tped info //
	///////////////

	// snp genotype
	vector<char> one;			//allele 1 ACGT
	vector<char> two;			//allele 2 ACGT

	vector<char>::iterator i1;	//allele 1 iterator
	vector<char>::iterator i2;	//allele 2 iterator

	double het;					//heterozygous snp of this person
	double vaildsnp;				//vaild snp num of this person
	///////////////
	// tfam info //
	///////////////
	string fid;
	string iid;
	string pat;
	string mat;
	string sex;
	string pheo;

};

#endif