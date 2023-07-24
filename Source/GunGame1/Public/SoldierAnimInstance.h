#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SoldierAnimInstance.generated.h"

UCLASS()
class GUNGAME1_API USoldierAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	public:
		virtual void NativeUpdateAnimation(float DeltaSeconds) override;

		UPROPERTY(BlueprintReadOnly,Category = "Human Speed")
			float characterSpeed;

		UPROPERTY(BlueprintReadOnly,Category = "Human Air")
			bool characterIsAir;
		
		UPROPERTY(BlueprintReadOnly,Category = "Human Hand")
			bool characterRightHandIsValid;

		UPROPERTY(BlueprintReadOnly,Category = "Human Direction")
			float characterDirection;

		UPROPERTY(BlueprintReadOnly,Category = "Human Head Rotation")
			FRotator HeadRotation;
		
		UPROPERTY(BlueprintReadOnly,Category = "Human Head Rotation")
			bool characterOnFire;

		UPROPERTY(BlueprintReadOnly,Category = "Human Head Rotation")
			float characterRotCoordinateYAW;

		UPROPERTY(BlueprintReadOnly,Category = "Human Head Rotation")
			float characterRotCoordinatePITCH;
		
		UPROPERTY(BlueprintReadOnly,Category = "Human IronSight State")
			bool characterIronSight;

	private:
		FRotator  GetLookRotation(class ASoldier* soldier);
};
