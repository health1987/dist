//
//  main.cpp
//  dst
//
//  Created by health on 15/3/5.
//  Copyright (c) 2015年 health. All rights reserved.
//
//	v1.0 calculate PGD.
//	v1.1 add calulating persentage of A,C,G,T per individuals 2015-05-04 12:43:10


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
//#include <sstream>
#include <ctime>
#include <iomanip>

#include "individual.h"
#include "locus.h"
#include "dist.h"

using namespace std;

/*
bool readString(FILE * fp, string & s)	//读到tped和tfam开头几列，如rs11028等字串的时候用的
{
	while (1)
	{
		s = "";
		bool done = false;
		char ch = fgetc(fp);

		if (ch == ' ' || ch == '\t')
		{
			if (done)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else if (ch == '\n' || ch == '\r' || feof(fp))
		{
			if (done)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			s += ch;
		}
	}
}
*/

// read tped and tfam file
void Dist::readTransposedData(string filename)
{

	/////////////////////
	// step1.read tfam //
	/////////////////////

	ifstream TFAM;
	string tfamname = filename + ".tfam";
	TFAM.open(tfamname.c_str());	//read .tfam

	//vector<Individual*> sample;	//hold indi's ptr
	//int ind = 0;	//num of individuals in tfam
	while (1)
	{
		//string st;
		Individual *person = new Individual;
		TFAM
			>> person->fid
			>> person->iid
			>> person->pat
			>> person->mat
			>> person->sex
			>> person->pheo;

		// last line
		if (person->fid == "")
		{
			delete person;
			break;
		}

		// increase person counter
		//ind++;

		// add individual to list
		SAMPLE.push_back(person);
	}

	TFAM.close();
	cout << "tfam done..." << endl;



	/////////////////////////////////////////////////////
	// step2.read tped //
	///////////////////////////////////////////////////

	ifstream TPED;
	string tpedname = filename + ".tped";
	TPED.open(tpedname.c_str());

	//vector<Locus*> snp;	//hold snps
	unsigned int numind = SAMPLE.size();
	//int snpnum = 0;
	while (!TPED.eof())
	{

		//////////////////////
		// 1).read snp info //
		//////////////////////

		Locus * loc = new Locus;	// snp info container
		//read (chr, rsid, start, end)
		TPED
			>> loc->chr
			>> loc->snpid
			>> loc->startpos
			>> loc->endpos;

		// skip last line
		if ( loc->chr == 0 )
		{
			delete loc;
			break;
		}

		SNP.push_back(loc);

		unsigned int ind = 0;	// count individual of samples-container
		//bool linedone = false;
		while (ind < numind)
		{
			//////////////////////
			// 2).read genotype //
			//////////////////////
			//cout << "line" <<linedone << endl;
			// reload individual && add 2 allele to it
			Individual *person = SAMPLE[ind];


			//first allele
			while (1)
			{
				char ch1 = TPED.get();	// tmp char

				// delimiter?
				if (ch1 == ' ' || ch1 == '\t') continue;

				// one individual finished?
				else if (ch1 == '\n' || ch1 == '\r' || TPED.eof()) break;

				else
				{
					// push this person's first-allele
					person->one.push_back(ch1); break;	// stop read first-allele
				}
			}	//has read first-allele
			//if (linedone) break;	//read next line

			//second allele
			while (1)
			{
				char ch2 = TPED.get();	// tmp char

				// delimiter?
				if (ch2 == ' ' || ch2 == '\t') continue;
				// one individual finished?
				else if (ch2 == '\n' || ch2 == '\r' || TPED.eof()) break;
				//read allele2
				else
				{
					// push this person's first-allele
					person->two.push_back(ch2);
					break;	// stop read first-allele
				}
			}	//has read second-allele
			ind++;
		}	//finish one line
	}	//finish tped
	//cout << "test" << endl;
	TPED.close();
}

/*
//calculate allele frequency
void Dist::calcFreq(vector<Locus*> & allsnp, vector<Individual*> & allsample)
{
	for (int i = 0; i < allsnp.size(); i++)
	{
		for (int j = 0; j < allsample.size(); j++)
		{
			char a1, a2;	//allele 1&2 tmp container
			a1 = allsample[j]->one[i];
			a2 = allsample[j]->two[i];
			int an = 0;		//allele num
			if (a1)
		}
	}

}
*/

