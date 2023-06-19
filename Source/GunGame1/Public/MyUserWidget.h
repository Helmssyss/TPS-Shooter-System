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
			class UTextBlock *WeaponName;
		
		UPROPERTY(meta=(BindWidget))
			class UTextBlock *AmmoCount;
			
		UPROPERTY(meta=(BindWidget))
			class UTextBlock *FireMode;
	
	private:
		class ASoldier* castCharacter;
		UFUNCTION()
			FText CurrentWeaponName();
		
		UFUNCTION()
			FText CurrentWeaponCount();

		// UFUNCTION()
		// 	FText CurrentFireMode();
};
