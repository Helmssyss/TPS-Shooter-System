#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ItemStructer.h"

#include "rifle_ar4.generated.h"

UCLASS()
class GUNGAME1_API Arifle_ar4 : public AWeapon
{
	GENERATED_BODY()
	public:
		Arifle_ar4();
		
		FORCEINLINE virtual int32 getMaxBullet() override { return maxMermi; }
		FORCEINLINE virtual int32 getCurrentBullet() override { return atilanMermi; }
		FORCEINLINE virtual FString GetCurrentWeaponName() override { return "AR4"; }
		FORCEINLINE virtual FName MagBoneName() override { return FName("b_gun_mag"); }
		FORCEINLINE virtual Arifle_ar4 *getChildObject() override { return this; }
		FORCEINLINE virtual void SetSelectMaxBullet(FString MagName) override { MagName == "ar4_magazine" ? maxMermi += 7 : NULL; }
		
		virtual void GunReload() override;

	private:
		virtual void BeginPlay() override;
		int32 kalanMermi;
		FItemStructer Item;
};
