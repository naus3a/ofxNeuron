#pragma once
#include "ofMain.h"

#ifdef TARGET_OSX
#define NEURONDATAREADER_EXPORTS
#define __OS_XRUN__
#endif

#include "NeuronDataReader.h"

class ofxNeuron{
public:
	ofxNeuron();
	~ofxNeuron();
		
	void registerCallbacks();
	void connect();
	void disconnect();
	
	inline bool areCallbacksRegistered(){return bCallbacks;}
	inline bool isTCPSocketRunning(){return isSocketRunning(sockTCPRef);}

protected:
	inline bool isSocketRunning(SOCKET_REF sockRef){return (BRGetSocketStatus(sockRef)==SocketStatus::CS_Running);}
	void disconnectSocket(SOCKET_REF sockRef);

	static void bvhDataReceived(void * customObject, SOCKET_REF sockRef, BvhDataHeader * header, float * data);
	static void cmdDataReceived(void * customObject, SOCKET_REF sockRef, CommandPack * pack, void * data);
	static void socketStatusChanged(void * customObject, SOCKET_REF sockeRef, SocketStatus status, char * message);

	static SOCKET_REF sockTCPRef;

	FrameDataReceived _DataReceived;
	CommandDataReceived _CmdDataReceived;
	SocketStatusChanged _SocketStatusChanged;

	CommandPack _cmdDataHeader;

	static float * _valuesBuffer;
	static int valuesBufferLeng;

	bool bCallbacks;
};