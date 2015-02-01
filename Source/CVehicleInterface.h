#pragma once

// Includes
#include "CNetworkInterface.h"

namespace rebop
{

class CVehicleInterface
{
public:
	// Pointers

	// Attributes
	CNetworkInterface m_networkInterface;

	//Methods
	CVehicleInterface();
	virtual ~CVehicleInterface();

	void Initialize();
	void Update();
	void Cleanup();
};

}
