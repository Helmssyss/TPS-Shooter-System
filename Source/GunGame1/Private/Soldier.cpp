#include "Soldier.h"
#include "Ammo.h"
#include "Weapon.h"
#include "PickUpItems.h"
#include "InventoryWidget.h"

#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/InputComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"

#define print(str,color) GEngine->AddOnScreenDebugMessage(-1, 10, color, str);

ASoldier::ASoldier(){
	this->PrimaryActorTick.bCanEverTick = true;

	this->movementComp = GetCharacterMovement();

	this->GetCapsuleComponent()->InitCapsuleSize(16.536459, 77.273834);

	this->springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("_SpringArmComponent"));
	this->springArmComponent->SetupAttachment(GetMesh(),"head");

	this->cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("_CameraComponent"));
	this->cameraComponent->SetupAttachment(this->springArmComponent);

	this->selectObject = CreateDefaultSubobject<UCapsuleComponent>(TEXT("_SelectObject"));
	this->selectObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->selectObject->SetWorldScale3D(FVector(0.317500, -0.362500, 6.0525));
	this->selectObject->SetupAttachment(this->cameraComponent);

	this->BaseTurn = 45.0f;
	this->BaseLookUp = 45.0f;
	this->movementComp->MaxWalkSpeed = 200.0f;

	SocketStatusMap.Add(FName("leftBackSocket"), false);
	SocketStatusMap.Add(FName("rightBackSocket"), false);
	SocketStatusMap.Add(FName("rightHandSocket"), false);
}

void ASoldier::BeginPlay(){
	Super::BeginPlay();
	
	this->selectObject->OnComponentBeginOverlap.AddDynamic(this, &ASoldier::SelectObjectBeginOverlap);
	this->selectObject->OnComponentEndOverlap.AddDynamic(this, &ASoldier::SelectObjectEndOverlap);
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ASoldier::CapsuleToHit);
	this->GetMesh()->OnComponentHit.AddDynamic(this, &ASoldier::BodyHit);
	this->TPSCameraAngleLimit();
}

void ASoldier::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
	if (bRightHandIsValid){
		bUseControllerRotationYaw = true; // TRUE
		movementComp->bOrientRotationToMovement = false;
	}else{
		bUseControllerRotationYaw = false;
		movementComp->bOrientRotationToMovement = true;
	}

	if(bInRagdoll){
		const FVector socketLoc = GetMesh()->GetSocketLocation(FName("pelvis"));
		GetCapsuleComponent()->SetWorldLocation(socketLoc + FVector(0.0f, 0.0f, 77.0f));
	}
}

void ASoldier::DeathSoldier(){ 
	for (int32 i = 0; i < BackWeapons.Num();i++)
		DetachWeaponFromSocket(BackWeapons[i], BackWeapons[i]->GetAttachParentSocketName());

	this->GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"),true);
	this->EndFire();
	this->movementComp->SetMovementMode(EMovementMode::MOVE_None);
	this->GetCapsuleComponent()->DestroyComponent();
	this->GetController()->UnPossess();
	this->bRightHandIsValid = false;

	GetWorldTimerManager().SetTimer(this->TH_Timer,this,&ASoldier::DestroyCharacter,10,false);
}

void ASoldier::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent){
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("SelectBackFirstGun", IE_Pressed, this, &ASoldier::SelectBackFirstGun);
	PlayerInputComponent->BindAction("SelectBackSecondGun", IE_Pressed, this, &ASoldier::SelectBackSecondGun);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ASoldier::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASoldier::StopSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASoldier::RapidFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASoldier::EndFire);
	PlayerInputComponent->BindAction("IronSight", IE_Pressed, this, &ASoldier::IronSight);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASoldier::GunReload);
	PlayerInputComponent->BindAction("FireType", IE_Pressed, this, &ASoldier::FireType);
	PlayerInputComponent->BindAction("OpenInventory", IE_Pressed, this, &ASoldier::OpenInventory);

	PlayerInputComponent->BindAction("EquipItem", IE_Pressed, this, &ASoldier::EquipItem);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &ASoldier::DropItem);

	PlayerInputComponent->BindAxis("ForwardBackward", this, &ASoldier::ForwardBackward);
	PlayerInputComponent->BindAxis("LeftRight", this, &ASoldier::LeftRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ASoldier::LookUp);
	PlayerInputComponent->BindAxis("TurnRightLeft", this, &ASoldier::TurnRightLeft);
}

