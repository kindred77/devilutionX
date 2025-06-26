#pragma once

#include <string>
#include <vector>
#include <memory>
#include <atomic>
#include <functional>

#include "Point.h"
#include "Size.h"
#include "Rectangle.h"
#include "Settings.h"
#include "SoundManager.h"

class MirControl;

using ControlCommonListener = std::function<void(MirControl*, void*)>;

class MirControl {
public:
    static std::shared_ptr<MirControl> ActiveControl;
    static std::shared_ptr<MirControl> MouseControl;

private:
    static std::atomic<int> idCounter;
    std::string ID;

    std::weak_ptr<MirControl> parent;
    ControlCommonListener onParentChanged;

    Size size = Size::Empty();
    ControlCommonListener onSizeChanged;

    Point location{0, 0};
    ControlCommonListener onLocationChanged;

    long cleanTime = 0;

    std::vector<std::shared_ptr<MirControl>> children;
    ControlCommonListener onChildAdded;
    ControlCommonListener onChildRemoved;

    bool isEnabled = true;
    ControlCommonListener onEnabledChanged;

    bool isDrawControlTexture = false;
    bool isShowChildren = true;

    bool isHasShown = false;
    long lastClickTime = 0L;

    ControlCommonListener onMouseLeftClick;
    ControlCommonListener onMouseLeftDoubleClick;
    ControlCommonListener onMouseEnter;
    ControlCommonListener onMouseLeave;
    ControlCommonListener onShown;
    ControlCommonListener onBeforeShown;
    ControlCommonListener onDisposing;

    ControlCommonListener onMouseMove;
    ControlCommonListener onMouseLeftDown;
    ControlCommonListener onMouseLeftUp;
    ControlCommonListener onMouseRightDown;
    ControlCommonListener onMouseRightUp;

    std::string hint;
    ControlCommonListener onHintChanged;

    bool isModal = false;
    ControlCommonListener onModalChanged;

    bool isStartToMove = false;
    bool isMovable = false;
    Point startToMovePos;
    ControlCommonListener onMovableChanged;
    ControlCommonListener onMoving;

    bool isMouseLeftDown = false;
    bool isMouseRightDown = false;

    bool isNotControl = false;
    ControlCommonListener onNotControlChanged;

    float opacity = 1.0f;
    ControlCommonListener onOpacityChanged;

    int sound = SoundList::None;
    ControlCommonListener onSoundChanged;

    bool isVisible = true;
    ControlCommonListener onVisibleChanged;

    bool isByPassEvent = false;

    bool isDisposed = false;

public:
    explicit MirControl(std::shared_ptr<MirControl> parent = nullptr);

    const std::string& getID() const;

    std::shared_ptr<MirControl> getParent();

    void setParent(std::shared_ptr<MirControl> newParent);

    const Point& getLocation() const;

    void setLocation(const Point& newLocation);

    const Size& getSize() const;

    void setSize(const Size& newSize);

    const std::vector<std::shared_ptr<MirControl>>& getChildren() const;

    void addChild(std::shared_ptr<MirControl> control);

    void removeChild(std::shared_ptr<MirControl> control);

    bool getIsEnabled() const;

    void setIsEnabled(bool enabled);

    const std::string& getHint() const;

    void setHint(const std::string& newHint);

    bool getIsModal() const;

    void setIsModal(bool modal);

    bool getIsMovable() const;

    void setIsMovable(bool movable);

    bool getIsNotControl() const;

    void setIsNotControl(bool notControl);

    float getOpacity() const;

    void setOpacity(float newOpacity);

    int getSound() const;

    void setSound(int newSound);

    bool getIsVisible() const;

    void setIsVisible(bool visible);

    bool IsByPassEvent() const;

    void setIsByPassEvent(bool byPass);

    Point Center() const;

    Point Left() const;

    Point Top() const;

    Point Right() const;

    Point Bottom() const;

    Point TopLeft() const;

    Point TopRight() const;

    Point BottomLeft() const;

    Point BottomRight() const;

    void bringToFront();

    virtual bool _drawControl();

    virtual bool _beginDraw();

    virtual bool _endDraw();

    bool show();

    void showChildren();

    void deactivate();

    void dehighlight();

    void activate();

    void highlight();

    Rectangle getLocationRectangle() const;

    bool isMouseOver(const Point& posInParent) const;

    void onParentChangedHandler();

    void onLocationChangedHandler();

    void onSizeChangedHandler();

    void onEnabledChangedHandler();

    void onChildAddedHandler();

    void onChildRemovedHandler();

    void onHintChangedHandler();

    void onModalChangedHandler();

    void onMovableChangedHandler();

    void onNotControlChangedHandler();

    void onOpacityChangedHandler();

    void onSoundChangedHandler();

    void onVisibleChangedHandler();

    void onDisposingHandler();

    void onMouseEnterHandler();

    void onMouseLeaveHandler();

    void onMouseMoveHandler(const Point& posInParent, bool isMouseLeave);

    void onMouseLeftDownHandler(const Point& posInParent);

    void onMouseLeftUpHandler(const Point& posInParent);

    void onMouseRightDownHandler(const Point& posInParent);

    void onMouseRightUpHandler(const Point& posInParent);

    bool onMouseLeftClickHandler(const Point& posInParent, bool isOriginal);

    bool onMouseLeftDoubleClickHandler(const Point& posInParent, bool isOriginal);

    bool onOriginalMouseEvent(CommonEvent::EventEnum eventEnum, const Point& posInParent);

    bool onCommonEvent(const CommonEvent& event);

    bool getIsDisposed() const;

    void dispose();

    virtual ~MirControl();

private:
    void onEnabledChangedInternal();

    void onVisibleChangedInternal();

    void onLocationChangedInternal();

    void onSizeChangedInternal();

    void onChildAddedInternal();

    void onChildRemovedInternal();

    void onHintChangedInternal();

    void onModalChangedInternal();

    void onMovableChangedInternal();

    void onNotControlChangedInternal();

    void onOpacityChangedInternal();

    void onSoundChangedInternal();

    void onVisibleChangedInternal();

    void onDisposingInternal();
};