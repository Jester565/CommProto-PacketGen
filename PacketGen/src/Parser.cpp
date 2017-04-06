#include "Parser.h"
#include <iostream>
#include <fstream>

const std::vector <char> Parser::SEPARATE_WORD_CHARS({ '{', '}', '=', ';', '<', '>', ',' });
const std::vector <char> Parser::IGNORE_CHARS({ '\r', '\t' });

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
				for (int i = 0; i < IGNORE_CHARS.size(); i++)
				{
						if (c == IGNORE_CHARS.at(i))
						{
								c = ' ';
						}
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
						for (int i = 0; i < SEPARATE_WORD_CHARS.size(); i++)
						{
								if (c == SEPARATE_WORD_CHARS.at(i))
								{
										if (fileContents.back() != ' ' || fileContents.back() != '\n')
										{
												fileContents.push_back(' ');
										}
								}
								if (fileContents.back() == SEPARATE_WORD_CHARS.at(i))
								{
										fileContents.push_back(' ');
								}
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
			 fileContents.push_back(c);
		}
		fileContents.push_back(' ');
		inQuote = false;
		inComment = false;
		int lineNum = 1;
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
														std::cerr << "Line #" << lineNum << std::endl;
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
				if (c == '\n')
				{
						lineNum++;
				}
		}
		if (activeMessage != nullptr)
		{
				std::cerr << "Message \"" << activeMessage->getName() << "\" in file was not ended properly" << std::endl;
				return false;
		}
		return true;
}

bool Parser::write(const std::string & msgOutPath, const std::string & templatePath, const std::string& templateObjPath, const std::string & fileEnding)
{
		for (int i = 0; i < messages.size(); i++)
		{
			if (messages.at(i)->getIsObj()) {
				if (!writeMessage(messages.at(i), msgOutPath, templateObjPath, fileEnding))
				{
					return false;
				}
			}
			else
			{
				if (!writeMessage(messages.at(i), msgOutPath, templatePath, fileEnding))
				{
					return false;
				}
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
					if (!fields.at(j)->isCustomType()) {
						Type* typePtr = fields.at(j)->type;
						if (!typeMap->convertType(typePtr))
						{
							std::cerr << typePtr->name << " did not match a type. Field \"" << *fields.at(j)->name << "\" in message \"" << messages.at(i)->getName() << "\"" << std::endl;
							return false;
						}
						for (int k = 0; k < fields.at(j)->subTypes.size(); k++) {
							Type* subTypePtr = fields.at(j)->subTypes.at(k);
							if (!subTypePtr->custom) {
								if (subTypePtr->name.at(subTypePtr->name.size() - 1) == '*') {
									subTypePtr->name.pop_back();
									subTypePtr->isPtr = true;
								}
								if (!typeMap->convertType(subTypePtr)) {
									std::cerr << subTypePtr->name << " did not match a type. Field \"" << *fields.at(j)->name << "\" in message \"" << messages.at(i)->getName() << "\"" << std::endl;
									return false;
								}
							}
						}
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
				else if (word == "obj")
				{
					activeMessage = new ObjSerializable();
				}
				else
				{
						std::cerr << "Expecting \'message\' or\'obj\'" << std::endl;
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
						bool custom = false;
						std::string typeName = word;
						if (word.size() > 2 && word.at(0) == '#' && word.at(word.size() - 1) == '#') {
							typeName = word.substr(1, word.size() - 2);
							custom = true;
						}
						activeField->type = new Type(typeName, custom);
						if (!comment.empty())
						{
								activeField->comment = new std::string(comment);
						}
				}
				else if (activeField->name == nullptr)
				{
					if (word == "obj" || word == "id") {
						std::cout << word << " is not a legal type name. It may already be reserved for a parameter or field" << std::endl;
						return false;
					}
					activeField->name = new std::string(word);
				}
				else if (word == "<" && activeField->assignmentMode == Field::ASSIGN_MODE_NONE)
				{
					activeField->assignmentMode = Field::ASSIGN_MODE_SUB_TYPE;
				}
				else if (word == "=" && activeField->assignmentMode == Field::ASSIGN_MODE_NONE)
				{
					if (activeField->isCustomType()) {
						std::cerr << "= not allowed for custom types" << std::endl;
						return false;
					}
					activeField->assignmentMode = Field::ASSIGN_MODE_SET_DEFAULT;
				}
				else if (word == "~" && activeField->assignmentMode == Field::ASSIGN_MODE_NONE)
				{
					if (activeField->isCustomType()) {
						std::cerr << "~ not allowed for custom types" << std::endl;
						return false;
					}
					activeField->assignmentMode = Field::ASSIGN_MODE_CREATE_CONSTRUCT;
				}
				else if (activeField->assignmentMode != Field::ASSIGN_MODE_NONE)
				{
						if (word == ";")
						{
							if (activeField->assignmentMode == Field::ASSIGN_MODE_SUB_TYPE) {
								if (activeField->subTypes.size() == 0) {
									std::cerr << "No types were given" << std::endl;
									return false;
								}
							}
							else if (activeField->defaultArg == nullptr)
							{
								std::cerr << "No default argument was put after = or ~" << std::endl;;
								return false;
							}
							if (activeMessage->getIsObj() && activeField->assignmentMode == Field::ASSIGN_MODE_CREATE_CONSTRUCT) {
								std::cerr << "WARNING: A default constructor (constructor with no parameters) is required. It is recommended that you set all default values for parameters here using the = sign." << std::endl << std::endl;
							}
							activeMessage->addField(activeField);
							activeField = nullptr;
						}
						else if (activeField->assignmentMode == Field::ASSIGN_MODE_SUB_TYPE) {
							if (word != ">" && word != ",") {
								std::string typeName = word;
								bool custom = false;
								if (word.size() > 2 && word.at(0) == '#' && word.at(word.size() - 1) == '#') {
									typeName = word.substr(1, word.size() - 2);
									custom = true;
								}
								Type* subType = new Type(typeName, custom);
								activeField->subTypes.push_back(subType);
							}
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
					if (activeMessage->getIsObj()) {
						std::cerr << "WARNING: A default constructor (constructor with no parameters) is required. It is recommended that you set all default values for parameters here using the = sign." << std::endl << std::endl;
					}
					activeMessage->addField(activeField);
					activeField = nullptr;
				}
				else
				{
						std::cerr << "Was expecting = or ~ or ;" << std::endl;
						return false;
				}
		}
		else
		{
				if (activeField != nullptr)
				{
						std::cerr << "Field \"";
						if (activeField->type != nullptr)
						{
								std::cerr << activeField->type->name;
								std::cerr << " ";
						}
						if (activeField->name != nullptr)
						{
								std::cerr << activeField->name;
						}
						std::cerr << "\" in message \"" << activeMessage->getName() << "\" was not ended properly" << std::endl;
						return false;
				}
				messages.push_back(activeMessage);
				activeMessage = nullptr;
		}
		return true;
}


Parser::~Parser()
{
		for (int i = 0; i < messages.size(); i++)
		{
				delete messages.at(i);
		}
		messages.clear();
		if (activeMessage != nullptr)
		{
				delete activeMessage;
				activeMessage = nullptr;
		}
		if (activeField != nullptr)
		{
				delete activeField;
				activeField = nullptr;
		}
}
