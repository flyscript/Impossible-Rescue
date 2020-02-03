/*

	EventListener.h
	Will Chapman
	07/11/2019

	This header file describes the architecture of the EventListener class.


*/

#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H



// Includes
#include <functional>
#include "Helper/GameConfig.h"

/*
	EventListener
	-------------
	A class used by EventManager to map trigger requests to the firing of the relevant events

	Could have just been a function pointer, but this allows for any potential future expandability
*/
class CEventListener
{
private:

	// Pointer to the function that'll be called when this event is triggered
	std::function<void(void*)> m_pfnCallback;

	// The kind of event that this listener is for
	GameConfig::EEventType m_eEventType;

public:

	// Default constructor
	CEventListener();

	// A constructor that sets the callback and event type
	CEventListener(std::function<void(void*)> pfnCallback, GameConfig::EEventType eEventType);

	// Destructor
	~CEventListener();

	// Runs the callback
	void Trigger(void* data);


	///////////////////////////////////
	//	Setters and Getters
	///////////////////////////////////

	// Setter for event type
	inline void SetEventType( GameConfig::EEventType eEventType)
	{
		// Only set if not already set
		if (!m_eEventType)
		{
			m_eEventType = eEventType;
		}
	}

	// Getter for event type
	inline GameConfig::EEventType GetEventType()
	{
		return m_eEventType;
	}

	// Setter for callback
	inline void SetCallback(std::function<void(void*)> pfnCallback)
	{
		m_pfnCallback = pfnCallback;
	}

	// Getter for callback
	inline std::function<void(void*)> GetCallback()
	{
		return m_pfnCallback;
	}

};

#endif