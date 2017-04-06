#include "CppRunner.h"
#include <functional>

const std::string CppRunner::TEMPLATE_FILE_PATH = "./template.h";
const std::string CppRunner::OBJ_TEMPLATE_FILE_PATH = "./templateObj.h";
const std::string CppRunner::DEFAULT_FILE_ENDING = "h";


CppRunner::CppRunner(CommandArgumentManager* cmdArgManager)
		:Runner(cmdArgManager, TEMPLATE_FILE_PATH, OBJ_TEMPLATE_FILE_PATH)
{
}

bool CppRunner::checkArguments()
{
		fileEnding = (*cmdArgManager->getValue("E"));
		if (fileEnding.empty())
		{
				fileEnding = DEFAULT_FILE_ENDING;
		}
		return Runner::checkArguments();
}

bool CppRunner::linkFields()
{
		Runner::linkFields();
		typeMap->setLangTypeName(UINT8_PACKGEN_TYPE_NAME, "uint8_t");
		typeMap->setLangTypeName(UINT16_PACKGEN_TYPE_NAME, "uint16_t");
		typeMap->setLangTypeName(UINT32_PACKGEN_TYPE_NAME, "uint32_t");
		typeMap->setLangTypeName(UINT64_PACKGEN_TYPE_NAME, "uint64_t");
		typeMap->setLangTypeName(INT8_PACKGEN_TYPE_NAME, "int8_t");
		typeMap->setLangTypeName(INT16_PACKGEN_TYPE_NAME, "int16_t");
		typeMap->setLangTypeName(INT32_PACKGEN_TYPE_NAME, "int32_t");
		typeMap->setLangTypeName(INT64_PACKGEN_TYPE_NAME, "int64_t");
		typeMap->setLangTypeName(FLOAT_PACKGEN_TYPE_NAME, "real32_t");
		typeMap->setLangTypeName(DOUBLE_PACKGEN_TYPE_NAME, "real64_t");
		typeMap->setLangTypeName(STRING_PACKGEN_TYPE_NAME, "std::string");
		return true;
}

bool CppRunner::linkStatements()
{
		parser->addStatement("Define", std::bind(&CppRunner::HandleDefine, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Include", std::bind(&CppRunner::HandleInclude, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Constructor", std::bind(&CppRunner::HandleConstructor, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("PackBody", std::bind(&CppRunner::HandlePackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("UnpackBody", std::bind(&CppRunner::HandleUnpackBody, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		parser->addStatement("Data", std::bind(&CppRunner::HandleData, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
		return Runner::linkStatements();
}

CppRunner::~CppRunner()
{
}
