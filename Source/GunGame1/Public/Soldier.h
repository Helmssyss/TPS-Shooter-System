#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Soldier.generated.h"

UCLASS()
class GUNGAME1_API ASoldier : public ACharacter
{
	GENERATED_BODY()

    public:
        ASoldier();
        
        virtual void BeginPlay() override;
        virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
        virtual void Tick(float DeltaSeconds) override;
        FString IWeaponName();

        void DeathSoldier();
        void Fire();
        void RapidFire();
        void EndFire();
        void SwapWeapons();
        void DetachWeaponFromSocket(class AWeapon* Weapon,FName SocketName);
        void AttachWeaponToSocket(class AWeapon* Weapon, FName SocketName);
        int32 getBulletCount();
        int32 getTotalBullet(); //{ return maxMermi; }
        FVector GetUnitDirectionVector(const FVector& StartLocation, const FVector& TargetLocation);

        class UCharacterMovementComponent* movementComp;

        uint8_t Health = 100;
        uint8_t BackWeaponsIndex;

        bool bIsDead;
        bool bOnFire;
        bool bIsSwap;
        bool bIronSight;
        bool bFireType;

        enum WeaponFireMode fireType;
        FHitResult HitBullet;

        UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
            class USpringArmComponent* springArmComponent;

        UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
            class UCameraComponent* cameraComponent;

        UPROPERTY(EditDefaultsOnly)
            class UCapsuleComponent* selectObject;
            
        UPROPERTY(EditDefaultsOnly, Category="Ammo")
            TSubclassOf<class AAmmo> SpawnAmmoClass;

        UPROPERTY(EditDefaultsOnly)
            class UAnimMontage* SelectGunAnim;
            
        UPROPERTY(EditDefaultsOnly)
            class UAnimMontage* RifleReloadAnim;

        UPROPERTY(EditDefaultsOnly)
            class UAnimationAsset *deathAnim;

        UPROPERTY(BlueprintReadOnly)
            bool bRightHandIsValid;
        
        UPROPERTY(BlueprintReadOnly)
            bool bOnHit;

        UPROPERTY(BlueprintReadOnly, Category = "Weapons")
            TArray<class AWeapon*> BackWeapons;

        UPROPERTY(EditDefaultsOnly)
            class USoundBase *EmptyGunSound;

        UPROPERTY(BlueprintReadOnly)
            TMap<FName, bool> SocketStatusMap;
    private:
        void ForwardBackward(float value);
        void LeftRight(float value);
        void LookUp(float value);
        void TurnRightLeft(float value);
        void Sprint();
        void StopSprint();
        void EquipItem();
        void DropItem();
        void SelectBackFirstGun();
        void SelectBackSecondGun();
        void CharacterHitRagdoll(const float *resultVelocity);
        void CharacterHitRagdollRecover();
        void GunReload();
        void FireType();
        inline void IronSight() { bIronSight == true ? bIronSight = false : bIronSight = true; }
        inline void DestroyCharacter() { this->Destroy();}
        FHitResult EquipGunLineTraceHitActor(const int16_t &traceDistance = 470, const float &Thickness = 5.0f, const FColor &lineColor = FColor::Red);

        float BaseTurn;
        float BaseLookUp;
        
        bool bInRagdoll;

        // int32 sarjordekiMermi = 25;
        // int32 maxMermi = 100;
        // int32 atilanMermi = 25;
        // int32 kalanMermi;

        FTimerHandle TH_Timer;
        FTimerHandle TH_RagdollTimer;

        const FString ParticlePath = TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion");
        FString CurrentWeaponName;

        class UParticleSystem *ParticleSystem;

        UFUNCTION()
        void SelectObjectBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

        UFUNCTION()
        void SelectObjectEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
        
        UFUNCTION()
		void BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
        
        UFUNCTION()
        void CapsuleToHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
};
