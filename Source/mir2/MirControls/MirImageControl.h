#pragma once

#include "mir2/MirControls/MirControl.h"

namespace devilution {

class MirImageControl
using MirImageControlPtr = std::shared_ptr<MirImageControl>;

class MirImageControl : public MirControl, public std::enable_shared_from_this<MirImageControl> {

};

}
