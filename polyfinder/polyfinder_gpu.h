#define CL_HPP_ENABLE_EXCEPTIONS

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

#include <CL/cl.h>
#include <string>
#include <stdexcept>
#include <iostream>

class polyfinder_gpu
{
public:
	int m, t, d, n, k, count;
	long long iterations;

	// OpenCL structures
	cl_int error;
	cl_platform_id platform;
	cl_context context;
	cl_device_id device;
	cl_program program;
	cl_command_queue queue;
	char* kernelSource;
	cl_kernel kernel;
	size_t global_size, local_size;
	unsigned int i, j;

	// Data and buffers
	int stop;
	int kernelArgIndex;
	cl_mem candidates_buffer;
	cl_mem stop_buffer;
	cl_mem result_candidate_buffer;


	polyfinder_gpu() {};
	polyfinder_gpu(unsigned short, unsigned short);
	~polyfinder_gpu() {};
	unsigned short* check_length;
	unsigned short* codeword_length;
	long long getBitsLength(long long);
	long long getWeight(long long);
	long long* get_candidates(int);

	char* loadKernelSource(const char* filename);
	void checkOpenCLError(cl_int error, const char* message);

private:
	long long get_candidate(int);
	int get_k(int, int);
	int get_n(int, int);
	unsigned long long factorial(unsigned short, unsigned short);
	unsigned long long factorial(unsigned short);
};

