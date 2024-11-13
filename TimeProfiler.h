#pragma once

#include <chrono>
#include <string>
#include <functional>
#include <stack>
#include <cassert>

using TimeProfilerCallback = std::function<bool(const std::string& message)>;

class TimeProfiler {
public:
    void Push(const std::string& title);

    void Pop(const std::string& title, bool shouldCommit = true);
    
    void Commit(const std::string& title, int64_t nano);
    
    void SetCallback(TimeProfilerCallback callback);

protected:
    std::chrono::time_point<std::chrono::steady_clock> Now();
    
    std::string GetStr(int64_t nano);

protected:
    TimeProfilerCallback callback;
    std::stack<std::chrono::time_point<std::chrono::steady_clock>> timeMarks;
    std::stack<std::string> timeMarksTitle;

public:
    bool forceShouldNotCommit = false;
};
