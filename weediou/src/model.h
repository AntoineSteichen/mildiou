#ifndef WEEDIOU_MODEL_H
#define WEEDIOU_MODEL_H

#include <string>
#include <vector>

#include "cohorte.h"
#include "default_values.h"

class Model {
public:
    Model(double primary_inoculum, Parameters& parameters);

    void compute();
    void fillData(const std::string&, const std::string&);

    const std::vector < double >& temperatures() const
    { return _hourly_temperatures; }
    const std::vector < double >& humidities() const
    { return _hourly_humidities; }
    void temperatures(const std::vector < double >& t)
    { _hourly_temperatures = t; }
    void humidities(const std::vector < double >& h)
    { _hourly_humidities = h; }
    const std::string& next_date() const
    { return nextDate; }
    void next_date(const std::string& d)
    { nextDate = d; }

    double total_ready_spore_number() const
    { return _total_ready_spore_number_t; } ;

private:
    bool condEndCohorte(int);
    bool condBeginCohorte();
    void createCohorte();
    std::string getDate(const std::string&);
    std::vector < std::string > getAllDailyData(const std::string&);
    double getValueFromData(const std::string&);
    std::vector < double > getDoubleVector(const std::vector < std::string >&);

    // parameters
    double p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14;
    double Tmin, Topt;
    double D0, D1, Dc;
    double CUM0, CUM1;
    double FACT, SSA;
    double _primary_inoculum;
    double seuil_HR;
    double seuil_Tmoy;

    // inputs
    std::vector < double > _hourly_temperatures;
    std::vector < double > _hourly_humidities;

    std::string nextDate;

    // state variables
    int rank;
    double TotalSporeReady_1;
    double TotalSporeReady;
    double generation;
    double active_generation;
    double new_spore;
    double cohorte_number;
    double start;
    int dead_cohort_number;
    double _total_ready_spore_number_t;
    double _total_ready_spore_number_t2;
    double _total_ready_spore_number_t3;
    std::vector < Cohorte > _cohortes;
};

#endif
