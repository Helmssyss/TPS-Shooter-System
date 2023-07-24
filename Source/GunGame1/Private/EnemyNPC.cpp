#include "EnemyNPC.h"
#include "Ammo.h"

#include "AIController.h"
#include "Perception/PawnSensingComponent.h"
#include "NavigationSystem.h"
#include "Components/SkeletalMeshComponent.h"

AEnemyNPC::AEnemyNPC(){
	PrimaryActorTick.bCanEverTick = false;

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("_PawnSensing"));
	PawnSensing->SetPeripheralVisionAngle((double)32.638489);
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyNPC::EnemySeeSoldier);
}

void AEnemyNPC::BeginPlay(){
	Super::BeginPlay();

	FNavLocation NavLoc;
	UWorld *getWorld = this->GetWorld();
	this->GetMesh()->OnComponentHit.AddDynamic(this, &AEnemyNPC::BodyHit);
	NavSYS = FNavigationSystem::GetCurrent<UNavigationSystemV1>(getWorld);
	NavSYS->GetRandomReachablePointInRadius(GetActorLocation(), 10'000, NavLoc);

	AIController = Cast<AAIController>(this->GetController());
	if(AIController)
		AIController->MoveToLocation(NavLoc.Location);
}

void AEnemyNPC::EnemySeeSoldier(APawn *Pawn){
	AIController->MoveToActor(Pawn);
}

void AEnemyNPC::BodyHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
		if(AAmmo *bullet = Cast<AAmmo>(OtherActor)){
			if(bIsDead){
				this->GetMesh()->SetAllBodiesSimulatePhysics(true);
				this->GetMesh()->AddImpulse(Hit.ImpactNormal * 5000.0f, Hit.BoneName);
		}
	}
}
