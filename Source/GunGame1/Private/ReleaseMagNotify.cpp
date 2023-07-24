#include "ReleaseMagNotify.h"

#include "Soldier.h"
#include "PickUpItems.h"
#include "Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UReleaseMagNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    ASoldier *casToSoldier = Cast<ASoldier>(MeshComp->GetOwner());
    if(casToSoldier){
        TArray<USceneComponent *> childrenComponents;
        casToSoldier->GetMesh()->GetChildrenComponents(false, childrenComponents);
        for(auto &&i : childrenComponents){
            AActor *childActor = i->GetOwner();
            if(childActor->GetAttachParentSocketName() == "leftHandSocket"){
                APickUpItems *releaseMag = Cast<APickUpItems>(childActor);
                if(releaseMag){
                    releaseMag->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    releaseMag->StaticMesh->SetSimulatePhysics(false);
                    releaseMag->StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                    releaseMag->Destroy();
                }
            }
        }
        const AWeapon *weapon = casToSoldier->BackWeapons[casToSoldier->BackWeaponsIndex];
        weapon->weaponMesh->SetHiddenInGame(false);
        weapon->hiddenWeaponMesh->SetHiddenInGame(true);
        casToSoldier->BackWeapons[casToSoldier->BackWeaponsIndex]->GunReload();
    }
}
