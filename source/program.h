/*!
 * @file derpcl/program.h
 * Definiton of the derpcl program class.
 *
 * @author Rahul A. G.
 */

#ifndef __DERPCL_PROGRAM_H__
#define __DERPCL_PROGRAM_H__

#include "common.h"
#include "env.h"

#include <ostream>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

namespace derpcl {

	class Program {

	public:

		Program(Environment& env, string file, bool binary = defaultBinary) throw(string);
		Program(Environment& env, DeviceType device, string file, bool binary = defaultBinary) throw(string);
		~Program();

		cl_program program();
		Environment& environment();

		static bool defaultBinary;

	private:

		void _fromSrc(cl_device_id device, string& filename) throw(string);
		void _fromBin(cl_device_id device, string& filename) throw(string);

		Environment& _env;

		cl_program _program;


	};

}

#endif
