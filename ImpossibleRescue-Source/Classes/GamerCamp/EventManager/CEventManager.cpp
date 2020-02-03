/*

EventManager.cpp
Will Chapman
07/11/2019

This header file defines the functionality of the EventManager class


*/

// Include .h
#include "CEventManager.h"
#include <iostream>



// Singleton behaviour
CEventManager& CEventManager::Manager()
{
	static CEventManager cInstance;

	return cInstance;
}


// Constructor
CEventManager::CEventManager()
{
	// Initialize list of event listener lists
	m_aListOfEventListenerLists = new std::vector<std::vector<CEventListener*>>();

	// Add in a list for each event type
	for ( int iIndex = 0; iIndex != GameConfig::EEventType::iterator_end; iIndex++ )
	{
		// Create new vector of event listeners
		std::vector<CEventListener*>* aNewListOfListeners = new std::vector<CEventListener*>();

		// Add it to the list of event listener lists
		m_aListOfEventListenerLists->push_back( *aNewListOfListeners );
	}
}


// Destructor
CEventManager::~CEventManager()
{
	// Vectors delete all their contents so this is as much as needs to be managed
	delete m_aListOfEventListenerLists;
}


// Adds a listener to the appropriate list via an explicit listener
CEventListener* CEventManager::AddListener( GameConfig::EEventType eEventType, CEventListener* pListener )
{
	// Get the relevant index in the list of event types
	int iIndex = static_cast< int >( eEventType );

	// Get the relevant list with that index
	std::vector<CEventListener*>* listOfListeners = &m_aListOfEventListenerLists->at( iIndex );

	// Add the new listener into that list
	listOfListeners->push_back( pListener );

	//Return the listener
	return pListener;
}

// Adds a listener to the appropriate list via a callback function - returns the listener it makes
CEventListener* CEventManager::AddListener( GameConfig::EEventType eEventType, std::function<void( void* )> pfnCallback )
{
	// Get the relevant index in the list of event types
	int iIndex = static_cast< int >( eEventType );

	// Get the relevant list with that index
	std::vector<CEventListener*>* listOfListeners = &m_aListOfEventListenerLists->at( iIndex );

	// Create new event listener
	CEventListener* aNewListener = new CEventListener( pfnCallback, eEventType );

	// Add the new listener into that list
	listOfListeners->push_back( aNewListener );

	//Return the listener
	return aNewListener;
}

// Simple brute-force remove
void CEventManager::RemoveListener( GameConfig::EEventType eEventType, CEventListener* pListener )
{
	// Get the relevant index in the list of event types
	int iIndex = static_cast< int >( eEventType );

	// Get the relevant list with that index
	std::vector<CEventListener*>* listOfListeners = &m_aListOfEventListenerLists->at( iIndex );

	// Search that list
	for ( unsigned i = 0; i < listOfListeners->size(); i++ )
	{
		// Delete the listener
		if ( listOfListeners->at( i ) == pListener )
		{
			listOfListeners->erase( listOfListeners->begin() + i );
			break;
		}
	}
}

// Removes all events of a certain type
void CEventManager::RemoveAllListenersOfType( GameConfig::EEventType eEventType )
{
	// Get the relevant index in the list of event types
	int iIndex = static_cast< int >( eEventType );

	// Get the relevant list with that index
	std::vector<CEventListener*>* listOfListeners = &m_aListOfEventListenerLists->at( iIndex );

	// Delete contents of the array of listeners
	listOfListeners->clear();
}

//Removes ALL events
void CEventManager::RemoveAllListeners()
{
	// Loop through each event type
	for ( int iIndex = 0; iIndex != GameConfig::EEventType::iterator_end; iIndex++ )
	{
		// Clear the list
		std::vector<CEventListener*>* eventListenerList = &m_aListOfEventListenerLists->at( iIndex );
		eventListenerList->clear();
	}
}

// Tells event manager to trigger events of a given type
void CEventManager::Trigger( GameConfig::EEventType eEventType, void* pData )
{
	// Get the relevant index in the list of event types
	int iIndex = static_cast<int>( eEventType );

	// Get list of listeners to trigger
	std::vector<CEventListener*>* aListOfListeners = &m_aListOfEventListenerLists->at( iIndex );

	// Trigger all listeners for that event
	for each ( auto eventListener in *aListOfListeners )
	{
		eventListener->Trigger( pData );
	}
}