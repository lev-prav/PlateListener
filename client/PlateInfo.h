//
// Created by lev on 15.10.22.
//
#pragma once

#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>

namespace PlateInfo {
    enum class Direction {
        IN,
        OUT,
        UNDEFINED
    };

    struct TruckPassInfo {
        Direction dir;
        std::string plate_number;
        int64_t timestamp;
    };

    Direction get_direction(const std::string& dir);

    std::vector<std::string> split_string(const std::string &str, const std::string &delim);
    TruckPassInfo deserialize_TruckPass(const std::string &plateString);

}