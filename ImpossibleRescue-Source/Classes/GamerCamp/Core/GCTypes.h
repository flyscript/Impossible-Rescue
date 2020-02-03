////////////////////////////////////////////////////////////////////////////////////////////////////////////
// (C) Gamer Camp / Alex Darby 2018
// Distributed under the MIT license - see readme.md
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _GCTYPES_H_
#define _GCTYPES_H_

#include <tuple>

//////////////////////////////////////////////////////////////////////////
// as well as containing the basic definitions of all our types, this 
// header is full of (simple) template magic to make a class ID system
//

// integer types
typedef		char			i8;
typedef		unsigned char	u8;
typedef		short			i16;
typedef		unsigned short	u16;
typedef		int				i32;
typedef		unsigned int	u32;
typedef		long			i64;
typedef		unsigned long	u64;

// floating point types 
typedef		float			f32;
typedef		double			f64;


//////////////////////////////////////////////////////////////////////////
// typedef for our typeID value
typedef		unsigned int	GCTypeID;


//////////////////////////////////////////////////////////////////////////
// type specialised templated TypeID class
//
// This 'generates' a typeid using the address of a static variable.
//
// Since 2 variables can't occupy the same memory address they will be unique.
//
template< typename T >
class TGCTypeIDGenerator
{
public:
	// this generates a typeID for each class that instantiates the template
	static GCTypeID GetTypeID()
	{
		static char s_chTheAdressOfThisIsTheClassID;
		return reinterpret_cast<GCTypeID>( &s_chTheAdressOfThisIsTheClassID );
	}

	// no instance of this class can be created.
private:
	TGCTypeIDGenerator()
	{}
};

// since all GCTypeIDs are essentially valid memory addresses the only safe thing to 
// use for the invalid ID is the system invalid pointer default: NULL - i.e. 0
#define GCTYPEID_INVALID	0

// macro for getting hold of a type's ID
#define GetGCTypeIDOf( TYPENAME )		( TGCTypeIDGenerator< TYPENAME >::GetTypeID() )

//////////////////////////////////////////////////////////////////////////
// template function that resolves to the correct form of 
// CReflectionData< T >::GetTypeID() based on the input type
template< typename T >
GCTypeID GetTypeIDOfVariable( T instance )
{
	return TGCTypeIDGenerator< T >::GetTypeID();
}

namespace GCMaths
{
	template< typename T >
	static T Min( T tOne, T tTwo )
	{
		return ( ( tOne < tTwo ) ? tOne : tTwo );
	}

	template< typename T >
	static T Max( T tOne, T tTwo )
	{
		return ( ( tOne > tTwo ) ? tOne : tTwo );
	}

	template< typename T >
	static T Clamp( T tValue, T tMin, T tMax )
	{
		return Max( tMin, Min( tMax, tValue ) );
	}
}

namespace GCHelpers
{
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// helpful nuggets of code for use in the test program
	//
	// N.B. big ups to Andrei Alexandrescu for writing "Modern C++ Design" and to my friend Wayne Coles
	// for telling me about it in 2008. 
	//
	// N.N.B. If you think the code in this namespace looks mental, you ought to read the book. It will blow 
	// your mindgaskets.

	// this type detects whether TFrom is derived from, or the same as, TTo
	// i.e. checks implicit convertibility
	template< class TTo, class TFrom >
	class Conversion
	{
		typedef char	ConvWorks;
		class			ConvDoesnt { char dummy[ 2 ]; };

		static ConvWorks	Test( const TTo* );	// will match only types implicitly convertible to TTo
		static ConvDoesnt	Test( ... );		// will match anything else
		static TFrom*		MakeTFrom();		// in case TFrom has inaccessible / explicit constructor
	public:
		enum
		{
			// 'exists' will get set to true if the TFrom is the same or implicitly convertible to TTo,
			// else false. This code isn't bulletproof, passing it const types or references will 
			// probably freak it out. You probably want to use boost or TR1 or something but that would have 
			// been a little big for a pastebin...
			exists = ( sizeof( Test( MakeTFrom() ) ) == sizeof( ConvWorks ) )
		};
	};

	// this template is used to create the "compile time static assert" below.
	template< bool > struct StaticAssert
	{
		StaticAssert( ... );
	};

	template<> struct StaticAssert< false > {};


	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// function traits is taken from:https://github.com/aminroosta/sqlite_modern_cpp/blob/master/hdr/sqlite_modern_cpp/utility/function_traits.h
	// for use example see template function GCCollisionManager::RegisterCollisionHandler
	template<typename> struct function_traits;

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template <typename Function>
	struct function_traits
		: public function_traits< decltype( &std::remove_reference<Function>::type::operator() ) >
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template < typename    ClassType, typename    ReturnType, typename... Arguments>
	struct function_traits< ReturnType( ClassType::* )( Arguments... ) const >
		: function_traits< ReturnType( *)( Arguments... )>
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// support the non-const operator () this will work with user defined functors
	template <typename ClassType, typename    ReturnType, typename... Arguments >
	struct function_traits< ReturnType( ClassType::* )( Arguments... )>
		: function_traits<ReturnType( *)( Arguments... )>
	{};

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	template < typename ReturnType, typename... Arguments >
	struct function_traits< ReturnType( *)( Arguments... ) >
	{
		typedef ReturnType result_type;

		template <std::size_t Index>
		using argument = typename std::tuple_element< Index, std::tuple< Arguments... > >::type;

		static const std::size_t arity = sizeof...( Arguments );
	};

}//namespace Helpers


// checks for implicit convertibility of FromType to ToType (i.e. whether FromType is derived from ToType)
// it will cause a compile error if the conversion is not valid
//
// N.B. the name of the type it complains about froms the error message so ErrorMsg has to be a valid C++ 
// identifier - I suggest using '_' instead of ' ' :)
#define STATIC_ASSERT_CONVERSION_VALID( ToType, FromType, ErrorMsg )																	\
{																																		\
	class TYPE_CONVERSION_ERROR_##ErrorMsg{ public: TYPE_CONVERSION_ERROR_##ErrorMsg(void){} };											\
	(void)sizeof( (GCHelpers::StaticAssert< GCHelpers::Conversion< ToType, FromType >::exists >( TYPE_CONVERSION_ERROR_##ErrorMsg() )) );	\
}

// hacky quick assert for use in places we can't / don't want to have CCAssert included
#if defined(_DEBUG) && defined(WIN32)
#define GCASSERT( BoolExpression, MessageNotUsed )	do{ if(!(BoolExpression)){__debugbreak();}}while(0)
#define DEBUG_ONLY( expr )							expr
#else
#define GCASSERT( ... )								/*nothing*/
#define DEBUG_ONLY( ... )							/*nothing*/
#endif		 

#endif//#ifndef _GCTYPES_H_