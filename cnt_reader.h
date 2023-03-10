#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <string_view>

#include <stat_reader.h> // for axes -> перенести аксес в др файл

using namespace std::literals;

constexpr int COUNT_CNT_GYROSCOPE = 17;
constexpr int COUNT_AXIS = 3;

constexpr int COUNT_CNT_ACCELEROMETR = 6;
constexpr int AFTER_ACCELEROMETR = 9;

struct Constants_mt {
    std::vector<std::vector<int>> data;

    explicit Constants_mt() {
        data.resize(COUNT_AXIS);

        std::vector<int> x_mt(COUNT_CNT_GYROSCOPE);
        std::vector<int> y_mt(COUNT_CNT_GYROSCOPE);
        std::vector<int> z_mt(COUNT_CNT_GYROSCOPE);

        data[0] = std::move(x_mt);
        data[1] = std::move(y_mt);
        data[2] = std::move(z_mt);
    }
};

struct Constant_accelerometer {
    std::vector<std::vector<int>> data;
    std::vector<std::vector<double>> data_double;
    std::vector<double> constan_base;

    std::vector<int> tail_data;

    explicit Constant_accelerometer() {
        data.resize(COUNT_AXIS);
        data_double.resize(COUNT_AXIS);
        constan_base.resize(COUNT_AXIS);
// TUT
        tail_data.resize(AFTER_ACCELEROMETR);

        std::vector<int> x_acc(COUNT_CNT_ACCELEROMETR);
        std::vector<int> y_acc(COUNT_CNT_ACCELEROMETR);
        std::vector<int> z_acc(COUNT_CNT_ACCELEROMETR);
        data[0] = std::move(x_acc);
        data[1] = std::move(y_acc);
        data[2] = std::move(z_acc);
        //  55   -2   -0.000381469    KTPx1   067   FE   11111110
        // считанные данные  # (-2) -> константа   (-0.000381469) -> данные
        std::vector<double> x_acc_double(COUNT_CNT_ACCELEROMETR);
        std::vector<double> y_acc_double(COUNT_CNT_ACCELEROMETR);
        std::vector<double> z_acc_double(COUNT_CNT_ACCELEROMETR);
        data_double[0] = std::move(x_acc_double);
        data_double[1] = std::move(y_acc_double);
        data_double[2] = std::move(z_acc_double);


    }
};

class ReaderCNT {
public:
    //explicit ReaderCNT()  = default;
    explicit ReaderCNT(const std::string& path) {
        std::fstream in_file(path, std::ios::in);
        if (!in_file) {
            throw std::invalid_argument("ReaderCNT - Can't open input file");
        }
        std::getline(in_file, info_about_);
        info_about_ = info_about_.substr(1, 39); // GIB123-6   N 21322879 -401 N 22122573

        // ================MT==============================
        for (int i = 0; i < COUNT_CNT_GYROSCOPE; ++i) {
            for (int j = 0; j < COUNT_AXIS; ++j) {
                std::string buf;
                std::getline(in_file, buf);

                cnt_mt_.data[j][i] = std::stoi(buf.substr(4, 4));// 6 символов пропустить и взять 4, устарел коммент
            }
        }
        // ===============================================

        // ================ACC==============================
        for (int i = 0; i < COUNT_CNT_ACCELEROMETR; ++i) {
            for (int j = 0; j < COUNT_AXIS; ++j) {
                std::string buf;
                std::getline(in_file, buf);

                cnt_acc_.data[j][i] = std::stoi(buf.substr(4, 4));
            }
        }
        // =================================================

        for (int i = 0; i < AFTER_ACCELEROMETR; ++i) {
            std::string buf;
            std::getline(in_file, buf);

            cnt_acc_.tail_data[i] = std::stoi(buf.substr(4, 4));
        }
    }

    std::vector<std::vector<int>> GetConstantsMT() {
        return cnt_mt_.data;
    }

    ~ReaderCNT() = default;

private:
    Constants_mt cnt_mt_;
    Constant_accelerometer cnt_acc_;

    std::string info_about_;
};
