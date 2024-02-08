#pragma once
#include "Physics.h"

using namespace physx;

class CRigidActor :
	public PxRigidActor
{
private:
	
public:
	CRigidActor(PxType concreteType, PxBaseFlags baseFlags);
	CRigidActor(PxBaseFlags baseFlags);
	virtual ~CRigidActor();

	virtual void release() override;
	virtual PxU32 getInternalActorIndex() const override;

	virtual	PxTransform getGlobalPose() const override;
	virtual	void setGlobalPose(const PxTransform& pose, bool autowake = true) override;

	virtual bool attachShape(PxShape& shape) override;
	virtual void detachShape(PxShape& shape, bool wakeOnLostTouch = true) override;
	virtual	PxU32 getNbShapes()	const override;

	virtual	PxU32 getShapes(PxShape** userBuffer, PxU32 bufferSize, PxU32 startIndex = 0) const override;

	virtual	PxU32 getNbConstraints() const override;
	virtual	PxU32 getConstraints(PxConstraint** userBuffer, PxU32 bufferSize, PxU32 startIndex = 0)	const override;

public:
	
	
};

