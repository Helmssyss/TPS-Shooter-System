// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AK47_MAG.generated.h"

UCLASS()
class GUNGAME1_API AAK47_MAG : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAK47_MAG();

	UPROPERTY(EditDefaultsOnly)
		class UStaticMeshComponent *ak47MagMesh;

	UPROPERTY(EditDefaultsOnly)
		class USphereComponent *ak47MagSphereCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
