#ifndef _GCOBJPERSPECTIVEPLATFORM_H_
#define _GCOBJPERSPECTIVEPLATFORM_H_

#include "../../GCCocosInterface/GCFactory_ObjSpritePhysics.h"
#include "../../GCCocosInterface/GCObjSpritePhysics.h"

class CObjPerspectivePlatform
	: public CGCObjSpritePhysics
{
private:
	// Current sprite frame name
	// Designer can edit the property in the OgmoEditor
	// It will affect this value and switch use the correct sprite image
	std::string m_strFloorSpriteFrameName;

public:
	//////////////////////////////////////////////////////////////////////////
	// Construcotr
	//////////////////////////////////////////////////////////////////////////
	CObjPerspectivePlatform();

	//////////////////////////////////////////////////////////////////////////
	// GCFactory Marcos
	//////////////////////////////////////////////////////////////////////////
	GCFACTORY_DECLARE_CREATABLECLASS( CObjPerspectivePlatform );

	//////////////////////////////////////////////////////////////////////////
	// CGCObjSpritePhysics Virtual Methods
	//////////////////////////////////////////////////////////////////////////
	virtual void VOnResourceAcquire() override;
	virtual void VOnResurrected() override;
};
#endif