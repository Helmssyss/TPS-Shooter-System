#include "CrossHairWidget.h"

#include "Soldier.h"
#include "Components/Image.h"

void UCrossHairWidget::NativeConstruct(){
    Super::NativeConstruct();
    castCharacter = Cast<ASoldier>(GetOwningPlayerPawn());
}

bool UCrossHairWidget::Initialize(){
    bool initialize = Super::Initialize();
    if(!initialize){
        return false;
    } 
    // }if (hitCross){
    //     hitCross->BrushDelegate.BindUFunction(this, "VisibleHitCross");
    // }
    
    return true;
}

void UCrossHairWidget::VisibleHitCross(){
    if(castCharacter){
        if(castCharacter->bOnHit){
            GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("sa"));
            defaultCross->SetVisibility(ESlateVisibility::Visible);
            // castCharacter->bOnHit = false;
        }else{
            // GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Cyan, TEXT("ZOOORRTTT"));
            defaultCross->SetVisibility(ESlateVisibility::Hidden);
        }
    }
}