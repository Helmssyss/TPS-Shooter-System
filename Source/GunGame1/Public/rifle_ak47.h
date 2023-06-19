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
		inline virtual int32 getMaxBullet() override { return maxMermi; }
		inline virtual int32 getCurrentBullet() override { return atilanMermi; }
		inline virtual FString GetCurrentWeaponName() override { return FString("AK47"); }
		virtual void GunReload() override;

	private:
        int32 kalanMermi;
		virtual void BeginPlay() override;
};