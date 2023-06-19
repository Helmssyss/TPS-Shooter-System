#include "Weapon.h"
#include "Soldier.h"

#include "RifleReloadNotify.h"
#include "Components/SkeletalMeshComponent.h"

void URifleReloadNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    const ASoldier *casToSoldier = Cast<ASoldier>(MeshComp->GetOwner());
    if(casToSoldier){
        // GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("reload"));
        // casToSoldier->BackWeapons[casToSoldier->BackWeaponsIndex]->weaponMesh->UnHideBoneByName("b_gun_mag");
    }
}
