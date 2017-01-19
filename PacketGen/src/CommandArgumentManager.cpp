#include "CommandArgumentManager.h"
#include <iostream>

CommandArgumentManager::CommandArgumentManager()
{
}

void CommandArgumentManager::addArgument(char * key, bool usesStr, char * defaultValue)
{
		arguments.emplace(std::make_pair(std::string(key), new CommandArgument(usesStr, defaultValue)));
}

bool CommandArgumentManager::readArgs(int numArgs, char * args[])
{
		for (int i = 1; i < numArgs; i++)
		{
				std::string argStr(args[i]);
				while (argStr.size() > 0)
				{
						if (argStr.front() != '-')
						{
								break;
						}
						argStr.erase(argStr.begin());
				}
				std::size_t equalIdx = argStr.find('=');
				if (equalIdx == std::string::npos)
				{
						auto argIter = arguments.find(argStr);
						if (argIter != arguments.end())
						{
								if (!argIter->second->usesStr())
								{
										argIter->second->setArgument("1");
								}
								else
								{
										std::cerr << "Argument: " << argIter->second << " requires a value" << std::endl;
										return false;
								}
						}
						else
						{
								std::cerr << "Argument: " << argStr << " was not found" << std::endl;
								return false;
						}
				}
				else
				{
						std::string argKey = argStr.substr(0, equalIdx);
						std::string argVal = argStr.substr(equalIdx + 1, argStr.size() - (equalIdx + 1));
						auto argIter = arguments.find(const_cast<char*>(argKey.c_str()));
						if (argIter != arguments.end())
						{
								if (argIter->second->usesStr())
								{
										argIter->second->setArgument(const_cast<char*>(argVal.c_str()));
								}
								else
								{
										std::cerr << "Argument: " << argIter->second << " does not take a value" << std::endl;
										return false;
								}
						}
						else
						{
								std::cerr << "Argument: " << argStr << " was not found" << std::endl;
								return false;
						}
				}
		}
		for (auto it = arguments.begin(); it != arguments.end(); it++)
		{
				if (!it->second->hasValue())
				{
						return false;
				}
		}
		return true;
}

std::string * CommandArgumentManager::getValue(char * key)
{
		auto it = arguments.find(key);
		if (it != arguments.end())
		{
				return it->second->getValue();
		}
		return nullptr;
}

CommandArgumentManager::~CommandArgumentManager()
{
		for (auto it = arguments.begin(); it != arguments.end(); it++)
		{
				delete it->second;
		}
		arguments.clear();
}
