#pragma once
#include "MirControlWithDynamicImagesMouseEventDriven.h"
#include <functional>
#include <memory>

// 使用 std::function 替代 Java 的 ControlCommonListener
using ControlCommonListener = std::function<void(MirControl*, void*)>;

class MirLabel;

class MirButton : public MirControlWithDynamicImagesMouseEventDriven {
public:
    explicit MirButton(MirControl* parent, long renderer_id,
                       std::shared_ptr<MirImage> normalImage,
                       std::shared_ptr<MirImage> hoverImage,
                       std::shared_ptr<MirImage> pressedImage,
                       const Size& size, const Point& pos);

    // 构造带文本的按钮
    MirButton(MirControl* parent, long renderer_id,
              std::shared_ptr<MirImage> normalImage,
              std::shared_ptr<MirImage> hoverImage,
              std::shared_ptr<MirImage> pressedImage,
              const std::string& text,
              const Size& labelSize, Font* font,
              const Color& foreColor, const Color& backColor, int wraplength);

    virtual ~MirButton();

    std::shared_ptr<MirImage> getHoverImage() const;
    void setHoverImage(std::shared_ptr<MirImage> image);

    std::shared_ptr<MirImage> getPressedImage() const;
    void setPressedImage(std::shared_ptr<MirImage> image);

    // 设置点击事件回调
    void setOnMouseLeftClick(const ControlCommonListener& listener);

    // 获取/设置文本
    std::string getText() const;
    void setText(const std::string& text);

    // 资源释放
    void dispose(bool disposing) override;

protected:
    void onHoverImageChanged();
    void onPressedImageChanged();

private:
    bool isCenterText = false;
    std::shared_ptr<MirImage> hoverImage;
    std::shared_ptr<MirImage> pressedImage;
    ControlCommonListener hoverImageChanged;
    ControlCommonListener pressedImageChanged;
    std::unique_ptr<MirLabel> textLabel;
};