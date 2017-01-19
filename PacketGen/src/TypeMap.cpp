#include "TypeMap.h"
#include "Parser.h"


TypeMap::TypeMap()
{
}

void TypeMap::addType(const std::string & key, const std::string & val, int numBytes)
{
		types.emplace(std::make_pair(key, std::make_pair(val, numBytes)));
}

bool TypeMap::convertType(Type * typePtr)
{
		auto it = types.find(typePtr->name);
		if (it != types.end())
		{
				typePtr->name = it->second.first;
				typePtr->numBytes = it->second.second;
				return true;
		}
		return false;
}

TypeMap::~TypeMap()
{
}
