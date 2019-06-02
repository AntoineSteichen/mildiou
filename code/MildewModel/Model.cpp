//
// Created by Corentin on 19/06/2018.
//


#include "Model.h"

#include <utility>

Model::Model(double seuilHR, double seuilTmoy):
        p1(DEFAULT_p1), p2(DEFAULT_p2), p3(DEFAULT_p3), p4(DEFAULT_p4), p5(DEFAULT_p5),
        p6(DEFAULT_p6), p7(DEFAULT_p7), p8(DEFAULT_p8), p9(DEFAULT_p9), p10(DEFAULT_p10),
        p11(DEFAULT_p11), p12(DEFAULT_p12), p13(DEFAULT_p13), p14(DEFAULT_p14), Tmin(DEFAULT_tmin), Topt(DEFAULT_topt),
        D0(DEFAULT_d0), D1(DEFAULT_d1), Dc(DEFAULT_dc), CUM0(DEFAULT_cum0), CUM1(DEFAULT_cum1), FACT(DEFAULT_fact), SSA(DEFAULT_ssa),
        seuil_HR(seuilHR), seuil_Tmoy(seuilTmoy)
{
    generation = 0; 
    active_generation = 0;
    TotalSporeReady_1 = 0;
    TotalSporeReady = 0;
    _total_ready_spore_number_t3 = 0;
    dead_cohort_number = 0;
    start = 0;
    _cohortes.clear();
    _primary_inoculum = 1;
    _total_ready_spore_number_t = 0.0;
    nextDate = "";

}

double Model::getseuil_HR() {
    return seuil_HR;
}


double Model::getseuil_Tmoy() {
    return seuil_Tmoy;
}


bool Model::condEndCohorte(int k)
{ return _cohortes[k].age() > p9 || _cohortes[k].spore() == 0; }

bool Model::condBeginCohorte()
{ return _primary_inoculum > 0.0; }

void Model::compute() {
    _total_ready_spore_number_t = 0.0;
    createCohorte();

    for (unsigned int i = 0; i < _cohortes.size(); ++i)
    {
        for (unsigned int j = 0; j < 24; j++)
        {
            _cohortes[i].compute(_hourly_temperatures[j], _hourly_humidities[j]);
            _total_ready_spore_number_t += _cohortes[i].sporul();
            if (condEndCohorte(i))
            {
                ++dead_cohort_number;
                _cohortes.erase(_cohortes.begin() + i);
                i--;
                break;
            }
        }
    }
}

void Model::createCohorte() {
    if (condBeginCohorte()) {
        ++generation;
        _cohortes.emplace_back((int)generation, _primary_inoculum, p1, p2, p3, p4, p5,
                        p6, p7,
                        p8, p9, p10, p11, p12, p13, p14, D0, D1, Dc, CUM0,
                        CUM1, Topt, Tmin, FACT, SSA, seuil_HR, seuil_Tmoy);
    }
}


std::string Model::getDate(std::string maString)
{
  unsigned int i = 0;
  int cpt = 0;
  while(cpt < 3)
    {
      if(maString[i] == ';')
	cpt++;
      i++;
    }
  if(cpt == 3)
    return maString.substr(0, i-1);
  else return 0;
}

std::vector<std::string> Model::getAllDailyData(std::string file)
{
  std::vector<std::string> pastDays;
  std::string myString ;
  std::string currentDate;
  //std::string nextDate; // DOIT ETRE UN ATTRIBUT DE MODEL
  std::vector<std::string> datas;
  //nextDate = "16;2;2016";
   std::ifstream fichier(file, std::ios::in);
   // if pastDays != "" alors pushback nextDate
   if(fichier)
     {
       getline(fichier, myString);
       if(nextDate != "")
	 {
	   while(getDate(myString) != nextDate)
	     {
	       getline(fichier, myString);
	     }
	 }
        
       currentDate = getDate(myString);
       //     std::cout << "Current Date: " << currentDate << std::endl;
       pastDays.push_back(currentDate);

       while(getDate(myString) == currentDate)
	 {
	   datas.push_back(myString.substr(currentDate.size()+1));
	   getline(fichier, myString);
	 }
       nextDate = getDate(myString);
       /*    for(int l = 0; l < datas.size(); l++)
	 {
	  std::cout << datas[l] << " ";
	 }
	 std::cout << std::endl;*/
       return datas; 
     }
   else
     {
       std::cout << "Erreur lecture fichier " << file << std::endl;
       exit(-1);
     }  
}

double Model::getValueFromData(std::string data)
{

  //std::cout << "Value: " <<  data.substr(data.find(";")+1) << std::endl;
  return std::stod(data.substr(data.find(";")+1));
}
  

std::vector<double> Model::getDoubleVector(std::vector<std::string> vector)
{
  std::string delimiter = ":";
  double sum = 0;
  int valueNbPH = 0;
  std::vector<double> finalResult; 
  std::string tmpHour =  vector[0].substr(0,  vector[0].find(delimiter));
  for(unsigned int i = 0; i < vector.size(); i++)
    {
      if(vector[i].substr(0,  vector[i].find(delimiter)) == tmpHour)
	{
	  sum += getValueFromData(vector[i]);
	  valueNbPH++;
	}
      else // new Hour
	{
	  finalResult.push_back(sum/valueNbPH);
	  sum = 0;
	  valueNbPH = 0;
	  tmpHour =  vector[i].substr(0,  vector[i].find(delimiter));
	  sum += getValueFromData(vector[i]);
	  valueNbPH++;
	}
      if(i == vector.size()-1) // Push back de la derniere valeur
	{
	   finalResult.push_back(sum/valueNbPH);
	}
      
    }
  return finalResult;
}

void Model::fillData(std::string t_fileName, std::string h_fileName)
{
  _hourly_temperatures.clear();
  _hourly_humidities.clear();
  std::string saveDate = nextDate;
  std::vector<std::string> allResultsT =  getAllDailyData(t_fileName);
  nextDate = saveDate; 
  std::vector<std::string> allResultsH =  getAllDailyData(h_fileName);

 _hourly_temperatures = getDoubleVector(allResultsT);
 _hourly_humidities   = getDoubleVector(allResultsH);
 std::ofstream fichier("ResultVectors.csv", std::ios::app);
  for(int i = 0; i < _hourly_temperatures.size(); i++)
    {
      fichier << "MILDIOU" << " " <<  _hourly_temperatures[i] << " " << _hourly_humidities[i] <<std::endl; 
    }
  
}
