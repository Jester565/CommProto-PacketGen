#pragma once
#include "TypeMap.h"
#include "Parser.h"
#include "CommandArgumentManager.h"
#include <fstream>
#include <string>

static const int TAB_SPACE = 2;

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
				for (int i = 0; i < comment.size(); i++)
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

		void HandleName(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << msg->getName();
		}

		Runner(CommandArgumentManager* cmdArgManager, const std::string& templatePathStr);

		bool run(int argc, char* argv[]);

		virtual ~Runner();

protected:
		virtual bool checkArguments();
		virtual bool parse();
		virtual bool linkFields() = 0;
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

