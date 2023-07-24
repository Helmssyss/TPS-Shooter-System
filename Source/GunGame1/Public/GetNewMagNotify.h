#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GetNewMagNotify.generated.h"

UCLASS()
class GUNGAME1_API UGetNewMagNotify : public UAnimNotify
{
	GENERATED_BODY()
	public:
		virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
