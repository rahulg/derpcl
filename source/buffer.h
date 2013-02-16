/*!
 * @file curt/buffer.h
 * Definiton of the curt buffer class.
 *
 * @author Rahul A. G.
 */

#ifndef __CURT_BUFFER_H__
#define __CURT_BUFFER_H__

#include "common.h"
#include "env.h"

using namespace std;

namespace curt {

	enum class MemoryType : cl_mem_flags
	{
		ReadOnly = CL_MEM_READ_ONLY,
		WriteOnly = CL_MEM_WRITE_ONLY,
		ReadWrite = CL_MEM_READ_WRITE,
		PinnedReadOnly = CL_MEM_READ_ONLY | CL_MEM_ALLOC_HOST_PTR,
		PinnedWriteOnly = CL_MEM_WRITE_ONLY | CL_MEM_ALLOC_HOST_PTR,
		PinnedReadWrite = CL_MEM_READ_WRITE | CL_MEM_ALLOC_HOST_PTR
	};

	enum class MapMode : cl_map_flags
	{
		Read = CL_MAP_READ,
		Write = CL_MAP_WRITE,
		ReadWrite = CL_MAP_READ | CL_MAP_WRITE
	};

	template <class Tdata>
	class Buffer {

	public:

		Buffer(Environment env, MemoryType m_type, size_t size) throw(string) : _env(env), _m_type(m_type), _size(size) {

			cl_int error;

			_block = clCreateBuffer(_env.context(), (cl_mem_flags)_m_type, _size, NULL, &error);
			if (error != CL_SUCCESS) {
				throw string("Error creating buffer: ") + to_string(error);
			}

		}

		~Buffer() { clReleaseMemObject(_block); }

		Environment const& env() const { return _env; }
		MemoryType const& type() const { return _m_type; }
		cl_mem memory() const { return _block; }
		size_t size() const { return _size; }
		Tdata* data() { return _data; }

		void map(MapMode map_mode) {
			
			cl_int error;
			
			_data = clEnqueueMapBuffer(_env.queue(), _block, CL_TRUE, (cl_map_flags)_m_type, 0, _size, 0, NULL, NULL, &error);
			if (error != CL_SUCCESS) {
				throw string("Error mapping buffer: ") + to_string(error);
			}
		}

		void unmap() { clEnqueueUnmapMemObject(_env.queue(), _block, _data, 0, NULL, NULL); }

		cl_event queueRead(Tdata* destination) {
			cl_event ev;
			clEnqueueReadBuffer(_env.queue(), _block, CL_FALSE, 0, _size, destination, 0, NULL, &ev);
			return ev;
		}

		cl_event queueWrite(Tdata* source) {
			cl_event ev;
			clEnqueueWriteBuffer(_env.queue(), _block, CL_FALSE, 0, _size, source, 0, NULL, &ev);
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
