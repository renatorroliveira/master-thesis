/**
 * @brief Implementation of the execution monitor.
 * @file ExecutionMonitor.cpp
 * @author Renato Oliveira
 * @date 05/07/2012
 * @version 1.0.0
 * @since GMPI 1.0.0
 */

#include "ExecutionMonitor.hpp"

namespace GMPI {
	ExecutionMonitor::ExecutionMonitor(int& argc, char** argv) :
	environment(argc, argv), worldCommunicator(),
	masterRank(0), controlTag(-1) {
		this->myRank = this->worldCommunicator.rank();
		this->ranks = this->worldCommunicator.size();
		std::stringstream sout;
		sout << "logs/monitor_" << this->myRank << ".log";
		this->log_ostream = new std::ofstream(sout.str().c_str(), std::ios::out);
		if (!(*(this->log_ostream))) {
			delete this->log_ostream;
			this->log_ostream = 0;
		}
		loggerpp::set_output_stream(this->log_ostream);
	}

	ExecutionMonitor::~ExecutionMonitor() {
		if (this->log_ostream != 0) {
			this->log_ostream->close();
			delete this->log_ostream;
		}
	}
}