void Dist::calcDist()
{
	clock_t start, stop;
	double lastsize = 0;
	//double during = 0;
	// first person
	for (unsigned int i = 0; i < SAMPLE.size(); i++)
	{
		lastsize = SAMPLE.size() - i;	// how many individuals last

		//count allele A,C,G,T's number
		
		A_CONTENT.push_back(\
			count(SAMPLE[i]->one.begin(), SAMPLE[i]->one.end(), 'A') \
			+ \
			count(SAMPLE[i]->two.begin(), SAMPLE[i]->two.end(), 'A') \
			);
		C_CONTENT.push_back(\
			count(SAMPLE[i]->one.begin(), SAMPLE[i]->one.end(), 'C') \
			+ \
			count(SAMPLE[i]->two.begin(), SAMPLE[i]->two.end(), 'C') \
			);
		G_CONTENT.push_back(\
			count(SAMPLE[i]->one.begin(), SAMPLE[i]->one.end(), 'G') \
			+ \
			count(SAMPLE[i]->two.begin(), SAMPLE[i]->two.end(), 'G') \
			);
		T_CONTENT.push_back(\
			count(SAMPLE[i]->one.begin(), SAMPLE[i]->one.end(), 'T') \
			+ \
			count(SAMPLE[i]->two.begin(), SAMPLE[i]->two.end(), 'T') \
			);
		
		start = clock();
        //cout << SAMPLE[i]->fid << endl;	// the person now dealed with
		// second person
		for (unsigned int j = i + 1; j < SAMPLE.size(); j++)
		{
			double cnt = 0;		//count valid number of allele between persons

			/* 150315
			vector<char>::iterator ia1 = SAMPLE[i]->one.begin();	//iterator
			vector<char>::iterator ia2 = SAMPLE[i]->two.begin();

			vector<char>::iterator ib1 = SAMPLE[j]->one.begin();
			vector<char>::iterator ib2 = SAMPLE[j]->two.begin();
			*/

			unsigned long alcnt = 0;		//allele counter
			char a1, a2, b1, b2;	// tmp allele container


			double distall = 0;		//total distance
			double dist0 = 0;		//distance 0
			double dist05 = 0;		//distance 0.5
			double dist1 = 0;		//distance 1



			while (alcnt < SAMPLE[i]->one.size())	//end of allele?
			{
				/*150315
				//read alleles
				a1 = *ia1; a2 = *ia2;
				b1 = *ib1; b2 = *ib2;
				*/
				a1 = SAMPLE[i]->one[alcnt];
				a2 = SAMPLE[i]->two[alcnt];
				b1 = SAMPLE[j]->one[alcnt];
				b2 = SAMPLE[j]->two[alcnt];

				// calculate the distance
				if ((a1 != a2) || (b1 != b2))	// AT vs. any type. dist==0.5 include NN
				{
					dist05++;
					cnt++;
				}
				else if ((a1 == a2) && (b1 == b2) && (a1 != b1) && (a1 != 'N') && (a1 != '0') && (b1 != 'N') && (b1 != '0'))	// AA vs TT and not N
				{
					dist1++;
					cnt++;
				}
				else if ((a1 == a2) && (b1 == b2) && (a1 == b1))	// AA vs AA
				{
					dist0++;
					cnt++;
				}
				/* 150315
				// next allele of this person
				ia1++; ia2++;
				ib1++; ib2++;
				*/
				alcnt++;
			}
			
			DISTANCE.push_back(dist05 *0.5 + dist1);
			DISTANCE_HET.push_back(dist05 *0.5);
			DISTANCE_HOM.push_back(dist1);
			VAILDSNP.push_back(cnt);
			//cout << SAMPLE[i]->fid <<"vs"<<SAMPLE[j]->fid<< endl;
		}	//second person is finished
		// calc the het num of first person
		vector<char>::iterator i1 = SAMPLE[i]->one.begin();	//iterator
		vector<char>::iterator i2 = SAMPLE[i]->two.begin();

		double het = 0;		//het num
		double notn = 0;	//snp num of NOT NN
		while (i1 != SAMPLE[i]->one.end())
		{
			char a1, a2;
			a1 = *i1; a2 = *i2;
			if (a1 != '0' && a1 != 'N' && a2 != '0' && a2 != 'N')
			{
				if (a1 != a2)	het++;	//
				notn++;					// add NOT-NN snp num
			}
			i1++; i2++;		//next snp
		}// calc finish
		SAMPLE[i]->het = het;	// het num of this person
		SAMPLE[i]->vaildsnp = notn;	//vaild snp of this person

		stop = clock();
		
		//cout.precision(2);
		//cout << (stop - start)*1.0/CLOCKS_PER_SEC << endl;
                double lasttime = (stop - start)*1.0 / CLOCKS_PER_SEC * (lastsize) / 2;
		cout << '\r' << "last  time: " << fixed << lasttime  << " seconds" << flush;
                //printf("\r%2f", ((stop - start)*1.0 / CLOCKS_PER_SEC * (lastsize) / 2));

		//cout << "persentage: " << fixed << 2 / lastsize * 100 << "%" << endl;
		//cout.flush();
	}
}

