"Nothing gets the taste of shame and humiliation out of your mouth like [BebopRebopAPI]" 
 \-Garrison Keillor

The official Parrot ArDroneSDK3 is pretty user unfriendly and poorly documented, with very little sample code, especially for the Bebop. This library aims to fix that. Stay tuned.

Current todo-list:

* Thread that monitors network connection to automatically reconnect upon disconnection
* Reader thread that pulls and parses all incoming data into user buffers for consumption on a per message basis
* Vehicle specific status manager (latest and greatest of status info parsed in, so users can poll when they want instead of consuming messages)
* SendWithTimeout (combines send with readwithtimeout to let you do a blocking send in one call, for things like takeoff, land, etc)
* Finishing adding all of the sendable commands for the Bebop (piloting commands are in, but there is a bunch of media and network stuff to add)
* Probably more, but I don't know what yet

Feel free to make suggestions!



Note: I've only partially tested this stuff! 

I PROVIDE NO GUARANTEES OF THE FUNCTIONALITY OF THIS CODE, YOUR SAFETY, OTHERS' SAFETY, OR YOUR PROPERTY'S SAFETY!
USE AT YOUR OWN RISK!
