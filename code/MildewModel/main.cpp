#include <iostream>
#include <functional>
#include <ctime>
#include "Model.h"
//#include <omp.h>





int main() {
  
  // double startTime2 = omp_get_wtime();
  std::string fileName = "sporuls";
  std::mt19937 gen(static_cast<unsigned int>(time(0)));
  std::normal_distribution<double> distributionHR(90,0.9);
  std::normal_distribution<double> distributionTmoy(18,0.18);

  std::vector<Model> modeles;

  //    #pragma omp parallel for ordered schedule(dynamic, anyChunkSizeGreaterThan1)
  for (int i = 0; i < 100; i++)
    {
      auto random_double_HR = distributionHR(gen);
      auto random_double_Tmoy = distributionTmoy(gen);
      //    	    std::cout << "HR : " << random_double_HR <<" Tmoy: " << random_double_Tmoy << std::endl;
      //     #pragma omp ordered
      modeles.emplace_back(random_double_HR, random_double_Tmoy);

    }
 
      //    #pragma omp parallel for ordered schedule(dynamic, anyChunkSizeGreaterThan1)
    
 
  for (int j = 0; j < 118; j++) // 15 fevrier - 30 septembre
    {
      std::cout << "Jour : " << j << std::endl;
      fileName = "../../data/results/sporuls"+std::to_string(j);
      std::ofstream fichier(fileName); 
      fichier << "sporuls" << ";" <<"seuilHR" << ";" << "seuilTmoy" <<std::endl; // Entrée dans le fichier
      if(fichier)
     	{
	  for (unsigned int i = 0; i< modeles.size(); i++)
	    {
	      modeles[i].fillData(R"(../../data/temperature2.csv)", R"(../../data/humidity2.csv)");
	      modeles[i].compute();
	      //  std::cout <<   modeles[i]._total_ready_spore_number_t << std::endl;
	      fichier <<  modeles[i]._total_ready_spore_number_t << ";" <<  modeles[i].getseuil_HR() << ";" <<  modeles[i].getseuil_Tmoy() <<std::endl; 
	    }
	 
	}
      else
	std::cout << "Impossible d'ouvrir le fichier !" << std::endl;
      fichier.close();
    }
  //   std::cout << "Resultat du modele : "  << i._total_ready_spore_number_t << std::endl;

  
 

  //  double endTime2 = omp_get_wtime();

  //std::cout << "Temps d'execution : " << endTime2 - startTime2 << std::endl;
  
  
  // std::vector<std::string> test1 =  testFunction(R"(../../data/data/20_22_2016/T.csv)");
  // std::vector<std::string> test2 =  testFunction(R"(../../data/data/20_22_2016/H.csv)");
  //  fillData(R"(../../data/data/cordier_2016/T.csv)", R"(../../data/data/cordier_2016/H.csv)");
  
  return 0;
}
