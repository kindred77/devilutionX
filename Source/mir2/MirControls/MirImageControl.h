#pragma once

#include "mir2/MirControls/MirControl.h"
#include "mir2/Common/Point.h"
#include "mpq/mir_lib.hpp"

namespace devilution {

class MirImageControl;
using MirImageControlPtr = std::shared_ptr<MirImageControl>;

class MirImageControl : public MirControl, public std::enable_shared_from_this<MirImageControl>
{
private:
	bool isUseOffSet_;
	int index_;
	MirLibPtr mirLib_;

protected:
	bool GetIsUseOffset();
	void OnIndexChanged();
	void OnLibraryChanged();

public:
	PointPtr GetDisplayLocation() override;

	virtual int GetIndex();
	virtual void SetIndex(int index);

	void SetMirLib(MirLibPtr mirLib);
	MirLibPtr GetMirLib();

};

}
