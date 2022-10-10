#include <queue>
#include <string>
#include <mutex>
#include <thread>

class LicensePlateListener{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    void run(){
//        std::thread([mutex_](){
//            std::string plate = getPlate_();
//            std::lock_guard<std::mutex> locker(mutex_);
//
//        })
    }

    std::string getPlate(){
        std::lock_guard<std::mutex> locker(mutex_);
        auto plate = plates_.front();
        plates_.pop();
        return plate;
    }

protected:
    std::mutex mutex_;
    std::queue<std::string> plates_;
};