void ASoldier::ForwardBackward(float value){
	if (Controller && value != 0.0f){
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRot(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRot).GetUnitAxis(EAxis::X);		
		AddMovementInput(direction, value);
	}
}

void ASoldier::LeftRight(float value){
	if (Controller && value != 0.0f){
		const FRotator rotation = Controller->GetControlRotation();
		const FRotator yawRot(0, rotation.Yaw, 0);
		const FVector direction = FRotationMatrix(yawRot).GetUnitAxis(EAxis::Y);
		AddMovementInput(direction, value);
	}
}

void ASoldier::LookUp(float value){
	AddControllerPitchInput(value * BaseLookUp * GetWorld()->GetDeltaSeconds());
}

void ASoldier::TurnRightLeft(float value){
	AddControllerYawInput(value * BaseTurn * GetWorld()->GetDeltaSeconds());
}

void ASoldier::Sprint(){movementComp->MaxWalkSpeed = 500.0f;}
void ASoldier::StopSprint(){movementComp->MaxWalkSpeed = 250.0f;}

void ASoldier::TPSCameraAngleLimit(){
	APlayerCameraManager *camera = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
	camera->ViewPitchMax = 80.0f;
	camera->ViewPitchMin = -70.0f;
}

int32 ASoldier::getTotalBullet(){
	if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
		return BackWeapons[BackWeaponsIndex]->getMaxBullet();
	}
	return -1;
}

FVector ASoldier::GetUnitDirectionVector(const FVector &StartLocation, const FVector &TargetLocation){
    FVector Direction = TargetLocation - StartLocation;
    Direction.Normalize();
    return Direction;
}

FString ASoldier::IWeaponName(){return CurrentWeaponName;}

int32 ASoldier::getBulletCount(){
	if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
		return BackWeapons[BackWeaponsIndex]->atilanMermi;
	}
	return -1;
}

void ASoldier::RapidFire(){
	if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
		if(BackWeapons[BackWeaponsIndex]->atilanMermi > 0){
			if (bRightHandIsValid && BackWeapons[BackWeaponsIndex]->WeaponMode == WeaponFireMode::Auto)
				GetWorldTimerManager().SetTimer(this->TH_Timer, this, &ASoldier::Fire, 0.1, true);

			else if(bRightHandIsValid && BackWeapons[BackWeaponsIndex]->WeaponMode == WeaponFireMode::Single)
				GetWorldTimerManager().SetTimer(this->TH_Timer, this, &ASoldier::Fire, 0.001, false);
		}else{
			UGameplayStatics::PlaySound2D(this->GetWorld(), EmptyGunSound);
		}
	}
}

