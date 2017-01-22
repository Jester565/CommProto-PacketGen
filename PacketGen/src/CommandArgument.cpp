#include "CommandArgument.h"

CommandArgument::CommandArgument(bool usesStr, char * defaultValue, const std::string& description)
		:description(description), useStr(usesStr), value(nullptr)
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
		if (value != nullptr)
		{
				delete value;
				value = nullptr;
		}
}
