#pragma once

#include "mir2/MirControls/MirControl.h"
#include "mir2/Common/Point.h"

namespace devilution {

class MirImageControl
using MirImageControlPtr = std::shared_ptr<MirImageControl>;

class MirImageControl : public MirControl, public std::enable_shared_from_this<MirImageControl>
{
private:
	bool isUseOffSet_;

protected:
	bool GetIsUseOffset()
	{
		return isUseOffSet_;
	}

public:
	override PointPtr GetDisplayLocation()
	{
		return GetIsUseOffset() ? base.DisplayLocation.Add(Library.GetOffSet(Index)) : base.DisplayLocation; 
	}

	
};

}
