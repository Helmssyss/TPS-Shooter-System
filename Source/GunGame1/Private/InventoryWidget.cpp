#include "InventoryWidget.h"

#include "Soldier.h"
#include "InventoryItemWidget.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UInventoryWidget::NativeConstruct(){
    Super::NativeConstruct();
    interface = Cast<ASoldier>(UGameplayStatics::GetPlayerCharacter(this,0));
    if(interface){
        for(auto &&i : interface->Inventory){
            UInventoryItemWidget *itemWidget = Cast<UInventoryItemWidget>(CreateWidget(GetWorld(), InventoryItemWidget));
            itemWidget->S_item = i.Key;
            if(uint8 *find = interface->Inventory.Find(i.Key)){
                itemWidget->Count = (*find);
            }
            ScrollBox_->AddChild(itemWidget);
        }
    }
}