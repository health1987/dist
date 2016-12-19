#dist
A tool to calculate nucleotide diversity

The part of loading PLINK .tped & .tfam file is using PLINK 1.7 as a reference.

##Install:
* g++ dist.cpp -o dist

##Run:
* If you have PLINK format file named file.tped & file.tfam which generate by PLINK command '--recode transpose tab'. 
* You can run dist like: dist file file.results
* file.results is output file.

