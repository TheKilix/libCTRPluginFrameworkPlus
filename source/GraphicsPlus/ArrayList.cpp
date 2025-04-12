#include "ArrayList.hpp"

namespace CTRPluginFramework 
{    
    ArrayList* ArrayList::instance = nullptr;
    
    ArrayList::ArrayList(const u32 MaxSize, const Color& foreground, const Color& background, const bool showFrame, const Color& frame)
        : MaxSize(MaxSize), foreground(foreground), background(background), showArrayList(true), showFrame(showFrame), frame(frame)
    {}

    ArrayList::~ArrayList(void) {
        OSD::Stop(ArrayListOSDCallback);
        instance = nullptr;
    }

    void ArrayList::Run() noexcept {
        instance = this;
        OSD::Run(ArrayListOSDCallback);
    }

    void ArrayList::Add(const std::string& name) noexcept {
        bool SameStringExists = false;

        if (!name.empty() && instance->Items.size() < instance->MaxSize) {
            for (const auto& i : instance->Items) {
                if (i == name) {
                    SameStringExists = true;
                    break;
                } 
            }

            if (!SameStringExists) {
                instance->Items.emplace_back(name);
                
                SortByStringLength(instance->Items);
            }
        }
    }
    
    void ArrayList::Remove(const std::string& name) noexcept {
        if (!instance->Items.empty() && !name.empty()) {
            for (auto it = instance->Items.begin(), end = instance->Items.end(); it != end; ++it) {
                if (name == *it) {
                    instance->Items.erase(it);
                    break;
                }
            }
            SortByStringLength(instance->Items);
        }
    }

    void ArrayList::Clear() noexcept {
        instance->Items.clear();
    }

    void ArrayList::Hide() noexcept {
        if (instance->showArrayList) {
            OSD::Stop(ArrayListOSDCallback);
            instance->showArrayList = false;
        }
    };
    
    void ArrayList::Show() noexcept {
        if (!instance->showArrayList) {
            OSD::Run(ArrayListOSDCallback);
            instance->showArrayList = true;
        }
    }

    Color& ArrayList::foregroundColor() noexcept {
        return instance->foreground;
    }
    
    Color& ArrayList::backgroundColor() noexcept {
        return instance->background;
    }

    Color& ArrayList::frameColor() noexcept {
        return instance->frame;
    }

    bool ArrayList::ArrayListOSDCallback(const Screen& scr) noexcept {
        if (instance->showArrayList && !instance->Items.empty()) {
            if (scr.IsTop) {
                u32 posY = 0;
                
                for (auto it = instance->Items.begin(), end = instance->Items.end(); it != end; ++it) {
                    const u32 posX = alignToRightEdge(*it);

                    posY = scr.Draw(*it, posX, posY, instance->foreground, instance->background);

                    // Draw Frame
                    if (instance->showFrame) {
                        const u32  newPosX = posX - 1;
                        
                        for (u32 y = posY - 10; y < posY; ++y) {
                            scr.DrawPixel(newPosX, y, instance->frame);
                        }
                        
                        const auto nextIt = std::next(it);
                        for (u32 x = newPosX, endX = (nextIt == end ? TOPSCREEN_WIDTH : alignToRightEdge(*nextIt)); x < endX; ++x) {
                            scr.DrawPixel(x, posY, instance->frame);
                        }
                    }
                }
            }
        }
        return true;
    }

    void ArrayList::SortByStringLength(std::vector<std::string>& vec) noexcept {
        std::sort(
             vec.begin()
            ,vec.end()
            ,[](const std::string& str1, const std::string& str2) {
                return str1.size() > str2.size(); 
            }
        );
    }

    u32 ArrayList::alignToRightEdge(const std::string& str) noexcept {
        return TOPSCREEN_WIDTH - (str.length() * 6.1f);
    }
}