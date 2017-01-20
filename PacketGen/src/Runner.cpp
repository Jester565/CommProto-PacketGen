#include "Runner.h"
#include "FileManager.h"
#include <functional>
#include <iostream>

Runner::Runner(CommandArgumentManager* cmdArgManager, const std::string& templatePathStr)
		:cmdArgManager(cmdArgManager), templatePath(templatePathStr)
{
		parser = new Parser();
		typeMap = new TypeMap();
}

bool Runner::run(int argc, char* argv[])
{
		if (!checkArguments())
		{
				return false;
		}
		if (!parse())
		{
				return false;
		}
		if (!linkFields())
		{
				return false;
		}
		if (!convertFields())
		{
				return false;
		}
		if (!linkStatements())
		{
				return false;
		}
		if (!write())
		{
				return false;
		}
		return true;
}


Runner::~Runner()
{
		delete parser;
		parser = nullptr;
		delete typeMap;
		typeMap = nullptr;
}

bool Runner::checkArguments()
{
		msgInPath = *cmdArgManager->getValue("I");
		for (int i = 0; i < msgInPath.size(); i++)
			{
				if (msgInPath.at(i) == '\\')
					{
						msgInPath.at(i) = '/';
					}
			}
		if (!FileManager::IsFile(msgInPath))
		{
				std::cerr << "Input path does not exist" << std::endl;
				return false;
		}

		if (!FileManager::IsFile(templatePath))
		{
				templatePath = "." + templatePath;
				if (!FileManager::IsFile(templatePath))
				{
						int slashIdx = templatePath.find('/');
						slashIdx++;
						if (slashIdx >= templatePath.size())
						{
								slashIdx = 0;
						}
						std::cerr << "Template file does not exist: the file \"" << templatePath.substr(slashIdx, templatePath.size() - slashIdx) << 
								"\" must be in the same directory or the parent directory of the executable" << std::endl;
						return false;
				}
		}

		msgOutPath = *cmdArgManager->getValue("O");
		if (msgOutPath.size() > 0)
		{
				for (int i = 0; i < msgOutPath.size(); i++)
				{
						if (msgOutPath.at(i) == '\\')
						{
								msgOutPath.at(i) = '/';
						}
				}
				if (msgOutPath.back() == '/')
				{
						msgOutPath.pop_back();
				}
		}
		if (!FileManager::IsDirectory(msgOutPath))
		{
				std::cerr << "Output path does not exist" << std::endl;
				return false;
		}
		return true;
}

bool Runner::parse()
{
		if (!parser->parse(msgInPath))
		{
				std::cerr << "Parsing failed" << std::endl;
				return false;
		}
		return true;
}

bool Runner::linkFields()
{
		typeMap->addType(UINT8_PACKGEN_TYPE_NAME, 1, "0");
		typeMap->addType(UINT16_PACKGEN_TYPE_NAME, 2, "0");
		typeMap->addType(UINT32_PACKGEN_TYPE_NAME, 4, "0");
		typeMap->addType(UINT64_PACKGEN_TYPE_NAME, 8, "0");
		typeMap->addType(INT8_PACKGEN_TYPE_NAME, 1, "0");
		typeMap->addType(INT16_PACKGEN_TYPE_NAME, 2, "0");
		typeMap->addType(INT32_PACKGEN_TYPE_NAME, 4, "0");
		typeMap->addType(INT64_PACKGEN_TYPE_NAME, 8, "0");
		typeMap->addType(FLOAT_PACKGEN_TYPE_NAME, 4, "0");
		typeMap->addType(DOUBLE_PACKGEN_TYPE_NAME, 8, "0");
		typeMap->addType(STRING_PACKGEN_TYPE_NAME, 0, "\"\"");
		return true;
}

bool Runner::convertFields()
{
		if (!parser->convertTypes(typeMap))
		{
				std::cerr << "Convert fields failed" << std::endl;
				return false;
		}
		return true;
}

bool Runner::linkStatements()
{
		parser->addStatement("MsgComment", std::bind(&Runner::HandleMsgComment, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Name", std::bind(&Runner::HandleName, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("CreateConstruct", std::bind(&Runner::HandleCreateConstruct, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return true;
}

bool Runner::write()
{
		if (!parser->write(msgOutPath, templatePath, fileEnding))
		{
				std::cerr << "Writing failed" << std::endl;
		}
		return true;
}
