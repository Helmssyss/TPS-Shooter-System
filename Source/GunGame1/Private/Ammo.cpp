#include "Ammo.h"
#include "Soldier.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"

AAmmo::AAmmo(){
	PrimaryActorTick.bCanEverTick = false;

	ammoCollision = CreateDefaultSubobject<USphereComponent>(TEXT("_AmmoCollision"));
	ammoCollision->IgnoreActorWhenMoving(this->GetInstigator(), true);
	ammoCollision->SetupAttachment(GetRootComponent());

	ammoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_AmmoMesh"));
	ammoMesh->SetupAttachment(ammoCollision);

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("_ProjectileMovement"));
	projectileMovement->SetUpdatedComponent(GetRootComponent());
}

void AAmmo::BeginPlay(){
	Super::BeginPlay();
	ammoCollision->OnComponentHit.AddDynamic(this, &AAmmo::BulletHit);
}

void AAmmo::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}

void AAmmo::BulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	// DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 5.0f);
	if(ASoldier* soldier = Cast<ASoldier>(OtherActor)){
		soldier->HitBullet = Hit;
		if(Hit.BoneName.IsEqual("Head") && !soldier->bIsDead){
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodEffect, FTransform(this->GetActorRotation(), Hit.ImpactPoint, FVector(1, 1, 1)));
			
			soldier->Health = 0;
			soldier->bIsDead = true;
			soldier->bIsDead ? soldier->DeathSoldier() : nullptr;

			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Purple, FString::Printf(TEXT("KAFADAN!")));

		}else{
			GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%i"),soldier->Health));
			if(soldier->Health == 0 && !soldier->bIsDead){
				soldier->DeathSoldier();
				soldier->bIsDead = true;
				
			}else{
				GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("GOVDE!: %s"),*Hit.BoneName.ToString()));
			}
			soldier->Health -= 5;
		}
	}
	this->Destroy();
}