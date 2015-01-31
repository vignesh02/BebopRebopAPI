#include "CBebopInterface.h"

#include "Utility.hpp"

// Set up easylogger
INITIALIZE_EASYLOGGINGPP


using namespace bebop;

int main()
{
	CBebopInterface bebopInterface;

	// Initialize the interface - Connects to the drone and sets up everything necessary for communications
	if( !bebopInterface.Initialize() )
	{
		return 0;
	}

	// You're connected and ready to fly! Do stuff here!

	// Cleanup - Kill the network and clean up memory
	bebopInterface.Cleanup();

	return 0;
}
