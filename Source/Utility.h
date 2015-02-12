#pragma once

#include <easylogging.h>
#include <rapidjson/document.h>

// Semi-safe pointer deletion
#define SafeDelete(pPtr) { delete pPtr; pPtr = nullptr; }

namespace util
{
	bool ParseIntFromJsonString( const std::string &jsonStringIn, const std::string &keyIn, int &valueOut );
}
