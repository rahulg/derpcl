#include "program.h"

using namespace derpcl;

bool Program::defaultBinary = false;

Program::Program(Environment& env, string filename, bool binary) throw(string) : _env(env) {
	if (binary) {
		_fromBin(_env.device(), filename);
	} else {
		_fromSrc(_env.device(), filename);
	}
}

Program::Program(Environment& env, DeviceType device, string filename, bool binary) throw(string) : _env(env) {
	if (binary) {
		_fromBin(_env.device(device), filename);
	} else {
		_fromSrc(_env.device(device), filename);
	}
}

Program::~Program() {
	clReleaseProgram(_program);
}

cl_program Program::program() {
	return _program;
}

Environment& Program::environment() {
	return _env;
}

void Program::_fromSrc(cl_device_id device, string& filename) throw(string) {

	struct stat prog_stat;
	int prog_fd = -1;
	char *prog_buf;
	size_t prog_size;

#ifdef DEBUG
	char *log_buf;
	size_t log_size;
#endif

	cl_int error;

	prog_fd = open(filename.c_str(), O_RDONLY);
	if (prog_fd == -1)
	{
		throw string("Failed to open file.");
	}

	fstat(prog_fd, &prog_stat);
	prog_size = prog_stat.st_size;
	prog_buf = (char *)mmap(NULL, prog_size, PROT_READ, MAP_SHARED, prog_fd, 0);
	if (prog_buf == MAP_FAILED)
	{
		throw string("Failed to map file.");
	}

	_program = clCreateProgramWithSource(_env.context(), 1, (const char**)&prog_buf, &prog_size, &error);
	if (error < 0)
	{
		throw string("Failed to create program from source: ") + cl_err_to_string(error);
	}

	munmap(prog_buf, prog_size);
	close(prog_fd);

#ifdef CL_RELAXED_MATH
	char opts[] = "-cl-fast-relaxed-math";
#else
	char opts[] = "";
#endif

	error = clBuildProgram(_program, 0, NULL, opts, NULL, NULL);
	if (error < 0)
	{

#ifdef DEBUG
		clGetProgramBuildInfo(_program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		log_buf = (char *)malloc(log_size + 1);
		log_buf[log_size] = '\0';
		clGetProgramBuildInfo(_program, device, CL_PROGRAM_BUILD_LOG, log_size+1, log_buf, NULL);
		printf("%s\n", log_buf);
		free(log_buf);
#endif

		throw string("Failed to build program: ") + cl_err_to_string(error);

	}

}

void Program::_fromBin(cl_device_id device, string& filename) throw(string) {

#ifdef DEBUG
	char *log_buf;
	size_t log_size;
#endif

	cl_int error;

	size_t fn_len = filename.length();
	char* fn = new char[fn_len+1];
	strcpy(fn, filename.c_str());
	_program = clCreateProgramWithBinary(_env.context(), 1, &device, &fn_len, (const unsigned char**)(&fn), NULL, &error);

#ifdef CL_RELAXED_MATH
	char opts[] = "-cl-fast-relaxed-math";
#else
	char opts[] = "";
#endif

	error = clBuildProgram(_program, 0, NULL, opts, NULL, NULL);
	if (error < 0)
	{

#ifdef DEBUG
		clGetProgramBuildInfo(_program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
		log_buf = (char *)malloc(log_size + 1);
		log_buf[log_size] = '\0';
		clGetProgramBuildInfo(_program, device, CL_PROGRAM_BUILD_LOG, log_size+1, log_buf, NULL);
		printf("%s\n", log_buf);
		free(log_buf);
#endif

		throw string("Failed to build program: ") + cl_err_to_string(error);

	}

}
