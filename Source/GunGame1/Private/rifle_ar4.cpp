#include "rifle_ar4.h"
#include "MAG_AR4.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"

Arifle_ar4::Arifle_ar4(){

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AR4GUN"));
	weaponMesh->SetupAttachment(GetRootComponent());

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("AR4SphereCollision"));
	sphereCollision->SetupAttachment(weaponMesh);

	hiddenWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("AR4GUNHiddenGunMesh"));
	hiddenWeaponMesh->SetupAttachment(sphereCollision);

    maxMermi = 0;
    sarjordekiMermi = 25;
    atilanMermi = 25;
}

void Arifle_ar4::BeginPlay(){
    Super::BeginPlay();
    
	hiddenWeaponMesh->SetCollisionProfileName("NoCollision");
	hiddenWeaponMesh->HideBoneByName("b_gun_mag",EPhysBodyOp::PBO_None);
	hiddenWeaponMesh->SetHiddenInGame(true);
	sphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void Arifle_ar4::GunReload(){

    if (atilanMermi < sarjordekiMermi){
		int32 eksikMermi = sarjordekiMermi - atilanMermi;
        int32 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

        atilanMermi += eklenecekMermi;
        maxMermi -= eklenecekMermi;
        kalanMermi = atilanMermi;
    }
}