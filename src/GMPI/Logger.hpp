/**
 * @name Logger++ (LoggerPP)
 * @brief A logging system for C++, compatible with GMPI.
 * @file Logger.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @date 10/07/2012
 */

#ifndef GRUBI_LOGGERPP_HPP_INCLUDED
#define GRUBI_LOGGERPP_HPP_INCLUDED

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <string>
#include <sstream>
#include <sys/time.h>
#include <typeinfo>

/// The main namespace for the Logger++.
namespace loggerpp {
	/// Logging levels definitions remains in this namespace.
	namespace level {
		typedef const unsigned int log_level;
		/// Information purposes messages.
		log_level INFO = 3;
		/// Warnings (medium priority).
		log_level WARNING = 2;
		/// Severe problems, but didn't crash the system.
		log_level SEVERE = 1;
		/// Fatal errors, highest priority, crash in the system. Aborts execution.
		log_level FATAL_ERROR = 0;
	}
	extern unsigned int logging_level;
	extern std::ostream* output_stream;
	
	void log(std::string message, level::log_level& message_level);
	void set_logging_level(const unsigned int level);
	void set_output_stream(std::ostream * output);
	
	/// Logs message with the origin type name.
	template<typename T>
	void log(std::string message, level::log_level& message_level, const T* origin) {
		if (logging_level >= message_level && output_stream != 0) {
			std::stringstream output;
			char date[64];
			time_t current = std::time(0);
			std::strftime(date, 64, "[%d/%m/%Y %H:%M:%S]", std::gmtime(&current));
			output << date << " " << typeid(origin).name() << ":\n                      ";
			if (message_level == level::FATAL_ERROR)
				output << "ERROR: ";
			else if (message_level == level::SEVERE)
				output << "SEVERE: ";
			else if (message_level == level::WARNING)
				output << "WARNING: ";
			else if (message_level == level::INFO)
				output << "INFO: ";
			else
				output << "OTHER: ";
			output << message << std::endl;
		
			(*output_stream) << output.str();
		}
		if (message_level == level::FATAL_ERROR) {
			output_stream->flush();
			abort();
		}
	}
}

#endif
