#include "CTRPluginFrameworkPlus.hpp"
#include "CTRPluginFramework.hpp"

using namespace CTRPluginFramework;

std::vector<std::string> Security::bannedList = {};

void Security::AddToBlacklist(const std::string& friendCode)
{
    if (std::find(bannedList.begin(), bannedList.end(), friendCode) == bannedList.end())
    {
        bannedList.push_back(friendCode);
    }

    CheckBan();
}

bool Security::IsBanned(const std::string& friendCode)
{
    return std::find(bannedList.begin(), bannedList.end(), friendCode) != bannedList.end();
}

void Security::CheckBan()
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

void Security::DisplayBlacklist()
{
    if (bannedList.empty())
    {
        MessageBox("There is no banned FC")();
        return;
    }

    std::string bannedListMessage = "Blacklist :\n";
    for (const auto& friendCode : bannedList)
    {
        bannedListMessage += "- " + friendCode + "\n";
    }

    MessageBox messageBox(bannedListMessage);
    messageBox();
}

void Security::LogUserInfo()
{
    // Get FC
    frdInit();
    FriendKey key;
    FRD_GetMyFriendKey(&key);
    u64 localFriendCode = 0;
    FRD_PrincipalIdToFriendCode(key.principalId, &localFriendCode);
    std::string str = Utils::Format("%012lld", localFriendCode);
    std::string FC = Utils::Format("%s-%s-%s", str.substr(0, 4).c_str(), str.substr(4, 4).c_str(), str.substr(8, 4).c_str());

    // (to do) Recuperer nom utilisateur. 

    // Create userinfo.bin
    File output;
    File::Create("userinfo.bin");
    File::Open(output, "userinfo.bin", 1 << 1);

 /* output.WriteLine("User name: ");
    output.WriteLine(userName);          */
    output.WriteLine("Friend Code: ");
    output.WriteLine(FC);
}

/* 
 
A faire:

-Recuperer userinfo.bin
-Envoyer a un Bot discord (ou webhook)
-Envoyer si lutilisateur est connecté
-Ajouter code ami a la liste noir via le bot

*/

/*

void sendUserInfo(){



}

*/
