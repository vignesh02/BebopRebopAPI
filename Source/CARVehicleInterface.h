#pragma once

// Includes
#include "CARNetworkInterface.h"

namespace rebop
{

class CARVehicleInterface
{
public:
	// Pointers

	// Attributes
	CARNetworkInterface m_networkInterface;

	//Methods
	CARVehicleInterface();
	virtual ~CARVehicleInterface();

	void Initialize();
	void Update();
	void Cleanup();
};

}