void ASoldier::Fire(){
	if (bRightHandIsValid){
		this->bOnFire = true;
		FHitResult ResultHit;
		const int32 length = 35000;
		const FVector Start = cameraComponent->GetComponentLocation();
		const FVector distance = cameraComponent->GetForwardVector() * length;
		const FVector End = Start + distance;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);
		bool trace = this->GetWorld()->LineTraceSingleByChannel(ResultHit, Start, End, ECollisionChannel::ECC_PhysicsBody, QueryParams);

		const FVector selectVector = UKismetMathLibrary::SelectVector(ResultHit.ImpactPoint, ResultHit.TraceEnd, trace);
		const FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketLocation(FName("Muzzle")), selectVector);
		const FRotator makeRotator = FRotator(lookAtRot.Pitch + 0.3, lookAtRot.Yaw, lookAtRot.Roll);
		const FTransform makeTransform = FTransform(makeRotator, BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketLocation(FName("Muzzle")),FVector(1.0f,1.0f,1.0f));

		FActorSpawnParameters params;
		params.Instigator = this;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
		const AAmmo *bullet = GetWorld()->SpawnActor<AAmmo>(SpawnAmmoClass, makeTransform, params);

		if(bullet){
			this->AddControllerPitchInput(FMath::RandRange(-0.3f, 0.0f));
			this->AddControllerYawInput(FMath::RandRange(-0.1f, 0.0f));
			FTransform muzzleSocketTransform = BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketTransform(FName("Muzzle"));
			BackWeapons[BackWeaponsIndex]->atilanMermi--;
			UGameplayStatics::SpawnEmitterAttached(ParticleSystem,BackWeapons[BackWeaponsIndex]->weaponMesh,"Muzzle",muzzleSocketTransform.GetLocation(),this->cameraComponent->GetComponentRotation(),FVector(1.0f, 1.0f, 1.0f),EAttachLocation::Type::KeepWorldPosition);
			UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, GetActorLocation(),5.0f,0.0f);
			switch (BackWeapons[BackWeaponsIndex]->WeaponMode){
				case WeaponFireMode::Auto:
					UGameplayStatics::PlaySound2D(GetWorld(), BackWeapons[BackWeaponsIndex]->WeaponAutoSound);
					break;
			
				default:
					UGameplayStatics::PlaySound2D(GetWorld(), BackWeapons[BackWeaponsIndex]->WeaponSingleSound);
					break;
			}
		}
	}
}

void ASoldier::EndFire(){
	if (bRightHandIsValid){
		GetWorldTimerManager().ClearTimer(this->TH_Timer);
		this->bOnFire = false;
	}
}

void ASoldier::SwapWeapons(){
	if(BackWeaponsIndex == 0 && bIsSwap){ // 1. tuş
		if(BackWeapons.Num() == 2 && BackWeapons[BackWeaponsIndex + 1]->GetAttachParentSocketName() == FName("rightHandSocket")){
			print(TEXT("sirtta 2 adet silah varken 1. tus"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex + 1], BackWeapons[BackWeaponsIndex + 1]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex + 1], FName("rightBackSocket"));
			UGameplayStatics::PlaySound2D(GetWorld(), WeaponLowerSound);
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("rightHandSocket"));
			bRightHandIsValid = true;
			UGameplayStatics::PlaySound2D(GetWorld(), GetWeaponSound);
		}else{
			print(TEXT("1. silahi yerine koydun"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("leftBackSocket"));
			bRightHandIsValid = false;
			bIsSwap = false;
			UGameplayStatics::PlaySound2D(GetWorld(), WeaponLowerSound);
		}

    }else if(BackWeaponsIndex == 1 && bIsSwap){ // 2. tuş
		if(BackWeapons[BackWeaponsIndex - 1]->GetAttachParentSocketName() == FName("rightHandSocket")){
			print(TEXT("sirtta 2 adet silah varken 2. tus"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex - 1], BackWeapons[BackWeaponsIndex - 1]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex - 1], FName("leftBackSocket"));
			UGameplayStatics::PlaySound2D(GetWorld(), WeaponLowerSound);
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("rightHandSocket"));
			bRightHandIsValid = true;
			UGameplayStatics::PlaySound2D(GetWorld(), GetWeaponSound);

		}else{
			print(TEXT("2. silahi yerine koydun"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("rightBackSocket"));
			bRightHandIsValid = false;
			bIsSwap = false;
			UGameplayStatics::PlaySound2D(GetWorld(), WeaponLowerSound);
		}
	}
}

void ASoldier::SelectBackFirstGun(){
	if(BackWeapons.Num() > 0){
		BackWeaponsIndex = 0;
		if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
			bRightHandIsValid = false;
			this->GetMesh()->GetAnimInstance()->Montage_Play(SelectGunAnim);
		}
	}		
}

void ASoldier::SelectBackSecondGun(){
	if(BackWeapons.Num() > 0){
		BackWeaponsIndex = 1;
		if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
			bRightHandIsValid = false;
			this->GetMesh()->GetAnimInstance()->Montage_Play(SelectGunAnim);
		}
	}
}

