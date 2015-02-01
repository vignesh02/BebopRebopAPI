#pragma once

#include <stdint.h>

namespace rebop
{

class CDataPacket
{
public:
	// Attributes
	uint8_t *m_pData;	// Raw Data
	int 	m_size;		// Size in bytes

	CDataPacket();
	virtual ~CDataPacket();
};

}
