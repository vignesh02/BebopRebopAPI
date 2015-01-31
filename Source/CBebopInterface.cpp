// Includes
#include "CBebopInterface.h"
#include "Utility.hpp"

// Namespaces
using namespace std;
using namespace bebop;

CBebopInterface::CBebopInterface()
{
	// Initialize pointers
	m_pNetworkALManager 		= nullptr;
	m_pNetworkManager			= nullptr;
	m_tRxThread					= nullptr;
	m_tTxThread					= nullptr;
}

CBebopInterface::~CBebopInterface()
{
}

bool CBebopInterface::Initialize()
{
	LOG( INFO ) << "Initializing Bebop Interface...";

	// Perform network discovery
	if( !PerformNetworkDiscovery() )
	{
		return false;
	}

	// Start network connection with drone
	if( !InitializeNetworkManager() )
	{
		return false;
	}

	// Start network connection with drone
	if( !StartNetworkThreads() )
	{
		return false;
	}

	return true;
}

bool CBebopInterface::PerformNetworkDiscovery()
{
	LOG( INFO ) << "Performing network discovery...";

	bool failed = false;

	eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

	// Create connection data object - allocation
	// Use the JSON callbacks to get the data back to the network settings object
	ARDISCOVERY_Connection_ConnectionData_t *discoveryData = ARDISCOVERY_Connection_New( SendJsonCallback, ReceiveJsonCallback, &m_networkSettings, &err );

	// Check for errors in creation
	if( discoveryData == nullptr || err != ARDISCOVERY_OK )
	{
		LOG( ERROR ) << "Error while creating discoveryData: " << ARDISCOVERY_Error_ToString( err );
		failed = true;
	}

	// If successful, perform network discovery with the target
	if( !failed )
	{
		eARDISCOVERY_ERROR err = ARDISCOVERY_Connection_ControllerConnection( discoveryData, m_networkSettings.DISCOVERY_PORT, m_networkSettings.TARGET_IP_ADDRESS );

		if( err != ARDISCOVERY_OK )
		{
			LOG( ERROR ) << "Error while opening discovery connection: " << ARDISCOVERY_Error_ToString( err );
			failed = true;
		}
	}

	// Free memory
	ARDISCOVERY_Connection_Delete( &discoveryData );

	if( failed )
	{
		LOG( INFO ) << "Network discovery failed!";
		return false;
	}
	else
	{
		LOG( INFO ) << "Network discovery completed.";
		return true;
	}
}

bool CBebopInterface::InitializeNetworkManager()
{
	eARNETWORK_ERROR netError 		= ARNETWORK_OK;
	eARNETWORKAL_ERROR netAlError 	= ARNETWORKAL_OK;

	// 0 means default, -1 means no ping
	int pingDelay 					= 0;
	int timeoutSecs					= 1;

	// Create the ARNetworkALManager - allocation
	m_pNetworkALManager = ARNETWORKAL_Manager_New( &netAlError );

	if( m_pNetworkALManager == nullptr || netAlError != ARNETWORKAL_OK )
	{
		LOG( ERROR ) << "Failed to create Network Abstraction Layer Manager.";
		return false;
	}

	// Initialize the ARNetworkALManager
	netAlError = ARNETWORKAL_Manager_InitWifiNetwork( m_pNetworkALManager, m_networkSettings.TARGET_IP_ADDRESS, m_networkSettings.m_outboundPort, m_networkSettings.m_inboundPort, timeoutSecs );

	if( netAlError != ARNETWORKAL_OK )
	{
		LOG( ERROR ) << "Failed to initialize Network Abstraction Layer Manager.";
		return false;
	}

	// Create the ARNetworkManager.
	m_pNetworkManager = ARNETWORK_Manager_New( 	m_pNetworkALManager,
												m_networkSettings.m_outboundParameters.size(),
												m_networkSettings.m_outboundParameters.data(),
												m_networkSettings.m_inboundParameters.size(),
												m_networkSettings.m_inboundParameters.data(),
												pingDelay,
												OnDisconnect,
												NULL,
												&netError );

	if (netError != ARNETWORK_OK)
	{
		LOG( ERROR ) << "Failed to create Network Manager.";
		return false;
	}

	return true;
}

void CBebopInterface::Cleanup()
{
	// Close all connections, kill threads, and clean up memory used for networking
	StopNetwork();
}

