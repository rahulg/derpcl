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

#endif