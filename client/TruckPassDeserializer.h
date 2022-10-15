//
// Created by lev on 15.10.22.
//

#pragma once

#include <vector>
#include <boost/algorithm/string.hpp>
#include "PlateInfo.h"

namespace PlateInfo {
    class TruckPassDeserializer {
        std::vector<std::string> split_string(const std::string &str, const std::string &delim);

    public:
        //  Direction;plate;time
        //  std::string plate = "in;x111xx;1665591901 ";
        TruckPassInfo deserialize(const std::string &plateString);
    };

};