void ASoldier::GunReload(){
	if(bRightHandIsValid && BackWeapons[BackWeaponsIndex]->getMaxBullet() != 0){
		this->GetMesh()->GetAnimInstance()->Montage_Play(RifleReloadAnim);
	}
}

void ASoldier::FireType(){
	if(!bFireType){
		BackWeapons[BackWeaponsIndex]->WeaponMode = WeaponFireMode::Auto;
		bFireType = true;
		BackWeapons[BackWeaponsIndex]->bWeponFireChange = bFireType;
	}else{
		BackWeapons[BackWeaponsIndex]->WeaponMode = WeaponFireMode::Single;
		bFireType = false;
		BackWeapons[BackWeaponsIndex]->bWeponFireChange = bFireType;
	}
}

void ASoldier::EquipItem(){
	AActor *lineTraceHitActor = this->EquipGunLineTraceHitActor(500,5.0f,FColor::Green).GetActor();
	if (AWeapon *castWeapon = Cast<AWeapon>(lineTraceHitActor)){
		DrawDebugBox(GetWorld(), lineTraceHitActor->GetActorLocation(), FVector(5, 5, 5), FColor::Cyan, false, 5.0f);
		if(!SocketStatusMap[FName("leftBackSocket")]){
			AttachWeaponToSocket(castWeapon, FName("leftBackSocket"));
			BackWeapons.Add(castWeapon);

		}else if(!SocketStatusMap[FName("rightBackSocket")]){
			AttachWeaponToSocket(castWeapon, FName("rightBackSocket"));
			BackWeapons.Add(castWeapon);

		}else if(SocketStatusMap[FName("rightBackSocket")] && SocketStatusMap[FName("leftBackSocket")] && !SocketStatusMap[FName("rightHandSocket")]){
			bRightHandIsValid = true;
			BackWeaponsIndex = 2;
			AttachWeaponToSocket(castWeapon, FName("rightHandSocket"));
			BackWeapons.Add(castWeapon);
		}
		UGameplayStatics::PlaySound2D(GetWorld(), EquipSound);
	}else if(APickUpItems* items = Cast<APickUpItems>(lineTraceHitActor)){
		TArray<USceneComponent *> ChildrenComponents;
		GetMesh()->GetChildrenComponents(false, ChildrenComponents);

		for(auto && i : SocketStatusMap)
			for(auto &&j : ChildrenComponents)
				if(i.Key == j->GetAttachSocketName())
					UpdateInventoryItemCount(items->ItemData, items, j->GetName());
	}
}

void ASoldier::UpdateInventoryItemCount(FItemStructer &Item, APickUpItems *pickUpItem, FString GetGunName){
	const TMap<FString,FString> Gun = { {"ar4_magazine","AR4GUN"} , { "ak47_magazine" , "AK47GUN"} };
	uint8 *getValue = Inventory.Find(Item);
	
	for(auto && i : Gun){
		const FString GunPart = i.Key;
		const FString GunName = i.Value;
		
		if(Item.ItemName == GunPart && GetGunName == GunName){
			print(FString::Printf(TEXT("%s : %s"), *GunPart, *GunName),FColor::Emerald);
			
			for(auto && j : BackWeapons)
				j->weaponMesh->GetName() == GetGunName ? j->SetSelectMaxBullet(GunPart) : NULL;

			pickUpItem->Destroy();
			UGameplayStatics::PlaySound2D(GetWorld(), EquipSound);
			getValue ? Inventory.Add(Item, (*getValue)+1) : Inventory.Add(Item, 1);

		}
	}
}

void ASoldier::DropItem(){
	print(FString::Printf(TEXT("%i"),BackWeaponsIndex), FColor::Emerald);
	if(BackWeapons.Num() > 0){
		if(SocketStatusMap[FName("rightHandSocket")]){
			print(TEXT("Eldeki Silah Atildi"), FColor::Emerald);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
			BackWeapons.RemoveAt(BackWeaponsIndex);
			bRightHandIsValid = false;
			bIsSwap = false;

		}else{
			if(BackWeapons.IsValidIndex(BackWeaponsIndex)){
				DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
				BackWeapons.RemoveAt(BackWeaponsIndex);
			}
		}
		BackWeaponsIndex = 0;
	}
}

