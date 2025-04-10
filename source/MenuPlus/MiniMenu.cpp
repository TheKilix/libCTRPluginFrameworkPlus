#include "CTRPluginFramework.hpp"
#include "CTRPluginFrameworkPlus.hpp"

namespace CTRPluginFramework
{

    //プロトタイプ宣言
    bool DrawMainMenu   (const Screen &screen);

    //終わりの部分を指定して描写
    void DrawBackword(const Screen &screen, std::string &str, u32 posX, u32 posY, const Color &color)
    {
        int VertextWidth = OSD::GetTextWidth(true, str);    //文字列の長さ取得
        screen.DrawSysfont(str, (posX - VertextWidth), posY, color);
    }

    //タッチ
    bool TouchBox(u32 x, u32 y, u32 w, u32 h)
    {
        if (Touch::IsDown())
        {
            UIntVector pos = Touch::GetPosition();
            if (pos.x >= x && pos.y >= y && pos.x <= (x + w) && pos.y <= (y + h))
            {
                return true;
            }
        }
        return false;
    }

    //改行できるSysfont
    void DrawSysfont_newLinePlus(const Screen &screen, const std::string &str, u32 posX, u32 posY, const Color &color)
    {
        const int gap = 12;
        
        std::vector<std::pair<int, int>> LineRange;
        int start = 0;
        for (int i = 0; i < str.length(); i++)
        {
            if (str[i] == '\n')
            {
                LineRange.emplace_back(start, i); // i - 1 ではなく i を使う
                start = i + 1;
            }
        }
        LineRange.emplace_back(start, str.length());

        for (int i = 0; i < LineRange.size(); i++)
        {
            screen.DrawSysfont(str.substr(LineRange[i].first, LineRange[i].second - LineRange[i].first), posX, posY + i * 12, color);
        }
    }

    struct Cheat_Item{
        //UserSetting
        int                         type;                       //エントリータイプ
        std::string                 name, note;                 //名前, ノート
        u32                         Address, Value;             //アドレス, 値
        VoidNoArgFunctionPointer    cheat;                      //Func
        //Others
        std::string                 NameBeforeModification;     //名前修正前
        bool                        LongName        = false;    //名前修正の必要があったか
        bool                        Information     = false;    //ノートの有無
        bool                        ProgramRunning  = false;    //プログラムがうごいてるか(type 1のみ)
        bool                        UseFunc         = false;    //関数を使っているか
        bool                        Folder          = false;    //ファイルかどうか
    };

    namespace MiniMenuOverview
    {
        std::string             TitleName, VerString;       //name, version
        std::vector<Cheat_Item> Entry;                      //概要
        static bool Cheating                = false;        //一つでもチートがオンか
        static bool WelcomeMessageOpened    = false;        //ウェルカムメッセージを開いてるか
        static bool InformationOpened       = false;        //(i)を開いてるか
        static bool SettingOpened           = false;        //設定を開いてるか
        static bool Optimize                = false;        //軽量版かどうか
        std::string Copyright               = "Basic developer: FPS 18";
    }

    //type 0:チェックタイプ    1:チェック不可
    void AutoEntry(const std::string& name, short type, u32 Address, u32 Value, const std::string& note, VoidNoArgFunctionPointer cheat = nullptr, bool Folder = false)
    {
        Cheat_Item newItem;
        //重複確認
        bool Duplicate = false;
        for (const Cheat_Item& entry : MiniMenuOverview::Entry)
        {
            if (entry.NameBeforeModification == name && entry.type == type && entry.Address == Address && entry.Value == Value && entry.note == note)
            {
                Duplicate = true;
                break;
            }
        }
        if (!Duplicate)
        {
            //////////////////
            //名前の長さ確認
            const int MaxWidth              = 184;
            newItem.NameBeforeModification  = name;
            int nameTextWidth               = OSD::GetTextWidth(true, newItem.NameBeforeModification);
            newItem.LongName                = nameTextWidth > MaxWidth;
            if (newItem.LongName)
            {
                for (int i = 0; i < newItem.NameBeforeModification.length(); i++)
                {
                    int newnameTextWidth = OSD::GetTextWidth(true, newItem.NameBeforeModification.substr(0, i));
                    if (newnameTextWidth <= MaxWidth)
                    {
                        newItem.name = newItem.NameBeforeModification.substr(0, i);
                    }
                    else
                    {
                        break;
                    }
                }
            }
            else
            {
                newItem.name = newItem.NameBeforeModification;
            }

            //////////////////
            newItem.note = note;
            newItem.type = type;
            newItem.Address = Address;
            newItem.Value = Value;
            //Informationの有無
            if (note != "")
            {
                newItem.Information = true;
            }
            //関数を使っているのか
            if (cheat != nullptr)
            {
                newItem.UseFunc = true;
                newItem.cheat   = cheat;
            }
            //ファイルかどうか
            if (Folder)
            {
                newItem.Folder = true;
            }


            MiniMenuOverview::Entry.push_back(newItem);
        }
    }

    //ファイル
    void new_folder(const std::string &name, const std::string &note)
    {
        AutoEntry(name, 0, 0, 0, note, nullptr, true);
    }

    //ファイル終了
    void point_folder()
    {
        
    }

    //べつの関数利用しない
    void new_entry(const std::string &name, short type, u32 Address, u32 Value, const std::string &note)
    {
        AutoEntry(name, type, Address, Value, note);
    }

    //べつの関数利用する
    void new_entry(const std::string &name, short type, VoidNoArgFunctionPointer cheat, const std::string &note)
    {
        AutoEntry(name, type, 0, 0, note, cheat);
    }
}