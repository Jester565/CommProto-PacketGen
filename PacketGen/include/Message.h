#pragma once
#include <vector>
#include <string>

struct Type
{
		Type(const std::string& name)
				:name(name), numBytes(0)
		{

		}
		std::string name;
		uint8_t numBytes;
};

struct Field
{
		Field()
				:type(nullptr), name(nullptr), defaultArg(nullptr), comment(nullptr), expectingDefaultArg(false)
		{
				
		}

		Type* type;
		std::string* name;
		std::string* comment;
		std::string* defaultArg;
		bool expectingDefaultArg;

		~Field()
		{
				if (type != nullptr) {
						delete type;
						type = nullptr;
				}
				if (name != nullptr) {
						delete name;
						name = nullptr;
				}
				if (comment != nullptr) {
						delete comment;
						comment = nullptr;
				}
				if (defaultArg != nullptr) {
						delete defaultArg;
						defaultArg = nullptr;
				}
		}
};

class Message
{
public:
		Message();

		bool hasName()
		{
				return name != nullptr;
		}

		std::string getName()
		{
				return *name;
		}

		void setName(const std::string& nameStr)
		{
				name = new std::string(nameStr);
		}

		void setComment(const std::string& commentStr)
		{
				comment = new std::string(commentStr);
		}

		std::string* getComment()
		{
				return comment;
		}

		bool hasScopeStarted()
		{
				return scopeStarted;
		}

		void setScopeStarted()
		{
				scopeStarted = true;
		}

		void addField(Field* field);

		std::vector<Field*>& getFields()
		{
				return fields;
		}

		~Message();

private:
		bool scopeStarted;
		std::string* comment;
		std::string* name;
		std::vector<Field*> fields;
};

