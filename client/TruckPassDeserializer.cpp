//
// Created by lev on 15.10.22.
//

#include "TruckPassDeserializer.h"

using namespace PlateInfo;

std::vector<std::string> TruckPassDeserializer::split_string(const std::string &str, const std::string &delim) {
    std::vector<std::string> strings;
    boost::split(strings, str, boost::is_any_of(delim), boost::token_compress_on);
    return strings;
}

TruckPassInfo TruckPassDeserializer::deserialize(const std::string &plateString) {
    auto truckInfo = split_string(plateString, ";");

    TruckPassInfo truckPassInfo;

    truckPassInfo.dir = get_direction(truckInfo[0]);
    truckPassInfo.plate_number = truckInfo[1];
    truckPassInfo.timestamp = std::stoll(truckInfo[2]);

    return truckPassInfo;
}
