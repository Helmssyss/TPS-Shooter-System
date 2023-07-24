#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"

#include "rifle_ak47.generated.h"

UCLASS()
class GUNGAME1_API Arifle_ak47 : public AWeapon
{
	GENERATED_BODY()
	public:
		Arifle_ak47();

		FORCEINLINE virtual int32 getMaxBullet() override { return maxMermi; }
		FORCEINLINE virtual int32 getCurrentBullet() override { return atilanMermi; }
		FORCEINLINE virtual FString GetCurrentWeaponName() override { return "AK47"; }
		FORCEINLINE virtual FName MagBoneName() override { return FName("b_gun_mag"); }
		FORCEINLINE virtual Arifle_ak47 *getChildObject() override { return this; }
		FORCEINLINE virtual void SetSelectMaxBullet(FString MagName) override { MagName == "ak47_magazine" ? maxMermi += 5 : NULL; }
		
		virtual void GunReload() override;

	private:
		virtual void BeginPlay() override;
		int32 kalanMermi;
		FItemStructer Item;

};