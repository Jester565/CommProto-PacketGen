#include "CommandArgument.h"

CommandArgument::CommandArgument(bool usesStr, char * defaultValue)
		:value(nullptr), useStr(usesStr)
{
		if (defaultValue != nullptr)
		{
				value = new std::string(defaultValue);
		}
}

bool CommandArgument::usesStr()
{
		return useStr;
}

bool CommandArgument::hasValue()
{
		return value != nullptr;
}

std::string* CommandArgument::getValue()
{
		return value;
}

void CommandArgument::setArgument(char* val)
{
		if (value != nullptr)
		{
				delete value;
				value = nullptr;
		}
		value = new std::string(val);
}

CommandArgument::~CommandArgument()
{
		delete value;
		value = nullptr;
}