void ASoldier::AttachWeaponToSocket(AWeapon *Weapon, FName SocketName){
	if(Weapon){
		SocketStatusMap[SocketName] = true;

		Weapon->weaponMesh->SetActive(false);
		Weapon->weaponMesh->SetSimulatePhysics(false);
		Weapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Weapon->sphereCollision->SetActive(false);
		Weapon->sphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Weapon->AttachToComponent(this->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
	}
}

void ASoldier::OpenInventory(){}

void ASoldier::DetachWeaponFromSocket(AWeapon *Weapon,FName SocketName){
	if(Weapon != nullptr && Weapon->GetAttachParentSocketName() != NAME_None){
		SocketStatusMap[SocketName] = false;

		Weapon->weaponMesh->SetActive(true);
        Weapon->weaponMesh->SetSimulatePhysics(true);
        Weapon->weaponMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    	Weapon->sphereCollision->SetActive(true);
		Weapon->sphereCollision->SetCollisionProfileName(FName("OverlapAllDynamic"));
		Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	}
}

FHitResult ASoldier::EquipGunLineTraceHitActor(const int16_t &traceDistance, const float &Thickness, const FColor &lineColor){
	FHitResult hit;
	FVector start = this->cameraComponent->GetComponentLocation();
	FVector end = start + (this->cameraComponent->GetComponentRotation().Vector() * traceDistance);
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByObjectType(hit, start, end, FCollisionObjectQueryParams::AllDynamicObjects, params);
	DrawDebugLine(GetWorld(), start, end, lineColor, false, Thickness);
	return hit;
}

void ASoldier::SelectObjectBeginOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult){
	if(AWeapon *castWeapon = Cast<AWeapon>(OtherActor)){
		CurrentWeaponName = castWeapon->GetCurrentWeaponName();
		// castWeapon->weaponMesh->SetMaterial(0, castWeapon->changeMaterial);
	}
}

void ASoldier::SelectObjectEndOverlap(UPrimitiveComponent *OverlappedComponent,AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex){
	CurrentWeaponName = "";
}

void ASoldier::BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	if(AAmmo *bullet = Cast<AAmmo>(OtherActor)){
		this->bOnHit = true;
		if(bIsDead){
			this->GetMesh()->SetAllBodiesSimulatePhysics(true);
			this->GetMesh()->AddImpulse(Hit.ImpactNormal * 5000.0f, Hit.BoneName);
		}
	}
}

void ASoldier::CapsuleToHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
	if(Hit.GetActor()){
		const FVector capsuleVelocity = GetCapsuleComponent()->GetComponentVelocity();
		const FVector  hitActorVelocity = OtherComp->GetPhysicsLinearVelocity(Hit.BoneName); //Hit.GetActor()->GetVelocity();
		const float  resultValue = (capsuleVelocity - hitActorVelocity).Size();
		const float *resultVelocity = &resultValue;
		this->CharacterHitRagdoll(resultVelocity);
	}
}

void ASoldier::CharacterHitRagdoll(const float *resultVelocity){
	if(*resultVelocity >= 1200.0f){
		GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), true);
		GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(FName("pelvis"), 1.0f);
		this->bInRagdoll = true;
		this->movementComp->SetMovementMode(EMovementMode::MOVE_None);
		GetWorldTimerManager().SetTimer(TH_RagdollTimer, this, &ASoldier::CharacterHitRagdollRecover, 2, false);
	}
}

void ASoldier::CharacterHitRagdollRecover(){
	GetMesh()->SetAllBodiesBelowSimulatePhysics(FName("pelvis"), false);
	this->bInRagdoll = false;
	if(GetMesh()->AttachTo(GetCapsuleComponent(),NAME_None,EAttachLocation::SnapToTarget,true))
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -77.0f), FRotator(0.0f, -90.000183,0.0f));
	GetWorldTimerManager().ClearTimer(this->TH_RagdollTimer);
	this->movementComp->SetMovementMode(EMovementMode::MOVE_Walking);
}