#pragma once

#include <vector>



extern "C"
{
#include <stdio.h>
#include <malloc.h>
#include "libARSAL/ARSAL.h"
#include "libARSAL/ARSAL_Print.h"
#include "libARNetwork/ARNetwork.h"
#include "libARNetworkAL/ARNetworkAL.h"
#include "libARCommands/ARCommands.h"
#include "libARDiscovery/ARDiscovery.h"

}




namespace bebop
{

// Constants


class CDeviceManager
{
public:
	const char* k_ipAddress 		= "192.168.43.1";
	const int k_discoveryPort 		= 44444;
	const int k_outPort 			= 54321;
	const int k_inPort 				= 43210;
	const int k_outNonAckId 		= 10;
	const int k_outAckId 			= 11;
	const int k_inNonAckId 			= 127;
	const int k_inAckId 			= 126;
	static const int k_errorStringLength 	= 2048;
	const char* k_tag 				= "DeviceManager";


	// Pointers
	ARNETWORKAL_Manager_t 	*m_pNetworkAbstractionLayerManager;
	ARNETWORK_Manager_t 	*m_pNetworkManager;

	ARSAL_Thread_t 				m_pRxThread;
	ARSAL_Thread_t 				m_pTxThread;
	std::vector<ARSAL_Thread_t>	m_pReaderThreads;

	// Attributes
	int m_portIn;
	int m_portOut;
	int run;

	char gErrorStr[k_errorStringLength];

	ARNETWORK_IOBufferParam_t m_outWithAck;
	ARNETWORK_IOBufferParam_t m_outNoAck;
	ARNETWORK_IOBufferParam_t m_inWithAck;
	ARNETWORK_IOBufferParam_t m_inNoAck;

	// Methods
	CDeviceManager();
	virtual ~CDeviceManager();

	void Initialize();
	bool NetworkDiscoveryConnection();
	bool StartNetwork();
	void Cleanup();


	static eARDISCOVERY_ERROR SendJsonCallback( uint8_t *txDataIn, uint32_t *txDataSizeIn, void *customDataIn );
	static eARDISCOVERY_ERROR ReceiveJsonCallback( uint8_t *dataRxIn, uint32_t dataRxSizeIn, char *ipIn, void *customDataIn );

};

}
