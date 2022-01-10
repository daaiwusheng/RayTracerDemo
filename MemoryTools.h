#pragma once
#include <memory>

namespace std
{
//it seems that c++11 doesn't contain std::make_unique and std::make_shared
    template<class T, typename ...Args>
    inline auto makeUnique(Args &&...args) -> std::unique_ptr<T>{
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }

    template<class T, typename ...Args>
    inline auto makeShared(Args &&...args) -> std::shared_ptr<T>{
        return std::shared_ptr<T>(new T(std::forward<Args>(args)...));
    }


};

