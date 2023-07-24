#include "rifle_ak47.h"
#include "PickUpItems.h"
#include "MAG_AK47.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

Arifle_ak47::Arifle_ak47(){

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AK47GUN"));
	weaponMesh->SetupAttachment(GetRootComponent());

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AK47SphereCollision"));
	sphereCollision->SetupAttachment(weaponMesh);

	hiddenWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AK47GUNHiddenGunMesh"));
	hiddenWeaponMesh->SetupAttachment(sphereCollision);

    maxMermi = 0;
    sarjordekiMermi = 30;
    atilanMermi = 30;
}

void Arifle_ak47::BeginPlay(){
    Super::BeginPlay();
    
	hiddenWeaponMesh->SetCollisionProfileName("NoCollision");
	hiddenWeaponMesh->HideBoneByName("b_gun_mag",EPhysBodyOp::PBO_None);
	hiddenWeaponMesh->SetHiddenInGame(true);
	sphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void Arifle_ak47::GunReload(){
    
    if (atilanMermi < sarjordekiMermi){
		int32 eksikMermi = sarjordekiMermi - atilanMermi;
        int32 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}