#pragma once
#include "Runner.h"

class CppRunner : public Runner
{
public:
		void HandleDefine(Message* msg, std::ofstream& fileOut, int space)
		{
				std::string defineStr = "NGCP";
				for (uint32_t i = 0; i < msg->getName().size(); i++)
				{
						if (isupper(msg->getName().at(i)))
						{
								defineStr += "_";
						}
						defineStr += toupper(msg->getName().at(i));
				}
				defineStr += "_";
				for (uint32_t i = 0; i < fileEnding.size(); i++)
				{
						defineStr += toupper(fileEnding.at(i));
				}
				fileOut << defineStr;
		}

		void HandleInclude(Message* msg, std::ofstream& fileOut, int space)
		{
			for (int i = 0; i < msg->getFields().size(); i++) {
				if (msg->getFields().at(i)->custom) {
					fileOut << "#include \"" << msg->getFields().at(i)->type->name << "." << fileEnding << "\"";
					Runner::NewLine(fileOut, space);
				}
			}
		}

		void HandleConstructor(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << msg->getName() << "(";
				std::vector <Field*> orderedFields;
				for (int i = 0; i < msg->getFields().size(); i++) {
					if (!msg->getFields().at(i)->custom) {
						orderedFields.push_back(msg->getFields().at(i));
					}
				}
				int noDefaultArgI = 0;
				for (uint32_t i = 0; i < orderedFields.size(); i++)
				{
					if (orderedFields.at(i)->assignmentMode != Field::ASSIGN_MODE_SET_DEFAULT)
					{
							Field* temp = orderedFields.at(i);
							orderedFields.at(i) = orderedFields.at(noDefaultArgI);
							orderedFields.at(noDefaultArgI) = temp;
							noDefaultArgI++;
					}
				}
				for (uint32_t i = 0; i < orderedFields.size(); i++)
				{
					fileOut << orderedFields.at(i)->type->name << " " << *orderedFields.at(i)->name;
					if (orderedFields.at(i)->assignmentMode == Field::ASSIGN_MODE_SET_DEFAULT)
					{
							fileOut << " = ";
							fileOut << *orderedFields.at(i)->defaultArg;
					}
					if (i != orderedFields.size() - 1)
					{
							fileOut << ",";
							Runner::NewLine(fileOut, space + TAB_SPACE);
					}
				}
				fileOut << ")";
				Runner::NewLine(fileOut, space + TAB_SPACE);
				if (msg->getIsObj())
				{
					fileOut << ": CHAIN_OBJSERIALIZABLE";
				}
				else
				{
					fileOut << ": CHAIN_ABSPACKET(";
					fileOut << msg->getName();
					fileOut << ")";
				}
				for (uint32_t i = 0; i < orderedFields.size(); i++)
				{
					fileOut << ",";
					Runner::NewLine(fileOut, space + TAB_SPACE);
					fileOut << *orderedFields.at(i)->name << "(" << *orderedFields.at(i)->name << ")";
				}
				Runner::NewLine(fileOut, space);
				fileOut << "{";
				Runner::NewLine(fileOut, space);
				fileOut << "}";
		}

		void HandlePackBody(Message* msg, std::ofstream& fileOut, int space)
		{
			for (uint32_t i = 0; i < msg->getFields().size(); i++)
			{
				fileOut << "obj << ";
				fileOut << *msg->getFields().at(i)->name;
				fileOut << ";";
				if (i != msg->getFields().size() - 1)
				{
					Runner::NewLine(fileOut, space);
				}
			}
		}

	 void HandleUnpackBody(Message* msg, std::ofstream& fileOut, int space)
		{
			for (int i = msg->getFields().size() - 1; i >= 0; i--)
			{
				fileOut << "obj >> ";
				fileOut << *msg->getFields().at(i)->name;
				fileOut << ";";
				if (i != 0)
				{
					Runner::NewLine(fileOut, space);
				}
			}
		}

		void HandleData(Message* msg, std::ofstream& fileOut, int space)
		{
				for (uint32_t i = 0; i < msg->getFields().size(); i++)
				{
						if (msg->getFields().at(i)->comment != nullptr)
						{
								Runner::HandleSurroundComment(*msg->getFields().at(i)->comment, fileOut, space);
						}
						fileOut << msg->getFields().at(i)->type->name;
						fileOut << " ";
						fileOut << *msg->getFields().at(i)->name;
						fileOut << ";";
						if (i != msg->getFields().size() - 1)
						{
								Runner::NewLine(fileOut, space);
						}
				}
		}

		static const std::string DEFAULT_FILE_ENDING;
		static const std::string TEMPLATE_FILE_PATH;
		static const std::string OBJ_TEMPLATE_FILE_PATH;

		CppRunner(CommandArgumentManager* cmdArgManager);
		
		virtual bool checkArguments() override;

		virtual bool linkFields() override;

		virtual bool linkStatements() override;

		~CppRunner();
};

