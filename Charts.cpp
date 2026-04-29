#include "charts.h"
#include "matplot/matplot.h"
#include <string>

void Wykres(
    const std::vector<float>& x,
    const std::vector<float>& y_an,
    const std::vector<float>& y_num,
    const std::string x_label,
    const std::string y_label,
    const std::string tt
) {
    using namespace matplot;

    std::vector<double> xx(x.begin(), x.end());
    std::vector<double> yn(y_num.begin(), y_num.end());
    std::vector<double> ya(y_an.begin(), y_an.end());

    figure();
    hold(on);

    plot(xx, yn)->display_name("Numerycznie");
    plot(xx, ya, "--r")->display_name("Analitycznie");

    legend();
    xlabel(x_label);
    ylabel(y_label);
    title(tt);
    grid(on);

    show();
}

void wykres_por(const float m, const float k, const float c)
{
    float beta = c / (2.0f * m);
    float w0 = sqrt(k / m);
    float wd = sqrt(w0 * w0 - beta * beta);
}

void Figure_holdon() {
    using namespace matplot;

    figure();
    hold(on);
}