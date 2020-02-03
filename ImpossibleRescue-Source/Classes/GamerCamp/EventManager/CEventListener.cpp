/*

	EventListener.cpp
	Will Chapman
	07/11/2019

	This header file defines the very basic functionality of the EventListener class


*/


// Include .h
#include "CEventListener.h"


// Constructor
CEventListener::CEventListener()
{

}


// Constructor with default values
CEventListener::CEventListener(std::function<void(void*)> pfnCallback, GameConfig::EEventType eEventType)
	: m_pfnCallback(pfnCallback)
	, m_eEventType(eEventType)
{

}


// Destructor
CEventListener::~CEventListener()
{

}


// Triggers an individual listener's callback
void CEventListener::Trigger(void* data)
{
	if (m_pfnCallback)
	{
		m_pfnCallback(data);
	}
}

