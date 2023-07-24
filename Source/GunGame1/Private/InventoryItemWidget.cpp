#include "InventoryItemWidget.h"
#include "Components/TextBlock.h"


bool UInventoryItemWidget::Initialize(){
    bool initialize = Super::Initialize();
    if(!initialize) return false;

    if(ItemName){
        ItemName->TextDelegate.BindUFunction(this, "GetItemText");
        ItemCount->TextDelegate.BindUFunction(this, "GetItemCount");
    }

    return true;
}

FText UInventoryItemWidget::GetItemText(){
    return FText::FromString(S_item.ItemName);
}

FText UInventoryItemWidget::GetItemCount(){
    return FText::FromString(FString::FromInt(Count));
}