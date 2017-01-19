#include "Message.h"



Message::Message()
		:name(nullptr), comment(nullptr), scopeStarted(false)
{
}


void Message::addField(Field * field)
{
		fields.push_back(field);
}

Message::~Message()
{
		if (name != nullptr)
		{
				delete name;
				name = nullptr;
		}
		if (comment != nullptr)
		{
				delete comment;
				comment = nullptr;
		}
		for (int i = 0; i < fields.size(); i++)
		{
				delete fields.at(i);
		}
		fields.clear();
}
