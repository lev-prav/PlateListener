//
// Created by lev on 18.10.22.
//

#include "PlateInfo.h"

using namespace PlateInfo;

Direction PlateInfo::get_direction(const std::string& dir){
    if (dir == "in") {
        return Direction::IN;
    } else if (dir == "out") {
        return Direction::OUT;
    }
    return Direction::UNDEFINED;
}

std::vector<std::string> PlateInfo::split_string(const std::string &str, const std::string &delim) {
    std::vector<std::string> strings;
    boost::split(strings, str, boost::is_any_of(delim), boost::token_compress_on);
    return strings;
}

TruckPassInfo PlateInfo::deserialize_TruckPass(const std::string &plateString) {
    auto truckInfo = split_string(plateString, ";");

    TruckPassInfo truckPassInfo;

    truckPassInfo.dir = get_direction(truckInfo[0]);
    truckPassInfo.plate_number = truckInfo[1];
    truckPassInfo.timestamp = std::stoll(truckInfo[2]);

    return truckPassInfo;
}