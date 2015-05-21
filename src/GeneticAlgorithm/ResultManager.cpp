/**
 * @file ResultManager.cpp
 * @author Renato Oliveira (renatorro@comp.ufla.br)
 * @version 1.0
 * @since GeneticNet 1.0
 * @date 02/12/2012
 */

#include "ResultManager.hpp"

using namespace loggerpp;

namespace geneticnet {
	ResultManager::ResultManager(std::string resultsFilename, int nExecutions) : out(resultsFilename.c_str(), ios::out) {
		if (!(this->out)) {
			log("Error opening results file: " + resultsFilename, level::FATAL_ERROR, this);
		}
		this->outfilename = resultsFilename;
		this->execution_started = false;
		this->headers_setted = false;
		this->best_headers_setted = false;
		this->executions = 0;
		
		this->executions_headers.reserve(nExecutions);
		this->executions_first_column.reserve(nExecutions);
		this->executions_values.reserve(nExecutions);
		this->executions_best_headers.reserve(nExecutions);
		this->executions_best_first_column.reserve(nExecutions);
		this->executions_best_values.reserve(nExecutions);
		
		this->headers = NULL;
		this->first_column = NULL;
		this->values = NULL;
		
		this->best_headers = NULL;
		this->best_first_column = "";
		this->best_values = NULL;
	}
	
	ResultManager::~ResultManager() {
		out.close();
	}
	
	std::ostream& ResultManager::getOutput() {
		return this->out;
	}
	
	void ResultManager::startExecution() {
		if (this->execution_started)
			log("Execution result manager already started.",level::FATAL_ERROR, this);
		
		this->execution_started = true;
		this->executions++;
		this->headers_setted = false;
		this->best_headers_setted = false;
		this->out << std::endl << "\n|===,Execution:," << this->executions << ",===|\n" << std::endl;
	}
	void ResultManager::finishExecution() {
		if (!(this->execution_started))
			log("Execution result manager not started yet.",level::FATAL_ERROR, this);
		
		if (this->headers_setted) {
			this->executions_headers.push_back(this->headers);
			this->executions_first_column.push_back(this->first_column);
			this->executions_values.push_back(this->values);
		}
		
		if (this->best_headers_setted) {
			out << std::endl << "Best,Result" << std::endl;
			for (size_t h = 0; h < this->best_headers->size(); h++) {
				if (h > 0)
					out << ",";
				out << this->best_headers->at(h);
			}
			out << std::endl << this->best_first_column;
			for (size_t v = 0; v < this->best_values->size(); v++) {
				out << "," << this->best_values->at(v);
			}
			out << std::endl;
			this->executions_best_headers.push_back(this->best_headers);
			this->executions_best_first_column.push_back(this->best_first_column);
			this->executions_best_values.push_back(this->best_values);
		}
		
		this->execution_started = false;
		this->headers = NULL;
		this->first_column = NULL;
		this->values = NULL;
		this->best_headers = NULL;
		this->best_first_column = "";
		this->best_values = NULL;
		this->out << std::endl;
		this->out.flush();
	}
	
	void ResultManager::put_headers(int columns, string *headers, int nLines) {
		if (!(this->execution_started))
			log("Need to start an execution first.",level::FATAL_ERROR, this);
		else if (columns < 2)
			log("You need 2 headers at least to use this class.",level::FATAL_ERROR, this);
		
		this->headers_setted = true;
		this->headers = new vector<string>(columns);
		this->first_column = new vector<string>();
		this->first_column->reserve(nLines);
		this->values = new vector<vector<double>*>();
		this->values->reserve(nLines);
		for (int h = 0; h < columns; h++) {
			if (h > 0)
				out << ",";
			out << headers[h];
			this->headers->at(h) = headers[h];
		}
		out << std::endl;
	}

	void ResultManager::put_line(string firstColumn, double valueColumns[]) {
		if (!(this->execution_started))
			log("Need to start an execution first.",level::FATAL_ERROR, this);
		else if (!(this->headers_setted))
			log("You must set the headers first.",level::FATAL_ERROR, this);
		
		out << firstColumn;
		this->first_column->push_back(firstColumn);
		vector<double> *aux = new vector<double>(this->headers->size()-1);
		this->values->push_back(aux);
		for (size_t v = 0; v < aux->size(); v++) {
			out << "," << valueColumns[v];
			aux->at(v) = valueColumns[v];
		}
		out << std::endl;
	}
	
	void ResultManager::put_best_headers(int columns, string headers[]) {
		if (!(this->execution_started))
			log("Need to start an execution first.",level::FATAL_ERROR, this);
		else if (columns < 2)
			log("You need 2 headers at least to use this class.",level::FATAL_ERROR, this);
		
		this->best_headers_setted = true;
		this->best_headers = new vector<string>(columns);
		this->best_first_column = "N/A";
		this->best_values = new vector<double>(columns-1);
		for (int h = 0; h < columns; h++) {
			this->best_headers->at(h) = headers[h];
			if (h > 0)
				this->best_values->at(h-1) = -1e200;
		}
	}
	void ResultManager::put_best_values(string firstColumn, double valueColumns[]) {
		if (!(this->execution_started))
			log("Need to start an execution first.",level::FATAL_ERROR, this);
		else if (!(this->best_headers_setted))
			log("You must set the headers for best result first.",level::FATAL_ERROR, this);
		
		this->best_first_column = firstColumn;
		for (size_t v = 0; v < this->best_values->size(); v++) {
			this->best_values->at(v) = valueColumns[v];
		}
	}
	
	
	
	void ResultManager::calculate_averages() {
		out.flush();
	}
}
