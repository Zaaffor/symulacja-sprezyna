#include "charts.h"
#include "matplot/matplot.h"

void WykWych(const std::vector<float>& czas, const std::vector<float>& wy) {
    using namespace matplot;

    std::vector<double> x(czas.begin(), czas.end());
    std::vector<double> y(wy.begin(), wy.end());

    figure();
    plot(x, y);
    xlabel("Czas [s]");
    ylabel("Wychylenie [px]");
    title("Wychylenie sprê¿yny w czasie");
    grid(on);
    show();
}

void WykPrzys(const std::vector<float>& czas, const std::vector<float>& przysp) {
    using namespace matplot;

    std::vector<double> x(czas.begin(), czas.end());
    std::vector<double> y(przysp.begin(), przysp.end());

    figure();
    plot(x, y);
    xlabel("Czas [s]");
    ylabel("Przyspieszenie");
    title("Przyspieszenie drgania w czasie");
    grid(on);
    show();
}