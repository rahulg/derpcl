#include "log.h"

using namespace curt;
using namespace std;

string Log::_filename;
bool Log::_usingFile;
ostream* Log::_stream;

Log::Log(){
	toConsole();
}

Log::Log(string const& name) {
	toFile(name);
}

Log::~Log() {
	if (Log::_usingFile && Log::_stream != nullptr && Log::_stream != &cerr)
	{
		((ofstream*)Log::_stream)->close();
		delete Log::_stream;
	}
}

void Log::toFile(string name) {
	Log::_filename = name;
	Log::_usingFile = true;
	Log::_stream = new ofstream(Log::_filename.c_str());
}

void Log::toConsole() {
	if (Log::_usingFile && Log::_stream != nullptr && Log::_stream != &cerr)
	{
		((ofstream*)Log::_stream)->close();
		delete Log::_stream;
	}
	Log::_filename = "";
	Log::_usingFile = false;
	Log::_stream = &cerr;
}

void Log::endl() {
	*_stream << std::endl;
}
