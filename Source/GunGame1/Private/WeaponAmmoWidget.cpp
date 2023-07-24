// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponAmmoWidget.h"

#include "Soldier.h"
#include "Weapon.h"

#include "Components/TextBlock.h"

void UWeaponAmmoWidget::NativeConstruct(){
    Super::NativeConstruct();
    castCharacter = Cast<ASoldier>(GetOwningPlayerPawn());
}

bool UWeaponAmmoWidget::Initialize(){
    bool intialize = Super::Initialize();
    if(!intialize)
        return false;

    if(WeaponName && AmmoCount){
        WeaponName->TextDelegate.BindUFunction(this, "CurrentWeaponName");
        AmmoCount->TextDelegate.BindUFunction(this, "CurrentWeaponCount");
        FireMode->TextDelegate.BindUFunction(this, "CurrentFireMode");
    }
    return true;
}

FText UWeaponAmmoWidget::CurrentWeaponName(){
    if(castCharacter){
        return FText::FromString(castCharacter->IWeaponName());
    }else{
        return FText::FromString("NULL");
    }
    return FText::FromString("NULL");
}

FText UWeaponAmmoWidget::CurrentWeaponCount(){
    if(castCharacter && castCharacter->bRightHandIsValid && castCharacter->BackWeapons.IsValidIndex(castCharacter->BackWeaponsIndex)){
        TArray<FStringFormatArg> args;
        args.Add(FStringFormatArg(castCharacter->getBulletCount()));
        args.Add(FStringFormatArg(castCharacter->getTotalBullet()));
        return FText::FromString(FString::Format(TEXT("{0}/{1}"),args));
    }
    return FText::FromString("");
}

FText UWeaponAmmoWidget::CurrentFireMode(){
    if(castCharacter && castCharacter->bRightHandIsValid && castCharacter->BackWeapons.IsValidIndex(castCharacter->BackWeaponsIndex)){
        if(!castCharacter->BackWeapons[castCharacter->BackWeaponsIndex]->bWeponFireChange)
            return FText::FromString("Mode: Single");
        else
            return FText::FromString("Mode: Auto");
    }
    return FText::FromString("");
}