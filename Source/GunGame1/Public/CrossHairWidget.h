// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CrossHairWidget.generated.h"

/**
 * 
 */
UCLASS()
class GUNGAME1_API UCrossHairWidget : public UUserWidget
{
	GENERATED_BODY()
	public:
		virtual void NativeConstruct() override;
		virtual bool Initialize() override;

		UPROPERTY(meta=(BindWidget))
			class UImage *defaultCross;

		UPROPERTY(meta=(BindWidget))
			class UImage *hitCross;
	
	private:
		class ASoldier *castCharacter;

		UFUNCTION()
			void VisibleHitCross();
};
