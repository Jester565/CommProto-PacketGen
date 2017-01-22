#include "Runner.h"

class DiagramManager;

class DiagramRunner : public Runner
{
public:
		static const std::string DEFAULT_FILE_ENDING;

		DiagramRunner(CommandArgumentManager* cmdArgManager);

		bool run(int argc, char* argv[]) override;

		~DiagramRunner();

protected:
		bool checkArguments() override;

		bool write() override;

		DiagramManager* diagramManager;
};
