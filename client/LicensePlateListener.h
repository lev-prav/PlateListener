#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include "PlateInfo.h"


class LicensePlateListener{
public:
    virtual void stop() = 0;
    virtual void run() = 0;

    PlateInfo::TruckPassInfo popPlate(){
        std::lock_guard<std::mutex> locker(mutex_);
        auto plate = plates_.front();
        plates_.pop();
        return plate;
    }

    void pushPlate(const std::string& plate){

        auto truckPassInfo = PlateInfo::deserialize_TruckPass(plate);

        std::lock_guard<std::mutex> locker(mutex_);
        plates_.push(std::move(truckPassInfo));
    }

protected:
    std::mutex mutex_;
    std::queue<PlateInfo::TruckPassInfo> plates_;
};