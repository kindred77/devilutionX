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

void MirImageControl::SetIsUseOffset(bool isUseOffSet)
{
    if (isUseOffSet_ == isUseOffSet) return;
    isUseOffSet_ = isUseOffSet;
    OnUseOffSetChanged();
}

void MirImageControl::OnUseOffSetChanged()
{
    MirControl::OnLocationChanged();
    //if (UseOffSetChanged != null)
    //    UseOffSetChanged.Invoke(this, EventArgs.Empty);
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

bool MirImageControl::GetIsDrawImage()
{
    return isDrawImage_;
}

void MirImageControl::SetIsDrawImage(bool isDrawImage)
{
    if (isDrawImage_ == isDrawImage) return;
    isDrawImage_ = isDrawImage;
    OnDrawImageChanged();
}

void MirImageControl::OnDrawImageChanged()
{
    MirControl::Redraw();
    //if (DrawImageChanged != null)
    //    DrawImageChanged.Invoke(this, EventArgs.Empty);
}

bool MirImageControl::GetIsPixelDetect()
{
    return isPixelDetect_;
}

void MirImageControl::SetIsPixelDetect(bool isPixelDetect)
{
    if (isPixelDetect_ == isPixelDetect) return;
    isPixelDetect_ = isPixelDetect;
    OnPixelDetectChanged();
}

void MirImageControl::OnPixelDetectChanged()
{
    MirControl::Redraw();
    //if (PixelDetectChanged != null)
    //    PixelDetectChanged.Invoke(this, EventArgs.Empty);
}


MirLibPtr MirImageControl::GetMirLib()
{
    return mirLib_;
}

SizePtr MirImageControl::GetSize() const
{
    if (Library != null && Index >= 0) return Library.GetTrueSize(Index);
    return base.Size;
}

}
