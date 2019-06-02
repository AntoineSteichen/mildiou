#include "model.h"

#include <fstream>
#include <iostream>

Model::Model(double primary_inoculum, Parameters& parameters):
    p1(parameters["p1"]), p2(parameters["p2"]), p3(parameters["p3"]),
    p4(parameters["p4"]), p5(parameters["p5"]), p6(parameters["p6"]),
    p7(parameters["p7"]), p8(parameters["p8"]), p9(parameters["p9"]),
    p10(parameters["p10"]), p11(parameters["p11"]), p12(parameters["p12"]),
    p13(parameters["p13"]), p14(parameters["p14"]), Tmin(parameters["tmin"]),
    Topt(parameters["topt"]), D0(parameters["d0"]), D1(parameters["d1"]),
    Dc(parameters["dc"]), CUM0(parameters["cum0"]), CUM1(parameters["cum1"]),
    FACT(parameters["fact"]), SSA(parameters["ssa"]),
    seuil_HR(parameters["seuilHR"]), seuil_Tmoy(parameters["seuilTmoy"])
{
    generation = 0;
    active_generation = 0;
    TotalSporeReady_1 = 0;
    TotalSporeReady = 0;
    _total_ready_spore_number_t3 = 0;
    dead_cohort_number = 0;
    start = 0;
    _cohortes.clear();
    _primary_inoculum = primary_inoculum;
    _total_ready_spore_number_t = 0.0;
    nextDate = "";
}

bool Model::condEndCohorte(int k)
{ return _cohortes[k].age() > p9 or _cohortes[k].spore() == 0; }

bool Model::condBeginCohorte()
{ return _primary_inoculum > 0.0; }

void Model::compute()
{
    _total_ready_spore_number_t = 0.0;
    createCohorte();
    for (unsigned int i = 0; i < _cohortes.size(); ++i) {
        for (unsigned int j = 0; j < 24; j++) {
            _cohortes[i].compute(_hourly_temperatures[j],
                                 _hourly_humidities[j]);
            _total_ready_spore_number_t += _cohortes[i].sporul();
            if (condEndCohorte(i)) {
                ++dead_cohort_number;
                _cohortes.erase(_cohortes.begin() + i);
                i--;
                break;
            }
        }
    }
}

void Model::createCohorte()
{
    if (condBeginCohorte()) {
        ++generation;
        _cohortes.emplace_back((int)generation, _primary_inoculum, p1, p2, p3,
                               p4, p5, p6, p7, p8, p9, p10, p11, p12, p13, p14,
                               D0, D1, Dc, CUM0, CUM1, Topt, Tmin, FACT, SSA,
                               seuil_HR, seuil_Tmoy);
    }
}

std::string Model::getDate(const std::string& maString)
{
    unsigned int i = 0;
    int cpt = 0;

    while (cpt < 3) {
        if (maString[i] == ';') {
            cpt++;
        }
        i++;
    }
    if (cpt == 3) {
        return maString.substr(0, i - 1);
    } else {
        return 0;
    }
}

std::vector < std::string > Model::getAllDailyData(const std::string& file_name)
{
    std::vector < std::string > pastDays;
    std::string myString;
    std::string currentDate;
    std::vector < std::string > datas;
    std::ifstream file(file_name);

    if (file) {
        getline(file, myString);
        if (nextDate != "") {
            while (getDate(myString) != nextDate) {
                getline(file, myString);
            }
        }
        currentDate = getDate(myString);
        pastDays.push_back(currentDate);
        while (getDate(myString) == currentDate) {
            datas.push_back(myString.substr(currentDate.size()+1));
            getline(file, myString);
        }
        nextDate = getDate(myString);
        return datas;
    } else {
        std::cout << "Erreur lecture fichier " << file_name << std::endl;
        exit(-1);
    }
}

double Model::getValueFromData(const std::string& data)
{
    return std::stod(data.substr(data.find(";")+1));
}

std::vector < double > Model::getDoubleVector(
    const std::vector < std::string >& vector)
{
    std::string delimiter = ":";
    double sum = 0;
    int valueNbPH = 0;
    std::vector < double > finalResult;
    std::string tmpHour = vector[0].substr(0, vector[0].find(delimiter));

    for (unsigned int i = 0; i < vector.size(); i++) {
        if (vector[i].substr(0, vector[i].find(delimiter)) == tmpHour) {
            sum += getValueFromData(vector[i]);
            valueNbPH++;
        } else { // new Hour
            finalResult.push_back(sum / valueNbPH);
            sum = 0;
            valueNbPH = 0;
            tmpHour = vector[i].substr(0,  vector[i].find(delimiter));
            sum += getValueFromData(vector[i]);
            valueNbPH++;
	}
        if (i == vector.size()-1) { // Push back de la derniere valeur
            finalResult.push_back(sum / valueNbPH);
        }
    }
    return finalResult;
}

void Model::fillData(const std::string& t_fileName,
                     const std::string& h_fileName)
{
    _hourly_temperatures.clear();
    _hourly_humidities.clear();

    std::string saveDate = nextDate;
    std::vector < std::string > allResultsT = getAllDailyData(t_fileName);

    nextDate = saveDate;

    std::vector < std::string > allResultsH = getAllDailyData(h_fileName);

    _hourly_temperatures = getDoubleVector(allResultsT);
    _hourly_humidities   = getDoubleVector(allResultsH);
}
