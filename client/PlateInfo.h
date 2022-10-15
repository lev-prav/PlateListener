//
// Created by lev on 15.10.22.
//
#pragma once

#include <string>

namespace PlateInfo {
    enum class Direction {
        IN,
        OUT,
        UNDEFINED
    };

    static Direction get_direction(const std::string& dir) {
        if (dir == "in") {
            return Direction::IN;
        } else if (dir == "out") {
            return Direction::OUT;
        }
        return Direction::UNDEFINED;
    }

    struct TruckPassInfo {
        Direction dir;
        std::string plate_number;
        int64_t timestamp;
    };

}