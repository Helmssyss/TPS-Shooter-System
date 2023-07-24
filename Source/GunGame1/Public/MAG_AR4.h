#pragma once

#include "CoreMinimal.h"
#include "PickUpItems.h"
#include "MAG_AR4.generated.h"

UCLASS()
class GUNGAME1_API AMAG_AR4 : public APickUpItems{
	GENERATED_BODY()
	AMAG_AR4();

	public:
		FORCEINLINE virtual AMAG_AR4 *GetItemObject() override { return this; }
		virtual FString MagName() override { return ItemData.ItemName; }
};