void Dist::getDist()
{
	for (unsigned long i = 0; i < DISTANCE.size(); i++)
	{
		cout << "now is:" << i << endl;
		cout << DISTANCE[i] << endl;
		cout << endl;

	}
}

void Dist::outPut(string outname)
{

	ofstream OUT;
	string of = outname + ".txt";
	OUT.open(outname.c_str(), ios::out);
	OUT.clear();

	////step1.///// wirte first 'ID' line
	OUT << "Matrix";						// fisrt line first column
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << SAMPLE[i]->fid;		//first line individual's ID
	}
	OUT << "\n";							//finish first line

	////step2.///// write matrix
	unsigned long total_step	= 0;							//counter in the DISTANCE,HET,VAILDSNP... vector
	unsigned long het_step		= 0;
	unsigned long hom_step		= 0;
	unsigned long vaild_step		= 0;

	for (unsigned int i = 0; i < SAMPLE.size() - 1; i++)				
	{
		OUT << SAMPLE[i]->fid;
		OUT << "\n";		//first column, indiv's ID

		///first/// het-pairwise-distance line
		OUT << "HET_dist";						//first column
		OUT << insertDelitmer(i + 1);			//blank befort the MATRIX
		// this indiv's PGD
		for (unsigned int j = i + 1; j < SAMPLE.size(); j++)
		{
			OUT << "\t" << fixed << DISTANCE_HET[het_step++];	//step +1
		}
		OUT << "\n";

		///second/// homo-pairwise-distance line
		OUT << "HOM_dist";						//first column
		OUT << insertDelitmer(i + 1);			//blank befort the MATRIX
		// this indiv's PGD
		for (unsigned int j = i + 1; j < SAMPLE.size(); j++)
		{
			OUT << "\t" << fixed << DISTANCE_HOM[hom_step++];	//step +1
		}
		OUT << "\n";

		///third/// total-pairwise-distance line
		OUT << "Tot_dist" ;					//first column
		OUT << insertDelitmer(i+1);				//blank befort the MATRIX
		// this indiv's PGD
		for (unsigned int j = i + 1; j < SAMPLE.size(); j++)
		{
			OUT << "\t" << fixed << DISTANCE[total_step++];		//step +1
		}
		OUT << "\n";

		///last/// vaildsnp-pairwise-distance line
		OUT << "Vaild_SNP";						//first column
		OUT << insertDelitmer(i + 1);			//blank befort the MATRIX
		// this indiv's PGD
		for (unsigned int j = i + 1; j < SAMPLE.size(); j++)
		{
			OUT << "\t" << fixed << VAILDSNP[vaild_step++];		//step +1
		}
		OUT << "\n";
		OUT << "\n";
	}


	/////step3.///// write second 'het num' line
	OUT << "HET_num";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << SAMPLE[i]->het;		//second line individual's het num
	}
	OUT << "\n";							//finish second line

	/////step4.///// write third 'vaild snp num' line
	OUT << "NotMiss";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << SAMPLE[i]->vaildsnp;	//second line individual's not-missing-snp num
	}
	OUT << "\n";							//finish third line

	/////step5.///// write 4th 'total snp num' line
	OUT << "Total";
	OUT << "\t" << SAMPLE[0]->one.size();
	OUT << "\n" << "\n";							//finish 4th line

	/////step6.///// write 5th 'A allele content' line
	OUT << "A CONT.";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << A_CONTENT[i];
	}
	OUT << "\n";

	/////step7.///// write 6th 'C allele content' line
	OUT << "C CONT.";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << C_CONTENT[i];
	}
	OUT << "\n";

	/////step8.///// write 7th 'G allele content' line
	OUT << "G CONT.";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << G_CONTENT[i];
	}
	OUT << "\n";

	/////step9.///// write 8th 'T allele content' line
	OUT << "T CONT.";
	for (unsigned int i = 0;
		i < SAMPLE.size(); i++)
	{
		OUT << "\t" << fixed << T_CONTENT[i];
	}
	OUT << "\n" << "\n";

	OUT.close();
}

string Dist::insertDelitmer(unsigned int num)
{
	string tmp = "";
	for (unsigned int i = 0; i < num; i++)
	{
		tmp += "\t";
	}
	return tmp;
}

int main(int argc, const char * argv[]) {
	// insert code here...
	//for (int i = 0; i < argc; i++)

	string fin = argv[1];
	//string fin = "test";

	vector<Locus*> inputloc;
	vector<Individual*> inputind;

	Dist d;
	cout << "reading tfile..." << endl;
	d.readTransposedData(fin);
	cout << "read tfile done..." << endl;
	d.calcDist();

	string fout = argv[2];
	d.outPut(fout);
	//d.outPut("test.txt");

	//return 0;
	//system("pause");
}
