#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Character.h"
#include "EnemyNPC.generated.h"


UCLASS()
class GUNGAME1_API AEnemyNPC : public  ACharacter
{
	GENERATED_BODY()

	public:
		AEnemyNPC();

		bool bIsDead;
		uint8 EnemyHealth = 100;

	protected:
		virtual void BeginPlay() override;

		class AAIController *AIController;
		class UNavigationSystemV1* NavSYS;
		
		UPROPERTY(EditDefaultsOnly)
			class UPawnSensingComponent *PawnSensing;
	
	private:

		UFUNCTION()
			void EnemySeeSoldier(APawn *Pawn);
		
		UFUNCTION()
			void BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
