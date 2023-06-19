#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Ammo.generated.h"

UCLASS()
class GUNGAME1_API AAmmo : public AActor
{
	GENERATED_BODY()
	
	public:	
		AAmmo();
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class UStaticMeshComponent *ammoMesh;
		
		UPROPERTY(EditDefaultsOnly)
			class USphereComponent *ammoCollision;

		UPROPERTY(EditDefaultsOnly)
			class UProjectileMovementComponent *projectileMovement;
		
		UPROPERTY(EditDefaultsOnly,Category="Effect")
			class UParticleSystem* bloodEffect;
		
	protected:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
		class USoundBase* HitSound;
		
		FTimerHandle tHandle;
		FORCEINLINE void DestroyBullet() { this->Destroy(); };
		void zort(class ASoldier *s);
		
		UFUNCTION()
		void BulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};