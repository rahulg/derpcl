#include <iostream>
#include "curt.h"

using namespace std;
using namespace curt;

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
	cout << "Testing curt::Environment." << endl;
	Environment env;
	try {
		env.setDefaultDevice(DeviceType::GPU);
		cout << "Platform ID: " << env.platform() << endl;
		cout << "CPU Device " << env.device(DeviceType::CPU) << endl;
		cout << "GPU Device " << env.device(DeviceType::GPU) << endl;
		cout << "Accel Device " << env.device(DeviceType::Accel) << endl;
		cout << R"("Finish"ing)" << endl;
		env.finish();
	} catch (string const& excep) {
		logger << excep;
		logger.endl();
	}

	return 0;
}