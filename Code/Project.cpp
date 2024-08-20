#include <string>
#include <Engine/EngineError.h>
#include <thread>
#include <Sounds.h>

namespace Project
{
	// This is the name of the current project
	const char* ProjectName = "Robot Adventure";

	// This function returns the map that will be loaded when the game is opened
	std::string GetStartupScene()
	{
		return "Menu";
	}
	
	void OnLaunch()
	{
		Sounds::Load();
	}
}