// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "R4_MAG.generated.h"

UCLASS()
class GUNGAME1_API AR4_MAG : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AR4_MAG();
	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent *ar4MagMesh;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent *ar4MagSphereCollision;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

};
