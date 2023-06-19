#include "AK47_MAG.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AAK47_MAG::AAK47_MAG(){
	PrimaryActorTick.bCanEverTick = false;
	ak47MagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_AK47MagMesh"));
	ak47MagMesh->SetupAttachment(GetRootComponent());

	ak47MagSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("_AK47SphereCollision"));
	ak47MagSphereCollision->SetupAttachment(ak47MagMesh);
}

void AAK47_MAG::BeginPlay(){
	Super::BeginPlay();
	
	ak47MagSphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	ak47MagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AAK47_MAG::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}