#include <chrono>
#include <iomanip>
#include <sstream>
#include <cmath>

namespace UserCommon_206480972_206899163 {
std::string getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::chrono::duration<double> ts = now.time_since_epoch();
    constexpr size_t NUM_DIGITS = 9;
    size_t NUM_DIGITS_P = std::pow(10, NUM_DIGITS);
    std::ostringstream oss;
    oss << std::setw(NUM_DIGITS) << std::setfill('0') << size_t(ts.count() * NUM_DIGITS_P) % NUM_DIGITS_P;
    return oss.str();
}
}