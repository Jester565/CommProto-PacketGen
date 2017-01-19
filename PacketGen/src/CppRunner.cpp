#include "CppRunner.h"
#include <functional>

const std::string CppRunner::TEMPLATE_FILE_PATH = "./template.h";

CppRunner::CppRunner(CommandArgumentManager* cmdArgManager)
		:Runner(cmdArgManager, TEMPLATE_FILE_PATH)
{
}

bool CppRunner::checkArguments()
{
		fileEnding = (*cmdArgManager->getValue("E"));
		if (fileEnding.empty())
		{
				fileEnding = "h";
		}
		return Runner::checkArguments();
}

bool CppRunner::linkFields()
{
		typeMap->addType("uint8", "uint8_t", 1);
		typeMap->addType("uint16", "uint16_t", 2);
		typeMap->addType("uint32", "uint32_t", 4);
		typeMap->addType("uint64", "uint64_t", 8);
		typeMap->addType("int8", "int8_t", 1);
		typeMap->addType("int16", "int16_t", 2);
		typeMap->addType("int32", "int32_t", 4);
		typeMap->addType("int64", "int64_t", 8);
		typeMap->addType("float", "real32_t", 4);
		typeMap->addType("double", "real64_t", 8);
		typeMap->addType("string", "std::string", 0);
		return true;
}

bool CppRunner::linkStatements()
{
		parser->addStatement("Define", std::bind(&CppRunner::HandleDefine, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Constructor", std::bind(&CppRunner::HandleConstructor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("PackBody", std::bind(&CppRunner::HandlePackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("UnpackBody", std::bind(&CppRunner::HandleUnpackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Data", std::bind(&CppRunner::HandleData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return Runner::linkStatements();
}

CppRunner::~CppRunner()
{
}
