#pragma once

// Includes
#include <array>
#include <libARNetwork/ARNetwork.h>

namespace rebop
{

enum EInboundBufferId
{
	INBOUND								= 127,
	INBOUND_WITH_ACK					= 126
};

enum EOutboundBufferId
{
	OUTBOUND							= 10,
	OUTBOUND_WITH_ACK					= 11,
};

class CNetworkSettings
{
public:
	// Constants
	const int OUTBOUND_INDEX 				= 0;
	const int OUTBOUND_WITH_ACK_INDEX		= 1;
	const int INBOUND_INDEX					= 0;
	const int INBOUND_WITH_ACK_INDEX 		= 1;

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
