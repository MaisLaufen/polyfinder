#include "polyfinder.h"

using namespace std;

namespace fs = std::filesystem;

void append_int(std::string& text, int number) {
	text += std::to_string(number);
}

polyfinder::polyfinder(unsigned short _m, unsigned short _t)
{
	m = _m; // message length
	t = _t; // error count
	d = 2 * t + 1; // min code distance
	int w = d; // initial polynomial weight = min distance
	k = get_k(m, t);
	n = get_n(m, k);
	long long full_count = 0;
	iterations = 0;
	count = 0;

	std::string folder = "output";
	if (!fs::exists(folder)) {
		fs::create_directory(folder);
	}

	std::string filename = folder + "/";
	append_int(filename, m);
	filename += "_";
	append_int(filename, t);
	filename += ".txt";

	std::ofstream fstream(filename);
	if (!fstream) {
		std::cerr << "Error while loading file: " << filename << std::endl;
		return;
	}

	long long* candidates = new long long[10000000];
	int stop = 0;
	double start_time = omp_get_wtime();
	omp_set_num_threads(1);
	do
	{
		candidates = get_candidates(k);
		for (int i = 0; i < count; i++)
		{
			full_count++;
			if (!stop)
			{
				if (check_d(candidates[i]))
				{
					double end_time = omp_get_wtime();
					printf("full time: "); printf("%f - ", end_time - start_time);
					printf("%llu ", iterations); printf("iterations\n");
					printf("polynomials count: "); printf("%llu\n", full_count);
					printf("%d,", m); printf("%d ", t);
					if (!check_reminder(candidates[i])) {
						cout << "\n# Polynom is cyclic! #\n" << candidates[i] << endl;
						fstream << "polynom is cyclic!\n";
						fstream << iterations << "iterations.\n";
						fstream << "full time:" << (end_time - start_time) << "\n";
						fstream << "full count:" << full_count << "\n";
						fstream << "m: " << m << " t: " << t << "\n";
						fstream << int_to_bin(candidates[i]);
						fstream.close();
						stop = 1;
					}
				}
			}
		}
		if (!stop)
		{
			k++;
			n++;
			w = d;
		}
	} while (!stop);
}
long long polyfinder::codeword(long long data, long long generator) {
	long double _data = data * pow((long double)2, getBitsLength(generator) - 1);
	return _data + reminder(_data, generator);
}
long long polyfinder::reminder(long long value1, long long value2) {
	int count1 = getBitsLength(value1);
	int count2 = getBitsLength(value2);
	int delta = count1 - count2;
	long long result = value1;
	int i = 0;
	while (count1 >= count2) {
		result = result ^ (value2 << delta);
		count1 = getBitsLength(result);
		delta = count1 - count2;
	}
	return result;
}
int polyfinder::getBitsLength(long long value) {
	int iters = 0;
	while (value != 0) {
		value = value >> 1;
		iters++;
	}
	return iters;
}
int polyfinder::getWeight(long long value) {
	int weight = 0;

	while (value >= 1) {
		if (value % 2 == 1) {
			weight++;
		}
		value = value / 2;
	}
	return weight;
}
int polyfinder::check_d(long long candidate) {
	long long data = 1;
	do
	{
		iterations++;
		long long first = codeword(data, candidate);
		int distance = getWeight(first);
		if (d > distance) {
			return 0;
		}
		data++;
	} while (data < pow(2, m));
	return 1;
}
std::string polyfinder::int_to_bin(long decimal)
{
	if (decimal == 0) return "0";

	std::string binary = "";
	while (decimal > 0) {
		binary = std::to_string(decimal % 2) + binary;
		decimal /= 2;
	}
	return binary;
}
long long polyfinder::get_candidate(int k) {
	return pow(2, k) + 1;
}
long long* polyfinder::get_candidates(int k) {
	long long* candidates = new long long[10000000];
	long long candidate = get_candidate(k);
	count = 0;
	do
	{
		if (getWeight(candidate) >= d) {
			candidates[count] = candidate;
			count++;
		}
		candidate += 2;
	} while (candidate < pow(2, getBitsLength(candidate)) - 2);
	return candidates;
}
int polyfinder::get_k(int  m, int  t)
{
	short flag = 1;
	unsigned long long mask, count, fact1, fact2;
	int k = 1, n;
	do
	{
		count = 1;
		n = k + m;
		mask = 1 << k;
		for (short i = 1; i <= t; i++)
		{
			fact1 = factorial(i, n);
			fact2 = factorial(i);
			count += fact1 / fact2;
		}
		if (mask >= count) break;
		k++;
	} while (flag);
	return k;
}
int polyfinder::get_n(int m, int  k)
{
	return m + k;
}
unsigned long long polyfinder::factorial(unsigned short x, unsigned short y)
{
	unsigned long long result = 1;
	if (x <= y)
	{
		for (unsigned short i = (y - x) + 1; i <= y; i++)
			result = result * i;
	}
	return result;
}
unsigned long long polyfinder::factorial(unsigned short x)
{
	unsigned long long result = 1;
	for (unsigned short i = 2; i <= x; i++)
		result *= i;
	return result;
}
bool polyfinder::check_reminder(long long candidate) {
	long long poly = pow(2, n) + 1;
	long long _reminder = reminder(poly, candidate);
	if (_reminder == 0) return false;
	else return true;
};