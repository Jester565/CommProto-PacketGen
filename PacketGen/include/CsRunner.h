#pragma once
#include "Runner.h"

class CsRunner : public Runner
{
public:
		void HandleConstructor(Message* msg, std::ofstream& fileOut, int space)
		{
				fileOut << "public ";
				fileOut << msg->getName() << "(";
				std::vector <Field*> orderedFields(msg->getFields());
				int noDefaultArgI = 0;
				for (int i = 0; i < orderedFields.size(); i++)
				{
						if (orderedFields.at(i)->assignmentMode != Field::ASSIGN_MODE_SET_DEFAULT)
						{
								Field* temp = orderedFields.at(i);
								orderedFields.at(i) = orderedFields.at(noDefaultArgI);
								orderedFields.at(noDefaultArgI) = temp;
								noDefaultArgI++;
						}
				}
				for (int i = 0; i < orderedFields.size(); i++)
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
				fileOut << ": base(\"";
				fileOut << msg->getName();
				fileOut << "\")";
				Runner::NewLine(fileOut, space);
				fileOut << "{";
				for (int i = 0; i < orderedFields.size(); i++)
				{
						Runner::NewLine(fileOut, space + TAB_SPACE);
						fileOut << "this." << *orderedFields.at(i)->name << " = " << *orderedFields.at(i)->name << ";";
				}
				Runner::NewLine(fileOut, space);
				fileOut << "}";
		}

		void HandlePackBody(Message* msg, std::ofstream& fileOut, int space)
		{
				for (int i = 0; i < msg->getFields().size(); i++)
				{
						fileOut << "obj.Input(";
						fileOut << *msg->getFields().at(i)->name;
						fileOut << ");";
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
						fileOut << *msg->getFields().at(i)->name;
						fileOut << " = obj.Output";
						fileOut << msg->getFields().at(i)->type->name;
						fileOut << "();";
						if (i != 0)
						{
								Runner::NewLine(fileOut, space);
						}
				}
		}

		void HandleData(Message* msg, std::ofstream& fileOut, int space)
		{
				for (int i = 0; i < msg->getFields().size(); i++)
				{
						if (msg->getFields().at(i)->comment != nullptr)
						{
								Runner::HandleSurroundComment(*msg->getFields().at(i)->comment, fileOut, space);
						}
						fileOut << "public ";
						fileOut << msg->getFields().at(i)->type->name;
						fileOut << " ";
						fileOut << *msg->getFields().at(i)->name;
						fileOut << " { get; set; }";
						if (i != msg->getFields().size() - 1)
						{
								Runner::NewLine(fileOut, space);
						}
				}
		}

		static const std::string TEMPLATE_PATH;
		static const std::string DEFAULT_FILE_ENDING;

		CsRunner(CommandArgumentManager* cmdArgManager);

		virtual bool checkArguments() override;

		virtual bool linkFields() override;

		virtual bool linkStatements() override;

		~CsRunner();
};

