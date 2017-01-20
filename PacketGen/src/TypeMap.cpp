#include "TypeMap.h"
#include "Parser.h"
#include "Message.h"

TypeMap::TypeMap()
{
}

void TypeMap::addType(const std::string & key, const std::string & typeName, int numBytes, const std::string& defaultValue)
{
		Type* type = new Type(typeName, numBytes, defaultValue);
		types.emplace(std::make_pair(key, type));
}

void TypeMap::addType(const std::string & packGenTypeName, int numBytes, const std::string & defaultVal)
{
		Type* type = new Type("PLACEHOLDER", numBytes, defaultVal);
		types.emplace(std::make_pair(packGenTypeName, type));
}

bool TypeMap::setLangTypeName(const std::string & packGenTypeName, const std::string & langTypeName)
{
		auto it = types.find(packGenTypeName);
		if (it != types.end())
		{
				it->second->name = langTypeName;
				return true;
		}
		return false;
}

bool TypeMap::convertType(Type * typePtr)
{
		auto it = types.find(typePtr->name);
		if (it != types.end())
		{
				typePtr->name = it->second->name;
				typePtr->numBytes = it->second->numBytes;
				typePtr->defaultVal = it->second->defaultVal;
				return true;
		}
		return false;
}

TypeMap::~TypeMap()
{
}
