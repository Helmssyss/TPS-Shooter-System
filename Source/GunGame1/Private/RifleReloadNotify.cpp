#include "RifleReloadNotify.h"
#include "Weapon.h"
#include "Soldier.h"
#include "PickUpItems.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void URifleReloadNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    Super::Notify(MeshComp, Animation);

    const ASoldier *casToSoldier = Cast<ASoldier>(MeshComp->GetOwner());
    if(casToSoldier){
        const AWeapon *weapon = casToSoldier->BackWeapons[casToSoldier->BackWeaponsIndex];
        weapon->weaponMesh->SetHiddenInGame(true);
        weapon->hiddenWeaponMesh->SetHiddenInGame(false);
        FActorSpawnParameters params;
        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        APickUpItems *spawnMagazine = MeshComp->GetOwner()->GetWorld()->SpawnActor<APickUpItems>(weapon->WeaponMagazine, weapon->weaponMesh->GetSocketTransform("Magazine"), params);
        
        if(spawnMagazine){
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Sarjor Spawn Oldu"));
            spawnMagazine->StaticMesh->SetStaticMesh(spawnMagazine->EmptyMag);
            spawnMagazine->StaticMesh->SetSimulatePhysics(false);
            spawnMagazine->StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            spawnMagazine->AttachToComponent(casToSoldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("leftHandSocket"));
        }
    }
}