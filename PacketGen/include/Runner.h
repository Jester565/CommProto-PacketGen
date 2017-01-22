#pragma once
#include "TypeMap.h"
#include "Parser.h"
#include "CommandArgumentManager.h"
#include <fstream>
#include <string>

static const int TAB_SPACE = 2;

const std::string UINT8_PACKGEN_TYPE_NAME = "uint8";
const std::string UINT16_PACKGEN_TYPE_NAME = "uint16";
const std::string UINT32_PACKGEN_TYPE_NAME = "uint32";
const std::string UINT64_PACKGEN_TYPE_NAME = "uint64";
const std::string INT8_PACKGEN_TYPE_NAME = "int8";
const std::string INT16_PACKGEN_TYPE_NAME = "int16";
const std::string INT32_PACKGEN_TYPE_NAME = "int32";
const std::string INT64_PACKGEN_TYPE_NAME = "int64";
const std::string FLOAT_PACKGEN_TYPE_NAME = "float";
const std::string DOUBLE_PACKGEN_TYPE_NAME = "double";
const std::string STRING_PACKGEN_TYPE_NAME = "string";

class Runner
{
public:
		void NewLine(std::ofstream& fileOut, int space)
		{
				fileOut << "\n";
				for (int i = 0; i < space; i++)
				{
						fileOut << " ";
				}
		}

		void HandleSurroundComment(const std::string& comment, std::ofstream& fileOut, int space)
		{
				if (comment.find('\n') == std::string::npos)
				{
						fileOut << "//";
						HandlePureComment(comment, fileOut, space);
				}
				else
				{
						fileOut << "/*";
						HandlePureComment(comment, fileOut, space);
						fileOut << "*/";
				}
				Runner::NewLine(fileOut, space);
		}

		void HandlePureComment(const std::string& comment, std::ofstream& fileOut, int space)
		{
				bool inTabZone = true;
				for (uint32_t i = 0; i < comment.size(); i++)
				{
						if (comment.at(i) == '\n')
						{
								Runner::NewLine(fileOut, space);
								inTabZone = true;
						}
						else
						{
								if (inTabZone && comment.at(i) != ' ' && comment.at(i) != '\t')
								{
										inTabZone = false;
								}
								if (!inTabZone)
								{
										fileOut << comment.at(i);
								}
						}
				}
		}

		void HandleMsgComment(Message* msg, std::ofstream& fileOut, int space)
		{
				if (msg->getComment() == nullptr)
				{
						fileOut << msg->getName();
				}
				else
				{
						HandlePureComment(*msg->getComment(), fileOut, space);
				}
		}

		void HandleCreateConstruct(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << msg->getName();
				fileOut << "(";
				bool first = true;
				for (uint32_t i = 0; i < msg->getFields().size(); i++)
				{
						if (msg->getFields().at(i)->assignmentMode != Field::ASSIGN_MODE_SET_DEFAULT)
						{
								if (!first)
								{
										fileOut << ", ";
								}
								else
								{
										first = false;
								}
								if (msg->getFields().at(i)->assignmentMode == Field::ASSIGN_MODE_CREATE_CONSTRUCT)
								{
										fileOut << *msg->getFields().at(i)->defaultArg;
								}
								else
								{
										fileOut << msg->getFields().at(i)->type->defaultVal;
								}
						}
				}
				fileOut << ")";
		}

		void HandleName(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << msg->getName();
		}

		Runner(CommandArgumentManager* cmdArgManager, const std::string& templatePathStr);

		virtual bool run(int argc, char* argv[]);

		virtual ~Runner();

protected:
		virtual bool checkArguments();
		virtual bool parse();
		virtual bool linkFields();
		virtual bool convertFields();
		virtual bool linkStatements();
		virtual bool write();

		CommandArgumentManager* cmdArgManager;
		TypeMap* typeMap;
		Parser* parser;

		std::string fileEnding;
		std::string msgInPath;
		std::string msgOutPath;
		std::string templatePath;
};

