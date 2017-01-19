#include "Parser.h"
#include <iostream>
#include <fstream>

Parser::Parser()
		:activeMessage(nullptr), activeField(nullptr)
{
}

bool Parser::parse(const std::string& path)
{
		std::ifstream ifs(path);
		std::string content((std::istreambuf_iterator<char>(ifs)),
				(std::istreambuf_iterator<char>()));

		std::string fileContents(" ");

		bool inComment = false;
		bool inQuote = false;
		char endChar;
		for (int i = 0; i < content.size(); i++)
		{
				char c = content.at(i);
				if (c == '\t')
				{
						c = ' ';
				}
				if (!inQuote && !inComment)
				{
						if (c == '\'' || c == '\"')
						{
								inQuote = true;
								endChar = c;
						}
						if (c == '/' && fileContents.back() == '/')
						{
								fileContents.pop_back();
								fileContents.push_back(' ');
								fileContents.push_back('/');
								inComment = true;
								endChar = '\n';
						}
						if (c == '*' && fileContents.back() == '/')
						{
								fileContents.pop_back();
								fileContents.push_back(' ');
								fileContents.push_back('/');
								inComment = true;
								endChar = '/';
						}
						if (c == '{' || c == '}' || c == ';' || c == '=')
						{
								if (fileContents.back() != ' ' || fileContents.back() != '\n')
								{
										fileContents.push_back(' ');
								}
						}
						if ((c != ' ' || c != '\n') && (fileContents.back() == '{' || fileContents.back() == ';' || fileContents.back() == '}' || fileContents.back() == '='))
						{
								fileContents.push_back(' ');
						}
				}
				else if (inQuote)
				{
						if (c == endChar && fileContents.back() != '\\')
						{
								inQuote = false;
						}
				}
				else
				{
						if (endChar == c && (c == '\n' || fileContents.back() == '*'))
						{
								inComment = false;
						}
				}
				if (c != '\r')
				{
					fileContents.push_back(c);
				}
		}
		fileContents.push_back(' ');
		inQuote = false;
		inComment = false;
		std::string comment;
		std::string word;
		for (int i = 0; i < fileContents.size(); i++)
		{
				char c = fileContents.at(i);
				
				if (!inQuote && !inComment)
				{
						if (c == '\"' || c == '\'')
						{
								inQuote = true;
								endChar = c;
								word += c;
						}
						else if (c == '/' && fileContents.at(i - 1) == '/')
						{
								inComment = true;
								endChar = '\n';
						}
						else if (c == '*' && fileContents.at(i - 1) == '/')
						{
								inComment = true;
								endChar = '/';
						}
						else
						{
								if (c == '\n' || c == ' ')
								{
										if (word.size() > 0)
										{
												if (!handleWord(word, comment))
												{
														return false;
												}
												word.clear();
												comment.clear();
										}
								}
								else
								{
										word += c;
								}
						}
				}
				else if (inQuote)
				{
						if (c == endChar && fileContents.at(i - 1) != '\\')
						{
								inQuote = false;
						}
						word += c;
				}
				else
				{
						if (endChar == c && (endChar == '\n' || fileContents.at(i - 1) == '*'))
						{
								if (endChar != '\n')
								{
										comment.pop_back();
								}
								inComment = false;
								word.clear();
						}
						else
						{
								comment += c;
						}
				}
		}
		if (activeMessage != nullptr)
		{
				std::cerr << "Message in file was not ended properly" << std::endl;
				return false;
		}
		return true;
}

bool Parser::write(const std::string & msgOutPath, const std::string & templatePath, const std::string & fileEnding)
{
		for (int i = 0; i < messages.size(); i++)
		{
				if (!writeMessage(messages.at(i), msgOutPath, templatePath, fileEnding))
				{
						return false;
				}
		}
		return true;
}

