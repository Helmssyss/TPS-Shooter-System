#include "GetNewMagNotify.h"
#include "Weapon.h"
#include "Soldier.h"
#include "PickUpItems.h"

#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

void UGetNewMagNotify::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation){
    const ASoldier *casToSoldier = Cast<ASoldier>(MeshComp->GetOwner());
    if(casToSoldier){
        const AWeapon *weapon = casToSoldier->BackWeapons[casToSoldier->BackWeaponsIndex];
        FActorSpawnParameters params;
        params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        APickUpItems *spawnMagazine = MeshComp->GetOwner()->GetWorld()->SpawnActor<APickUpItems>(weapon->WeaponMagazine, casToSoldier->GetMesh()->GetSocketTransform("leftHandSocket"), params);
        if(spawnMagazine){
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("Dolu Sarjor Alindi"));
            spawnMagazine->StaticMesh->SetStaticMesh(spawnMagazine->NewMag);
            spawnMagazine->StaticMesh->SetSimulatePhysics(false);
            spawnMagazine->StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
            spawnMagazine->AttachToComponent(casToSoldier->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("leftHandSocket"));
        }
    }
}