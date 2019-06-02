#include "model.h"

#include <fstream>
#include <iostream>
#include <limits>
#include <random>

extern Parameters default_values;
extern Parameters fractions;

int main()
{
    const double threshold = 1.e7;
    const unsigned int model_number = 10000;
    const double step_number = 10000;
    std::string fileName = "sporuls";
    std::mt19937 generator(73766);
    std::vector < Model* > models;

    for (unsigned int i = 0; i < model_number; i++) {
        Parameters parameters = default_values;

        for (Parameters::iterator it = parameters.begin();
             it != parameters.end(); ++it) {
            std::normal_distribution < double > random(
                it->second, it->second / fractions[it->first]);

            it->second = random(generator);
        }
        models.push_back(new Model(1, parameters));
    }

    std::ofstream probability_file("./probability.csv");

    for (int j = 0; j < 250; j++) {
        std::ofstream output_file("./sporuls" + std::to_string(j) + ".csv");
        std::vector < double > values;
        double max = 0;
        double min = std::numeric_limits < double >::max();

        std::cout << "Jour : " << j << std::endl;

        for (unsigned int i = 0; i < models.size(); i++) {
            if (i == 0) {
                models[i]->fillData("../data/T.csv",
                                    "../data/H.csv");
            } else {
                models[i]->temperatures(models[0]->temperatures());
                models[i]->humidities(models[0]->humidities());
                models[i]->next_date(models[0]->next_date());
            }
            models[i]->compute();
            values.push_back(models[i]->total_ready_spore_number());
            if (max < models[i]->total_ready_spore_number()) {
                max = models[i]->total_ready_spore_number();
            }
            if (min > models[i]->total_ready_spore_number()) {
                min = models[i]->total_ready_spore_number();
            }
        }

        const double step = (max - min) / step_number;
        std::vector < unsigned int > histogram(step_number, 0);

        for (std::vector < double >::iterator it = values.begin();
             it != values.end(); ++it) {
            double value = *it < 0 ? 0 : *it;
            unsigned int index = std::floor((value - min) / step);

            ++histogram[index];
        }

        unsigned int i = 0;

        for (std::vector < unsigned int >::iterator it = histogram.begin();
             it != histogram.end(); ++it) {
            output_file << (min + (i * step)) << ";" << *it << std::endl;
            ++i;
        }
        output_file.close();

        if (max > threshold) {
            double sum = 0.;
            unsigned int first = min < threshold ? threshold - min : 0;

            for (unsigned int i = std::floor(first / step);
                 i < histogram.size(); ++i) {
                sum += histogram[i];
            }
            probability_file << j << ";" << ((sum / models.size()) * 100)
                             << std::endl;
        } else {
            probability_file << j << ";0" << std::endl;
        }
    }
    probability_file.close();
    for (std::vector < Model* >::iterator it = models.begin();
         it != models.end(); ++it) {
        delete *it;
    }
    models.clear();
    return 0;
}
