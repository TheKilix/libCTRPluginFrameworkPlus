#pragma once

#include <string>
#include <vector>

class Blacklist
{
public:
	// Add FC to the blacklist
    static void Add(const std::string& friendCode);

	// Check if the FC is in the blacklist
    static bool IsBanned(const std::string& friendCode);

	// Check if user is banned
    static void CheckBan();

	// Display the banned list
	static void DisplayBannedList();


private:
	// FC banned list
    static std::vector<std::string> bannedList;
};
