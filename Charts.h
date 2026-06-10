#ifndef CHARTS_H
#define CHARTS_H

#include <vector>
#include <string>

void Wykres(
    const std::vector<float>& x, const std::vector<float>& y2, const std::vector<float>& y,
    const std::string x_label, const std::string y_label,
    const std::string tt
);

void wykres_por(const float m, const float k, const float c);


void Figure_holdon();

#endif 
