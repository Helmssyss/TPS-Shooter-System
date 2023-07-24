// this base class includes all collectibles Eg: Mag, Laser, Suppressor...(no weapons)

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ItemStructer.h"

#include "PickUpItems.generated.h"

UCLASS()
class GUNGAME1_API APickUpItems : public AActor{
	GENERATED_BODY()
	
	public:	
		APickUpItems();
		virtual void Tick(float DeltaTime) override;
		virtual APickUpItems *GetItemObject() { return nullptr; }
		virtual FString MagName() { return ItemData.ItemName; }
		FItemStructer ItemData;

		UPROPERTY(EditDefaultsOnly)
			class UStaticMeshComponent *StaticMesh;

		UPROPERTY(EditDefaultsOnly)
			class USphereComponent *SphereCollision;
		
		UPROPERTY(EditDefaultsOnly)
			class UStaticMesh *EmptyMag;
			
		UPROPERTY(EditDefaultsOnly)
			class UStaticMesh *NewMag;

	protected:
		virtual void BeginPlay() override;
};
