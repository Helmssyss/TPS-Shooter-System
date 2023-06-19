#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class GUNGAME1_API AWeapon : public AActor
{
	GENERATED_BODY()
	
	public:	
		AWeapon();
		virtual AWeapon *getChildObject() { return nullptr; }
		virtual int32 getMaxBullet() { return 0; }
		virtual int32 getCurrentBullet() { return atilanMermi; }
		virtual FString GetCurrentWeaponName() { return ""; }
		virtual void GunReload(){}

		UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
			class USkeletalMeshComponent *weaponMesh;

		UPROPERTY(EditDefaultsOnly)
			class USphereComponent *sphereCollision;

		UPROPERTY(EditDefaultsOnly)
			class UWidgetComponent* widgetComponent;

		UPROPERTY(EditDefaultsOnly, Category="Materials")
			class UMaterialInterface  *changeMaterial;
		
		UPROPERTY(EditDefaultsOnly, Category="Materials")
			class UMaterialInterface  *defaultMaterial;

		int32 atilanMermi;

	protected:
		virtual void BeginPlay() override;
        int32 sarjordekiMermi;
		int32 maxMermi;
};

enum WeaponFireMode{
	Single,
	Auto
};