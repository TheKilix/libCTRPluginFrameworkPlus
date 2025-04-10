#pragma once

#include <CTRPluginFramework.hpp>
#define TOPSCREEN_WIDTH 400

namespace CTRPluginFramework 
{
    class ArrayList {
    public:
        explicit ArrayList(void) : MaxSize(24), foreground(Color::White), background(Color::Black), showArrayList(true), showFrame(true), frame(Color::White) {}
        explicit ArrayList(const u32 MaxSize, const Color& foreground, const Color& background, const bool showFrame = true, const Color& frame = Color::White);
        ~ArrayList();
        
        void Run() noexcept;
        
        static void Add(const std::string& name)    noexcept;
        static void Remove(const std::string& name) noexcept;
        
        static void Hide() noexcept;
        static void Show() noexcept;

        static Color& foregroundColor() noexcept;
        static Color& backgroundColor() noexcept;
        static Color& frameColor()      noexcept;

        static void Clear() noexcept;
    private:
        static ArrayList* instance;

        static void SortByStringLength(std::vector<std::string>& vec) noexcept;
        static u32  alignToRightEdge(const std::string& str) noexcept;
        
        static bool ArrayListOSDCallback(const Screen& scr) noexcept;

        std::vector<std::string> Items;
        bool  showArrayList;
        bool  showFrame;
        u32	  MaxSize;

        Color foreground;
        Color background;
        Color frame;
    };
}
