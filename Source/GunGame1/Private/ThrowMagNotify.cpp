#include "ThrowMagNotify.h"

#include "Soldier.h"
#include "PickUpItems.h"
#include "Weapon.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UThrowMagNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation){
    ASoldier *casToSoldier = Cast<ASoldier>(MeshComp->GetOwner());
    if(casToSoldier){
        TArray<USceneComponent *> childrenComponents;
        casToSoldier->GetMesh()->GetChildrenComponents(false, childrenComponents);
        for(auto &&i : childrenComponents){
            AActor *childActor = i->GetOwner();
            if(childActor->GetAttachParentSocketName() == "leftHandSocket"){
                APickUpItems *throwMag = Cast<APickUpItems>(childActor);
                if(throwMag){
                    throwMag->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
                    throwMag->StaticMesh->SetSimulatePhysics(true);
                    throwMag->StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

                    throwMag->StaticMesh->AddAngularImpulse(casToSoldier->GetMesh()->GetSocketTransform("leftHandSocket").GetTranslation(), "leftHandSocket");
                    throwMag->SetLifeSpan(3);
                }
            }
        }
    }
}