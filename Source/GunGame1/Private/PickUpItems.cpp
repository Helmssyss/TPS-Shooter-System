#include "PickUpItems.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

APickUpItems::APickUpItems()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_Mesh"));
	StaticMesh->SetupAttachment(GetRootComponent());

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("_SphereCollision"));
	SphereCollision->SetupAttachment(StaticMesh);
}

void APickUpItems::BeginPlay(){
	Super::BeginPlay();

	SphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void APickUpItems::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

}

