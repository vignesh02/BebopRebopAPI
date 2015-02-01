// Includes
#include "CBebopInterface.h"
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

	// Connected and ready to do stuff, theoretically
	if( bebop.IsConnected() == false )
	{
		LOG( ERROR ) << "No connection! Can't do anything!";
	}
	else
	{
		LOG( ERROR ) << "Ready to do stuff!";
		bebop.Takeoff();
		bebop.Land();
	}


	// Cleanup - Kill the network and clean up memory
	bebop.Cleanup();

	return 0;

}
