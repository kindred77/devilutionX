#include "mir2/MirControls/MirImageControl.h"

namespace devilution {

bool MirImageControl::GetIsUseOffset()
{
	return isUseOffSet_;
}

PointPtr MirImageControl::GetDisplayLocation()
{
    const auto display_location = MirControl::GetDisplayLocation();
    if (GetIsUseOffset())
    {
        const auto mirlib = GetMirLib();
        const auto mirImg = mirlib->operator[](GetIndex());
        return display_location->Add(mirImg->x, mirImg->y);
    }
    else
    {
        return display_location;
    }
}

int MirImageControl::GetIndex()
{
    return index_;
}

void MirImageControl::SetIndex(int index)
{
    if (index_ == index) return;
    index_ = index;
    OnIndexChanged();
}

void MirImageControl::OnIndexChanged()
{
    MirControl::OnSizeChanged();
    //if (IndexChanged != null)
    //    IndexChanged.Invoke(this, EventArgs.Empty);
}

void MirImageControl::SetMirLib(MirLibPtr mirLib)
{
    if (mirLib_ == mirLib) return;
    mirLib_ = mirLib;
    OnLibraryChanged();
}

void OnLibraryChanged()
{
    MirControl::OnSizeChanged();
    //if (LibraryChanged != null)
    //    LibraryChanged.Invoke(this, EventArgs.Empty);
}

MirLibPtr MirImageControl::GetMirLib()
{
    return mirLib_;
}

}
