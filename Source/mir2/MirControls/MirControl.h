#pragma once

#include "mir2/Common/Point.h"
#include "mir2/Common/Color.h"
#include "mir2/Common/Rectangle.h"
#include "mir2/Common/Size.h"
#include "mir2/Common/Vector2.h"
#include "mir2/Engine/Texture.h"
#include "mir2/Engine/SoundList.h"
#include "mir2/Engine/SoundManager.h"

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

	void RemoveControl(MirControlPtr control)
	{
		controls_.erase(std::remove_if(controls_.begin(), controls_.end(), [control](MirControlPtr control_ptr) { return control_ptr.get() == control.get(); }), controls_.end());
		OnControlRemoved();
	}

	void OnBorderChanged()
	{
		Redraw();
		//if (BorderChanged != null)
		//	BorderChanged.Invoke(this, EventArgs.Empty);
	}

	void OnBorderColourChanged()
	{
		Redraw();
		//if (BorderColourChanged != null)
		//	BorderColourChanged.Invoke(this, EventArgs.Empty);
	}

	void OnControlAdded()
	{
		Redraw();
		//if (ControlAdded != null)
		//	ControlAdded.Invoke(this, EventArgs.Empty);
	}

	void OnControlRemoved()
	{
		Redraw();
		//if (ControlRemoved != null)
		//	ControlRemoved.Invoke(this, EventArgs.Empty);
	}

	void OnHintChanged()
	{
		Redraw();
		//if (HintChanged != null)
		//	HintChanged.Invoke(this, e);
	}

	void OnIsModalChanged()
	{
		Redraw();
		//if (ModalChanged != null)
		//	ModalChanged.Invoke(this, EventArgs.Empty);
	}

	void OnIsMovableChanged()
	{
		Redraw();
		//if (MovableChanged != null)
		//	MovableChanged.Invoke(this, EventArgs.Empty);
	}

	void OnIsNotControlChanged()
	{
		Redraw();
		//if (NotControlChanged != null)
		//	NotControlChanged.Invoke(this, EventArgs.Empty);
	}

	void OnOpacityChanged()
	{
		Redraw();
		//if (OpacityChanged != null)
		//	OpacityChanged.Invoke(this, EventArgs.Empty);
	}

	void OnSoundChanged()
	{
		//if (SoundChanged != null)
		//	SoundChanged.Invoke(this, EventArgs.Empty);
	}

	void OnIsSortChanged()
	{
		Redraw();
		//if (SortChanged != null)
		//	SortChanged.Invoke(this, EventArgs.Empty);
	}

	void AddControl(MirControlPtr control)
	{
		controls_.push_back(control);
		OnControlAdded();
	}

