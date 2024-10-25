#include <SDL_main.h>
#include <boost/compute/detail/lru_cache.hpp>
#include <iostream>
#include <memory>

class MirLib

{
    int i = 0;
    bool initialized = false;
public:
    MirLib(int id)
    {
        i = id;
        initialized = true;
    }
    virtual ~MirLib()
    {
        std::cout << "destroied--" << i << std::endl;
        i = 0;
        initialized = false;
    }
};

using MirLibPtr = std::shared_ptr<MirLib>;

int SDL_main(int argv, char** args)
{
    const int SIZE = 5;
    boost::compute::detail::lru_cache<std::string, MirLibPtr> mir_libs(SIZE);
    for (int i = 0; i < SIZE * 2; i++)
    {
        auto mirlib = std::make_shared<MirLib>(i);
        mir_libs.insert(std::to_string(i), mirlib);
    }
    return 0;
}