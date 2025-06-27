#include "MirMain.h"
#include <SDL.h>

namespace mir {

int MirMain(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_EVERYTHING) <= -1) {
        ErrSdl();
    }
}

}
