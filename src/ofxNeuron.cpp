#include "ofxNeuron.h"

SOCKET_REF ofxNeuron::sockTCPRef = NULL;
float * ofxNeuron::_valuesBuffer = NULL;
int ofxNeuron::valuesBufferLeng  = 0;

ofxNeuron::ofxNeuron(){
	sockTCPRef = NULL;
	bCallbacks = false;

	registerCallbacks();
}

ofxNeuron::~ofxNeuron(){
	disconnect();
	sockTCPRef = NULL;
}

void ofxNeuron::disconnectSocket(SOCKET_REF sockRef){
	if(sockRef!=NULL){
		BRCloseSocket(sockRef);
		sockRef = NULL;
	}
}

void ofxNeuron::disconnect(){
	cout<<"ofxNeuron: disconnecting."<<endl;
	disconnectSocket(sockTCPRef);
}

void ofxNeuron::connect(){
	cout<<"ofxNeuron: trying to connect: ";
	if(isTCPSocketRunning()){
		cout<<"socket busy. Disconnecting.";
		disconnectSocket(sockTCPRef);
	}else{
		sockTCPRef = BRConnectTo("127.0.0.1",7001);
	}
	cout<<endl;
}

void ofxNeuron::registerCallbacks(){
	bool bBVH=false;
	bool bCmd=false;
	bool bSSt=false;
	void * ptr = NULL;
	cout<<"ofxNeuron: registering callbacks: "<<endl;
	cout<<"- BVHDataReceived... ";
	_DataReceived = FrameDataReceived(bvhDataReceived);
	if(_DataReceived){
		BRRegisterFrameDataCallback(ptr, _DataReceived);
		bBVH=true;
		cout<<"done."<<endl;
	}else{
		cout<<"failed."<<endl;
	}
	cout<<"- CMDDataReceived... ";
	_CmdDataReceived = CommandDataReceived(cmdDataReceived);
	if(_CmdDataReceived){
		BRRegisterCommandDataCallback(ptr, _CmdDataReceived);
		bCmd=true;
		cout<<"done."<<endl;
	}else{
		cout<<"failed."<<endl;
	}
	cout<<"- SocketStatusChanged... ";
	_SocketStatusChanged = SocketStatusChanged(socketStatusChanged);
	if(_SocketStatusChanged){
		BRRegisterSocketStatusCallback(ptr, _SocketStatusChanged);
		bSSt=true;
		cout<<"done."<<endl;
	}else{
		cout<<"failed."<<endl;
	}

	bCallbacks = bBVH&&bCmd&&bSSt;
}

void ofxNeuron::bvhDataReceived(void * customObject, SOCKET_REF sockRef, BvhDataHeader * header, float * data){
	BvhDataHeader * ptr = header;
	if(ptr->DataCount!=valuesBufferLeng || _valuesBuffer==NULL){
		_valuesBuffer = new float[ptr->DataCount];
		valuesBufferLeng = ptr->DataCount;
	}

	memcpy((char*)_valuesBuffer,(char*)data, (int)ptr->DataCount*sizeof(float));
}

void ofxNeuron::cmdDataReceived(void * customObject, SOCKET_REF sockRef, CommandPack * pack, void * data){
	
}

void ofxNeuron::socketStatusChanged(void * customObject, SOCKET_REF sockeRef, SocketStatus status, char * message){
	cout<<"ofxNeuron: new socket status: "<<message<<endl;
}