#include "Weapon.h"
#include "PickUpItems.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#define print(str) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Blue,str);

void AWeapon::WeaponHideMag(FName &MagBoneName){
	weaponMesh->HideBoneByName(MagBoneName, EPhysBodyOp::PBO_None);
	weaponMesh->MarkRenderStateDirty();
}

void AWeapon::WeaponUnHideMag(FName &MagBoneName){
	weaponMesh->UnHideBoneByName(MagBoneName);
	weaponMesh->MarkRenderStateDirty();
}