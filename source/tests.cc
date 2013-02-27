#include <iostream>
#include "derpcl.h"

using namespace std;
using namespace derpcl;

Environment env;

int main() {

	Log logger;

	cout << "Testing derpcl::Log to console." << endl;
	logger.toConsole();
	logger << "Logging to console: YAY.";
	logger.endl();

	cout << R"(Testing derpcl::Log to "testfile".)" << endl;
	logger.toFile("testfile");
	logger << "Logging to file: YAY.";
	logger.endl();

	logger.toConsole();

	try {

		cout << "Testing derpcl::Environment." << endl;
		env.setDefaultDevice(DeviceType::GPU);
		cout << "Platform ID: " << env.platform() << endl;
		cout << "CPU Device " << env.device(DeviceType::CPU) << endl;
		cout << "GPU Device " << env.device(DeviceType::GPU) << endl;
		cout << "Accel Device " << env.device(DeviceType::Accel) << endl;
		cout << R"("Finish"ing)" << endl;
		env.finish();

		cout << "Testing derpcl::Program." << endl;
		Program pgm(env, "source/tests.cl");

		cout << "Testing derpcl::Kernel." << endl;
		Kernel krn1(pgm, "test1");
		Kernel krn2(pgm, "test2");

		cout << "Testing derpcl::Kernel::setArgument<int>." << endl;
		krn1.setArgument<int>(0, 5);
		krn1.setArgument<int>(1, 5);

		krn2.setArgument<int>(0, 5);
		krn2.setArgument<int>(1, 5);

		cout << "Testing derpcl::Buffer." << endl;
		Buffer<int> buf1(env, MemoryType::WriteOnly, 1);
		Buffer<int> buf2(env, MemoryType::WriteOnly, 1);

		cout << "Testing derpcl::Kernel::setArgument<int>(Buffer)." << endl;
		krn1.setArgumentBuffer<int>(2, buf1);
		krn2.setArgumentBuffer<int>(2, buf2);

		cout << "Testing derpcl::Kernel::run." << endl;
		krn1.run(1);
		krn2.run(1);

		cout << "Testing derpcl::Buffer::map." << endl;
		buf1.map(MapMode::Read);
		buf2.map(MapMode::Read);

		cout << "Kernel 1: ";
		if (*buf1.data() == 25) {
			cout << "YAY" << endl;
		} else {
			cout << "FAILED" << endl;
		}

		cout << "Kernel 2: ";
		if (*buf2.data() == 1) {
			cout << "YAY" << endl;
		} else {
			cout << "FAILED" << endl;
		}

	} catch (string const& excep) {
		logger << excep;
		logger.endl();
	}

	return 0;
}