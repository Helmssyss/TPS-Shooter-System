#pragma once

#include "CoreMinimal.h"
#include "ItemStructer.generated.h"

USTRUCT(BlueprintType)
struct FItemStructer{
    GENERATED_BODY()

    public:
        FString ItemName;
        uint8 ItemCount;
};

FORCEINLINE uint32 GetTypeHash(const FItemStructer& Item){
	return HashCombine(GetTypeHash(Item.ItemName), GetTypeHash(Item.ItemCount));
}

FORCEINLINE bool operator==(const FItemStructer& A, const FItemStructer& B){
    return A.ItemName == B.ItemName && A.ItemCount == B.ItemCount;
}