#include "Soldier.h"
#include "Ammo.h"
#include "Weapon.h"

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

#define print(str,color) GEngine->AddOnScreenDebugMessage(-1, 2, color, str);

ASoldier::ASoldier(){
	this->PrimaryActorTick.bCanEverTick = true;

	this->movementComp = GetCharacterMovement();

	this->GetCapsuleComponent()->InitCapsuleSize(16.536459, 77.273834);

	this->springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("_SpringArmComponent"));
	this->springArmComponent->SetupAttachment(GetRootComponent());

	this->cameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("_CameraComponent"));
	this->cameraComponent->SetupAttachment(this->springArmComponent,FName("head"));

	this->selectObject = CreateDefaultSubobject<UCapsuleComponent>(TEXT("_SelectObject"));
	this->selectObject->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	this->selectObject->SetWorldScale3D(FVector(0.317500, -0.362500, 6.0525));
	this->selectObject->SetupAttachment(this->cameraComponent);

	this->BaseTurn = 45.0f;
	this->BaseLookUp = 45.0f;
	this->movementComp->MaxWalkSpeed = 200.0f;

	this->ParticleSystem = LoadObject<UParticleSystem>(nullptr, *ParticlePath);

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
}

void ASoldier::Tick(float DeltaSeconds){
	Super::Tick(DeltaSeconds);
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

	GetWorldTimerManager().SetTimer(this->TH_Timer,this,&ASoldier::DestroyCharacter,4,false);
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

int32 ASoldier::getTotalBullet(){
	return BackWeapons[BackWeaponsIndex]->getMaxBullet();
}

FVector ASoldier::GetUnitDirectionVector(const FVector &StartLocation, const FVector &TargetLocation)
{
    FVector Direction = TargetLocation - StartLocation;
    Direction.Normalize();
    return Direction;
}

FString ASoldier::IWeaponName(){return CurrentWeaponName;}
int32 ASoldier::getBulletCount(){return BackWeapons[BackWeaponsIndex]->atilanMermi;}

void ASoldier::RapidFire(){
	if (bRightHandIsValid && fireType == WeaponFireMode::Auto)
		GetWorldTimerManager().SetTimer(this->TH_Timer, this, &ASoldier::Fire, 0.06, true);
	else if(bRightHandIsValid && fireType == WeaponFireMode::Single)
		this->Fire();
}

void ASoldier::Fire(){
	if (bRightHandIsValid){
		if(BackWeapons[BackWeaponsIndex]->atilanMermi > 0){
			this->bOnFire = true;
			const int32 length = 35'000;
			FHitResult ResultHit;
			FVector Start = cameraComponent->GetComponentLocation();
			FVector distance = cameraComponent->GetForwardVector() * length;
			FVector End = Start + distance;

			FCollisionQueryParams QueryParams;
			QueryParams.AddIgnoredActor(this);
			bool trace = GetWorld()->LineTraceSingleByChannel(ResultHit, Start, End, ECollisionChannel::ECC_PhysicsBody, QueryParams);

			FVector selectVector = UKismetMathLibrary::SelectVector(ResultHit.ImpactPoint, ResultHit.TraceEnd, trace); //ResultHit.TraceEnd
			FRotator lookAtRot = UKismetMathLibrary::FindLookAtRotation(BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketLocation(FName("Muzzle")), selectVector);
			FRotator makeRotator = FRotator(lookAtRot.Pitch + 0.3, lookAtRot.Yaw, lookAtRot.Roll);
			FTransform makeTransform = FTransform(makeRotator, BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketLocation(FName("Muzzle")),FVector(1.0f,1.0f,1.0f));

			FActorSpawnParameters params;
			params.Instigator = this;
			params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			AAmmo *bullet = GetWorld()->SpawnActor<AAmmo>(SpawnAmmoClass, makeTransform, params);
			
			if(bullet){
				this->AddControllerPitchInput(FMath::RandRange(-0.3f, 0.0f));
				this->AddControllerYawInput(FMath::RandRange(-0.1f, 0.0f));
				FTransform muzzleSocketTransform = BackWeapons[BackWeaponsIndex]->weaponMesh->GetSocketTransform(FName("Muzzle"));
				BackWeapons[BackWeaponsIndex]->atilanMermi--;
				if (ParticleSystem)
					UGameplayStatics::SpawnEmitterAttached(ParticleSystem,BackWeapons[BackWeaponsIndex]->weaponMesh,"Muzzle",muzzleSocketTransform.GetLocation(),
						this->cameraComponent->GetComponentRotation(),FVector(0.1f, 0.1f, 0.1f),EAttachLocation::Type::KeepWorldPosition);
			}
		}else{
			UGameplayStatics::PlaySound2D(GetWorld(), EmptyGunSound);
		}
	}
}

void ASoldier::EndFire(){
	if (bRightHandIsValid){
		GetWorldTimerManager().ClearTimer(this->TH_Timer);
		this->bOnFire = false;
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
			AttachWeaponToSocket(castWeapon, FName("rightHandSocket"));
		}
	}
}

