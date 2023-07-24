#include "GetHandleGunAnimNotify.h"
#include "Weapon.h"
#include "Soldier.h"
#include "ItemStructer.h"

#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimSequenceBase.h"
#include "Kismet/GameplayStatics.h"

void UGetHandleGunAnimNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Notify Ani!"));

    ASoldier *casTo = Cast<ASoldier>(MeshComp->GetOwner());
    if(casTo){
        if(casTo->bIsSwap)
            casTo->SwapWeapons();
        else{
            if(casTo->BackWeapons.IsValidIndex(casTo->BackWeaponsIndex)){
                AWeapon *weapon = casTo->BackWeapons[casTo->BackWeaponsIndex];
                if(weapon){
                    casTo->DetachWeaponFromSocket(weapon, weapon->GetAttachParentSocketName());
                    casTo->AttachWeaponToSocket(weapon, FName("rightHandSocket"));
                    casTo->bRightHandIsValid = true;
                    casTo->bIsSwap = true;
                    UGameplayStatics::PlaySound2D(MeshComp->GetOwner()->GetWorld(), casTo->GetWeaponSound);
                }
            }
        }
    }
}