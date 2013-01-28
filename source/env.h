/*!
 * @file curt/env.h
 * Definiton of the curt environment class.
 *
 * @author Rahul A. G.
 */

#ifndef __CURT_ENV_H__
#define __CURT_ENV_H__

#include "common.h"
#include <array>

using namespace std;

namespace curt {

	enum class DeviceType : int {
		CPU = 0,
		GPU = 1,
		Accel = 2
	};

	class Environment {

	public:
		Environment() throw(string);
		Environment(const Environment& other) = delete;
		~Environment();

		cl_platform_id const& platform() const;
		cl_context const& context() const;

		cl_device_id const& device() const;
		cl_command_queue const& queue() const;

		cl_device_id const& device(DeviceType device) const;
		cl_command_queue const& queue(DeviceType device) const;

		void setDefaultDevice(DeviceType device) throw(string);
		void finish();
		void finish(DeviceType device);

		Environment& operator=(Environment const& other) = delete;

	private:

		Log logger;

		cl_platform_id _platform;
		cl_context _context;

		cl_uint _num_devices;
		bool _has_device[3] = { false, false, false };
		cl_device_id _devices[3];
		cl_command_queue _queues[3];

		DeviceType _default_device = DeviceType::CPU;

	};

}

#endif
