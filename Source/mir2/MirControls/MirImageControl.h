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
	bool isDrawImage_;
	bool isPixelDetect_;
	int index_;
	MirLibPtr mirLib_;

protected:
	bool GetIsUseOffset();
	void OnUseOffSetChanged();
	void OnIndexChanged();
	void OnLibraryChanged();
	void OnDrawImageChanged();
	void OnPixelDetectChanged();

public:
	PointPtr GetDisplayLocation() override;

	void SetIsUseOffset(bool isUseOffSet);

	virtual int GetIndex();
	virtual void SetIndex(int index);

	bool GetIsDrawImage();
	void SetIsDrawImage(bool isDrawImage);

	bool GetIsPixelDetect();
	void SetIsPixelDetect(bool isPixelDetect);

	void SetMirLib(MirLibPtr mirLib);
	MirLibPtr GetMirLib();

	SizePtr GetSize() const override;

};

}
