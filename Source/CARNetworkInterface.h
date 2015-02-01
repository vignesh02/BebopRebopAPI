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

namespace rebop
{

// Typedefs
typedef std::function<void()> TDisconnectionCallback;

class CARNetworkInterface
{
public:
	// Pointers
	ARNETWORKAL_Manager_t 		*m_pNetworkALManager;
	ARNETWORK_Manager_t 		*m_pNetworkManager;

	TDisconnectionCallback		m_pDisconnectionCallback;

	// Threads
	ARSAL_Thread_t 				m_tRxThread;
	ARSAL_Thread_t 				m_tTxThread;

	std::vector<ARSAL_Thread_t>	m_tRxThreads;

	// Attributes
	CNetworkSettings m_networkSettings;

	// Methods
	CARNetworkInterface();
	virtual ~CARNetworkInterface();

	bool Initialize();
	bool PerformNetworkDiscovery();
	bool InitializeNetworkManagers();
	bool StartNetworkThreads();
	void StopNetwork();
	void Cleanup();

	void RegisterDisconnectionCallback( TDisconnectionCallback &callbackIn );
	void UnregisterDisconnectionCallback();

	// Callbacks
	static void OnDisconnect( ARNETWORK_Manager_t *networkManagerIn, ARNETWORKAL_Manager_t *networkALManagerIn, void *customDataIn );
	static eARDISCOVERY_ERROR SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn );
	static eARDISCOVERY_ERROR ReceiveJsonCallback( uint8_t *dataRxIn, uint32_t dataRxSizeIn, char *ipIn, void *customDataIn );
};

}
