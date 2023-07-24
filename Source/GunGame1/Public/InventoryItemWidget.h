#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStructer.h"
#include "InventoryItemWidget.generated.h"

UCLASS()
class GUNGAME1_API UInventoryItemWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
		virtual bool Initialize() override;

		uint8 Count;
		FItemStructer S_item;

	private:
		UPROPERTY(meta=(BindWidget))
			class UTextBlock *ItemName;
		
		UPROPERTY(meta=(BindWidget))
			class UTextBlock* ItemCount;

		UFUNCTION()
			FText GetItemText();
		
		UFUNCTION()
			FText GetItemCount();
};
