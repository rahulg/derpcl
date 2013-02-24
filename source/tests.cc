#include <iostream>
#include "curt.h"

using namespace std;
using namespace curt;

Environment env;

int main() {

	Log logger;

	cout << "Testing curt::Log to console." << endl;
	logger.toConsole();
	logger << "Logging to console: YAY.";
	logger.endl();

	cout << R"(Testing curt::Log to "testfile".)" << endl;
	logger.toFile("testfile");
	logger << "Logging to file: YAY.";
	logger.endl();

	logger.toConsole();

	try {

		cout << "Testing curt::Environment." << endl;
		env.setDefaultDevice(DeviceType::GPU);
		cout << "Platform ID: " << env.platform() << endl;
		cout << "CPU Device " << env.device(DeviceType::CPU) << endl;
		cout << "GPU Device " << env.device(DeviceType::GPU) << endl;
		cout << "Accel Device " << env.device(DeviceType::Accel) << endl;
		cout << R"("Finish"ing)" << endl;
		env.finish();

		cout << "Testing curt::Program." << endl;
		Program pgm(env, "source/tests.cl");

		cout << "Testing curt::Kernel." << endl;
		Kernel krn1(pgm, "test1");
		Kernel krn2(pgm, "test2");

		cout << "Testing curt::Kernel::setArgument<int>." << endl;
		krn1.setArgument<int>(0, 5);
		krn1.setArgument<int>(1, 5);

		krn2.setArgument<int>(0, 5);
		krn2.setArgument<int>(1, 5);

		cout << "Testing curt::Buffer." << endl;
		Buffer<int> buf1(env, MemoryType::WriteOnly, 1);
		Buffer<int> buf2(env, MemoryType::WriteOnly, 1);

		cout << "Testing curt::Kernel::setArgument<int>(Buffer)." << endl;
		krn1.setArgument<int>(2, buf1);
		krn2.setArgument<int>(2, buf2);

		cout << "Testing curt::Kernel::run." << endl;
		krn1.run(1);
		krn2.run(1);

		cout << "Testing curt::Buffer::map." << endl;
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