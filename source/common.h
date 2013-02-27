/*!
 * @file derpcl/common.h
 * Common includes for derpcl.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_COMMON_H__
#define __DERPCL_COMMON_H__

#ifdef __APPLE__
// OpenCL.framework
#include <OpenCL/cl.h>
#else
// libOpenCL
#include <CL/cl.h>
#endif

#include <exception>

#include "log.h"

namespace derpcl {

	enum class DeviceType : int {
		CPU = 0,
		GPU = 1,
		Accel = 2
	};

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

	string cl_err_to_string(cl_int error);

}

#endif