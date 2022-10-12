#include <queue>
#include <string>
#include <mutex>
#include <thread>

class LicensePlateListener{
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void run() = 0;

    std::string popPlate(){
        std::lock_guard<std::mutex> locker(mutex_);
        auto plate = plates_.front();
        plates_.pop();
        return plate;
    }

    void pushPlate(const std::string& plate){
        std::lock_guard<std::mutex> locker(mutex_);
        plates_.push(plate);
    }

protected:
    std::mutex mutex_;
    std::queue<std::string> plates_;
};