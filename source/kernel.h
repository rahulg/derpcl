/*!
 * @file derpcl/kernel.h
 * Definiton of the derpcl kernel class.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_KERNEL_H__
#define __DERPCL_KERNEL_H__

#include "common.h"
#include "env.h"
#include "buffer.h"
#include "image.h"
#include "program.h"

#include <ostream>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

namespace derpcl {

	class Kernel {

	public:

		Kernel(Program& program, string name) throw(string);
		~Kernel();

		template <class T>
		void setArgument(int position, T const& argument) throw(string) {

			cl_int error;

			error = clSetKernelArg(_kernel, position, sizeof(T), &argument);
			if (error != CL_SUCCESS) {
				throw string("Error setting kernel argument: ") + cl_err_to_string(error);
			}

		}

		template <class Tdata>
		void setArgumentBuffer(int position, Buffer<Tdata>& argument) {

			cl_int error;

			error = clSetKernelArg(_kernel, position, sizeof(cl_mem), argument.block_ptr());
			if (error != CL_SUCCESS) {
				throw string("Error setting kernel argument: ") + cl_err_to_string(error);
			}

		}

		// template <class Tdata>
		// void setArgument(int position, Image<Tdata> const& argument) {

		// 	cl_int error;

		// 	error = clSetKernelArg(_kernel, position, sizeof(cl_mem), &argument.block());
		// 	if (error != CL_SUCCESS) {
		// 		throw string("Error setting kernel argument: ") + cl_err_to_string(error);
		// 	}

		// }

		cl_event run(size_t units) throw(string);
		cl_event run(size_t x, size_t y) throw(string);

		cl_event run(size_t units, cl_event wait_event) throw(string);
		cl_event run(size_t x, size_t y, cl_event wait_event) throw(string);

		cl_event run(DeviceType device_type, size_t units) throw(string);
		cl_event run(DeviceType device_type, size_t x, size_t y) throw(string);

		cl_event run(DeviceType device_type, size_t units, cl_event wait_event) throw(string);
		cl_event run(DeviceType device_type, size_t x, size_t y, cl_event wait_event) throw(string);

	private:

		cl_event _run(cl_command_queue queue, size_t n_dim, size_t* dimensions, size_t n_wait, cl_event* wait_event) throw(string);

		Program& _program;

		cl_kernel _kernel;

	};

}

#endif
