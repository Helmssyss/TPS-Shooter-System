// Fill out your copyright notice in the Description page of Project Settings.


#include "R4_MAG.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

AR4_MAG::AR4_MAG(){
	PrimaryActorTick.bCanEverTick = false;

	ar4MagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("_AR4MagMesh"));
	ar4MagMesh->SetupAttachment(GetRootComponent());

	ar4MagSphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("_AR4SphereCollision"));
	ar4MagSphereCollision->SetupAttachment(ar4MagMesh);
}

void AR4_MAG::BeginPlay(){
	Super::BeginPlay();

	ar4MagSphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
	ar4MagMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void AR4_MAG::Tick(float DeltaTime){
	Super::Tick(DeltaTime);
}