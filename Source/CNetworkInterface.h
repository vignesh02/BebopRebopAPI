#pragma once

// Includes
#include <vector>
#include <string>
#include <functional>

extern "C"
{
#include <stdio.h>
#include <malloc.h>
#include <libARSAL/ARSAL.h>
#include <libARNetwork/ARNetwork.h>
#include <libARNetworkAL/ARNetworkAL.h>
#include <libARCommands/ARCommands.h>
#include <libARDiscovery/ARDiscovery.h>
}

#include "CNetworkSettings.h"
#include "CDataPacket.h"

namespace rebop
{

// Typedefs
typedef std::function<void()> TDisconnectionCallback;
typedef std::function<void()> TConnectionCallback;

class CNetworkInterface
{
public:
	// Pointers
	ARNETWORKAL_Manager_t 		*m_pNetworkALManager;
	ARNETWORK_Manager_t 		*m_pNetworkManager;

	TConnectionCallback			m_pConnectionCallback;
	TDisconnectionCallback		m_pDisconnectionCallback;

	// Threads
	ARSAL_Thread_t 				m_tRxThread;
	ARSAL_Thread_t 				m_tTxThread;

	std::vector<ARSAL_Thread_t>	m_tRxThreads;

	// Attributes
	CNetworkSettings m_networkSettings;

	// Methods
	CNetworkInterface();
	virtual ~CNetworkInterface();

	// Composite behaviour functions
	bool Initialize();
	bool PerformNetworkDiscovery();
	bool InitializeNetworkManagers();
	bool StartNetworkThreads();
	void StopNetwork();
	void Cleanup();

	// Wrapper functions
	bool Flush();
	bool SendData( const CDataPacket &dataIn, EOutputBufferId outputBufferIdIn );
	bool ReadData( const CDataPacket &dataIn, EInputBufferId inputBufferIdIn );
	bool TryReadData( const CDataPacket &dataIn, EInputBufferId inputBufferIdIn );
	bool ReadDataWithTimeout( const CDataPacket &dataIn, EInputBufferId inputBufferIdIn, uint32_t timeoutMsIn );
	bool FlushInputBuffer( EInputBufferId inputBufferIdIn );
	bool FlushOutputBuffer( EOutputBufferId outputBufferIdIn );
	bool SetMinimumTimeBetweenSends( uint32_t delayMsIn );
	int GetEstimatedLatency( EOutputBufferId outputBufferIdIn );
	int GetEstimatedMissPercentage();

	// Callback registration functions
	void RegisterConnectionCallback( TConnectionCallback &callbackIn );
	void UnregisterConnectionCallback();
	void RegisterDisconnectionCallback( TDisconnectionCallback &callbackIn );
	void UnregisterDisconnectionCallback();

	// Callback functions
	static eARNETWORK_MANAGER_CALLBACK_RETURN CommandCallback( int bufferIdIn, uint8_t *dataIn, void *customDataIn, eARNETWORK_MANAGER_CALLBACK_STATUS causeIn );
	static void OnDisconnect( ARNETWORK_Manager_t *networkManagerIn, ARNETWORKAL_Manager_t *networkALManagerIn, void *customDataIn );
	static eARDISCOVERY_ERROR SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn );
	static eARDISCOVERY_ERROR ReceiveJsonCallback( uint8_t *dataRxIn, uint32_t dataRxSizeIn, char *ipIn, void *customDataIn );
};

}
