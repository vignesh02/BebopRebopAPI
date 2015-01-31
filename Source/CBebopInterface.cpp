/*
 * CDeviceManager.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: spiderkeys
 */

#include "CBebopInterface.h"

#include "Utility.hpp"

using namespace std;
using namespace bebop;

CBebopInterface::CBebopInterface()
{
	m_pNetworkALManager 				= nullptr;
	m_pNetworkManager					= nullptr;
	m_pRxThread							= 0;
	m_pTxThread							= 0;

	// Attributes
	run 								= 1;
}

CBebopInterface::~CBebopInterface()
{
	// Free pointers
	ARNETWORK_Manager_Delete( &m_pNetworkManager );
	ARNETWORKAL_Manager_Delete( &m_pNetworkALManager );
}

void CBebopInterface::Initialize()
{
}


// Returns true if successful, false if failure
bool CBebopInterface::NetworkDiscoveryConnection()
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

bool CBebopInterface::StartNetwork()
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

//	if (!failed)
//	{
//		// Create and start Tx and Rx threads.
//		if (ARSAL_Thread_Create(&(deviceManager->rxThread), ARNETWORK_Manager_ReceivingThreadRun, deviceManager->netManager) != 0)
//		{
//			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of Rx thread failed.");
//			failed = 1;
//		}
//
//		if (ARSAL_Thread_Create(&(deviceManager->txThread), ARNETWORK_Manager_SendingThreadRun, deviceManager->netManager) != 0)
//		{
//			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "Creation of Tx thread failed.");
//			failed = 1;
//		}
//	}
//
//	// Print net error
//	if (failed)
//	{
//		if (netAlError != ARNETWORKAL_OK)
//		{
//			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARNetWorkAL Error : %s", ARNETWORKAL_Error_ToString(netAlError));
//		}
//
//		if (netError != ARNETWORK_OK)
//		{
//			ARSAL_PRINT(ARSAL_PRINT_ERROR, TAG, "ARNetWork Error : %s", ARNETWORK_Error_ToString(netError));
//		}
//	}

	return true;
}

void CBebopInterface::Cleanup()
{
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

