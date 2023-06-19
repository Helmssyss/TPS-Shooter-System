#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

#define print(str) GEngine->AddOnScreenDebugMessage(-1,2,FColor::Blue,str);

AWeapon::AWeapon(){
	PrimaryActorTick.bCanEverTick = false;

	weaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("_GunMesh"));
	weaponMesh->SetupAttachment(GetRootComponent());

	sphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("_SphereCollision"));
	sphereCollision->SetupAttachment(weaponMesh);

	changeMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("changeMaterial"));
	defaultMaterial = CreateDefaultSubobject<UMaterialInterface>(TEXT("defaultMaterial"));

}

void AWeapon::BeginPlay(){
	Super::BeginPlay();
	
	sphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}