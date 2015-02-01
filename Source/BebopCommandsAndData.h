#pragma once

// Enumerations
namespace rebop
{
	namespace bebop
	{
		enum ENavCommands
		{
			FlatTrim,
			Takeoff,
			SetPose,
			Land,
			Emergency,
			NavigateHome,
			SetAutoTakeoffMode,
			Flip,
			SetMaxAltitude,
			SetMaxTilt,
			SetAbsoluteControl,
			SetMaxVerticalSpeed,
			SetMaxRotationSpeed,
			SetHullProtectionPresence,
			SetOutdoorMode,
			SetGpsHome,
			ResetGpsHome
		};

		enum EMediaCommands
		{
			RecordPicture,
			RecordVideo,
			SetCameraOrientation,
			SetPictureFormatSelection,
			SetAutoWhiteBalanceSelection,
			SetExposureSelection,
			SetExpositionSelection,
			SetSaturationSelection,
			SetTimelapseSelection,
			SetVideoAutorecordSelection
		};

		enum ENetworkCommands
		{
			LaunchWifiScan,
			InquireAuthChannels,
			SetWifiSelection
		};

		enum EDebugCommands
		{
			VideoEnableWobbleCancellation,
			VideoSyncAngleGyros,
			VideoManualWhiteBalance,
			UseDrone2Battery
		};
	}
}

// Data structures
namespace rebop
{
	// Command structures
	namespace commands
	{
		namespace bebop
		{
			// Pilotting commands
			namespace pilot
			{
				struct TPose
				{
					int8_t flag;		// Boolean flag to activate roll/pitch movement
					int8_t roll;		// Roll consign for the drone 								[-100:100]
					int8_t pitch;		// Pitch consign for the drone 								[-100:100]
					int8_t yaw;			// Yaw rate consign for the drone 							[-100:100]
					int8_t gaz;			// Altitude rate consign for the drone 						[-100:100]
					int8_t psi;			// Magnetic north heading of the controlling device (deg) 	[-180:180]
				};

				struct TNavigateHome
				{
					uint8_t start;		// 1 - Start navigating home.  0 - Stop navigating home.
				};

				struct TAutoTakeoffMode
				{
					uint8_t state;		// 1 - Auto takeoff enabled.  0 - Auto takeoff disabled (?)
				};


			}

			// Animation commands
			namespace animation
			{
				struct TFlip
				{
					enum EDirection
					{
						FRONT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_FRONT,
						BACK = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_BACK,
						RIGHT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_RIGHT,
						LEFT = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_LEFT,
						MAX = eARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION::ARCOMMANDS_ARDRONE3_ANIMATIONS_FLIP_DIRECTION_MAX
					};

					EDirection direction;
				};
			}
		}
	}
}
