#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "rifle_ar4.generated.h"

UCLASS()
class GUNGAME1_API Arifle_ar4 : public AWeapon
{
	GENERATED_BODY()
	public:
		Arifle_ar4();
		inline virtual int32 getMaxBullet() override { return maxMermi; }
		inline virtual int32 getCurrentBullet() override { return atilanMermi; }
		inline virtual FString GetCurrentWeaponName() override { return FString("AR4"); }
		virtual void GunReload() override;

	private:
        int32 kalanMermi;
};
