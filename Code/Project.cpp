#include <string>
#include <Engine/EngineError.h>
#include <thread>

namespace Project
{
	// This is the name of the current project
	const char* ProjectName = "GMTK 2024";

	// This function returns the map that will be loaded when the game is opened
	std::string GetStartupScene()
	{
		return "Untitled";
	}
	
	void OnLaunch()
	{
		//ENGINE_ASSERT(std::thread::hardware_concurrency() >= 32, "At least 32 threads are required to run this game");
	}
}