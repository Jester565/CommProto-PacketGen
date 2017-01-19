#pragma once
#include "Message.h"
#include "TypeMap.h"
#include <string>
#include <unordered_map>
#include <functional>

typedef std::function<void(Message*, std::ofstream&, int)> StatementFunction;

class Parser
{
public:
		Parser();

		bool parse(const std::string& path);

		bool write(const std::string& msgOutPath, const std::string& templatePath, const std::string& fileEnding);

		bool convertTypes(TypeMap* typeMap);

		void addStatement(const std::string& key, StatementFunction statementHandler)
		{
				statements.emplace(std::make_pair(key, statementHandler));
		}

		~Parser();

private:
		bool handleStatement(const std::string& statement, Message* msg, std::ofstream& fileOut, int space);

		bool handleWord(const std::string& word, const std::string& comment);

		bool writeMessage(Message* msg, std::string msgInPath, const std::string& templatePath, const std::string& fileEnding);

		Parser* parser;

		Message* activeMessage;

		Field* activeField;

		std::vector <Message*> messages;

		std::unordered_map <std::string, StatementFunction> statements;
};
