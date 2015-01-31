#pragma once

// Includes
#include <array>
#include <libARNetwork/ARNetwork.h>

namespace bebop
{

class CNetworkSettings
{
public:
	// Constants
	const int OUTBOUND_WITHOUT_ACK_INDEX 	= 0;
	const int OUTBOUND_WITH_ACK_INDEX		= 1;
	const int INBOUND_WITHOUT_ACK_INDEX		= 0;
	const int INBOUND_WITH_ACK_INDEX 		= 1;

	const int OUTBOUND_WITHOUT_ACK_ID 		= 10;
	const int OUTBOUND_WITH_ACK_ID 			= 11;
	const int INBOUND_WITHOUT_ACK_ID 		= 127;
	const int INBOUND_WITH_ACK_ID 			= 126;

	const char* TARGET_IP_ADDRESS 			= "192.168.43.1";
	const int DISCOVERY_PORT 				= 44444;
	const int OUTBOUND_PORT 				= 54321;
	const int INBOUND_PORT 					= 43210;

	// Attributes
	int m_outboundPort;	// This is the port we write to. We'll ask the target for the port they are using during discovery. By default, this is 54321.
	int m_inboundPort;	// This is the port we listen on. We'll tell the target what this is during discovery. By default this is 43210.

	std::array<ARNETWORK_IOBufferParam_t, 2> m_outboundParameters;
	std::array<ARNETWORK_IOBufferParam_t, 2> m_inboundParameters;

	CNetworkSettings();
	virtual ~CNetworkSettings();

	void InitializeIOParameters();
};

}
