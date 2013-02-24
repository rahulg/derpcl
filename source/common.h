/*!
 * @file curt/common.h
 * Common includes for curt.
 *
 * @author Rahul A. G.
 */

#ifndef __CURT_COMMON_H__
#define __CURT_COMMON_H__

#ifdef __APPLE__
// OpenCL.framework
#include <OpenCL/cl.h>
#else
// libOpenCL
#include <CL/cl.h>
#endif

#include <exception>

#include "log.h"

namespace curt {

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

	string cl_err_to_string(cl_int error) {
		switch (error) {
			case 0: return string("CL_SUCCESS"); break;
			case -1: return string("CL_DEVICE_NOT_FOUND"); break;
			case -2: return string("CL_DEVICE_NOT_AVAILABLE"); break;
			case -3: return string("CL_COMPILER_NOT_AVAILABLE"); break;
			case -4: return string("CL_MEM_OBJECT_ALLOCATION_FAILURE"); break;
			case -5: return string("CL_OUT_OF_RESOURCES"); break;
			case -6: return string("CL_OUT_OF_HOST_MEMORY"); break;
			case -7: return string("CL_PROFILING_INFO_NOT_AVAILABLE"); break;
			case -8: return string("CL_MEM_COPY_OVERLAP"); break;
			case -9: return string("CL_IMAGE_FORMAT_MISMATCH"); break;
			case -10: return string("CL_IMAGE_FORMAT_NOT_SUPPORTED"); break;
			case -11: return string("CL_BUILD_PROGRAM_FAILURE"); break;
			case -12: return string("CL_MAP_FAILURE"); break;
			case -13: return string("CL_MISALIGNED_SUB_BUFFER_OFFSET"); break;
			case -14: return string("CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"); break;
			case -15: return string("CL_COMPILE_PROGRAM_FAILURE"); break;
			case -16: return string("CL_LINKER_NOT_AVAILABLE"); break;
			case -17: return string("CL_LINK_PROGRAM_FAILURE"); break;
			case -18: return string("CL_DEVICE_PARTITION_FAILED"); break;
			case -19: return string("CL_KERNEL_ARG_INFO_NOT_AVAILABLE"); break;
			case -30: return string("CL_INVALID_VALUE"); break;
			case -31: return string("CL_INVALID_DEVICE_TYPE"); break;
			case -32: return string("CL_INVALID_PLATFORM"); break;
			case -33: return string("CL_INVALID_DEVICE"); break;
			case -34: return string("CL_INVALID_CONTEXT"); break;
			case -35: return string("CL_INVALID_QUEUE_PROPERTIES"); break;
			case -36: return string("CL_INVALID_COMMAND_QUEUE"); break;
			case -37: return string("CL_INVALID_HOST_PTR"); break;
			case -38: return string("CL_INVALID_MEM_OBJECT"); break;
			case -39: return string("CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"); break;
			case -40: return string("CL_INVALID_IMAGE_SIZE"); break;
			case -41: return string("CL_INVALID_SAMPLER"); break;
			case -42: return string("CL_INVALID_BINARY"); break;
			case -43: return string("CL_INVALID_BUILD_OPTIONS"); break;
			case -44: return string("CL_INVALID_PROGRAM"); break;
			case -45: return string("CL_INVALID_PROGRAM_EXECUTABLE"); break;
			case -46: return string("CL_INVALID_KERNEL_NAME"); break;
			case -47: return string("CL_INVALID_KERNEL_DEFINITION"); break;
			case -48: return string("CL_INVALID_KERNEL"); break;
			case -49: return string("CL_INVALID_ARG_INDEX"); break;
			case -50: return string("CL_INVALID_ARG_VALUE"); break;
			case -51: return string("CL_INVALID_ARG_SIZE"); break;
			case -52: return string("CL_INVALID_KERNEL_ARGS"); break;
			case -53: return string("CL_INVALID_WORK_DIMENSION"); break;
			case -54: return string("CL_INVALID_WORK_GROUP_SIZE"); break;
			case -55: return string("CL_INVALID_WORK_ITEM_SIZE"); break;
			case -56: return string("CL_INVALID_GLOBAL_OFFSET"); break;
			case -57: return string("CL_INVALID_EVENT_WAIT_LIST"); break;
			case -58: return string("CL_INVALID_EVENT"); break;
			case -59: return string("CL_INVALID_OPERATION"); break;
			case -60: return string("CL_INVALID_GL_OBJECT"); break;
			case -61: return string("CL_INVALID_BUFFER_SIZE"); break;
			case -62: return string("CL_INVALID_MIP_LEVEL"); break;
			case -63: return string("CL_INVALID_GLOBAL_WORK_SIZE"); break;
			case -64: return string("CL_INVALID_PROPERTY"); break;
			case -65: return string("CL_INVALID_IMAGE_DESCRIPTOR"); break;
			case -66: return string("CL_INVALID_COMPILER_OPTIONS"); break;
			case -67: return string("CL_INVALID_LINKER_OPTIONS"); break;
			case -68: return string("CL_INVALID_DEVICE_PARTITION_COUNT"); break;
			default: return string("Unknown Error."); break;
		}
	}

}

#endif