#include <iostream>
#include <string>
#include <stdio.h>

#include "Runner.h"
#include "CppRunner.h"
#include "CsRunner.h"
#include "CommandArgumentManager.h"
#include "Parser.h"

#ifdef DIAGRAM
#include "DiagramRunner.h"
#endif

bool readArguments(CommandArgumentManager* cmdArgManager, int argc, char* argv[])
{
		cmdArgManager->addArgument("I", "Specify the ngen input file. This is a required argument.\nExample: -I=\"C:\\Users\\user\\Desktop\\example.ngen\"", true);
		cmdArgManager->addArgument("O", "Specify the directory to output generated files. If not specified, files will be outputed to current directory.\nExample: -O=\"C:\\Users\\user\\Desktop\\generated", true, "./");
#ifdef DIAGRAM
		cmdArgManager->addArgument("L", "Specify the language/form the generated files will be. \"cs\" for c#, \"cpp\" for c++, \"diag\" for byte diagrams. This is a required argument.\nExample: -L=cpp", true);
#else
		cmdArgManager->addArgument("L", "Specify the language/form the generated files will be. \"cs\" for c#, \"cpp\" for c++. This is a required argument.\nExample: -L=cpp", true);
#endif
		cmdArgManager->addArgument("E", "Specify the file ending. If not specified the most popular file ending for the language/form will be used\nExample: -E=hpp", true, "");
		cmdArgManager->addArgument("H", "Access command manual\nExample: -H", false);
		if (!cmdArgManager->readArgs(argc, argv))
		{
				std::cerr << "Arguments failed terminating..." << std::endl;
				return false;
		}
		return true;
}

int run(int argc, char* argv[])
{
		Runner* runner = nullptr;
		CommandArgumentManager* cmdArgManager = new CommandArgumentManager();
		bool cmdSuccess = readArguments(cmdArgManager, argc, argv);
		if (cmdArgManager->getValue("H") != nullptr)
		{
				cmdArgManager->printManual();
		}
		if (!cmdSuccess)
		{
				delete cmdArgManager;
				cmdArgManager = nullptr;
				return 1;
		}
		std::string lang = *cmdArgManager->getValue("L");
		if (lang == "cpp")
		{
				runner = new CppRunner(cmdArgManager);
		}
		else if (lang == "cs")
		{
				runner = new CsRunner(cmdArgManager);
		}
#ifdef DIAGRAM
		else if (lang == "diag")
		{
				runner = new DiagramRunner(cmdArgManager);
		}
#endif
		else
		{
				std::cerr << lang << " is not a language" << std::endl;
				delete cmdArgManager;
				cmdArgManager = nullptr;
				delete runner;
				runner = nullptr;
				return 1;
		}
		if (!runner->run(argc, argv))
		{
				delete cmdArgManager;
				cmdArgManager = nullptr;
				delete runner;
				runner = nullptr;
				return 1;
		}
		delete cmdArgManager;
		cmdArgManager = nullptr;
		delete runner;
		runner = nullptr;
		return 0;
}

int main(int argc, char *argv[])
{
		if (run(argc, argv) == 0)
		{
				std::cout << "Finished" << std::endl;
		}
		else
		{
				std::cout << "Failure" << std::endl;
		}
		system("pause");
}