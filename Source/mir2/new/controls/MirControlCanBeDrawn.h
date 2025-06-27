#pragma once
#include <vector>
#include <memory>
#include "Point.h"
#include "Size.h"
#include "Color.h"
#include "Rectangle.h"
#include "SDL.h"

class MirControl;

using ControlCommonListener = std::function<void(class MirControl*, void*)>;

class MirControl {
public:
    explicit MirControl(MirControl* parent);
    virtual ~MirControl();

    virtual bool show();
    virtual void dispose();

    virtual bool onCommonEvent(class CommonEvent& event);

    virtual Point getAbsoluteLocation();
    virtual Rectangle getAbsoluteRectangle();

    // 属性访问器
    MirControl* getParent() const { return parent; }
    void setParent(MirControl* newParent);

    Point getLocation() const { return location; }
    void setLocation(const Point& loc);

    Size getSize() const { return size; }
    void setSize(const Size& s);

    bool getIsVisible() const;
    void setIsVisible(bool visible);

    bool getIsEnabled() const;
    void setIsEnabled(bool enabled);

protected:
    virtual void onLocationChanged();
    virtual void onSizeChanged();
    virtual void onVisibleChanged();
    virtual void onEnabledChanged();

private:
    MirControl* parent = nullptr;
    Point location;
    Size size;
    bool isVisible = true;
    bool isEnabled = true;

    std::vector<std::shared_ptr<MirControl>> children;
};