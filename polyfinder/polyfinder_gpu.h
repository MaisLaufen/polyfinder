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
#include "polyfinder_result.h"

#include <CL/cl.h>
#include <string>
#include <stdexcept>
#include <iostream>

class polyfinder_gpu
{
public:
	polyfinder_gpu() {};
	polyfinder_gpu(unsigned short, unsigned short);
	PolyfinderResult find_cyclic_polynom_gpu();
	~polyfinder_gpu() {};

private:
	int m, t, d, n, k, count;
	long long iterations = 0;
	void init_opencl();

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

	// Data and buffers
	int stop;
	long long candidate;
	long long result_candidate;
	cl_mem candidates_buffer;
	cl_mem stop_buffer;
	cl_mem result_candidate_buffer;

	long long getBitsLength(long long);
	long long getWeight(long long);
	long long* get_candidates(int);

	char* loadKernelSource(const char* filename);
	void checkOpenCLError(cl_int error, const char* message);

	long long get_candidate(int);
	int get_k(int, int);
	int get_n(int, int);
	unsigned long long factorial(unsigned short, unsigned short);
	unsigned long long factorial(unsigned short);
};

