#include "kernel.h"

using namespace curt;

Kernel::Kernel(Program& program, string name) throw(string) : _program(program) {

	cl_int error;

	_kernel = clCreateKernel(_program.program(), name.c_str(), &error);
	if (error != CL_SUCCESS) {
		throw string("Error creating kernel: ") + cl_err_to_string(error);
	}

}

Kernel::~Kernel() {
	clReleaseKernel(_kernel);
}

cl_event Kernel::run(size_t units) throw(string) {
	return _run(_program.environment().queue(), 1, &units, 0, nullptr);
}

cl_event Kernel::run(size_t x, size_t y) throw(string) {
	size_t dimensions[2] = { x, y };
	return _run(_program.environment().queue(), 2, dimensions, 0, nullptr);
}

cl_event Kernel::run(size_t units, cl_event wait_event) throw(string) {
	return _run(_program.environment().queue(), 1, &units, 1, &wait_event);
}

cl_event Kernel::run(size_t x, size_t y, cl_event wait_event) throw(string) {
	size_t dimensions[2] = { x, y };
	return _run(_program.environment().queue(), 2, dimensions, 1, &wait_event);
}

cl_event Kernel::run(DeviceType device_type, size_t units) throw(string) {
	return _run(_program.environment().queue(), 1, &units, 0, nullptr);
}

cl_event Kernel::run(DeviceType device_type, size_t x, size_t y) throw(string) {
	size_t dimensions[2] = { x, y };
	return _run(_program.environment().queue(), 2, dimensions, 0, nullptr);
}

cl_event Kernel::run(DeviceType device_type, size_t units, cl_event wait_event) throw(string) {
	return _run(_program.environment().queue(), 1, &units, 1, &wait_event);
}

cl_event Kernel::run(DeviceType device_type, size_t x, size_t y, cl_event wait_event) throw(string) {
	size_t dimensions[2] = { x, y };
	return _run(_program.environment().queue(), 2, dimensions, 1, &wait_event);
}

cl_event Kernel::_run(cl_command_queue queue, size_t n_dim, size_t* dimensions, size_t n_wait, cl_event* wait_event) throw(string) {

	cl_int error;
	cl_event run_event;

	error = clEnqueueNDRangeKernel(queue, _kernel, n_dim, nullptr, dimensions, nullptr, n_wait, wait_event, &run_event);
	if (error != CL_SUCCESS) {
		throw string("Error running kernel: ") + cl_err_to_string(error);
	}

	return run_event;

}

