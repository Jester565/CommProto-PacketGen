#pragma once
#include "CommandArgument.h"
#include <unordered_map>

class CommandArgumentManager
{
public:
		CommandArgumentManager();

		void addArgument(char* key, const std::string& description, bool usesStr, char* defaultValue = nullptr);

		bool readArgs(int numArgs, char* args[]);

		std::string* getValue(char* key);

		void printManual();

		~CommandArgumentManager();

private:
		std::unordered_map <std::string, CommandArgument*> arguments;
};
