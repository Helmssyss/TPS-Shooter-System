// Fill out your copyright notice in the Description page of Project Settings.


#include "MyUserWidget.h"
#include "Components/TextBlock.h"
#include "Soldier.h"


void UMyUserWidget::NativeConstruct(){
    Super::NativeConstruct();
    castCharacter = Cast<ASoldier>(GetOwningPlayerPawn());
}

bool UMyUserWidget::Initialize(){
    bool intialize = Super::Initialize();
    if(!intialize)
        return false;

    if(WeaponName && AmmoCount && FireMode){
        WeaponName->TextDelegate.BindUFunction(this, "CurrentWeaponName");
        AmmoCount->TextDelegate.BindUFunction(this, "CurrentWeaponCount");
        // FireMode->TextDelegate.BindUFunction(this, "CurrentFireMode");
    }
    return true;
}

FText UMyUserWidget::CurrentWeaponName(){
    if(castCharacter){
        return FText::FromString(castCharacter->IWeaponName());
    }else{
        return FText::FromString("NULL");
    }
    return FText::FromString("NULL");
}

FText UMyUserWidget::CurrentWeaponCount(){
    if(castCharacter && castCharacter->bRightHandIsValid){
        TArray<FStringFormatArg> args;
        args.Add(FStringFormatArg(castCharacter->getBulletCount()));
        args.Add(FStringFormatArg(castCharacter->getTotalBullet()));
        // GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Green, TEXT("Mermi sayaci geldi"));
        return FText::FromString(FString::Format(TEXT("{0}/{1}"),args));
    }
    return FText::FromString("");
}

// FText UMyUserWidget::CurrentFireMode(){
//     if(castCharacter && castCharacter->bRightHandIsValid){
//         if(!castCharacter->bFireType)
//             return FText::FromString("Mode: Single");
//         else
//             return FText::FromString("Mode: Auto");
//     }
//     return FText::FromString("");
// }