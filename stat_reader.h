#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

class axis final {
public:
    axis(int x, int y, int z)
        : x_(x), y_(y), z_(z) {
    }

    void SetXYZ(int x, int y, int z) {
        x_ = x; y_ = y; z_ = z;
    }

private:
    int x_{0}, y_{0}, z_{0};
};

struct Rxyz {
    std::vector<axis> data;
};

struct Pxyz {
    std::vector<axis> data;
};

struct Temoerature_codes {
    std::vector<double> data;
};

class ReaderStat {
public:
    explicit ReaderStat(const std::string& path) {
        std::fstream in_file(path, std::ios::in);
        if (!in_file) {
            throw std::invalid_argument("ReaderStat - Can't open input file");
        }

        for(int i {0}; i < 12; ++i) {
            std::string str {};
            std::getline(in_file, str);
        }

        for (std::string str; std::getline(in_file, str);) {
            if (!str.empty() && (str[0] == '-' && str[3] == '-')) {
                break;
            }

            mt_.data.push_back(axis{std::stoi(str.substr(12, 6)),
                                    std::stoi(str.substr(19, 6)),
                                    std::stoi(str.substr(27, 6))}
                               );

            acc_.data.push_back(axis{std::stoi(str.substr(34, 6)),
                                    std::stoi(str.substr(42, 6)),
                                    std::stoi(str.substr(50, 6))}
                               );

            temp_.data.push_back(std::stod(str.substr(66, 6)));
        }

    }

private:
    Rxyz mt_;
    Pxyz acc_;
    Temoerature_codes temp_;
};
