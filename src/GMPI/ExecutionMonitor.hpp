/**
 * @brief Definition of the execution monitor.
 * @file ExecutionMonitor.hpp
 * @author Renato Oliveira
 * @date 05/07/2012
 * @version 1.0.0
 * @since GMPI 1.0.0
 */

#ifndef GRUBI_GMPI_EXECUTIONMONITOR_HPP_INCLUDED
#define GRUBI_GMPI_EXECUTIONMONITOR_HPP_INCLUDED

#include <boost/mpi.hpp>
#include <iostream>
#include <fstream>
//#include <boost/thread.hpp>
#include "Logger.hpp"

namespace GMPI {
	/**
	 * @brief Class that provides MPI communication services.
	 * It provides message passing and interfaces to implement master/slave based
	 * programs. There's a control protocol to provide management functionalities.
	 */
	class ExecutionMonitor {
	protected:
		/// The MPI environment.
		boost::mpi::environment environment;
		/// The world communicator, contain all processes.
		boost::mpi::communicator worldCommunicator;
		/// The rank of the current process.
		int myRank;
		/// Total number of processes.
		int ranks;
		/// The rank of the master process.
		const int masterRank;
		/// The message tag for control messages.
		const int controlTag;
		
		/// Logging outrput stream for this monitor.
		std::ofstream *log_ostream;
		
	public:
		ExecutionMonitor(int &argc, char **argv);
		~ExecutionMonitor();
		/// Returns the rank of current process.
		int getRank() const { return this->myRank; }
		/// Returns if the current process is the master process or not.
		bool isMaster() const { return this->myRank == this->masterRank; }
		boost::mpi::communicator* getCommunicator() { return &(this->worldCommunicator); }
	};
}

#endif
