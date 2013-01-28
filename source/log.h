/*!
 * @file curt/logging.h
 * Logging class.
 *
 * @author Rahul A. G.
 */

#ifndef __CURT_LOG_H__
#define __CURT_LOG_H__

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

namespace curt {

	class Log {

	public:

		Log();
		Log(string const& name);
		~Log();

		void toFile(string name);
		void toConsole();

		void endl();

		template <typename T>
		Log& operator<<(T const& data) {
			*Log::_stream << data;
			return *this;
		}

	private:

		static string _filename;
		static bool _usingFile;
		static ostream* _stream;

	};

}

#endif