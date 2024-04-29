#pragma once

#include "mir2/Common/Point.h"
#include "mir2/Common/Color.h"
#include "mir2/Common/Rectangle.h"
#include "mir2/Common/Size.h"
#include "mir2/Common/Vector2.h"
#include "mir2/Engine/Texture.h"
#include "mir2/Engine/SoundList.h"
#include "mir2/Engine/SoundManager.h"

#include <string>

namespace devilution {

class MirControl;

using MirControlPtr = std::shared_ptr<MirControl>;

class MirControl : public std::enable_shared_from_this<MirControl>{
private:
	MirControlPtr parent_ = nullptr;
	PointPtr location_ = nullptr;
	PointPtr movePoint_ = nullptr;
	ColorPtr backColor_ = nullptr;
	ColorPtr borderColor_ = nullptr;
	ColorPtr foreColor = Clolor::WHITE;
	RectanglePtr displayRectangle_ = nullptr;
	SizePtr size_ = nullptr;
	bool isDrawControlTexture_ = false;
	bool isBorder_ = false;
	bool isEnabled_ = true;
	bool isModal_ = false;
	bool isMovable_ = false;
	bool isNotControl_ = false;
	bool isSort_ = false;
	bool isVisible_ = true;
	bool isDisposed_ = false;
	float opacity_ = 1F;
	int sound_ = SoundList.None;
	std::vector<MirControlPtr> controls_;
	std::string hint_ = "";

	void RemoveControl(MirControlPtr control);

	void OnBorderChanged();

	void OnBorderColourChanged();

	void OnControlAdded();

	void OnControlRemoved();

	void OnHintChanged();

	void OnIsModalChanged();

	void OnIsMovableChanged();

	void OnIsNotControlChanged();

	void OnOpacityChanged();

	void OnSoundChanged();

	void OnIsSortChanged();

	void AddControl(MirControlPtr control);

protected:
	RectanglePtr borderRectangle_ = nullptr;
	std::vector<Vector2Ptr> borderInfo_;
	TexturePtr controlTexture_ = nullptr;
	SizePtr textureSize_ = nullptr;
	bool isHasShown_ = false;
	bool isMoving_ = false;
	bool isTextureValid_ = true;
	virtual void OnBackColourChanged();

	virtual void OnSizeChanged();

	virtual std::vector<Vector2Ptr> GetBorderInfo();

	PointPtr Center();

	PointPtr Left();

	PointPtr Top();

	PointPtr Right();

	PointPtr Bottom();

	PointPtr TopLeft();

	PointPtr TopRight();

	PointPtr BottomRight();

	PointPtr BottomLeft();

	virtual void CreateTexture();

	virtual void OnEnabledChanged();

	virtual void OnForeColourChanged();

	virtual void OnParentChanged();

	virtual void OnLocationChanged();

	virtual void OnMouseEnter();

	virtual void Highlight();

	virtual void OnIsVisibleChanged();

	void OnBeforeShown();

	void OnShown();

	virtual void BeforeDrawControl();

	virtual void DrawControl();

	void DrawChildControls();

	virtual void DrawBorder();

	void AfterDrawControl();

	virtual void Deactivate();

	virtual void OnMouseLeave();

	virtual void OnMouseClick(/* MouseEventArgs e */);

	void OnMouseDoubleClick(/* MouseEventArgs e */);

	virtual void OnMouseMove(/* MouseEventArgs e */);

	virtual void OnMouseDown(/* MouseEventArgs e */);

	virtual void OnMouseUp(/* MouseEventArgs e */);

	virtual void OnKeyPress(/* KeyEventArgs e */);

	virtual void OnKeyDown(/* KeyEventArgs e */);

	virtual void OnKeyUp(/* KeyEventArgs e */);

	virtual void Dehighlight();

	virtual void Dispose(bool disposing);

public:
	static MirControlPtr ActiveControl = nullptr;
	static MirControlPtr MouseControl = nullptr;

	bool IsGrayScale = false;
	bool IsBlending = false;

	long CleanTime = 0l;

	MirControl()
	{
		opacity_ = 1F;
		_enabled = true;
		_foreColour = Color.White;
		_visible = true;
		_sound = SoundList.None;
	}

	MirControlPtr GetParent() const
	{
		return parent_;
	};

	virtual void SetSize(SizePtr size)
	{
		if (size_ == size)
			return;
		size_ = size;
		OnSizeChanged();
	}

	virtual SizePtr GetSize() const
	{
		return size_;
	}

	virtual SizePtr GetTrueSize() const
	{
		return size_;
	}

	RectanglePtr GetDisplayRectangle() const
	{
		return std::make_shared<Rectangle>(GetDisplayLocation(), GetSize());
	}

	void SetParent(MirControlPtr parent)
	{
		if (parent_ == parent)
			return;
		if (parent_ != nullptr)
			parent_->RemoveControl(shared_from_this());
		parent_ = parent;
		if (parent_ != nullptr)
			parent_->AddControl(shared_from_this());
		OnParentChanged();
	}

	void SetBackColor(ColorPtr backColor)
	{
		if (backColor_ == backColor)
		{
			return;
		}
		backColor_ = backColor;
		OnBackColourChanged();
	}

