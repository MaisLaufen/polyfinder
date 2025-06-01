#include "polyfinder_gpu.h"

#define KERNEL_FILE "polyfinder_kernel.cl"

polyfinder_gpu::polyfinder_gpu(unsigned short _m, unsigned short _t)
	: m(_m), t(_t), stop(0), iterations(0)
{
	d = 2 * t + 1;
	k = get_k(m, t);
	n = get_n(m, k);
	count = 0;
}

void polyfinder_gpu::init_opencl() {
	cl_int error;

	error = clGetPlatformIDs(1, &platform, NULL);
	checkOpenCLError(error, "Failed to find OpenCL platform.");

	error = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	checkOpenCLError(error, "Failed to find OpenCL device.");

	context = clCreateContext(NULL, 1, &device, NULL, NULL, &error);
	checkOpenCLError(error, "Failed to create OpenCL context.");

	queue = clCreateCommandQueueWithProperties(context, device, 0, &error);
	checkOpenCLError(error, "Failed to create command queue.");

	kernelSource = loadKernelSource(KERNEL_FILE);
	if (!kernelSource) {
		checkOpenCLError(error, "Failed to load Kernel Source.");
	}

	program = clCreateProgramWithSource(context, 1, (const char**)&kernelSource, NULL, &error);
	checkOpenCLError(error, "Failed to create program.");

	error = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (error != CL_SUCCESS) {
		size_t logSize;
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &logSize);
		char* log = (char*)malloc(logSize);
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, logSize, log, NULL);
		fprintf(stderr, "Build log:\n%s\n", log);
		free(log);
		checkOpenCLError(error, "Failed to build program.");
	}

	kernel = clCreateKernel(program, "polyfinder_kernel", &error);
	checkOpenCLError(error, "Failed to create kernel.");
}

PolyfinderResult polyfinder_gpu::find_cyclic_polynom_gpu() {
	double start_time = omp_get_wtime();
	init_opencl();

	long long result_candidate = -1;
	do {
		long long* candidates = get_candidates(k);
		++iterations;

		cl_int error;
		cl_mem candidates_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(long long) * count, candidates, &error);
		checkOpenCLError(error, "Failed to create candidates buffer.");

		cl_mem stop_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, sizeof(int), 0, &error);
		checkOpenCLError(error, "Failed to create stop buffer.");

		cl_mem result_candidate_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(long long), NULL, &error);
		checkOpenCLError(error, "Failed to create result candidate buffer.");

		error = clEnqueueWriteBuffer(queue, stop_buffer, CL_TRUE, 0, sizeof(int), &stop, 0, nullptr, nullptr);
		checkOpenCLError(error, "Failed to write stop buffer.");

		int arg = 0;
		error |= clSetKernelArg(kernel, arg++, sizeof(cl_mem), &candidates_buffer);
		error |= clSetKernelArg(kernel, arg++, sizeof(cl_mem), &stop_buffer);
		error |= clSetKernelArg(kernel, arg++, sizeof(cl_mem), &result_candidate_buffer);
		error |= clSetKernelArg(kernel, arg++, sizeof(int), &count);
		error |= clSetKernelArg(kernel, arg++, sizeof(int), &m);
		error |= clSetKernelArg(kernel, arg++, sizeof(int), &t);
		error |= clSetKernelArg(kernel, arg++, sizeof(int), &d);
		error |= clSetKernelArg(kernel, arg++, sizeof(int), &n);
		checkOpenCLError(error, "Failed to set kernel arguments.");

		size_t global_size = count;
		error = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &global_size, nullptr, 0, nullptr, nullptr);
		checkOpenCLError(error, "Failed to enqueue NDRange kernel.");
		clFinish(queue);

		error = clEnqueueReadBuffer(queue, stop_buffer, CL_TRUE, 0, sizeof(int), &stop, 0, nullptr, nullptr);
		checkOpenCLError(error, "Failed to read stop buffer.");

		if (stop == 1) {
			error = clEnqueueReadBuffer(queue, result_candidate_buffer, CL_TRUE, 0, sizeof(long long), &result_candidate, 0, nullptr, nullptr);
			checkOpenCLError(error, "Failed to read result candidate buffer.");
		}

		clReleaseMemObject(candidates_buffer);
		clReleaseMemObject(stop_buffer);
		clReleaseMemObject(result_candidate_buffer);

		delete[] candidates;

		if (!stop) {
			k++;
			n++;
		}
	} while (!stop);

	double end_time = omp_get_wtime();

	// Очистка ресурсов
	clReleaseKernel(kernel);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	PolyfinderResult r;
	r.polynom = result_candidate;
	r.iterations = iterations;
	r.time_seconds = end_time - start_time;
	return r;
}

long long polyfinder_gpu::get_candidate(int k) {
	return pow(2, k) + 1;
}
long long* polyfinder_gpu::get_candidates(int k) {
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

long long polyfinder_gpu::getBitsLength(long long value) {
	int iters = 0;
	while (value != 0) {
		value = value >> 1;
		iters++;
	}
	return iters;
}
long long polyfinder_gpu::getWeight(long long value) {
	int weight = 0;

	while (value >= 1) {
		if (value % 2 == 1) {
			weight++;
		}
		value = value / 2;
	}
	return weight;
}

int polyfinder_gpu::get_k(int  m, int  t)
{
	short flag = 1;
	unsigned long long mask, count, fact1, fact2;
	int k = 1, n;
	do
	{
		count = 1;
		n = k + m;
		mask = static_cast<unsigned long long>(1) << k; // for what
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
int polyfinder_gpu::get_n(int m, int  k)
{
	return m + k;
}
unsigned long long polyfinder_gpu::factorial(unsigned short x, unsigned short y)
{
	unsigned long long result = 1;
	if (x <= y)
	{
		for (unsigned short i = (y - x) + 1; i <= y; i++)
			result = result * i;
	}
	return result;
}
unsigned long long polyfinder_gpu::factorial(unsigned short x)
{
	unsigned long long result = 1;
	for (unsigned short i = 2; i <= x; i++)
		result *= i;
	return result;
}
void polyfinder_gpu::checkOpenCLError(cl_int error, const char* message) {
	if (error != CL_SUCCESS) {
		fprintf(stderr, "Error: %s (%d)\n", message, error);
		exit(EXIT_FAILURE);
	}
}

char* polyfinder_gpu::loadKernelSource(const char* filename) {
	FILE* file;
	errno_t err = fopen_s(&file, filename, "rb");

	if (err != 0 || !file) {
		fprintf(stderr, "Failed to load kernel.\n");
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	size_t length = ftell(file);
	rewind(file);
	char* source = (char*)malloc(length + 1);
	fread(source, 1, length, file);
	source[length] = '\0';
	fclose(file);
	return source;
}