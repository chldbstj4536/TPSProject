// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnim.h"
#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>

void UPlayerAnim::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    ATPSPlayer* owner = Cast<ATPSPlayer>(TryGetPawnOwner());

    if (owner != nullptr)
    {
        Direction = FVector::DotProduct(owner->GetVelocity(), owner->GetActorRightVector());
        Speed = FVector::DotProduct(owner->GetVelocity(), owner->GetActorForwardVector());
        bIsInAir = owner->GetCharacterMovement()->IsFalling();
    }
}

void UPlayerAnim::PlayAttackAnim()
{
    Montage_Play(AttackAnimMontage);
}
