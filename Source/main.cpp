// Includes
#include "CBebopInterface.h"
#include "Utility.h"

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
		LOG( INFO ) << "Ready to do stuff!";

		std::string temp;
		LOG( INFO ) << "Enter anything to take off.";
		std::cin >> temp;

		// Take off
		bebop.Takeoff();

		LOG( INFO ) << "Enter anything to land.";
		std::cin >> temp;

		// Land
		bebop.Land();
	}


	// Cleanup - Kill the network and clean up memory
	bebop.Cleanup();

	return 0;

}
