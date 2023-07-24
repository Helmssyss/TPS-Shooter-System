#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ItemStructer.h"
#include "Soldier.generated.h"

UCLASS()
class GUNGAME1_API ASoldier : public ACharacter
{
	GENERATED_BODY()

    public:
        ASoldier();
        
        virtual void Tick(float DeltaSeconds) override;
        virtual void BeginPlay() override;

        void DeathSoldier();
        void Fire();
        void RapidFire();
        void EndFire();
        void SwapWeapons();
        void DetachWeaponFromSocket(class AWeapon* Weapon,FName SocketName);
        void AttachWeaponToSocket(class AWeapon* Weapon, FName SocketName);
        void OpenInventory();

        int32 getBulletCount();
        int32 getTotalBullet();
        FVector GetUnitDirectionVector(const FVector& StartLocation, const FVector& TargetLocation);
        FString IWeaponName();

        class UCharacterMovementComponent* movementComp;
        class UInventoryWidget *inventoryWidget;

        uint8 Health = 100;
        uint8 BackWeaponsIndex;

        bool bIsDead;
        bool bIsSwap;
        bool bIronSight;

        enum WeaponFireMode fireType;
        FHitResult HitBullet;

        UPROPERTY(BlueprintReadOnly)
            bool bOnHit;
            
        UPROPERTY(BlueprintReadOnly)
            bool bOnFire;

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

        UPROPERTY(BlueprintReadOnly)
            bool bRightHandIsValid;

        UPROPERTY(BlueprintReadOnly, Category = "Weapons")
            TArray<class AWeapon*> BackWeapons;

        UPROPERTY(EditDefaultsOnly)
            class USoundBase *EmptyGunSound;
            
        UPROPERTY(EditDefaultsOnly)
            class USoundBase *EquipSound;

        UPROPERTY(EditDefaultsOnly)
            class USoundBase *GetWeaponSound;

        UPROPERTY(EditDefaultsOnly)
            class USoundBase *WeaponLowerSound;

        UPROPERTY(BlueprintReadOnly)
            TMap<FName, bool> SocketStatusMap;
        
        UPROPERTY(BlueprintReadOnly,Category = "Soldier Inventory")
            TMap<FItemStructer, uint8> Inventory;

        UPROPERTY(EditDefaultsOnly)
            class UParticleSystem* ParticleSystem;
        
    private:
        virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

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
        void UpdateInventoryItemCount(FItemStructer &Item, class APickUpItems *pickUpItem, FString GetGunName);
        void TPSCameraAngleLimit();
        
        FORCEINLINE void IronSight() { bIronSight == true ? bIronSight = false : bIronSight = true; }
        FORCEINLINE void DestroyCharacter() { this->Destroy();}
        
        FHitResult EquipGunLineTraceHitActor(const int16_t &traceDistance = 470, const float &Thickness = 5.0f, const FColor &lineColor = FColor::Red);

        float BaseTurn;
        float BaseLookUp;
        
        bool bInRagdoll;
        bool bInventoryIsOpen;
        bool bFireType;

        FTimerHandle TH_Timer;
        FTimerHandle TH_RagdollTimer;

        const FString ParticlePath = TEXT("D:/UnrealEngineProjects/GunGame1/Content/ThirdPartyFolders/MilitaryWeapSilver/FX/P_AssaultRifle_MuzzleFlash.P_AssaultRifle_MuzzleFlash");
        FString CurrentWeaponName;
        class UMyUserWidget *UserWidget;
        
        UPROPERTY(EditDefaultsOnly,Category = "Camera")
            TSubclassOf<class UCameraShakeBase> CameraShakeClass;

        UFUNCTION()
            void SelectObjectBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

        UFUNCTION()
            void SelectObjectEndOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex);
        
        UFUNCTION()
		    void BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
        
        UFUNCTION()
            void CapsuleToHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit);
};
