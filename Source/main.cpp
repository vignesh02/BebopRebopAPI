// Includes
#include "CNetworkInterface.h"
#include "Utility.hpp"

// Set up easylogger
INITIALIZE_EASYLOGGINGPP

// Namespaces
using namespace rebop;

int main()
{
	CBebopInterface bebop;

	// Initialize
	bebop.Initialize();

	// You're connected and ready to fly! Do stuff here!
	bebop.Update();

	// Cleanup - Kill the network and clean up memory
	bebop.Cleanup();

	return 0;
}
