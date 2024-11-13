#include "TimeProfiler.h"
#include "TerminalControlsConfig.h"

void TimeProfiler::Push(const std::string& title) {
    timeMarksTitle.push(title);
    timeMarks.push(Now());
}

void TimeProfiler::Pop(const std::string& title, bool shouldCommit) {
    assert(title.starts_with(timeMarksTitle.top()));
    timeMarksTitle.pop();
    auto start = timeMarks.top();
    timeMarks.pop();
    if (shouldCommit && !forceShouldNotCommit) {
        const std::chrono::duration<int64_t, std::nano> diff = Now() - start;
        Commit(title, diff.count());
    }
}

void TimeProfiler::Commit(const std::string& title, int64_t nano) {
    if (callback && TControlsConfig().profileEnable) {
        callback(std::string(timeMarks.size(), ' ') + title + ": " + GetStr(nano));
    }
}

void TimeProfiler::SetCallback(TimeProfilerCallback callback) {
    this->callback = callback;
}

std::chrono::time_point<std::chrono::steady_clock> TimeProfiler::Now() {
    return std::chrono::steady_clock::now();
}

std::string TimeProfiler::GetStr(int64_t nano) {
    static const std::string format[4] = { "%.f ns", "%.f us", "%.f ms", "%.3f s" };
    int pos = 0;
    double dur = (double)nano;
    while (dur >= 1000 && pos < 3) {
        dur /= 1000;
        pos++;
    }

    char buf[200];
    sprintf_s(buf, format[pos].data(), dur);
    return buf;
}