	ColorPtr GetBackColor() const
	{
		return backColour_;
	}

	void SetLocation(PointPtr location)
	{
		if (location_ == location)
			return;
		location_ = location;
		OnLocationChanged(); 
	}

	PointPtr GetLocation()
	{
		return location_;
	}

	virtual PointPtr GetDisplayLocation()
	{
		if (!parent_)
		{
			return location_;
		}
		else
		{
			auto point = parent_->GetDisplayLocation();
			return point->Add(location_);
		}
	}

	virtual void SetIsBorder(bool isBorder)
	{
		if (isBorder_ == isBorder)
			return;
		isBorder_ = isBorder;
		OnBorderChanged();
	}

	virtual bool GetIsBorder() const
	{
		return isBorder_;
	}

	virtual void SetBorderColor(ColorPtr borderColor)
	{
		if (borderColor_ == borderColor)
			return;
		borderColor_ = borderColor;
		OnBorderColourChanged();
	}

	virtual ColorPtr GetBorderColor() const
	{
		return borderColor_;
	}

	void SetForeColour(ColorPtr foreColor)
	{
		if (foreColor_ == foreColor)
			return;
		foreColor_ = foreColor;
		OnForeColourChanged();
	}

	ColorPtr GetForeColour() const
	{
		return foreColor_;
	}

	void SetIsDrawControlTexture(bool isDrawControlTexture)
	{
		if (isDrawControlTexture_ == isDrawControlTexture)
			return;
		isDrawControlTexture_ = isDrawControlTexture;
		Redraw();
	}

	bool GetIsDrawControlTexture() const
	{
		return isDrawControlTexture_;
	}

	virtual void Redraw()
	{
		if (parent_)
			parent_->Redraw();
	}

	std::vector<MirControlPtr> GetControls() const
	{
		return controls_;
	}

	void InsertControl(int index, MirControlPtr control)
	{
		//if (control->GetParent() != this) {
		//	control.Parent = null;
		//	control._parent = this;
		//}

		control->SetParent(shared_from_this());

		if (index >= controls_.size())
			controls_.push_back(control);
		else {
			controls_.insert(index, control);
			OnControlAdded();
		}
	}

	void SetIsEnabled(bool isEnabled)
	{
		if (isEnabled_ == isEnabled)
			return;
		isEnabled_ = isEnabled;
		OnEnabledChanged();
	}

	bool GetIsEnabled()
	{
		return parent_ == nullptr ? isEnabled_ : parent_->GetIsEnabled() && isEnabled_;
	}

	void SetHint(std::string hint)
	{
		if (hint_ == hint)
			return;

		hint_ = hint;
		OnHintChanged();
	}

	std::string GetHint() const
	{
		return hint_;
	}

	void SetIsModal(bool isModal)
	{
		if (isModal_ == isModal)
			return;
		isModal_ = isModal;
		OnIsModalChanged();
	}

	bool GetIsModal()
	{
		return isModal_;
	}

	void SetIsMovable(bool isMovable)
	{
		if (isMovable_ == isMovable)
			return;
		isMovable_ = isMovable;
		OnIsMovableChanged();
	}

	bool GetIsMovable()
	{
		return isMovable_;
	}

	void SetIsNotControl(bool isNotControl)
	{
		if (isNotControl_ == isNotControl)
			return;
		isNotControl_ = isNotControl;
		OnIsNotControlChanged();
	}

	bool GetIsNotControl()
	{
		return isNotControl_;
	}

	void SetOpacity(float opacity)
	{
		if (opacity > 1F)
			opacity = 1F;
		if (opacity < 0F)
			opacity = 0;

		if (opacity_ == opacity)
			return;

		opacity_ = opacity;
		OnOpacityChanged();
	}

	float GetOpacity()
	{
		return opacity_;
	}

	void SetSound(int sound)
	{
		if (sound_ == sound)
			return;
		sound_ = sound;
		OnSoundChanged();
	}

	int GetSound()
	{
		return sound_
	}

	void SetIsSort(bool isSort)
	{
		if (isSort_ == isSort)
			return;
		isSort_ = isSort;
		OnIsSortChanged();
	}

	bool GetIsSort()
	{
		return isSort_;
	}

	void TrySort()
	{
		if (parent_ == null)
			return;

		parent_->TrySort();

		auto & parent_controls = parent_->GetControls();
		if (parent_controls.back().get() == this)
			return;

		if (!isSort_)
			return;

		parent_->RemoveControl(shared_from_this());
		parent_->AddControl(shared_from_this());

		Redraw();
	}

	void SetIsVisible(bool isVisible)
	{
		if (isVisible_ == isVisible)
			return;
		isVisible_ = isVisible;
		OnIsVisibleChanged();
	}

	bool GetIsVisible()
	{
		return parent_ == nullptr ? isVisible_ : parent_->GetIsVisible() && isVisible_;
	}

	virtual bool IsMouseOver(PointPtr p);

	virtual void MultiLine();

	void BringToFront();

	void SetIsDisposed(bool isDisposed);

	void GetIsDisposed();

	virtual void Draw();

	void Dispose();
};

} // namespace mir
