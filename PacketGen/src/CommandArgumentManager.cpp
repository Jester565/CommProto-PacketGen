#include "CommandArgumentManager.h"
#include <iostream>

CommandArgumentManager::CommandArgumentManager()
{
}

void CommandArgumentManager::addArgument(char * key, const std::string& description, bool usesStr, char * defaultValue)
{
		arguments.emplace(std::make_pair(std::string(key), new CommandArgument(usesStr, defaultValue, description)));
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
		bool success = true;
		for (auto it = arguments.begin(); it != arguments.end(); it++)
		{
				if (!it->second->hasValue() && it->second->usesStr())
				{
						if (getValue("H") == nullptr)
						{
								std::cerr << "Missing essential command argument " << it->first << "  " << it->second->getDescription() << std::endl << std::endl;
						}
						success = false;
				}
		}
		return success;
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

void CommandArgumentManager::printManual()
{
		std::cout << "Command Manual" << std::endl;
		for (auto it = arguments.begin(); it != arguments.end(); it++)
		{
				std::cout << it->first;
				for (int i = 0; i < 5; i++)
				{
						std::cout << " ";
				}
				std::cout << it->second->getDescription() << std::endl << std::endl;
		}
}

CommandArgumentManager::~CommandArgumentManager()
{
		for (auto it = arguments.begin(); it != arguments.end(); it++)
		{
				delete it->second;
		}
		arguments.clear();
}
