// Includes
#include "CVehicleInterface.h"
#include "Utility.hpp"

// Namespaces
using namespace rebop;

CVehicleInterface::CVehicleInterface()
{
	m_isConnected = false;
}

CVehicleInterface::~CVehicleInterface()
{

}

void CVehicleInterface::Initialize()
{
	// Initialize the network interface
	m_isConnected = m_networkInterface.Initialize();
}

void CVehicleInterface::Update()
{
	// TODO: Create a low frequency thread to manage reconnection behaviour and treat the entire interface as a subsystem
	if( m_isConnected == false )
	{
		LOG( ERROR ) << "No connection! Can't do anything!";
	}
	else
	{
		LOG( ERROR ) << "Ready to do stuff!";
	}
}

void CVehicleInterface::Cleanup()
{
	// Shutdown the network
	m_networkInterface.Cleanup();
}
