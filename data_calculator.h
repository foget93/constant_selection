#pragma once
#include <vector>
#include "stat_reader.h"
#include <unordered_map>
#include <algorithm>
#include <string_view>

using namespace std::string_literals;
struct Coefs {
    double tau {0.0};
    double d {0.0};
    double k {0.0};
};
class DataCalculator {
public:
    DataCalculator(const std::vector<Axes<int>>& cnt, const std::vector<Axes<int>>& r, Axes<int> sum192, GeographicCoefs cfs)
        : cfs_(cfs), sum192_(sum192), r_(r) {

        if (cnt.size() == constant_names_.size()) {
            for (int i {0}; i < static_cast<int>(cnt.size()); ++i) {
                constants_[constant_names_[i]] = cnt[i];
            }
        }
        else {
            throw std::invalid_argument("Bad cnt in construct"s);
        }
    }

    std::vector<Axes<double>> CalculationMT() {
        const double RFx = sum192_.GetX() / 192.0 - constants_["AF"].GetX() - HF * constants_["BF"].GetX();
        const double RFy = sum192_.GetY() / 192.0 - constants_["AF"].GetY() - HF * constants_["BF"].GetY();
        const double RFz = sum192_.GetZ() / 192.0 - constants_["AF"].GetZ() - HF * constants_["BF"].GetZ();

        std::vector<Axes<double>> result;
        result.reserve(5);
        double tau_x = constants_["AT"].GetX() * HAT * RFx + constants_["BT"].GetX() * HBT;
        double tau_y = constants_["AT"].GetY() * HAT * RFy + constants_["BT"].GetY() * HBT;
        double tau_z = constants_["AT"].GetZ() * HAT * RFz + constants_["BT"].GetZ() * HBT;
        result.push_back({tau_x, tau_y, tau_z});


        double y = ((RFy * RFy) * constants_["AD"].GetY()) + (RFy * constants_["BD"].GetY()) + constants_["CD"].GetY();


        double D_x = constants_["AD"].GetX() * HAD * RFx * RFx + constants_["BD"].GetX() * HBD * RFx + constants_["CD"].GetX() * E;
        double D_y = constants_["AD"].GetY() * HAD * RFy * RFy + constants_["BD"].GetY() * HBD * RFy + constants_["CD"].GetY() * E;
        double D_z = constants_["AD"].GetZ() * HAD * RFz * RFz + constants_["BD"].GetZ() * HBD * RFz + constants_["CD"].GetZ() * E;
        result.push_back({D_x, D_y, D_z});

        double K_x = constants_["AK"].GetX() * HAK * RFx * RFx + constants_["BK"].GetX() * HBK * RFx + constants_["CK"].GetX() * HCK;
        double K_y = constants_["AK"].GetY() * HAK * RFy * RFy + constants_["BK"].GetY() * HBK * RFy + constants_["CK"].GetY() * HCK;
        double K_z = constants_["AK"].GetZ() * HAK * RFz * RFz + constants_["BK"].GetZ() * HBK * RFz + constants_["CK"].GetZ() * HCK;
        result.push_back({K_x, K_y, K_z});

        double V_x = constants_["AV"].GetX() * HAV * RFx * RFx + constants_["BV"].GetX() * HBV * RFx + constants_["CV"].GetX() * E + constants_["DV"].GetX() * EE;
        double V_y = constants_["AV"].GetY() * HAV * RFy * RFy + constants_["BV"].GetY() * HBV * RFy + constants_["CV"].GetY() * E + constants_["DV"].GetY() * EE;
        double V_z = constants_["AV"].GetZ() * HAV * RFz * RFz + constants_["BV"].GetZ() * HBV * RFz + constants_["CV"].GetZ() * E + constants_["DV"].GetZ() * EE;
        result.push_back({V_x, V_y, V_z});

        double Km_x = K_0 + K_SH * constants_["G"].GetX();
        double Km_y = K_0 + K_SH * constants_["G"].GetY();
        double Km_z = K_0 + K_SH * constants_["G"].GetZ();
        result.push_back({Km_x, Km_y, Km_z});
        return result;
    }



private: //coef
    class HasherStringView {
    public:
        size_t operator()(std::string_view str_v) const {
            return hasher_(str_v); // + hasher_(key.second) * 16;
        }
    private:
        std::hash<std::string_view> hasher_;
    };

    const std::vector<std::string> constant_names_=      {"G"s, "AF"s, "BF"s,
                                                          "AT"s, "BT"s,
                                                          "AD"s, "BD"s, "CD"s,
                                                          "AK"s, "BK"s, "CK"s,
                                                          "AV"s, "BV"s, "CV"s, "DV"s,
                                                          "C1"s, "C2"s};

    static constexpr double HAT  {0.2};
    static constexpr double HBT  {8.0};
    static constexpr double HAD  {0.017};
    static constexpr double HBD  {0.07};
    static constexpr double HAK  {0.000019};
    static constexpr double HBK  {0.00008};
    static constexpr double HCK  {0.0003};
    static constexpr double HAV  {0.017};
    static constexpr double HBV  {0.07};
    static constexpr double E    {1.0};
    static constexpr double EE   {0.01};
    static constexpr double HF   {0.01};
    static constexpr double K_0  {3.3336};
    static constexpr double K_SH {0.000171};

private:
    GeographicCoefs cfs_;
    Axes<int> sum192_;
    std::vector<Axes<int>> r_;
    std::unordered_map<std::string_view, Axes<int>, HasherStringView> constants_;

};


