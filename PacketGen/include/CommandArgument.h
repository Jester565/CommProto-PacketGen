#pragma once
#include <string>

class CommandArgument
{
public:
		CommandArgument(bool useStr, char* defaultValue, const std::string& description);

		virtual bool usesStr();

		virtual bool hasValue();

		virtual std::string* getValue();

		virtual void setArgument(char* val);

		std::string getDescription()
		{
				return description;
		}

		~CommandArgument();

private:
		std::string* value;
		std::string description;
		bool useStr;
};

