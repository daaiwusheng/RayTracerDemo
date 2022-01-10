#include "RendererThread.h"

RendererThread::RendererThread()
{

}


//init func, create the the thread
auto RendererThread::init() -> void
{
    thread = std::thread(std::bind(&RendererThread::run,this));
}

//run func
auto RendererThread::run() -> void
{
    while(isRunning){
        if(drawFunc) {
            std::lock_guard<std::mutex> lock(executeMutex);
            drawFunc();
        }
        executeAll();
    }
}

auto RendererThread::join() -> void
{
    if(thread.joinable()){
        isRunning = false;
        thread.join();
    }
}


//execute all callback in the thread queue.
auto RendererThread::executeAll() -> void
{
    std::pair<std::promise<bool>, std::function<void()>> func;

    for (;;) {
       {
           std::lock_guard<std::mutex> lock(executeMutex);
           if (executeQueue.empty())
               break;
           func = std::move(executeQueue.front());
           executeQueue.pop();
       }

       if (func.second) {
           func.second();//set the future as true
           func.first.set_value(true);
       }
    }
}

//set draw callback
auto RendererThread::setDraw(const std::function<void()> & func) -> void
{
    drawFunc = func;
}

//post event in thread queue..
auto RendererThread::postEvent(const std::function<void()> & event) -> std::future<bool>
{
    std::lock_guard<std::mutex> locker(executeMutex);
    std::promise<bool> promise;
    std::future<bool> future = promise.get_future();
    executeQueue.emplace(std::move(promise), std::move(event));
    return future;
}
