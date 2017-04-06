#pragma once
#include "Message.h"
#include "ObjSerializable.h"
#include "TypeMap.h"
#include <string>
#include <unordered_map>
#include <functional>
#include <vector>

typedef std::function<void(Message*, std::ofstream&, int)> StatementFunction;

class Parser
{
public:
		const static std::vector <char> SEPARATE_WORD_CHARS;
		const static std::vector <char> IGNORE_CHARS;

		Parser();

		bool parse(const std::string& path);

		bool write(const std::string& msgOutPath, const std::string& templatePath, const std::string& templateObjPath, const std::string& fileEnding);

		bool convertTypes(TypeMap* typeMap);

		void addStatement(const std::string& key, StatementFunction statementHandler)
		{
				statements.emplace(std::make_pair(key, statementHandler));
		}

		std::vector <Message*> messages;

		~Parser();

private:
		bool handleStatement(const std::string& statement, Message* msg, std::ofstream& fileOut, int space);

		bool handleWord(const std::string& word, const std::string& comment);

		bool writeMessage(Message* msg, std::string msgInPath, const std::string& templatePath, const std::string& fileEnding);

		Message* activeMessage;

		Field* activeField;

		std::unordered_map <std::string, StatementFunction> statements;
};
