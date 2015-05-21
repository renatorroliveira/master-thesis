#include <algorithm>
#include <limits>

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <boost/serialization/string.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <GMPI/ExecutionMonitor.hpp>
#include <GMPI/Logger.hpp>
#include "GeneticAlgorithm/GeneticAlgorithm.hpp"
#include "GeneticAlgorithm/MultiPopulationGeneticAlgorithm.hpp"
#include "GeneticAlgorithm/BatchEngine.hpp"

//#include  "stdint.h"  // <--- to prevent uint64_t errors! 

//#include "/usr/local/hadoop/src/c++/install/include/hadoop/Pipes.hh"
//#include "/usr/local/hadoop/src/c++/install/include/hadoop/TemplateFactory.hh"
//#include "/usr/local/hadoop/src/c++/install/include/hadoop/StringUtils.hh"

using namespace std;

int main(int argc, char** argv) {
	//GMPI::ExecutionMonitor monitor(argc, argv);
	
	std::srand(std::time(0));
	geneticnet::BatchEngine executionEngine("mainBatch.xml");
	executionEngine.execute();
	/*{
		std::ofstream out("mpga.xml", std::ios::out);
		boost::archive::xml_oarchive outarc(out);
		geneticnet::MPGA::Setup setup;
		outarc & BOOST_SERIALIZATION_NVP(setup);
	}*/
	return 0;
}
/*
class GeneticMapper : public HadoopPipes::Mapper {
public:
  // constructor: does nothing
  GeneticMapper( HadoopPipes::TaskContext& context ) {
  }
 
  // map function: receives a line, outputs (word,"1")
  // to reducer.
  void map( HadoopPipes::MapContext& context ) {
    //--- get line of text ---
    string line = context.getInputValue();
    double fitness;
    char saida[1000];
    char strtempo[100];
    char strfitness[100];

    int inicio = HadoopUtils::getCurrentMillis();

    std::srand(std::time(0));
    geneticnet::BatchEngine executionEngine(line);
    executionEngine.execute();
    fitness = executionEngine.retornaFitness();
   
    int final = HadoopUtils::getCurrentMillis();
    int tempo = (final - inicio)/1000;

    sprintf(strtempo, "%i", tempo);
    sprintf(strfitness, "%f", fitness);
    strcpy(saida, strtempo);
    strcat(saida, ",");
    strcat(saida, strfitness);

    context.emit( line, saida);
  }
};
 
class GeneticReducer : public HadoopPipes::Reducer {
public:
  // constructor: does nothing
  GeneticReducer(HadoopPipes::TaskContext& context) {
  }
 
  // reduce function
  void reduce( HadoopPipes::ReduceContext& context ) {
    int count = 0;
    int i = 0;
    double vfitness[10];
    double somaquad = 0;
    double desviop = 0;
    double coefvar = 0;
    double melhor = 1e100;
    double pior = 0;
    double media = 0;
    double tempo = 0;
    char saida[1000];
    char strmelhor[100];
    char strpior[100];   
    char strmedia[100]; 
    char strtempo[100];
    char strdesvio[100];
    char strcoefvar[100];
    //--- get all tuples with the same key, and count their numbers ---
    while ( context.nextValue() ) {

      std::vector<std::string> valores = HadoopUtils::splitString(context.getInputValue(), ",");
      if (melhor > HadoopUtils::toFloat(valores[1])){
	melhor = HadoopUtils::toFloat(valores[1]);
      }
      if (pior < HadoopUtils::toFloat(valores[1])){
	pior = HadoopUtils::toFloat(valores[1]);
      }
      media = media + HadoopUtils::toFloat(valores[1]);
      tempo = tempo + HadoopUtils::toFloat(valores[0]);
      vfitness[i]= HadoopUtils::toFloat(valores[1]);
      i++;
    }
    media = media / i;
    tempo = tempo / i;

    for (int j=0; j<i; j++){
      somaquad = somaquad + pow(vfitness[j] - media, 2);
    }
    desviop = sqrt(somaquad/i);
    coefvar = desviop/media*100;

    sprintf(strmedia, "%f", media);
    sprintf(strmelhor, "%f", melhor);
    sprintf(strpior, "%f", pior);
    sprintf(strtempo, "%f", tempo);
    sprintf(strdesvio, "%f", desviop);
    sprintf(strcoefvar, "%f", coefvar);
    strcpy(saida, strmelhor);
    strcat(saida, ", ");
    strcat(saida, strpior);
    strcat(saida, ", ");
    strcat(saida, strmedia);
    strcat(saida, ", ");
    strcat(saida, strtempo);
    strcat(saida, ", ");
    strcat(saida, strdesvio);
    strcat(saida, ", ");
    strcat(saida, strcoefvar);
   
    //--- emit (word, count) ---
    context.emit(context.getInputKey(),saida );
    melhor = 1e100;
    pior = 0;
    tempo = 0;
    media = 0;

  }
};


int main(int argc, char *argv[]) {
  return HadoopPipes::runTask(HadoopPipes::TemplateFactory< GeneticMapper, GeneticReducer >() );
}
*/
