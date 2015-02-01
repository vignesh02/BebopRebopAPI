#pragma once

// Includes
#include "CVehicleInterface.h"
#include "BebopCommandsAndData.h"

// Normally, this is forbidden by my style guidelines, but you SHOULD only ever be including one vehicle's namespace
using namespace rebop::commands::bebop::navigation;

namespace rebop
{

class CBebopInterface : public CVehicleInterface
{
public:
	CBebopInterface();
	virtual ~CBebopInterface();

	void Takeoff();
	void Land();
	void Emergency();
	void NavigateHome( ENavigateHome startOrStopIn );
	void SetPose( const TPose &poseIn );
	void SetAutoTakeoffMode( EAutoTakeoffMode modeIn );
	void Flip( EFlipDirection directionIn );
	void SetMaxAltitude( float maxAltitudeMetersIn );
	void SetMaxTilt( float maxTiltDegreesIn );
	void SetMaxVerticalSpeed( float maxVerticalSpeedMetersPerSecIn );
	void SetMaxRotationSpeed( float maxRotationSpeedDegPerSecIn );
	void SetAbsoluteControlMode( EAbsoluteControlMode modeIn );
	void SetHullProtectionPresence( EHullPresence presenceIn );
	void SetOutdoorMode( EOutdoorMode modeIn );
	void SetGpsHomeLocation( const TGpsHomeLocation &locationIn );
	void ResetGpsHome();
};

}
