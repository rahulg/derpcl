/*!
 * @file derpcl/buffer.h
 * Definiton of the derpcl buffer class.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_BUFFER_H__
#define __DERPCL_BUFFER_H__

#include "common.h"
#include "env.h"

using namespace std;

namespace derpcl {

	template <class Tdata>
	class Buffer {

	public:

		Buffer(Environment& env, MemoryType m_type, size_t count) throw(string) : _env(env), _m_type(m_type) {

			cl_int error;

			_size = count * sizeof(Tdata);
			_block = clCreateBuffer(_env.context(), (cl_mem_flags)_m_type, _size, NULL, &error);
			if (error != CL_SUCCESS) {
				throw string("Error creating buffer: ") + cl_err_to_string(error);
			}

		}

		~Buffer() { clReleaseMemObject(_block); }

		Environment const& env() const { return _env; }
		MemoryType const& type() const { return _m_type; }
		cl_mem const& block() { return _block; }
		size_t size() const { return _size; }
		Tdata* data() { return _data; }

		void map(MapMode map_mode) {
			
			cl_int error;
			
			_data = (Tdata*)clEnqueueMapBuffer(_env.txQueue(), _block, CL_TRUE, static_cast<cl_map_flags>(map_mode), 0, _size, 0, NULL, NULL, &error);
			if (error != CL_SUCCESS) {
				throw string("Error mapping buffer: ") + cl_err_to_string(error);
			}
		}

		void unmap() {
			cl_event unmap_ev;
			clEnqueueUnmapMemObject(_env.txQueue(), _block, _data, 0, NULL, &unmap_ev);
			clWaitForEvents(1, &unmap_ev);
		}

		cl_event queueRead(Tdata* destination, bool blocking = false) {
			cl_event ev;
			clEnqueueReadBuffer(_env.txQueue(), _block, blocking ? CL_TRUE : CL_FALSE, 0, _size, destination, 0, NULL, &ev);
			return ev;
		}

		cl_event queueWrite(Tdata* source, bool blocking = false) {
			cl_event ev;
			clEnqueueWriteBuffer(_env.txQueue(), _block, blocking ? CL_TRUE : CL_FALSE, 0, _size, source, 0, NULL, &ev);
			return ev;
		}

		cl_event queueRead(Tdata* destination, cl_event wait_event, bool blocking = false) {
			cl_event ev;
			clEnqueueReadBuffer(_env.txQueue(), _block, blocking ? CL_TRUE : CL_FALSE, 0, _size, destination, 1, &wait_event, &ev);
			return ev;
		}

		cl_event queueWrite(Tdata* source, cl_event wait_event, bool blocking = false) {
			cl_event ev;
			clEnqueueWriteBuffer(_env.txQueue(), _block, blocking ? CL_TRUE : CL_FALSE, 0, _size, source, 1, &wait_event, &ev);
			return ev;
		}

		Buffer<Tdata> const& operator=(Buffer<Tdata> const& other) = delete;

	private:

		Environment& _env;
		MemoryType _m_type;

		cl_mem _block;
		size_t _size;
		Tdata* _data;

	};

}

#endif
