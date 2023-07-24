#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemStructer.h"

#include "InventoryWidget.generated.h"

UCLASS()
class GUNGAME1_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;

		UPROPERTY(EditDefaultsOnly)
			TSubclassOf<class UInventoryItemWidget> InventoryItemWidget;
		
		UPROPERTY(meta=(BindWidget))
			class UScrollBox* ScrollBox_;

	private:
		FItemStructer S_item;
		class ASoldier *interface;
};
