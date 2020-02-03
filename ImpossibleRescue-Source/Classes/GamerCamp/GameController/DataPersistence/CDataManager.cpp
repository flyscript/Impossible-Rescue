#include "CDataManager.h"
#include "base/CCUserDefault.h"

void CDataPersistence::SaveHealth( float fHealth )
{
	cocos2d::UserDefault::getInstance()->setFloatForKey( "Health", fHealth );
}

float CDataPersistence::GetHealth()
{
	return cocos2d::UserDefault::getInstance()->getFloatForKey( "Health" );
}

void CDataPersistence::SaveLives( int iLives )
{
	cocos2d::UserDefault::getInstance()->setIntegerForKey( "Lives", iLives );
}

int CDataPersistence::GetLives()
{
	return cocos2d::UserDefault::getInstance()->getIntegerForKey( "Lives" );
}

void CDataPersistence::SaveLevel( int iLevel )
{
	cocos2d::UserDefault::getInstance()->setIntegerForKey( "Level", iLevel );
}

int CDataPersistence::GetLevel()
{
	return cocos2d::UserDefault::getInstance()->getIntegerForKey( "Level" );
}

void CDataPersistence::SaveEffectsVolume( int iVolume )
{
	cocos2d::UserDefault::getInstance()->setIntegerForKey( "EffectsVolume", iVolume );
}

int CDataPersistence::GetEffectsVolume()
{
	return cocos2d::UserDefault::getInstance()->getIntegerForKey( "EffectsVolume" );
}

void CDataPersistence::SaveBackgroundVolume( int iVolume )
{
	cocos2d::UserDefault::getInstance()->setIntegerForKey( "BackgroundVolume", iVolume );
}

int CDataPersistence::GetBackgroundVolume()
{
	return cocos2d::UserDefault::getInstance()->getIntegerForKey( "BackgroundVolume" );
}
