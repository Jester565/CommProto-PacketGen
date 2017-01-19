#pragma once
#include "CommandArgument.h"
#include <unordered_map>

class CommandArgumentManager
{
public:
		CommandArgumentManager();

		void addArgument(char* key, bool usesStr = true, char* defaultValue = nullptr);

		bool readArgs(int numArgs, char* args[]);

		std::string* getValue(char* key);

		~CommandArgumentManager();

private:
		std::unordered_map <std::string, CommandArgument*> arguments;
};
