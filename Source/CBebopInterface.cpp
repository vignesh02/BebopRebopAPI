// Includes
#include "CBebopInterface.h"
#include "Utility.hpp"

// Namespaces
using namespace rebop;

CBebopInterface::CBebopInterface()
{

}

CBebopInterface::~CBebopInterface()
{
}


void CBebopInterface::Update()
{
	// TODO: Create a low frequency thread to manage reconnection behaviour and treat the entire interface as a subsystem
}

bool CBebopInterface::Takeoff()
{
	CCommandPacket packet( 128 );

	// Generate takeoff command
	eARCOMMANDS_GENERATOR_ERROR cmdError = ARCOMMANDS_Generator_GenerateARDrone3PilotingTakeOff( packet.m_pData, packet.m_bufferSize, &packet.m_dataSize );

	if( cmdError == ARCOMMANDS_GENERATOR_OK )
	{
		// Takeoff should be sent and acknowledged
		if( !m_networkInterface.SendData( packet, EOutboundBufferId::OUTBOUND_WITH_ACK, true ) )
		{
			LOG( ERROR ) << "Failed to send takeoff command.";
			return false;
		}
	}
	else
	{
		LOG( ERROR ) << "Failed to generate takeoff command.";
		return false;
	}

	return true;
}

void CBebopInterface::Land()
{
}

void CBebopInterface::Emergency()
{
}

void CBebopInterface::NavigateHome( ENavigateHome startOrStopIn )
{
}

void CBebopInterface::SetPose( const TPose& poseIn )
{
}

void CBebopInterface::SetAutoTakeoffMode( EAutoTakeoffMode modeIn )
{
}

void CBebopInterface::Flip( EFlipDirection directionIn )
{
}

void CBebopInterface::SetMaxAltitude( float maxAltitudeMetersIn )
{
}

void CBebopInterface::SetMaxTilt( float maxTiltDegreesIn )
{
}

void CBebopInterface::SetMaxVerticalSpeed( float maxVerticalSpeedMetersPerSecIn )
{
}

void CBebopInterface::SetMaxRotationSpeed( float maxRotationSpeedDegPerSecIn )
{
}

void CBebopInterface::SetAbsoluteControlMode( EAbsoluteControlMode modeIn )
{
}

void CBebopInterface::SetHullProtectionPresence( EHullPresence presenceIn )
{
}

void CBebopInterface::SetOutdoorMode( EOutdoorMode modeIn )
{
}

void CBebopInterface::SetGpsHomeLocation( const TGpsHomeLocation& locationIn )
{
}


void CBebopInterface::ResetGpsHome()
{
}