bool Parser::convertTypes(TypeMap * typeMap)
{
		for (int i = 0; i < messages.size(); i++)
		{
				std::vector <Field*>& fields = messages.at(i)->getFields();
				for (int j = 0; j < fields.size(); j++)
				{
						Type* typePtr = fields.at(j)->type;
						if (!typeMap->convertType(typePtr))
						{
								std::cerr << typePtr->name << " did not match a type" << std::endl;
								return false;
						}
				}
		}
		return true;
}

bool Parser::writeMessage(Message* msg, std::string msgOutPath, const std::string& templatePath, const std::string& fileEnding)
{
		msgOutPath += "/";
		msgOutPath += msg->getName();
		msgOutPath += ".";
		msgOutPath += fileEnding;
		std::ofstream fileOut(msgOutPath);
		std::ifstream fileIn(templatePath);
		bool countSpaces = true;
		int spaceCount = 0;
		bool inStatement = false;
		std::string statement;
		if (fileOut.is_open() && fileIn.is_open())
		{
				while (!fileIn.eof())
				{
						char c;
						fileIn.get(c);
						if (inStatement)
						{
								if (c == '^')
								{
										if (!handleStatement(statement, msg, fileOut, spaceCount))
										{
												return false;
										}
										inStatement = false;
										statement.clear();
								}
								else
								{
										statement += c;
								}
						}
						else if (c == '^')
						{
								inStatement = true;
						}
						else
						{
								if (c == '\n')
								{
										spaceCount = 0;
										countSpaces = true;
								}
								else if (c == ' ')
								{
										if (countSpaces) {
												spaceCount++;
										}
								}
								else
								{
										countSpaces = false;
								}
								fileOut << c;
						}
				}
		}
		return true;
}

bool Parser::handleStatement(const std::string & statement, Message * msg, std::ofstream & fileOut, int space)
{
		auto it = statements.find(statement);
		if (it != statements.end())
		{
				it->second(msg, fileOut, space);
				return true;
		}
		return false;
}

bool Parser::handleWord(const std::string& word, const std::string& comment)
{
		if (activeMessage == nullptr)
		{
				if (word == "message")
				{
						activeMessage = new Message();
						if (!comment.empty())
						{
								activeMessage->setComment(comment);
						}
				}
				else
				{
						std::cerr << "Expecting \'message\'" << std::endl;
						return false;
				}
		}
		else if (!activeMessage->hasName())
		{
				activeMessage->setName(word);
		}
		else if (!activeMessage->hasScopeStarted())
		{
				if (word == "{")
				{
						activeMessage->setScopeStarted();
						return true;
				}
				else
				{
						std::cerr << "Expecting a \'{\'" << std::endl;
						return false;
				}
		}
		else if (word != "}")
		{
				if (activeField == nullptr)
				{
						activeField = new Field();
						activeField->type = new Type(word);
						if (!comment.empty())
						{
								activeField->comment = new std::string(comment);
						}
				}
				else if (activeField->name == nullptr)
				{
						activeField->name = new std::string(word);
				}
				else if (word == "=")
				{
						activeField->expectingDefaultArg = true;
				}
				else if (activeField->expectingDefaultArg)
				{
						if (word == ";")
						{
								if (activeField->defaultArg == nullptr)
								{
										std::cerr << "No default argument was put after =" << std::endl;;
										return false;
								}
								activeMessage->addField(activeField);
								activeField = nullptr;
						}
						else
						{
								if (activeField->defaultArg == nullptr)
								{
										activeField->defaultArg = new std::string(word);
								}
								else
								{
										activeField->defaultArg->append(" ");
										activeField->defaultArg->append(word);
								}
						}
				}
				else if (word == ";")
				{
						activeMessage->addField(activeField);
						activeField = nullptr;
				}
				else
				{
						std::cerr << "Was expecting = or ;" << std::endl;
						return false;
				}
		}
		else
		{
				messages.push_back(activeMessage);
				activeMessage = nullptr;
		}
		return true;
}


Parser::~Parser()
{
}
