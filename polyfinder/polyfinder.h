#ifndef POLYFINDER_H
#define POLYFINDER_H

#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>
#include <string>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include "polyfinder_result.h" // Meta-data

class polyfinder
{
public:
	polyfinder() {};
	polyfinder(unsigned short, unsigned short);
	PolyfinderResult find_cyclic_polynom();
	~polyfinder() {};
private:
	int m, t, d, n, k, count;
	long long iterations;
	std::ofstream fstream;
	long long get_candidate(int);
	int get_k(int, int);
	int get_n(int, int);
	int check_d(long long);
	unsigned short* check_length;
	unsigned short* codeword_length;
	long long codeword(long long, long long);
	long long reminder(long long, long long);
	int getBitsLength(long long);
	int getWeight(long long);
	bool check_reminder(long long);
	long long* get_candidates(int);
	unsigned long long factorial(unsigned short, unsigned short);
	unsigned long long factorial(unsigned short);
};

#endif // POLYFINDER_H