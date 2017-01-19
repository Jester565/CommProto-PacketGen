#pragma once
#include <string>

class CommandArgument
{
public:
		CommandArgument(bool useStr, char* defaultValue);

		virtual bool usesStr();

		virtual bool hasValue();

		virtual std::string* getValue();

		virtual void setArgument(char* val);

		~CommandArgument();

private:
		std::string* value;

		bool useStr;
};

