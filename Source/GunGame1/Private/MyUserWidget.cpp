#include "MyUserWidget.h"

#include "Soldier.h"
#include "Weapon.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UMyUserWidget::NativeConstruct(){
    Super::NativeConstruct();
    castCharacter = Cast<ASoldier>(GetOwningPlayerPawn());
}

bool UMyUserWidget::Initialize(){
    bool intialize = Super::Initialize();
    if(!intialize)
        return false;

    if(WeaponImage_1 && WeaponImage_2){
        WeaponImage_1->BrushDelegate.BindUFunction(this, "WeaponSlotImage1");
        WeaponImage_2->BrushDelegate.BindUFunction(this, "WeaponSlotImage2");
    }
    return true;
}

FSlateBrush UMyUserWidget::WeaponSlotImage1(){
    if(castCharacter){
        if(castCharacter->BackWeapons.IsValidIndex(0)){
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("'%s' Silahinin Imagesi"), *castCharacter->BackWeapons[0]->GetName()));
            FSlateBrush Brush = WeaponImage_1->Brush;
            Brush.SetResourceObject(castCharacter->BackWeapons[0]->WeaponTexture);
            WeaponImage_1->SetBrush(Brush);
            WeaponImage_1->SetVisibility(ESlateVisibility::Visible);
            return Brush;
        }
    }
    FSlateBrush _;
    WeaponImage_1->SetVisibility(ESlateVisibility::Hidden);
    return _;
}

FSlateBrush UMyUserWidget::WeaponSlotImage2(){
    if(castCharacter){
        if(castCharacter->BackWeapons.IsValidIndex(1)){
            FSlateBrush Brush = WeaponImage_2->Brush;
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, FString::Printf(TEXT("'%s' Silahinin Imagesi"), *castCharacter->BackWeapons[1]->GetName()));
            Brush.SetResourceObject(castCharacter->BackWeapons[1]->WeaponTexture);
            WeaponImage_2->SetBrush(Brush);
            WeaponImage_2->SetVisibility(ESlateVisibility::Visible);
            return Brush;
        }
    }
    FSlateBrush _;
    WeaponImage_2->SetVisibility(ESlateVisibility::Hidden);
    return _;
}