void ASoldier::SwapWeapons(){
	if(BackWeaponsIndex == 0 && bIsSwap){ // 1. tuş
		if(BackWeapons.Num() == 2 &&BackWeapons[BackWeaponsIndex + 1]->GetAttachParentSocketName() == FName("rightHandSocket")){
				print(TEXT("sirtta 2 adet silah varken 1. tus"), FColor::Blue);
				DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex + 1], BackWeapons[BackWeaponsIndex + 1]->GetAttachParentSocketName());
				AttachWeaponToSocket(BackWeapons[BackWeaponsIndex + 1], FName("rightBackSocket"));
				bRightHandIsValid = false;
		}
		else{
			print(TEXT("1. silahi yerine koydun"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("leftBackSocket"));
			bRightHandIsValid = false;
		}
		bIsSwap = false;

    }else if(BackWeaponsIndex == 1 && bIsSwap){ // 2. tuş
		if(BackWeapons[BackWeaponsIndex - 1]->GetAttachParentSocketName() == FName("rightHandSocket")){
			print(TEXT("sirtta 2 adet silah varken 2. tus"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex - 1], BackWeapons[BackWeaponsIndex - 1]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex - 1], FName("leftBackSocket"));
			bRightHandIsValid = false;
		}else{
			print(TEXT("2. silahi yerine koydun"), FColor::Blue);
			DetachWeaponFromSocket(BackWeapons[BackWeaponsIndex], BackWeapons[BackWeaponsIndex]->GetAttachParentSocketName());
			AttachWeaponToSocket(BackWeapons[BackWeaponsIndex], FName("rightBackSocket"));
			bRightHandIsValid = false;
		}
		bIsSwap = false;
	}
}

void ASoldier::DropItem(){
	if(BackWeapons.Num() > 0)
		for (int32 i = 0; i < BackWeapons.Num();i++){
			try{
				DetachWeaponFromSocket(BackWeapons[i], BackWeapons[i]->GetAttachParentSocketName());
				BackWeapons.RemoveAt(i);
			}catch(const FString& e){
				print(FString::Printf(TEXT("%s"),*e),FColor::Purple);
			}
		}
}

void ASoldier::SelectBackFirstGun(){
	if(BackWeapons.Num() > 0){
		BackWeaponsIndex = 0;
		bRightHandIsValid = false;
		this->GetMesh()->GetAnimInstance()->Montage_Play(SelectGunAnim);
	}		
}

void ASoldier::SelectBackSecondGun(){
	if(BackWeapons.Num() > 0){
		BackWeaponsIndex = 1;
		bRightHandIsValid = false;
		this->GetMesh()->GetAnimInstance()->Montage_Play(SelectGunAnim);
	}
}

void ASoldier::GunReload(){
    // if (atilanMermi < sarjordekiMermi){
	BackWeapons[BackWeaponsIndex]->GunReload();
	this->GetMesh()->GetAnimInstance()->Montage_Play(RifleReloadAnim);
	// 	int32 eksikMermi = sarjordekiMermi - atilanMermi;
    //     int32 eklenecekMermi = FMath::Min(eksikMermi, maxMermi);

    //     atilanMermi += eklenecekMermi;
    //     maxMermi -= eklenecekMermi;
    //     kalanMermi = atilanMermi;
    // }
}

void ASoldier::FireType(){
	if(!bFireType){
		fireType = WeaponFireMode::Auto;
		bFireType = true;
	}else{
		fireType = WeaponFireMode::Single;
		bFireType = false;
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
	AWeapon *castWeapon = Cast<AWeapon>(OtherActor);
	if(castWeapon){
		CurrentWeaponName = castWeapon->GetCurrentWeaponName();
		// castWeapon->weaponMesh->SetMaterial(0, castWeapon->changeMaterial);
	}
}

void ASoldier::SelectObjectEndOverlap(UPrimitiveComponent *OverlappedComponent,AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex){
	CurrentWeaponName = "";
	// AWeapon *castWeapon = Cast<AWeapon>(OtherActor);
	// if(castWeapon){
	// 	CurrentWeaponName = "";
	// 	// castWeapon->weaponMesh->SetMaterial(0, castWeapon->defaultMaterial);
	// }
}

void ASoldier::BodyHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit){
	AAmmo *bullet = Cast<AAmmo>(OtherActor);
	if(bullet){
		this->bOnHit = true;
		if(Health == 0){
			this->GetMesh()->SetAllBodiesSimulatePhysics(true);
			this->GetMesh()->AddImpulse(Hit.ImpactNormal * 5000.0f, Hit.BoneName);
			print(TEXT("Impulse Uygulandi!!!"),FColor::Emerald);
		}
	}
}

void ASoldier::CapsuleToHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, FVector NormalImpulse, const FHitResult &Hit){
	if(Hit.GetActor()){
		const FVector capsuleVelocity = GetCapsuleComponent()->GetComponentVelocity();
		const FVector  hitActorVelocity = Hit.GetActor()->GetVelocity();
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