eARDISCOVERY_ERROR CBebopInterface::SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn )
{
	// Cast to get the network settings object
	CNetworkSettings *settings = (CNetworkSettings*)customDataIn;

	if( settings == nullptr )
	{
		LOG( ERROR ) << "SendJsonCallback() failed: Null network settings object.";
		return eARDISCOVERY_ERROR::ARDISCOVERY_ERROR;
	}

    if( ( txDataIn != nullptr ) && ( txDataSizeIn != nullptr ) )
    {
    	// Copy the json string to the tx buffer and get the length (+1 is for the null terminator, as sprintf does not include it)
        *txDataSizeIn = sprintf( (char *)txDataIn, "{ \"%s\": %d,\n \"%s\": \"%s\",\n \"%s\": \"%s\" }",
        ARDISCOVERY_CONNECTION_JSON_D2CPORT_KEY, settings->INBOUND_PORT,
        ARDISCOVERY_CONNECTION_JSON_CONTROLLER_NAME_KEY, "name",
        ARDISCOVERY_CONNECTION_JSON_CONTROLLER_TYPE_KEY, "type" ) + 1;

        // Looks like this:
		// 		{ "d2c_port": settings->INBOUND_PORT,
		// 		"controller_name": "name",
		// 		"controller_type": "type" }
    }
    else
    {
    	// Error occurred - inputs were null
    	LOG( ERROR ) << "SendJsonCallback() failed: No valid data buffer info provided.";
    	return eARDISCOVERY_ERROR::ARDISCOVERY_ERROR;
    }

    return eARDISCOVERY_ERROR::ARDISCOVERY_OK;
}

void CBebopInterface::OnDisconnect( ARNETWORK_Manager_t* networkManagerIn, ARNETWORKAL_Manager_t* networkALManagerIn, void* customDataIn )
{
}

bool bebop::CBebopInterface::StartNetworkThreads()
{
	// Create and start Rx Thread
	if( ARSAL_Thread_Create( &( m_tRxThread ), ARNETWORK_Manager_ReceivingThreadRun, m_pNetworkManager ) != 0 )
	{
		LOG( ERROR ) << "Creation of Rx thread failed.";
		return false;
	}

	// Create and start Tx Thread
	if( ARSAL_Thread_Create( &( m_tRxThread ), ARNETWORK_Manager_SendingThreadRun, m_pNetworkManager ) != 0 )
	{
		LOG( ERROR ) << "Creation of Tx thread failed.";
		return false;

		// TODO: Do I need to clean up the rx thread here before returning from failure?
	}

	return true;
}

void CBebopInterface::StopNetwork()
{
	int failed = 0;

	eARNETWORK_ERROR netError 		= ARNETWORK_OK;
	eARNETWORKAL_ERROR netAlError 	= ARNETWORKAL_OK;

	// ARNetwork cleanup
	if( m_pNetworkManager != nullptr )
	{
		// Stop the network manager
		ARNETWORK_Manager_Stop( m_pNetworkManager );

		// Clean up the Rx thread
		if( m_tRxThread != nullptr )
		{
			ARSAL_Thread_Join( m_tRxThread, nullptr );
			ARSAL_Thread_Destroy( &( m_tRxThread ) );
			m_tRxThread = nullptr;
		}

		// Clean up the Tx thread
		if( m_tTxThread != nullptr )
		{
			ARSAL_Thread_Join( m_tTxThread, nullptr );
			ARSAL_Thread_Destroy( &( m_tTxThread ) );
			m_tTxThread = nullptr;
		}
	}

	// Clean up Network Abstraction Layer Manager
	if( m_pNetworkALManager != nullptr )
	{
		// Unlock the Network AL manager
		ARNETWORKAL_Manager_Unlock( m_pNetworkALManager );

		// Close the wifi network
		ARNETWORKAL_Manager_CloseWifiNetwork( m_pNetworkALManager );
	}

	// Free memory for network managers
	ARNETWORK_Manager_Delete( &m_pNetworkManager );
	ARNETWORKAL_Manager_Delete( &m_pNetworkALManager );
}

eARDISCOVERY_ERROR CBebopInterface::ReceiveJsonCallback( uint8_t *rxDataIn, uint32_t rxDataSizeIn, char *ipIn, void *customDataIn )
{
	// Cast to get the network settings object
	CNetworkSettings *settings = (CNetworkSettings *)customDataIn;

	if( settings == nullptr )
	{
		LOG( ERROR ) << "ReceiveJsonCallback() failed: Null network settings object.";
		return eARDISCOVERY_ERROR::ARDISCOVERY_ERROR;
	}

    // Make sure we received data and have the device manager pointer
    if( ( rxDataIn != nullptr ) && ( rxDataSizeIn != 0 ) )
    {
    	// Parse the port number from the string sent by the drone
        if( !util::ParseIntFromJsonString( (const char*)rxDataIn, ARDISCOVERY_CONNECTION_JSON_D2CPORT_KEY, settings->m_outboundPort ) )
        {
        	// Failure
        	LOG( ERROR ) << "ReceiveJsonCallback() failed: Unable to parse port number from JSON string.";
        	return eARDISCOVERY_ERROR::ARDISCOVERY_ERROR;
        }
    }
    else
    {
    	LOG( ERROR ) << "ReceiveJsonCallback() failed: No valid data received.";
    	return eARDISCOVERY_ERROR::ARDISCOVERY_ERROR;
    }

    return eARDISCOVERY_ERROR::ARDISCOVERY_OK;
}

