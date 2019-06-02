//
// Created by Corentin on 19/06/2018.
//

#ifndef MILDEWMODEL_MODEL_H
#define MILDEWMODEL_MODEL_H


#include <vector>
#include <fstream>
#include "Cohorte.h"
#include "DefaultValues.h"

class Model {

private:

    std::vector<Cohorte> _cohortes;
    void createCohorte();
    double p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
    double D0, D1, Dc;
    double CUM0, CUM1;
    double Topt, Tmin;
    double FACT, SSA;
    double _primary_inoculum;
    std::vector < double > _hourly_temperatures;
    std::vector < double > _hourly_humidities;
    double seuil_HR;
    double seuil_Tmoy;
    std::string nextDate;

  //  std::vector<std::vector<double>> allTemperatures;
  //  std::vector<std::vector<double>> allHumidities;


    double TotalSporeReady_1;
    double TotalSporeReady;
    double generation;
    double active_generation;
    double new_spore;
    double cohorte_number;
    double start;
    int dead_cohort_number;
    double _total_ready_spore_number_t2;
    double _total_ready_spore_number_t3;
    
    bool condEndCohorte(int);
    bool condBeginCohorte();


public:

    Model(double, double);
   // void fillAllData(std::string, std::string);
   // double get_sporul();
    
    void compute();
    double _total_ready_spore_number_t;
    int rank;
    // std::string currentDate;
    double getseuil_HR();
    double getseuil_Tmoy();
    std::string getDate(std::string);
    std::vector<std::string> getAllDailyData(std::string);
    double getValueFromData(std::string );
    std::vector<double> getDoubleVector(std::vector<std::string>);
    void fillData(std::string, std::string);
};


#endif //MILDEWMODEL_MODEL_H
