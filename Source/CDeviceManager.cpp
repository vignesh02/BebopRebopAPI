/*
 * CDeviceManager.cpp
 *
 *  Created on: Jan 29, 2015
 *      Author: spiderkeys
 */

#include "CDeviceManager.h"



namespace bebop {

CDeviceManager::CDeviceManager()
{
	m_pNetworkAbstractionLayerManager 	= nullptr;
	m_pNetworkManager					= nullptr;
	m_pRxThread							= 0;
	m_pTxThread							= 0;

	// Attributes
	m_portIn 							= k_inPort;
	m_portOut 							= k_outPort;
	run 								= 1;

	m_outWithAck.ID 					= k_outAckId;
	m_outWithAck.dataType 				= eARNETWORKAL_FRAME_TYPE::ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK;
	m_outWithAck.sendingWaitTimeMs 		= 20;
	m_outWithAck.ackTimeoutMs 			= 500;
	m_outWithAck.numberOfRetry 			= 3;
	m_outWithAck.numberOfCell 			= 20;
	m_outWithAck.dataCopyMaxSize 		= 128;
	m_outWithAck.isOverwriting 			= 0;

	m_outNoAck.ID						= k_outNonAckId;
	m_outNoAck.dataType 				= eARNETWORKAL_FRAME_TYPE::ARNETWORKAL_FRAME_TYPE_DATA;
	m_outNoAck.sendingWaitTimeMs 		= 5;
	m_outNoAck.ackTimeoutMs 			= -1;
	m_outNoAck.numberOfRetry 			= -1;
	m_outNoAck.numberOfCell 			= 10;
	m_outNoAck.dataCopyMaxSize 			= 128;
	m_outNoAck.isOverwriting 			= 0;

	m_inWithAck.ID						= k_inAckId;
	m_inWithAck.dataType 				= eARNETWORKAL_FRAME_TYPE::ARNETWORKAL_FRAME_TYPE_DATA_WITH_ACK;
	m_inWithAck.sendingWaitTimeMs 		= 20;
	m_inWithAck.ackTimeoutMs 			= 500;
	m_inWithAck.numberOfRetry 			= 3;
	m_inWithAck.numberOfCell 			= 20;
	m_inWithAck.dataCopyMaxSize 		= 128;
	m_inWithAck.isOverwriting 			= 0;

	m_inNoAck.ID 						= k_inNonAckId;
	m_inNoAck.dataType 					= eARNETWORKAL_FRAME_TYPE::ARNETWORKAL_FRAME_TYPE_DATA;
	m_inNoAck.sendingWaitTimeMs 		= 20;
	m_inNoAck.ackTimeoutMs	 			= -1;
	m_inNoAck.numberOfRetry 			= -1;
	m_inNoAck.numberOfCell 				= 10;
	m_inNoAck.dataCopyMaxSize 			= 128;
	m_inNoAck.isOverwriting 			= 0;
}

CDeviceManager::~CDeviceManager()
{

}

void CDeviceManager::Initialize()
{
}

bool CDeviceManager::NetworkDiscoveryConnection()
{
	int failed = 0;

	eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

	// Perform network discovery
	ARDISCOVERY_Connection_ConnectionData_t *discoveryData = ARDISCOVERY_Connection_New( SendJsonCallback, ReceiveJsonCallback, this, &err );

	// Check for errors
	if( discoveryData == nullptr || err != ARDISCOVERY_OK )
	{
		ARSAL_PRINT( ARSAL_PRINT_ERROR, k_tag, "Error while creating discoveryData : %s", ARDISCOVERY_Error_ToString( err ) );
		failed = 1;
	}

	//
	if( !failed )
	{
		eARDISCOVERY_ERROR err = ARDISCOVERY_Connection_ControllerConnection( discoveryData, k_discoveryPort, k_ipAddress );
		if (err != ARDISCOVERY_OK)
		{
			ARSAL_PRINT(ARSAL_PRINT_ERROR, k_tag, "Error while opening discovery connection : %s", ARDISCOVERY_Error_ToString(err));
			failed = 1;
		}
	}

	// Free memory used to do discovery
	ARDISCOVERY_Connection_Delete( &discoveryData );

	return failed;
}

bool CDeviceManager::StartNetwork()
{
}

void CDeviceManager::Cleanup()
{
}

eARDISCOVERY_ERROR CDeviceManager::SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn )
{
	// I think this is what we send to the Bebop during discovery
	// customDataIn is used to get your system's info into the JSON file (doesn't necessarily have to be the device manager but it works)

	// Cast to get the device manager
	CDeviceManager *deviceManager = (CDeviceManager*)customDataIn;

    eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

    if( ( txDataIn != nullptr ) && ( txDataSizeIn != nullptr ) && ( deviceManager != nullptr ) )
    {
    	// Copy the json string to the tx buffer and get the length (+1 is for the null terminator, as sprintf does not include it)
        *txDataSizeIn = sprintf( (char *)txDataIn, "{ \"%s\": %d,\n \"%s\": \"%s\",\n \"%s\": \"%s\" }",
        ARDISCOVERY_CONNECTION_JSON_D2CPORT_KEY, deviceManager->m_portIn,
        ARDISCOVERY_CONNECTION_JSON_CONTROLLER_NAME_KEY, "name",
        ARDISCOVERY_CONNECTION_JSON_CONTROLLER_TYPE_KEY, "type" ) + 1;
    }
    else
    {
    	// Error occurred - inputs were null
        err = ARDISCOVERY_ERROR;
    }

    return err;
}

eARDISCOVERY_ERROR CDeviceManager::ReceiveJsonCallback( uint8_t *rxDataIn, uint32_t rxDataSizeIn, char *ipIn, void *customDataIn )
{
	// I think this is what the Bebop sends back to us during network discovery

	// Cast to get the device manager
	CDeviceManager *deviceManager = (CDeviceManager *)customDataIn;

    eARDISCOVERY_ERROR err = ARDISCOVERY_OK;

    // Make sure we received data and have the device manager pointer
    if( (rxDataIn != nullptr) && ( rxDataSizeIn != 0 ) && ( deviceManager != nullptr ) )
    {
    	// Allocate memory for the string
        char *json = (char*)malloc( rxDataSizeIn + 1 );

        // Copy the rx buffer
        strncpy(json, (char *)rxDataIn, rxDataSizeIn);

        // Add the null terminator
        json[ rxDataSizeIn ] = '\0';

        // normally c2dPort should be read from the json here.
        // Use JSON parser to find the ports and stuff you'll end up using to talk to the AR Drone

        // Free the allocated memory
        free(json);
    }
    else
    {
    	// Error
        err = ARDISCOVERY_ERROR;
    }

    return err;
}

}
