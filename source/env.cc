#include "env.h"

using namespace derpcl;

Environment::Environment(DeviceType dev_type) throw(string) {
	cl_int error;

	error = clGetPlatformIDs(1, &_platform, NULL);
	if (error != CL_SUCCESS) {
		throw string("OpenCL platform error: ") + cl_err_to_string(error);
	}

	_num_devices = 0;
	cl_device_id dev_id[3];

	error = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_CPU, 1, &_devices[0], NULL);
	if (error == CL_SUCCESS) {
		dev_id[_num_devices] = _devices[0];
		++_num_devices;
		_has_device[0] = true;
	}
	error = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_GPU, 1, &_devices[1], NULL);
	if (error == CL_SUCCESS) {
		dev_id[_num_devices] = _devices[1];
		++_num_devices;
		_has_device[1] = true;
	}
	error = clGetDeviceIDs(_platform, CL_DEVICE_TYPE_ACCELERATOR, 1, &_devices[2], NULL);
	if (error == CL_SUCCESS) {
		dev_id[_num_devices] = _devices[2];
		++_num_devices;
		_has_device[2] = true;
	}

	if (_num_devices == 0)
	{
		throw string("Unable to find any OpenCL devices.");
	}

	_context = clCreateContext(NULL, _num_devices, dev_id, NULL, NULL, &error);
	if (error != CL_SUCCESS)
	{
		throw string("OpenCL context error: ") + cl_err_to_string(error);
	}

	for (int i = 0; i < 2; ++i)
	{
		if (_has_device[i]) {
			_queues[i] = clCreateCommandQueue(_context, _devices[i], 0, &error);
			if (error != CL_SUCCESS)
			{
				throw string("OpenCL command queue error: ") + cl_err_to_string(error);
			}

			_txqueues[i] = clCreateCommandQueue(_context, _devices[i], 0, &error);
			if (error != CL_SUCCESS)
			{
				throw string("OpenCL command queue error: ") + cl_err_to_string(error);
			}
		}
	}

	setDefaultDevice(dev_type);
}

Environment::~Environment(){
	for (int i = 0; i < 2; ++i)
	{
		if (_has_device[i]) {
			clReleaseCommandQueue(_queues[i]);
		}
	}
	clReleaseContext(_context);
}

cl_platform_id const& Environment::platform() const {
	return _platform;
}

cl_context const& Environment::context() const {
	return _context;
}

cl_device_id const& Environment::device() const {
	return device(_default_device);
}

cl_command_queue const& Environment::queue() const {
	return queue(_default_device);
}

cl_command_queue const& Environment::txQueue() const {
	return txQueue(_default_device);
}

cl_device_id const& Environment::device(DeviceType device) const {
	return _devices[static_cast<int>(device)];
}

cl_command_queue const& Environment::queue(DeviceType device) const {
	return _queues[static_cast<int>(device)];
}

cl_command_queue const& Environment::txQueue(DeviceType device) const {
	return _txqueues[static_cast<int>(device)];
}

void Environment::setDefaultDevice(DeviceType device) throw(string) {
	if (_has_device[static_cast<int>(device)]) {
		_default_device = device;
	} else {
		throw string("No device of requested type available.");
	}
}

void Environment::finish() {
	finish(_default_device);
}

void Environment::finish(DeviceType device) {
	clFinish(_queues[static_cast<int>(device)]);
}
