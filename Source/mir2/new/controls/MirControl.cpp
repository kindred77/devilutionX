#include "MirControl.h"
#include <iostream>

std::atomic<int> MirControl::idCounter(0);

MirControl::MirControl(std::shared_ptr<MirControl> parent)
        : ID(std::to_string(idCounter.fetch_add(1, std::memory_order_relaxed) + 1)),
          parent(parent),
          opacity(1.0f),
          isEnabled(true),
          isVisible(true),
          sound(SoundList::None) {
    if (parent) {
        parent->addChild(shared_from_this());
    }
}

const std::string& MirControl::getID() const {
    return ID;
}

std::shared_ptr<MirControl> MirControl::getParent() {
    return parent.lock();
}

void MirControl::setParent(std::shared_ptr<MirControl> newParent) {
    if (parent.lock() == newParent) {
        return;
    }

    if (auto p = parent.lock()) {
        p->removeChild(shared_from_this());
    }

    parent = newParent;

    if (newParent) {
        newParent->addChild(shared_from_this());
    }

    onParentChangedHandler();
}

const Point& MirControl::getLocation() const {
    return location;
}

void MirControl::setLocation(const Point& newLocation) {
    if (location == newLocation) {
        return;
    }
    location = newLocation;
    onLocationChangedHandler();
}

const Size& MirControl::getSize() const {
    return size;
}

void MirControl::setSize(const Size& newSize) {
    if (size == newSize) {
        return;
    }
    size = newSize;
    onSizeChangedHandler();
}

const std::vector<std::shared_ptr<MirControl>>& MirControl::getChildren() const {
    return children;
}

void MirControl::addChild(std::shared_ptr<MirControl> control) {
    children.push_back(control);
    onChildAddedHandler();
}

void MirControl::removeChild(std::shared_ptr<MirControl> control) {
    children.erase(std::remove_if(children.begin(), children.end(),
                                  [&](const std::shared_ptr<MirControl>& c) { return c.get() == control.get(); }),
                   children.end());
    onChildRemovedHandler();
}

bool MirControl::getIsEnabled() const {
    return (parent.expired() ? isEnabled : parent.lock()->getIsEnabled() && isEnabled);
}

void MirControl::setIsEnabled(bool enabled) {
    if (isEnabled == enabled) {
        return;
    }
    isEnabled = enabled;
    onEnabledChangedHandler();
}

const std::string& MirControl::getHint() const {
    return hint;
}

void MirControl::setHint(const std::string& newHint) {
    if (hint == newHint) {
        return;
    }
    hint = newHint;
    onHintChangedHandler();
}

bool MirControl::getIsModal() const {
    return isModal;
}

void MirControl::setIsModal(bool modal) {
    if (isModal == modal) {
        return;
    }
    isModal = modal;
    onModalChangedHandler();
}

bool MirControl::getIsMovable() const {
    return isMovable;
}

void MirControl::setIsMovable(bool movable) {
    if (isMovable == movable) {
        return;
    }
    isMovable = movable;
    onMovableChangedHandler();
}

bool MirControl::getIsNotControl() const {
    return isNotControl;
}

void MirControl::setIsNotControl(bool notControl) {
    if (isNotControl == notControl) {
        return;
    }
    isNotControl = notControl;
    onNotControlChangedHandler();
}

float MirControl::getOpacity() const {
    return opacity;
}

void MirControl::setOpacity(float newOpacity) {
    if (opacity == newOpacity) {
        return;
    }
    opacity = std::clamp(newOpacity, 0.0f, 1.0f);
    onOpacityChangedHandler();
}

int MirControl::getSound() const {
    return sound;
}

void MirControl::setSound(int newSound) {
    if (sound == newSound) {
        return;
    }
    sound = newSound;
    onSoundChangedHandler();
}

bool MirControl::getIsVisible() const {
    return (parent.expired() ? isVisible : parent.lock()->getIsVisible() && isVisible);
}

void MirControl::setIsVisible(bool visible) {
    if (isVisible == visible) {
        return;
    }
    isVisible = visible;
    onVisibleChangedHandler();
}

bool MirControl::IsByPassEvent() const {
    return isByPassEvent;
}

void MirControl::setIsByPassEvent(bool byPass) {
    isByPassEvent = byPass;
}

Point MirControl::Center() const {
    int parentWidth = Settings::ScreenWidth;
    int parentHeight = Settings::ScreenHeight;

    if (auto p = parent.lock()) {
        parentWidth = p->getSize().getWidth();
        parentHeight = p->getSize().getHeight();
    }

    return Point((parentWidth - size.getWidth()) / 2, (parentHeight - size.getHeight()) / 2);
}

void MirControl::bringToFront() {
    if (auto p = parent.lock()) {
        p->removeChild(shared_from_this());
        p->addChild(shared_from_this());
    }
}

bool MirControl::_drawControl() {
    return true;
}

bool MirControl::_beginDraw() {
    return true;
}

bool MirControl::_endDraw() {
    return true;
}

bool MirControl::show() {
    if (isDisposed || !getIsVisible()) {
        return false;
    }

    Point pos = getLocation();

    int limitX = Settings::ScreenWidth;
    int limitY = Settings::ScreenHeight;

    if (auto p = parent.lock()) {
        limitX = p->getSize().getWidth();
        limitY = p->getSize().getHeight();
    }

    if (pos.getX() > limitX || pos.getY() > limitY) {
        return false;
    }

    onBeforeShownHandler();

    if (!isDrawControlTexture || !_beginDraw()) {
        return false;
    }

    if (!_drawControl()) {
        return false;
    }

    if (isShowChildren) {
        showChildren();
    }

    _endDraw();

    onShownHandler();

    return true;
}

void MirControl::showChildren() {
    for (auto& child : children) {
        if (child && !child->getIsDisposed()) {
            child->show();
        }
    }
}

void MirControl::deactivate() {
    if (ActiveControl.get() != this) {
        return;
    }

    ActiveControl = nullptr;
    isStartToMove = false;
    startToMovePos = Point::Empty();
}

void MirControl::dehighlight() {
    if (MouseControl.get() != this) {
        return;
    }

    MouseControl->onMouseLeaveHandler();
    MouseControl = nullptr;
}

void MirControl::activate() {
    if (ActiveControl.get() == this) {
        return;
    }

    if (ActiveControl) {
        ActiveControl->deactivate();
    }

    ActiveControl = shared_from_this();
}

void MirControl::highlight() {
    if (MouseControl.get() == this) {
        return;
    }

    if (isNotControl) {
        return;
    }

    if (MouseControl) {
        MouseControl->dehighlight();
    }

    if (ActiveControl && ActiveControl.get() != this) {
        return;
    }

    onMouseEnterHandler();
    MouseControl = shared_from_this();
}

Rectangle MirControl::getLocationRectangle() const {
    return Rectangle(location, size);
}

bool MirControl::isMouseOver(const Point& posInParent) const {
    return isVisible && (getLocationRectangle().contains(posInParent) || isStartToMove || isModal) && !isNotControl;
}