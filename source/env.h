/*!
 * @file derpcl/env.h
 * Definiton of the derpcl environment class.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_ENV_H__
#define __DERPCL_ENV_H__

#include "common.h"

using namespace std;

namespace derpcl {

	class Environment {

	public:

		Environment(DeviceType dev_type = DeviceType::CPU) throw(string);
		~Environment();

		cl_platform_id const& platform() const;
		cl_context const& context() const;

		cl_device_id const& device() const;
		cl_command_queue const& queue() const;
		cl_command_queue const& txQueue() const;

		cl_device_id const& device(DeviceType device) const;
		cl_command_queue const& queue(DeviceType device) const;
		cl_command_queue const& txQueue(DeviceType device) const;

		void setDefaultDevice(DeviceType device) throw(string);
		void finish();
		void finish(DeviceType device);

	private:

		cl_platform_id _platform;
		cl_context _context;

		cl_uint _num_devices;
		bool _has_device[3] = { false, false, false };
		cl_device_id _devices[3];
		cl_command_queue _queues[3], _txqueues[3];

		DeviceType _default_device;

	};

}

#endif
