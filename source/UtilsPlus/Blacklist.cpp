#include "CTRPluginFrameworkPlus.hpp"
#include "CTRPluginFramework.hpp"

using namespace CTRPluginFramework;

std::vector<std::string> Blacklist::bannedList = {};

void Blacklist::Add(const std::string& friendCode)
{
    if (std::find(bannedList.begin(), bannedList.end(), friendCode) == bannedList.end())
    {
        bannedList.push_back(friendCode);
    }

    CheckBan();
}

bool Blacklist::IsBanned(const std::string& friendCode)
{
    return std::find(bannedList.begin(), bannedList.end(), friendCode) != bannedList.end();
}

void Blacklist::CheckBan()
{
    frdInit();
    FriendKey key;
    FRD_GetMyFriendKey(&key);
    u64 localFriendCode = 0;
    FRD_PrincipalIdToFriendCode(key.principalId, &localFriendCode);
    std::string str = Utils::Format("%012lld", localFriendCode);
    std::string FC = Utils::Format("%s-%s-%s", str.substr(0, 4).c_str(), str.substr(4, 4).c_str(), str.substr(8, 4).c_str());
    frdExit();

    if (IsBanned(FC))
    {
        Sleep(Milliseconds(100));
        MessageBox("Access Denied!")();
        Process::ReturnToHomeMenu();
        svcExitThread();
        srvExit();
    }
}

void Blacklist::DisplayBannedList()
{
    if (bannedList.empty())
    {
        MessageBox("There is no banned FC")();
        return;
    }

    std::string bannedListMessage = "Banned FC :\n";
    for (const auto& friendCode : bannedList)
    {
        bannedListMessage += "- " + friendCode + "\n";
    }

    MessageBox messageBox(bannedListMessage);
    messageBox();
}
