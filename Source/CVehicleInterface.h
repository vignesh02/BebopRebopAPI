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

	bool m_isConnected;

	//Methods
	CVehicleInterface();
	virtual ~CVehicleInterface();

	void Initialize();
	void Update();
	void Cleanup();
};

}
