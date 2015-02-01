#pragma once

// Includes
#include "CVehicleInterface.h"

namespace rebop
{

class CBebopInterface : public CVehicleInterface
{
public:
	CBebopInterface();
	virtual ~CBebopInterface();

	void Takeoff();
	void Land();
};

}
