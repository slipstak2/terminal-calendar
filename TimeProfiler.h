#pragma once
#include <chrono>

class TimeProfiler {
public:
    TimeProfiler() {
        prv = std::chrono::steady_clock::now();
    }
    int64_t Get() {
        auto cur = std::chrono::steady_clock::now();
        const std::chrono::duration<int64_t, std::nano> diff = cur - prv;
        prv = cur;

        return diff.count();
    }
    std::string GetStr() {
        static const std::string format[4] = { "%.f ns", "%.f us", "%.f ms", "%.3f s"};
        int pos = 0;
        double dur = Get();
        while (dur >= 1000 && pos < 3) {
            dur /= 1000;
            pos++;
        }

        char buf[200];
        sprintf_s(buf, format[pos].data(), dur);
        return buf;
    }
protected:
    std::chrono::time_point<std::chrono::steady_clock> prv;
};