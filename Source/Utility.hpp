<<<<<<< HEAD
=======
#pragma once

>>>>>>> 514830fb8bc79f96d84021b583598aefe7761673
#include <easylogging.h>
#include <rapidjson/document.h>

// Set up easylogger
INITIALIZE_EASYLOGGINGPP

// Semi-safe pointer deletion
#define SafeDelete(pPtr) { delete pPtr; pPtr = nullptr; }

namespace util
{

bool ParseIntFromJsonString( const std::string &jsonStringIn, const std::string &keyIn, int &valueOut )
{
	// Parse the json string
	rapidjson::Document document;
	document.Parse( jsonStringIn.c_str() );

	// Make sure string was correctly parsed
	if( !document.IsObject() )						{ return false; }
	if( !document.HasMember( keyIn.c_str() ) )		{ return false; }
	if( !document[ keyIn.c_str() ].IsInt() )		{ return false; }

	// Get the port number
	valueOut = document[ keyIn.c_str() ].GetInt();

	return true;
}

}
