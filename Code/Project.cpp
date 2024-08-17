#include <string>

namespace Project
{
	// This is the name of the current project
	const char* ProjectName = "GMTK 2024";

	// This function returns the map that will be loaded when the game is opened
	std::string GetStartupScene()
	{
		return "Untitled";
	}
	
	// This function will be called on startup.
	void OnLaunch()
	{
	}
}