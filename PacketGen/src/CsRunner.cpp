#include "CsRunner.h"
#include <functional>

const std::string CsRunner::TEMPLATE_PATH = "./template.cs";
const std::string CsRunner::OBJ_TEMPLATE_PATH = "./templateObj.cs";
const std::string CsRunner::DEFAULT_FILE_ENDING = "cs";

CsRunner::CsRunner(CommandArgumentManager * cmdArgManager)
		:Runner(cmdArgManager, TEMPLATE_PATH, OBJ_TEMPLATE_PATH)
{
}

bool CsRunner::checkArguments()
{
		fileEnding = (*cmdArgManager->getValue("E"));
		if (fileEnding.empty())
		{
				fileEnding = DEFAULT_FILE_ENDING;
		}
		return Runner::checkArguments();
}

bool CsRunner::linkFields()
{
		Runner::linkFields();
		typeMap->setLangTypeName(UINT8_PACKGEN_TYPE_NAME, "Byte");
		typeMap->setLangTypeName(UINT16_PACKGEN_TYPE_NAME, "UInt16");
		typeMap->setLangTypeName(UINT32_PACKGEN_TYPE_NAME, "UInt32");
		typeMap->setLangTypeName(UINT64_PACKGEN_TYPE_NAME, "UInt64");
		typeMap->setLangTypeName(INT8_PACKGEN_TYPE_NAME, "SByte");
		typeMap->setLangTypeName(INT16_PACKGEN_TYPE_NAME, "Int16");
		typeMap->setLangTypeName(INT32_PACKGEN_TYPE_NAME, "Int32");
		typeMap->setLangTypeName(INT64_PACKGEN_TYPE_NAME, "Int64");
		typeMap->setLangTypeName(FLOAT_PACKGEN_TYPE_NAME, "Single");
		typeMap->setLangTypeName(DOUBLE_PACKGEN_TYPE_NAME, "Double");
		typeMap->setLangTypeName(STRING_PACKGEN_TYPE_NAME, "String");
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
