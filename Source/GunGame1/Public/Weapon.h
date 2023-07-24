// this base weapon class

#pragma once

#include "CoreMinimal.h"
#include "PickUpItems.h"

#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class GUNGAME1_API AWeapon : public AActor
{
	GENERATED_BODY()
	
	public:
		virtual AWeapon *getChildObject() { return nullptr; }
		virtual int32 getMaxBullet() { return 0; }
		virtual int32 getCurrentBullet() { return atilanMermi; }
		virtual FString GetCurrentWeaponName() { return ""; }
		virtual FName MagBoneName() { return FName(""); }
		
		virtual void SetSelectMaxBullet(FString MagName){}
		virtual void GunReload(){}

		virtual void WeaponHideMag(FName &MagBoneName);
		virtual void WeaponUnHideMag(FName &MagBoneName);

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class USkeletalMeshComponent *weaponMesh;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class USkeletalMeshComponent *hiddenWeaponMesh;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class USphereComponent *sphereCollision;

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class UWidgetComponent* widgetComponent;
		
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			TSubclassOf<APickUpItems> WeaponMagazine;
		
		UPROPERTY(EditDefaultsOnly, Category = "Shot Sounds")
            class USoundBase *WeaponSingleSound;

		UPROPERTY(EditDefaultsOnly, Category = "Shot Sounds")
			class USoundBase *WeaponAutoSound;
		
		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Texture")
			class UTexture2D *WeaponTexture;

		int32 atilanMermi;
		FItemStructer Item;
		enum WeaponFireMode WeaponMode;
		bool bWeponFireChange;

	protected:
        int32 sarjordekiMermi;
		int32 maxMermi;
};

enum WeaponFireMode{
	Single,
	Auto
};