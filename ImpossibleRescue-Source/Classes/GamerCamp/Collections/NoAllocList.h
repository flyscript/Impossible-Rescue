////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NOALLOCLIST_H_
#define _NOALLOCLIST_H_

#ifndef _GCTYPES_H_
	#include "GamerCamp/Core/GCTypes.h"
#endif

//////////////////////////////////////////////////////////////////////////
// fwd decl of list class so we can friend it in the element class
class CNoAllocList;


//////////////////////////////////////////////////////////////////////////
// list element used in the template class
class CNoAllocListable
{
	friend class CNoAllocList;

private:
	// standard list element gubbins
	CNoAllocListable* m_pPrevious;
	CNoAllocListable* m_pNext;

	CNoAllocListable* GetPrevious( void ) const 
	{ 
		return m_pPrevious; 
	}
	
	CNoAllocListable* GetNext( void ) const 
	{ 
		return m_pNext; 
	}

	void SetPrevious( CNoAllocListable* pcPrevious )	
	{ 
		m_pPrevious	= pcPrevious; 
	}
	
	void SetNext( CNoAllocListable* pcNext )		
	{ 
		m_pNext	= pcNext; 
	}

	void Link( CNoAllocListable* pcPrevious, CNoAllocListable* pcNext )
	{
		SetPrevious	( pcPrevious );
		SetNext		( pcNext );
		pcPrevious->SetNext( this );
		pcNext->SetPrevious( this );
	}

	void UnLink( void )
	{
		if( IsLinked() )
		{
			GetPrevious()->SetNext( GetNext() );
			GetNext()->SetPrevious( GetPrevious() );
			SetPrevious( 0 );
			SetNext( 0 );
		}
	}


// There are very few cases where I'd encourage using protected access. This is one.
protected:
	// constructor protected so that it can only be created via public inheritance (in this case, via template)
	CNoAllocListable( void )
	: m_pPrevious ( 0 )
	, m_pNext( 0 ) 
	{}

// I like to make the bare minimum of the functionality public
public:
	virtual	~CNoAllocListable( void ) 
	{}
	
	bool IsLinked( void ) const 
	{ 
		return ( GetPrevious() != 0 ) && ( GetNext() != 0 ); 
	}
};


//////////////////////////////////////////////////////////////////////////
// This is the base class for the templated list class. There are other 
// ways to architect the templating, others may be better - I've not tried 
// them as this seemed the most obvious.
//
// I'd be the first to admit that there's a LOT of assumed inlining of 
// accessors going on. Probably best to check it if you're worried.
//
// This list has constant time insertion and removal. Nice.
class CNoAllocList
{
private:
	// Using list elements for head and tail means that there's always 2 linked elements 
	// in the list, which removes all special cases for list insertion / deletion. Win.
	CNoAllocListable	m_cHead;
	CNoAllocListable	m_cTail;
	unsigned int		m_uCount;

	// inserts an element between two other elements
	void InsertBetween( CNoAllocListable* pcInsert, CNoAllocListable* pcAfter, CNoAllocListable* pcBefore )
	{
		pcInsert->Link( pcAfter, pcBefore );
		m_uCount++;
	}

public:
	// returns true if the list is empty
	bool IsEmpty( void ) const
	{
		return( 0 == m_uCount );
	}

	// returns the number of items in the list
	unsigned int GetCount( void ) const
	{
		return m_uCount;
	}

	// clears the list. Nothing is destroyed, all elements are just removed
	void Clear( void )
	{
		CNoAllocListable* pcNext	= GetHead()->GetNext();
		CNoAllocListable* pcCurrent = 0;
		while( pcNext != GetTail() )
		{
			pcCurrent	= pcNext;
			pcNext		= pcNext->GetNext();
			Remove( pcCurrent );
		}	
	}	

	// N.B. this could be very slow. Do it only when absolutely necessary.
	bool Contains( const CNoAllocListable* pSearchFor )	const
	{
		for(	const CNoAllocListable* pCurrent = const_cast< CNoAllocList* >( this )->GetFirst();
				pCurrent != 0;
				pCurrent = pCurrent->GetNext() )
		{
			if(	pCurrent == pSearchFor )
			{
				return true;
			}
		}
		return false;
	}


protected:
	// again, protected constructor to prevent construction except by inheritance
	// note: default destructor
	CNoAllocList( void )
	: m_uCount ( 0 )
	{ 
		GetHead()->SetNext		( &m_cTail );
		GetTail()->SetPrevious	( &m_cHead );
	}

	// functionality protected so that it can only be used via public inheritance (via template)
	// gets the (hidden) head of the list - i.e. not "in" the list
	CNoAllocListable* GetHead( void )
	{
		return &m_cHead;
	}

	// gets the (hidden) tail of the list - i.e. not "in" the list
	CNoAllocListable* GetTail( void )
	{
		return &m_cTail;
	}

	//////////////////////////////////////////////////////////////////////////
	// These functions allow client code to iterate the list. 
	// As I said before, an iterator in the template would be better. 

	// returns the 1st element in the list or 0 if empty
	CNoAllocListable* GetFirst( void )
	{
		return( m_uCount ? GetHead()->GetNext() : 0 );
	}

	// returns the 1st pointer in the list or 0 if empty 
	CNoAllocListable* GetLast( void )
	{
		return( m_uCount ? GetTail()->GetPrevious() : 0 );
	}

	// returns the next element in the list given the current one
	CNoAllocListable* GetNext( CNoAllocListable* pcCurrent )
	{
		return( (pcCurrent->GetNext() == GetTail() ) ? 0 : pcCurrent->GetNext() );
	}

	// returns the previous element int he list given the current one
	CNoAllocListable* GetPrevious( CNoAllocListable* pcCurrent )
	{
		return( ( pcCurrent->GetPrevious() == GetHead() ) ? 0 : pcCurrent->GetPrevious() );
	}

