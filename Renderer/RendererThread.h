#ifndef RENDERERTHREAD_H
#define RENDERERTHREAD_H

#include <thread>
#include <functional>
#include <queue>
#include <future>
#include <mutex>

class RendererThread
{
public:
    RendererThread();
    auto init() -> void;
    auto run() -> void;
    auto postEvent(const std::function<void()> & event) -> std::future<bool>;
    auto setDraw(const std::function<void()> & func) -> void;
    auto join() -> void;
    inline auto getMutex() -> std::mutex & { return executeMutex; }
private:
    auto executeAll() -> void;
    std::thread thread;
    std::queue<std::pair<std::promise<bool>, std::function<void()>>> executeQueue;
    std::mutex executeMutex;
    std::function<void()> drawFunc;
    bool isRunning = true;
};

#endif // RENDERERTHREAD_H
