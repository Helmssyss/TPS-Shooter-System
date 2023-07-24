#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyUserWidget.generated.h"

UCLASS()
class GUNGAME1_API UMyUserWidget : public UUserWidget
{
	GENERATED_BODY()

	public:
		virtual void NativeConstruct() override;
		virtual bool Initialize() override;


		UPROPERTY(meta=(BindWidget))
			class UImage* WeaponImage_1;

		UPROPERTY(meta=(BindWidget))
			class UImage *WeaponImage_2;

	private:
		class ASoldier* castCharacter;
		
		UFUNCTION()
			FSlateBrush WeaponSlotImage1();

		UFUNCTION()
			FSlateBrush WeaponSlotImage2();
};