	// handy to return the removed element so client code can do more work in one line.
	CNoAllocListable* Remove( CNoAllocListable* pcRemove )
	{
		GCASSERT( Contains(pcRemove), "trying to delete an element not in the list!" );									    
		GCASSERT( ((pcRemove != &m_cTail) && (pcRemove != &m_cHead )), "trying to delete hidden element!" );
		pcRemove->UnLink();
		m_uCount--;
		return pcRemove;
	}

	// These last two are just in case you want to remove stuff from the list whilst iterating it
	// note we circumvent GetPrevious() / GetNext() for these fns.
	//
	// This is so that we can return the head unit for RemoveAndGetPrevious() which will allow a loop
	// iterating on GetNext() to call this on the 1st element and then keep iterating - as GetNext() on the 
	// head will either get the new 1st element, or hit the tail and return 0 ending the loop.
	//
	// RemoveAndGetNext() works using the tail to achieve the same thing for reverse iteration using 
	// GetPrevious()
	CNoAllocListable* RemoveAndGetPrevious( CNoAllocListable* pcRemove )
	{
		GCASSERT( ( pcRemove != &m_cHead ), "trying to delete hidden element!" );
		CNoAllocListable* pReturn = pcRemove->GetPrevious();
		Remove( pcRemove );
		return pReturn;
	}

	CNoAllocListable* RemoveAndGetNext( CNoAllocListable* pcRemove )
	{
		GCASSERT( ( pcRemove != &m_cTail ), "trying to delete hidden element!" );
		CNoAllocListable* pReturn = pcRemove->GetNext();
		Remove( pcRemove );
		return pReturn;
	}

public:
	// I love that all these functions just call InsertBetween. It makes me feel all warm inside.

	// inserts an element at the front of the list
	void InsertFront( CNoAllocListable* pcInsert )
	{
		InsertBetween( pcInsert, &m_cHead, m_cHead.GetNext() );
	}

	// inserts an element at the back of the list
	void InsertBack( CNoAllocListable* pcInsert )
	{
		InsertBetween( pcInsert, m_cTail.GetPrevious(), &m_cTail );
	}

	// inserts an element after a specified element
	void InsertAfter( CNoAllocListable* pcInsert, CNoAllocListable* pcAfter )
	{
		InsertBetween( pcInsert, pcAfter, pcAfter->GetNext() );	
	}

	// inserts an item before a specified element
	void InsertBefore( CNoAllocListable* pcInsert, CNoAllocListable* pcBefore )
	{
		InsertBetween( pcInsert, pcBefore->GetPrevious(), pcBefore );	
	}
};



//////////////////////////////////////////////////////////////////////////
// template version of the CNoAllocList
//
// You'll find it much more pleasant to use if you wrap all the list 
// access with an iterator. 
// 
// I will leave this as an exercise for you, my dear reader - because 
// getting that part right is the most fiddly bit. 
//
// Also, I am perfectly well aware that this isn't STL compliant. It would 
// be nice to write one that is. Off you go :)
//
template< typename TType >
class TNoAllocList
: public CNoAllocList
{
public:
	// constructor does static assert to ensure TType is implicitly convertible 
	// to (i.e. derived from) CNoAllocListable
	TNoAllocList( void )
	{
		STATIC_ASSERT_CONVERSION_VALID( CNoAllocListable, TType, TTYPE_must_be_derived_from_CNoAllocListable );
	}

	// destructor removes all links from list just in case
	~TNoAllocList( void )
	{
		Clear();
	}

	// These functions allow client code to manipulate the list. 
	// As I said before, an iterator in this template would be better. 

	// gets first element in the list
	TType* GetFirst( void )
	{
		// N.B. a C-style case would work just as well here, since the compiler
		// automatically decides whether a reinterpret_cast<> or a dynamic_cast<>
		// is what you were trying to do and does that.
		return static_cast< TType* >( CNoAllocList::GetFirst() );
	}

	// get last element in the list
	TType* GetLast( void )
	{
		return static_cast< TType* >( CNoAllocList::GetLast() );
	}

	// gets the next element given the current
	// returns 0 if current is end of list
	TType* GetNext( TType* pcCurrent )
	{
		return static_cast< TType* >( CNoAllocList::GetNext( pcCurrent ) );
	}

	// gets the previous element given the current
	// returns 0 if current is start of list
	TType* GetPrevious( TType* pcCurrent )
	{
		return static_cast< TType* >( CNoAllocList::GetPrevious( pcCurrent ) );
	}

	// removes the specified element from the list 
	// returns a pointer to the removed element
	TType* Remove( TType* pcRemove )
	{
		return static_cast< TType* >( CNoAllocList::Remove( pcRemove ) );
	}

	// this removes the specified element and returns the previous
	// this is useful for removing during list iteration, e.g.:
	//
	//for(	CDemoType* pCurrent = cMyList.GetFirst(); 
	//		pCurrent != 0; 
	//		pCurrent = cMyList.GetNext( pCurrent ) )
	//{
	//	if( bSomeConditionIsTrue )
	//	{
	//		pCurrent = cMyList.RemoveAndGetPrevious( pCurrent );
	//	}
	//}
	TType* RemoveAndGetPrevious( TType* pcRemove )
	{
		return static_cast< TType* >( CNoAllocList::RemoveAndGetPrevious( pcRemove ) );
	}

	// this removes the specified element and returns the next
	// this is useful for removing during reverse list iteration	
	TType* RemoveAndGetNext( TType* pcRemove )
	{
		return static_cast< TType* >( CNoAllocList::RemoveAndGetNext( pcRemove ) );
	}
};

#endif // #ifndef _NOALLOCLIST_H_