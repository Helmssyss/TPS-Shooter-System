#include "Weapon.h"
#include "Soldier.h"

#include "GetHandleGunAnimNotify.h"
#include "Components/SkeletalMeshComponent.h"

void UGetHandleGunAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Notify Ani!"));

    ASoldier *casTo = Cast<ASoldier>(MeshComp->GetOwner());
    if(casTo){
        if(casTo->bIsSwap)
            casTo->SwapWeapons();
        else{
            casTo->DetachWeaponFromSocket(casTo->BackWeapons[casTo->BackWeaponsIndex], casTo->BackWeapons[casTo->BackWeaponsIndex]->GetAttachParentSocketName());
            casTo->AttachWeaponToSocket(casTo->BackWeapons[casTo->BackWeaponsIndex], FName("rightHandSocket"));
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("silahin mermisi: %i"),casTo->BackWeapons[casTo->BackWeaponsIndex]->getMaxBullet()));
            casTo->bRightHandIsValid = true;
            casTo->bIsSwap = true;
        }
    }
}