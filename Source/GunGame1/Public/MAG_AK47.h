#pragma once

#include "CoreMinimal.h"
#include "PickUpItems.h"
#include "MAG_AK47.generated.h"

UCLASS()
class GUNGAME1_API AMAG_AK47 : public APickUpItems{
	GENERATED_BODY()
	AMAG_AK47();
	
	public:
		FORCEINLINE virtual AMAG_AK47 *GetItemObject() override { return this; }
		virtual FString MagName() override { return ItemData.ItemName; }
};
