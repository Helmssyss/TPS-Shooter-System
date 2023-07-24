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
		
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class USphereComponent *ammoCollision;

		UPROPERTY(EditDefaultsOnly)
			class UProjectileMovementComponent *projectileMovement;
		
	protected:
		virtual void BeginPlay() override;
		virtual void Tick(float DeltaTime) override;

	private:
		FORCEINLINE void DestroyBullet() { this->Destroy(); };

		void AmmoImpactParticle(UPrimitiveComponent *OtherComp, const FHitResult &Hit);
		void zort(class ASoldier *s);

		UPROPERTY(EditDefaultsOnly,Category="Sound")
			class USoundBase *HitSound;
		UPROPERTY(EditDefaultsOnly,Category="Sound")
			class USoundBase *GrassHit;
		UPROPERTY(EditDefaultsOnly,Category="Sound")
			class USoundBase *SoilHit;
		UPROPERTY(EditDefaultsOnly,Category="Sound")
			class USoundAttenuation *SoundAttenuation;

		UPROPERTY(EditDefaultsOnly,Category="Effect")
			class UParticleSystem* bloodEffect;

		UPROPERTY(EditDefaultsOnly,Category="Effect")
			class UParticleSystem* BodyHitEffect;
		
		UPROPERTY(EditDefaultsOnly,Category="Effect")
			class UParticleSystem* GrassHitEffect;
		
		UPROPERTY(EditDefaultsOnly,Category="Effect")
			class UParticleSystem *SoilHitEffect;
		
		UPROPERTY(EditDefaultsOnly,Category="Materials")
			class UMaterialInterface *BloodDecalMaterial;

		UPROPERTY(EditDefaultsOnly,Category="Materials")
			class UMaterialInterface *BulletHoleDecalMaterial;

		UFUNCTION()
			void BulletHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

		FTimerHandle tHandle;
};