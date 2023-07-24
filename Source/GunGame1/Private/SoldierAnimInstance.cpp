#include "SoldierAnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Soldier.h"
#include "Kismet/KismetMathLibrary.h"

void USoldierAnimInstance::NativeUpdateAnimation(float DeltaSeconds){
    Super::NativeUpdateAnimation(DeltaSeconds);
    ASoldier *TryPawn = Cast<ASoldier>(this->TryGetPawnOwner());
    if(TryPawn != nullptr){
        characterSpeed = TryPawn->GetVelocity().Size();
        characterIsAir = TryPawn->GetMovementComponent()->IsFalling();
        characterRightHandIsValid = TryPawn->bRightHandIsValid;
        characterOnFire = TryPawn->bOnFire;
        characterIronSight = TryPawn->bIronSight;
        
        characterDirection = CalculateDirection(TryPawn->GetVelocity(), TryPawn->GetActorRotation());
        this->GetLookRotation(TryPawn);
        FRotator NormalizedDeltaRotator = UKismetMathLibrary::NormalizedDeltaRotator(TryPawn->GetActorRotation(), TryPawn->GetControlRotation());
        characterRotCoordinateYAW = NormalizedDeltaRotator.Yaw;
        characterRotCoordinatePITCH = NormalizedDeltaRotator.Pitch;
    }
}

FRotator USoldierAnimInstance::GetLookRotation(ASoldier* soldier){
    const FTransform pawnTransform = soldier->GetActorTransform();
    FRotator pawnControlRotation = soldier->GetControlRotation();
    FRotator inverseTransform = UKismetMathLibrary::InverseTransformRotation(pawnTransform, pawnControlRotation);
    FRotator makeRotator = FRotator(0.0f, FMath::ClampAngle(inverseTransform.Yaw, -70.0f, 70.0f), FMath::ClampAngle((inverseTransform.Pitch * -1.0f), -70.0f, 70.0f));
    if(FMath::Abs(inverseTransform.Yaw ) > 170.0f){
        HeadRotation = FRotator::ZeroRotator;
        soldier->EndFire();
    }else
        HeadRotation = FMath::RInterpTo(HeadRotation, makeRotator, GetWorld()->GetDeltaSeconds(), 9.0f);
    return HeadRotation;
}
