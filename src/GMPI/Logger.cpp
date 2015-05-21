/**
 * @name Logger++ (LoggerPP)
 * @brief A logging system for C++, compatible with GMPI.
 * @file Logger.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @date 10/07/2012
 */

#include "Logger.hpp"

namespace loggerpp {
	unsigned int logging_level = level::INFO;
	std::ostream* output_stream = &std::cerr;
	void log(std::string message, level::log_level& message_level) {
		if (logging_level >= message_level && output_stream != 0) {
			std::stringstream output;
			char date[64];
			time_t current = std::time(0);
			std::strftime(date, 64, "[%d/%m/%Y %H:%M:%S]", std::gmtime(&current));
			output << date << " ";
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
	
	void set_logging_level(const unsigned int level) {
		logging_level = level;
	}
	void set_output_stream(std::ostream *output) {
		output_stream = output;
	}
}
