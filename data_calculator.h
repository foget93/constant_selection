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
        Axes<double> RF = Axes<double>{static_cast<double>(sum192_.GetX()),
                static_cast<double>(sum192_.GetY()),
                static_cast<double>(sum192_.GetZ())} / 192.0 - constants_.at("AF").;
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

    const std::vector<std::string_view> constant_names_= {"G"s, "AF"s, "BF"s,
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


