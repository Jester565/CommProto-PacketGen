#include "CsRunner.h"
#include <functional>

const std::string CsRunner::TEMPLATE_PATH = "./template.cs";

CsRunner::CsRunner(CommandArgumentManager * cmdArgManager)
		:Runner(cmdArgManager, TEMPLATE_PATH)
{
}

bool CsRunner::checkArguments()
{
		fileEnding = (*cmdArgManager->getValue("E"));
		if (fileEnding.empty())
		{
				fileEnding = "cs";
		}
		return Runner::checkArguments();
}

bool CsRunner::linkFields()
{
		typeMap->addType("uint8", "Byte", 1);
		typeMap->addType("uint16", "UInt16", 2);
		typeMap->addType("uint32", "UInt32", 4);
		typeMap->addType("uint64", "UInt64", 8);
		typeMap->addType("int8", "SByte", 1);
		typeMap->addType("int16", "Int16", 2);
		typeMap->addType("int32", "Int32", 4);
		typeMap->addType("int64", "Int64", 8);
		typeMap->addType("float", "Single", 4);
		typeMap->addType("double", "Double", 8);
		typeMap->addType("string", "String", 0);
		return true;
}

bool CsRunner::linkStatements()
{
		parser->addStatement("Constructor", std::bind(&CsRunner::HandleConstructor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("PackBody", std::bind(&CsRunner::HandlePackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("UnpackBody", std::bind(&CsRunner::HandleUnpackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Data", std::bind(&CsRunner::HandleData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return Runner::linkStatements();
}

CsRunner::~CsRunner()
{
}
