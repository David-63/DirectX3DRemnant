#include "pch.h"
#include "CColCallBack.h"
#include "CGameObject.h"
#include "CCollider3D.h"


void CColCallBack::onContact(const PxContactPairHeader& pairHeader, const PxContactPair* pPairs, PxU32 iNbPairs)
{
    for (PxU32 i = 0; i < iNbPairs; i++)
    {
        const PxContactPair& cp = pPairs[i];

        if (cp.events & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
            {
                CGameObject* pContact = static_cast<CGameObject*>(pairHeader.actors[0]->userData);
                CGameObject* pOther = static_cast<CGameObject*>(pairHeader.actors[1]->userData);

                if (nullptr != pContact && nullptr != pOther)
                {
                    if (pContact->Collider3D())
                        pContact->Collider3D()->OnCollisionEnter(pOther->Collider3D());

                    if (pOther->Collider3D())
                        pOther->Collider3D()->OnCollisionEnter(pContact->Collider3D());
                }
            }
        }

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_PERSISTS)
        {
            if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
            {
                CGameObject* pContact = static_cast<CGameObject*>(pairHeader.actors[0]->userData);
                CGameObject* pOther = static_cast<CGameObject*>(pairHeader.actors[1]->userData);

                if (nullptr != pContact && nullptr != pOther)
                {
                    if (pContact->Collider3D())
                        pContact->Collider3D()->OnCollisionStay(pOther->Collider3D());

                    if (pOther->Collider3D())
                        pOther->Collider3D()->OnCollisionStay(pContact->Collider3D());
                }
            }
        }

        else if (cp.events & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            if (pairHeader.actors[0]->userData && pairHeader.actors[1]->userData)
            {
                CGameObject* pContact = static_cast<CGameObject*>(pairHeader.actors[0]->userData);
                CGameObject* pOther = static_cast<CGameObject*>(pairHeader.actors[1]->userData);

                if (nullptr != pContact && nullptr != pOther)
                {
                    if (pContact->Collider3D())
                        pContact->Collider3D()->OnCollisionExit(pOther->Collider3D());

                    if (pOther->Collider3D())
                        pOther->Collider3D()->OnCollisionExit(pContact->Collider3D());
                }
            }
        }
    }
}

void CColCallBack::onTrigger(PxTriggerPair* pairs, PxU32 count)
{
    for (PxU32 i = 0; i < count; i++)
    {
        const PxTriggerPair& pair = pairs[i];

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_FOUND)
        {
            if (pair.triggerActor && pair.otherActor)
            {
                CGameObject* pTrigger = static_cast<CGameObject*>(pair.triggerActor->userData);
                CGameObject* pOther = static_cast<CGameObject*>(pair.otherActor->userData);

                if (nullptr != pTrigger && nullptr != pOther)
                {
                    if (pTrigger->Collider3D())
                        pTrigger->Collider3D()->OnTriggerEnter(pOther->Collider3D());

                    if (pOther->Collider3D())
                        pOther->Collider3D()->OnTriggerEnter(pTrigger->Collider3D());
                }
            }
        }

        if (pair.status & PxPairFlag::eNOTIFY_TOUCH_LOST)
        {
            if (pair.triggerActor && pair.otherActor)
            {
                CGameObject* pTrigger = static_cast<CGameObject*>(pair.triggerActor->userData);
                CGameObject* pOther = static_cast<CGameObject*>(pair.otherActor->userData);

                if (nullptr != pTrigger && nullptr != pOther)
                {
                    if (pTrigger->Collider3D())
                        pTrigger->Collider3D()->OnTriggerExit(pOther->Collider3D());

                    if (pOther->Collider3D())
                        pOther->Collider3D()->OnTriggerExit(pTrigger->Collider3D());
                }

            }
        }
    }
}