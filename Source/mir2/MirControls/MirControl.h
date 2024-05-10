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

	MirControlPtr GetParent() const;

	virtual void SetSize(SizePtr size);

	virtual SizePtr GetSize() const;

	virtual SizePtr GetTrueSize() const;

	RectanglePtr GetDisplayRectangle() const;

	void SetParent(MirControlPtr parent);

	void SetBackColor(ColorPtr backColor);

	ColorPtr GetBackColor() const;

	void SetLocation(PointPtr location);;

	virtual PointPtr GetDisplayLocation();

	virtual void SetIsBorder(bool isBorder);

	virtual bool GetIsBorder() const;

	virtual void SetBorderColor(ColorPtr borderColor);

	virtual ColorPtr GetBorderColor() const;

	void SetForeColour(ColorPtr foreColor);

	ColorPtr GetForeColour() const;

	void SetIsDrawControlTexture(bool isDrawControlTexture);

	bool GetIsDrawControlTexture() const;

	virtual void Redraw();

	std::vector<MirControlPtr> GetControls() const;

	void InsertControl(int index, MirControlPtr control);

	void SetIsEnabled(bool isEnabled);

	bool GetIsEnabled();

	void SetHint(std::string hint);

	std::string GetHint() const;

	void SetIsModal(bool isModal);

	bool GetIsModal();

	void SetIsMovable(bool isMovable);

	bool GetIsMovable();

	void SetIsNotControl(bool isNotControl);

	bool GetIsNotControl();

	void SetOpacity(float opacity);

	float GetOpacity();

	void SetSound(int sound);

	int GetSound();

	void SetIsSort(bool isSort);

	bool GetIsSort();

	void TrySort();

	void SetIsVisible(bool isVisible);

	bool GetIsVisible();

	virtual bool IsMouseOver(const PointPtr p);

	virtual void MultiLine();

	void BringToFront();

	void SetIsDisposed(bool isDisposed);

	void GetIsDisposed();

	virtual void Draw();

	void Dispose();
};

} // namespace mir
