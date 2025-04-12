#pragma once

#include <string>
#include <vector>

class Security
{
public:
	// Add FC to the blacklist
    static void AddToBlacklist(const std::string& friendCode);

	// Check if the FC is in the blacklist
    static bool IsBanned(const std::string& friendCode);

	// Check if user is banned
    static void CheckBan();

	// Display the banned list
	static void DisplayBlacklist();

	// Logs user info
	static void LogUserInfo();


private:
	// FC banned list
    static std::vector<std::string> bannedList;
};
