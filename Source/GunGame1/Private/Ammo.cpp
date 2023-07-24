#include "Ammo.h"
#include "Soldier.h"
#include "EnemyNPC.h"
#include "Weapon.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Materials/MaterialInterface.h"
#include "Components/SkeletalMeshComponent.h"
#include "Blueprint/UserWidget.h"

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
	if(ASoldier* soldier = Cast<ASoldier>(OtherActor)){
		soldier->HitBullet = Hit;
		if(!soldier->bIsDead){
			UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
			if(Hit.BoneName.IsEqual("head") && !soldier->bIsDead){
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
				}
				else
					GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("GOVDE!: %s"),*Hit.BoneName.ToString()));
				
				soldier->Health -= 5;
				soldier->bOnHit = true;
			} 
		}
		UGameplayStatics::SpawnDecalAttached(BloodDecalMaterial, FVector(10, 10, 10), soldier->GetMesh(), Hit.BoneName, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
	
	}else if(AEnemyNPC *enemy = Cast<AEnemyNPC>(OtherActor)){
		if(!enemy->bIsDead){
			UGameplayStatics::PlaySound2D(GetWorld(), HitSound);
			if(Hit.BoneName.IsEqual("head") && !enemy->bIsDead){
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), bloodEffect, FTransform(this->GetActorRotation(), Hit.ImpactPoint, FVector(1, 1, 1)));
				enemy->EnemyHealth = 0;
				enemy->bIsDead = true;

			}else{
				enemy->EnemyHealth == 0 && !enemy->bIsDead ? enemy->bIsDead = true : enemy->EnemyHealth -= 5;
			}
		}
		UGameplayStatics::SpawnDecalAttached(BloodDecalMaterial, FVector(10, 10, 10), enemy->GetMesh(), Hit.BoneName, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
	}

	this->AmmoImpactParticle(OtherComp, Hit);
	this->Destroy();
}

void AAmmo::AmmoImpactParticle(UPrimitiveComponent *OtherComp, const FHitResult &Hit){

	if(UMaterialInterface* MaterialInterface = OtherComp->GetMaterial(0)){
		const UPhysicalMaterial* GetPhysicalMaterial = MaterialInterface->GetPhysicalMaterial();
		FTransform SpawnTransform( FRotator::ZeroRotator, Hit.ImpactPoint, FVector(0.5, 0.5, 0.5));
		switch (GetPhysicalMaterial->SurfaceType){
			case EPhysicalSurface::SurfaceType1:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrassHitEffect, SpawnTransform);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), GrassHit, Hit.ImpactPoint, FRotator::ZeroRotator,1,1,0,SoundAttenuation);
				break;

			case EPhysicalSurface::SurfaceType2:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BodyHitEffect, SpawnTransform);
				break;
			
			case EPhysicalSurface::SurfaceType3:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), SoilHitEffect, SpawnTransform);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoilHit, Hit.ImpactPoint, FRotator::ZeroRotator,1,1,0,SoundAttenuation);
				UGameplayStatics::SpawnDecalAttached(BulletHoleDecalMaterial, FVector(5, 5, 5), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
				break;

			default:
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), GrassHitEffect, SpawnTransform);
				UGameplayStatics::SpawnSoundAtLocation(GetWorld(), SoilHit, Hit.ImpactPoint, FRotator::ZeroRotator,1,1,0,SoundAttenuation);
				UGameplayStatics::SpawnDecalAttached(BulletHoleDecalMaterial, FVector(5, 5, 5), OtherComp, NAME_None, Hit.Location, FRotationMatrix::MakeFromX(Hit.Normal).Rotator(), EAttachLocation::KeepWorldPosition, 10);
				break;
		}
	}
}