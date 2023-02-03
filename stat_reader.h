#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

template<typename Type>
class Axes final { // оси x y z
public:
    Axes()
        : x_(0), y_(0), z_(0) {}

    Axes(Type x, Type y, Type z)
        : x_(x), y_(y), z_(z) {
    }

    Axes(const Axes& other)
        : x_(other.x_), y_(other.y_), z_(other.z_) {
    }

    Axes& operator=(const Axes& other) {
        x_ = other.x_;
        y_ = other.y_;
        z_ = other.z_;
        return *this;
    }
    Axes operator-(const Axes& other) {
        return Axes<Type> {
            x_ - other.x_,
            y_ - other.y_,
            z_ - other.z_
        };
    }

    Axes operator+(const Axes& other) {
        return Axes<Type> {
            x_ + other.x_,
            y_ + other.y_,
            z_ + other.z_
        };
    }

    Axes<Type> operator/(size_t other) {
        return Axes<Type> {
            x_ / static_cast<Type>(other),
            y_ / static_cast<Type>(other),
            z_ / static_cast<Type>(other)
        };
    }

    operator Axes<double>() const {
        return Axes<double>{double(x_),double(y_),double(z_)};
    }

    operator Axes<int>() const {
        return Axes<int>{int(x_),int(y_),int(z_)};
    }

    Type GetX() const {return x_;}
    Type GetY() const {return y_;}
    Type GetZ() const {return z_;}


private:
    Type x_, y_, z_;
};

struct Rxyz {
    std::vector<Axes<int>> data;
};

struct Pxyz {
    std::vector<Axes<int>> data;
};

struct Temoerature_codes {
    std::vector<double> data;
};

struct GeographicCoefs {
    double g {9.8156}; // ускорение свободного падения(Москва), на экваторе 9.7810
    double lat {55.85}; // широта москва
    double az {0.0}; // азимут
    int position {1};
};


class ReaderStat {
public:
    explicit ReaderStat(const std::string& path) {
        std::fstream in_file(path, std::ios::in);
        if (!in_file) {
            throw std::invalid_argument("ReaderStat - Can't open input file");
        }
        //pos
        for(int i {0}; i < 1; ++i) {
            std::string str {};
            std::getline(in_file, str);
            coefs_.position = std::stoi(str.substr(21, 1));
        }
        // propusk
        for(int i {0}; i < 1; ++i) {
            std::string str {};
            std::getline(in_file, str);
        }
        //g, lat, az
        for(int i {0}; i < 1; ++i) {
            std::string str {};
            std::getline(in_file, str);
            coefs_.g = std::stod(str.substr(2, 6));
            coefs_.lat = std::stod(str.substr(10, 5));
            coefs_.az = std::stod(str.substr(16, 6));
        }

        for(int i {0}; i < 3; ++i) {
            std::string str {};
            std::getline(in_file, str);
        }

        std::vector<int> buf_for_sum(3);
        //читаем sum_192
        for (int i {0}; i < 3; ++i) {
            std::string str {};
            std::getline(in_file, str);

            buf_for_sum[i] = std::stoi(str.substr(3, 5));
        }

        sum_192_ = Axes{buf_for_sum[0],
                        buf_for_sum[1],
                        buf_for_sum[2]};

        for(int i {0}; i < 3; ++i) {
            std::string str {};
            std::getline(in_file, str);
        }


        for (std::string str; std::getline(in_file, str);) {
            if (!str.empty() && (str[0] == '-' && str[3] == '-')) {
                break;
            }

            mt_.data.push_back(Axes<int>{std::stoi(str.substr(12, 6)),
                                    std::stoi(str.substr(19, 6)),
                                    std::stoi(str.substr(27, 6))}
                               );

            acc_.data.push_back(Axes<int>{std::stoi(str.substr(34, 6)),
                                    std::stoi(str.substr(42, 6)),
                                    std::stoi(str.substr(50, 6))}
                               );

            temp_.data.push_back(std::stod(str.substr(58, 7)));
        }
    }

    Axes<int> GetSum192() const {
        return sum_192_;
    }

    std::vector<Axes<int>> GetMtInfo() const {
        return mt_.data;
    }
    // acc info + temp ====================================
    std::vector<Axes<int>> GetAccInfo() const {
        return acc_.data;
    }

    std::vector<double> GetCodesTempInfo() const {
        return temp_.data;
    }
    // ====================================================

    GeographicCoefs GetGeoCfs() {
        return coefs_;
    }

private:
    Axes<int> sum_192_{Axes<int>{0,0,0}};
    Rxyz mt_;
    Pxyz acc_;
    Temoerature_codes temp_;
    GeographicCoefs coefs_;
};
