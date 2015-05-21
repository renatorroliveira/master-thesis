/**
 * @file ResultManager.hpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 02/12/2012
 */

#ifndef GRUBI_GENETICNET_ResultManager_HPP_INCLUDED
#define GRUBI_GENETICNET_ResultManager_HPP_INCLUDED

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <cstdlib>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_archive_exception.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/nvp.hpp>
#include <GMPI/Logger.hpp>

using std::ofstream;
using std::ios;
using std::string;
using std::vector;
using std::map;

namespace geneticnet {
	class ResultManager {
		friend class boost::serialization::access;
	public:
		ResultManager(string resultsFilename, int nExecutions = 10);
		~ResultManager();
		
		/// Calculate averages and write results to file.
		void calculate_averages();
		
		/// Returns the output stream.
		std::ostream& getOutput();
		
		/// Start an execution.
		void startExecution();
		/// Ends an execution.
		void finishExecution();
		
		/// Put the headers for a given execution.
		void put_headers(int columns, string headers[], int nLines = 10);
		/// Put a line of results.
		void put_line(string firstColumn, double valueColumns[]);
		/// Put the best result so far headers.
		void put_best_headers(int columns, string headers[]);
		/// Put the best result so far.
		void put_best_values(string firstColumn, double valueColumns[]);
		
		/*
		/// Put line with 2 values
		void put_line(double val1, double val2);
		void put_line(string val1, double val2);
		/// Put line with 3 values
		void put_line(double val1, double val2, double val3);
		void put_line(string val1, double val2, double val3);
		/// Put line with 4 values
		void put_line(double val1, double val2, double val3, double val4);
		void put_line(string val1, double val2, double val3, double val4);
		/// Put line with 5 values
		void put_line(double val1, double val2, double val3, double val4, double val5);
		void put_line(string val1, double val2, double val3, double val4, double val5);
		/// Put line with 6 values
		void put_line(double val1, double val2, double val3, double val4, double val5, double val6);
		void put_line(string val1, double val2, double val3, double val4, double val5, double val6);
		/// Put line with 7 values
		void put_line(double val1, double val2, double val3, double val4, double val5, double val6, double val7);
		void put_line(string val1, double val2, double val3, double val4, double val5, double val6, double val7);
		/// Put line with 8 values
		void put_line(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);
		void put_line(string val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8);
		/// Put line with 9 values
		void put_line(double val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8, double val9);
		void put_line(string val1, double val2, double val3, double val4, double val5, double val6, double val7, double val8, double val9);
		*/
	protected:
		ofstream out;
		bool execution_started;
		bool headers_setted;
		bool best_headers_setted;
		int executions;
		string outfilename;
		
		vector<string> *headers;
		vector<string> *first_column;
		vector<vector<double>*> *values;
		vector<vector<string>*> executions_headers;
		vector<vector<string>*> executions_first_column;
		vector<vector<vector<double>*>*> executions_values;
		
		vector<string> *best_headers;
		string best_first_column;
		vector<double> *best_values;
		vector<vector<string>*> executions_best_headers;
		vector<string> executions_best_first_column;
		vector<vector<double>*> executions_best_values;
	};
}

#endif