protected:
	RectanglePtr borderRectangle_ = nullptr;
	std::vector<Vector2Ptr> borderInfo_;
	TexturePtr controlTexture_ = nullptr;
	SizePtr textureSize_ = nullptr;
	bool isHasShown_ = false;
	bool isMoving_ = false;
	bool isTextureValid_ = true;
	virtual void OnBackColourChanged()
	{
		textureValid_ = false;
		Redraw();
		//if (BackColourChanged != null)
		//	BackColourChanged.Invoke(this, EventArgs.Empty);
	}

	virtual void OnSizeChanged()
	{
		textureValid_ = false;
		Redraw();

		//if (SizeChanged != null)
		//	SizeChanged.Invoke(this, EventArgs.Empty);
	}

	virtual std::vector<Vector2Ptr> GetBorderInfo()
	{
		if (Size == Size.Empty)
			return {};

		const auto rect = GetDisplayRectangle();
		if (borderRectangle_ != rect) {
			borderInfo_.reserve(8);
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Left - 1, rect.Top - 1));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Right, rect.Top - 1));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Left - 1, rect.Top - 1));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Left - 1, rect.Bottom));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Left - 1, rect.Bottom));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Right, rect.Bottom));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Right, rect.Top - 1));
			borderInfo_.emplace_back(std::make_shared<Vector2>(rect.Right, rect.Bottom));

			borderRectangle_ = rect;
		}
		return borderInfo_;
	}

	PointPtr Center()
	{
		return std::make_shared<Point>((Settings.ScreenWidth - size_->width) / 2, (Settings.ScreenHeight - size_->height) / 2);
	}

	PointPtr Left()
	{
		return std::make_shared<Point>(0, (Settings.ScreenHeight - size_.height) / 2);
	}

	PointPtr Top()
	{
		return std::make_shared<Point>((Settings.ScreenWidth - size_->width) / 2, 0);
	}

	PointPtr Right()
	{
		return std::make_shared<Point>(Settings.ScreenWidth - size_->width, (Settings.ScreenHeight - size_->height) / 2);
	}

	PointPtr Bottom()
	{
		return std::make_shared<Point>((Settings.ScreenWidth - size_->width) / 2, Settings.ScreenHeight - size_->height);
	}

	PointPtr TopLeft()
	{
		return std::make_shared<Point>(0, 0);
	}

	PointPtr TopRight()
	{
		return std::make_shared<Point>(Settings.ScreenWidth - size_->width, 0);
	}

	PointPtr BottomRight()
	{
		return std::make_shared<Point>(Settings.ScreenWidth - size_->width, Settings.ScreenHeight - size_->height);
	}

	PointPtr BottomLeft()
	{
		return std::make_shared<Point>(0, Settings.ScreenHeight - size_->height);
	}

	virtual void CreateTexture()
	{
		if (controlTexture_ != nullptr && !controlTexture_->IsDisposed() && GetSize() != textureSize_)
			controlTexture_->Dispose();

		if (controlTexture_ == nullptr || controlTexture_->IsDisposed()) {
			//TODO
			//DXManager.ControlList.Add(this);
			//controlTexture_ = new Texture(DXManager.Device, Size.Width, Size.Height, 1, Usage.RenderTarget, Format.A8R8G8B8, Pool.Default);
			controlTexture_ = std::make_shared<Texture>();
			//ControlTexture.Disposing += ControlTexture_Disposing;
			textureSize_ = GetSize();
		}

		textureValid_ = true;
	}

	virtual void OnEnabledChanged()
	{
		Redraw();

		//if (EnabledChanged != null)
		//	EnabledChanged.Invoke(this, EventArgs.Empty);

		//if (!Enabled && ActiveControl == this)
		//	ActiveControl.Deactivate();

		for (auto control : controls_)
		{
			control->OnEnabledChanged();
		}
	}

	virtual void OnForeColourChanged()
	{
		textureValid_ = false;
		//if (ForeColourChanged != null)
		//	ForeColourChanged.Invoke(this, EventArgs.Empty);
	}

	virtual void OnParentChanged()
	{
		OnLocationChanged();
		//if (ParentChanged)
		//	ParentChanged(shared_from_this());
	}

	virtual void OnLocationChanged()
	{
		Redraw();
		for (int i = 0; i < controls_.size; i++)
			controls_[i]->OnLocationChanged();

		//if (LocationChanged)
		//	LocationChanged(shared_from_this());
	}

	virtual void OnMouseEnter()
	{
		if (!isEnabled_)
			return;

		Redraw();

		//if (MouseEnter != null)
		//	MouseEnter.Invoke(this, EventArgs.Empty);
	}

	virtual void Highlight()
	{
		if (MirControl::MouseControl.get() == this)
			return;
		if (GetIsNotControl()) {

		}
		if (MirControl::MouseControl != nullptr)
			MirControl::MouseControl->Dehighlight();

		if (MirControl::ActiveControl != nullptr && MirControl::ActiveControl.get() != this)
			return;

		OnMouseEnter();
		MirControl::MouseControl = shared_from_this();
	}

	virtual void OnIsVisibleChanged()
	{
		Redraw();
		//if (VisibleChanged != null)
		//	VisibleChanged.Invoke(this, EventArgs.Empty);

		isMoving_ = false;

		movePoint_ = std::make_shared<Point>();


		if (isSort_ && parent_ != nullptr) {
			parent_->RemoveControl(shared_from_this());
			parent_->AddControl(shared_from_this());
		}

		if (MirControl::MouseControl == shared_from_this() && !GetIsVisible())
		{
			Dehighlight();
			Deactivate();
		}
		//TODO get mouse point
		//else if (IsMouseOver(CMain.MPoint))
		//{
		//	Highlight();
		//}

		for (MirControlPtr control : controls_)
		{
			control->OnIsVisibleChanged();
		}
	}

	void OnBeforeShown()
	{
		if (isHasShown_)
			return;

		//TODO
		//if (GetIsVisible() && IsMouseOver(CMain.MPoint))
		//	Highlight();

		//if (BeforeShown != null)
		//	BeforeShown.Invoke(this, EventArgs.Empty);
	}

	void OnShown()
	{
		if (isHasShown_)
			return;

		//if (Shown != null)
		//	Shown.Invoke(this, EventArgs.Empty);

		isHasShown_ = true;
	}

	virtual void BeforeDrawControl()
	{
		//if (BeforeDraw != null)
		//	BeforeDraw.Invoke(this, EventArgs.Empty);
	}

	virtual void DrawControl()
	{
		if (!GetIsDrawControlTexture())
			return;

		if (!isTextureValid_)
			CreateTexture();

		if (controlTexture_ == nullptr || controlTexture_->IsDisposed())
			return;

		//float oldOpacity = DXManager.Opacity;

		//DXManager.SetOpacity(Opacity);
		//DXManager.Sprite.Draw2D(ControlTexture, Point.Empty, 0F, DisplayLocation, Color.White);
		//DXManager.SetOpacity(oldOpacity);

		//CleanTime = CMain.Time + Settings.CleanDelay;
	}

	void DrawChildControls()
	{
		for (auto control : controls_)
		{
			control->Draw();
		}
	}

	virtual void DrawBorder()
	{
		if (!isBorder_)
			return;
		//DXManager.Sprite.Flush();
		//DXManager.Line.Draw(BorderInfo, _borderColour);
	}

	void AfterDrawControl()
	{
		//if (AfterDraw != null)
		//	AfterDraw.Invoke(this, EventArgs.Empty);
	}

	virtual void Deactivate()
	{
		if (ActiveControl.get() != this)
			return;

		ActiveControl = nullptr;
		isMoving_ = false;
		movePoint_ = Point.Empty;
	}

	virtual void OnMouseLeave()
	{
		if (!isEnabled_)
			return;

		Redraw();

		//if (MouseLeave != null)
		//	MouseLeave.Invoke(this, EventArgs.Empty);
	}

	virtual void OnMouseClick(/* MouseEventArgs e */)
	{
		if (!GetIsEnabled())
			return;

		if (sound_ != SoundList.None)
			SoundManager::PlaySound(Sound);

		//if (Click != null)
		//	InvokeMouseClick(e);
	}

	void OnMouseDoubleClick(/* MouseEventArgs e */)
	{
		if (!GetIsEnabled())
			return;

		if (DoubleClick != null) {
			if (sound_ != SoundList.None)
				SoundManager::PlaySound(Sound);
			//InvokeMouseDoubleClick(e);
		} else
			OnMouseClick(e);
	}

	virtual void OnMouseMove(/* MouseEventArgs e */)
	{
		if (!isEnabled_)
			return;

		/*if (isMoving_) {
			Point tempPoint = CMain.MPoint.Subtract(_movePoint);

			if (Parent == null) {
				if (tempPoint.Y + TrueSize.Height > Settings.ScreenHeight)
					tempPoint.Y = Settings.ScreenHeight - TrueSize.Height - 1;

				if (tempPoint.X + TrueSize.Width > Settings.ScreenWidth)
					tempPoint.X = Settings.ScreenWidth - TrueSize.Width - 1;
			} else {
				if (tempPoint.Y + TrueSize.Height > Parent.TrueSize.Height)
					tempPoint.Y = Parent.TrueSize.Height - TrueSize.Height;

				if (tempPoint.X + TrueSize.Width > Parent.TrueSize.Width)
					tempPoint.X = Parent.TrueSize.Width - TrueSize.Width;
			}

			if (tempPoint.X < 0)
				tempPoint.X = 0;
			if (tempPoint.Y < 0)
				tempPoint.Y = 0;

			Location = tempPoint;
			if (OnMoving != null)
				OnMoving.Invoke(this, e);
			return;
		}

		if (Controls != null)
			for (int i = Controls.Count - 1; i >= 0; i--)
				if (Controls[i].IsMouseOver(CMain.MPoint)) {
					Controls[i].OnMouseMove(e);
					return;
				}

		Highlight();

		if (MouseMove != null)
			MouseMove.Invoke(this, e);*/
	}

	virtual void OnMouseDown(/* MouseEventArgs e */)
	{
		if (!isEnabled_)
			return;

		/*Activate();

		TrySort();

		if (_movable) {
			Moving = true;
			_movePoint = CMain.MPoint.Subtract(Location);
		}

		if (MouseDown != null)
			MouseDown.Invoke(this, e);*/
	}

	virtual void OnMouseUp(/* MouseEventArgs e */)
	{
		if (!isEnabled_)
			return;

		/*if (Moving) {
			Moving = false;
			_movePoint = Point.Empty;
		}

		if (ActiveControl != null)
			ActiveControl.Deactivate();

		if (MouseUp != null)
			MouseUp.Invoke(this, e);*/
	}

	virtual void OnKeyPress(/* KeyEventArgs e */)
	{
		/*if (!_enabled)
			return;

		if (Controls != null)
			for (int i = Controls.Count - 1; i >= 0; i--)
				if (e.Handled)
					return;
				else
					Controls[i].OnKeyPress(e);

		if (KeyPress == null)
			return;
		KeyPress.Invoke(this, e);*/
	}

	virtual void OnKeyDown(/* KeyEventArgs e */)
	{
		/*if (!_enabled)
			return;

		if (Controls != null)
			for (int i = Controls.Count - 1; i >= 0; i--)
				if (e.Handled)
					return;
				else
					Controls[i].OnKeyDown(e);

		if (KeyDown == null)
			return;
		KeyDown.Invoke(this, e);*/
	}

	virtual void OnKeyUp(/* KeyEventArgs e */)
	{
		/*if (!_enabled)
			return;

		if (Controls != null)
			for (int i = Controls.Count - 1; i >= 0; i--)
				if (e.Handled)
					return;
				else
					Controls[i].OnKeyUp(e);

		if (KeyUp == null)
			return;
		KeyUp.Invoke(this, e);*/
	}

	virtual void Dehighlight()
	{
		if (MouseControl.get() != this)
			return;
		MouseControl->OnMouseLeave();
		MouseControl = nullptr;
	}

	virtual void Dispose(bool disposing)
	{
		/*if (disposing) {
			if (Disposing != null)
				Disposing.Invoke(this, EventArgs.Empty);

			Disposing = null;

			BackColourChanged = null;
			_backColour = Color.Empty;

			BorderChanged = null;
			_border = false;
			BorderRectangle = Rectangle.Empty;
			_borderInfo = null;

			BorderColourChanged = null;
			_borderColour = Color.Empty;

			DrawControlTexture = false;
			if (ControlTexture != null && !ControlTexture.Disposed)
				ControlTexture.Dispose();
			ControlTexture = null;
			TextureValid = false;

			ControlAdded = null;
			ControlRemoved = null;

			if (Controls != null) {
				for (int i = Controls.Count - 1; i >= 0; i--) {
					if (Controls[i] != null && !Controls[i].IsDisposed)
						Controls[i].Dispose();
				}

				Controls = null;
			}
			_enabled = false;
			EnabledChanged = null;

			HasShown = false;

			BeforeDraw = null;
			AfterDraw = null;
			Shown = null;
			BeforeShown = null;

			Click = null;
			DoubleClick = null;
			MouseEnter = null;
			MouseLeave = null;
			MouseMove = null;
			MouseDown = null;
			MouseUp = null;
			MouseWheel = null;

			KeyPress = null;
			KeyUp = null;
			KeyDown = null;

			ForeColourChanged = null;
			_foreColour = Color.Empty;

			LocationChanged = null;
			_location = Point.Empty;

			ModalChanged = null;
			_modal = false;

			MovableChanged = null;
			_movePoint = Point.Empty;
			Moving = false;
			OnMoving = null;
			_movable = false;

			NotControlChanged = null;
			_notControl = false;

			OpacityChanged = null;
			_opacity = 0F;

			if (Parent != null && Parent.Controls != null)
				Parent.Controls.Remove(this);
			ParentChanged = null;
			_parent = null;

			SizeChanged = null;
			_size = Size.Empty;

			SoundChanged = null;
			_sound = 0;

			VisibleChanged = null;
			_visible = false;

			if (ActiveControl == this)
				ActiveControl = null;
			if (MouseControl == this)
				MouseControl = null;
		}

		IsDisposed = true;*/
	}

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

	virtual bool IsMouseOver(PointPtr p)
	{
		return GetIsVisible() && (GetDisplayRectangle()->Contains(p) || isMoving_ || GetIsModal()) && !GetIsNotControl();
	}

	virtual void MultiLine()
	{
	}

	void BringToFront()
	{
		if (parent_ == nullptr)
			return;

		if (parent_controls.back().get() == this)
			return;

		parent_->RemoveControl(shared_from_this());
		parent_->AddControl(shared_from_this());
		Redraw();
	}

	void SetIsDisposed(bool isDisposed)
	{
		isDisposed_ = isDisposed;
	}

	void GetIsDisposed()
	{
		return isDisposed_;
	}

	virtual void Draw()
	{
		if (GetIsDisposed() || !GetIsVisible() /*|| Size.Width == 0 || Size.Height == 0*/ || GetSize()->width > Settings.ScreenWidth || GetSize()->height > Settings.ScreenHeight)
			return;

		OnBeforeShown();

		BeforeDrawControl();
		DrawControl();
		DrawChildControls();
		DrawBorder();
		AfterDrawControl();

		//CleanTime = CMain.Time + Settings.CleanDelay;

		OnShown();
	}

	void Dispose()
	{
		if (isDisposed_)
			return;
		Dispose(true);
	}
};

} // namespace mir
