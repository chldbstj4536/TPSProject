// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayAttackAnim();
	
public:
	UPROPERTY(EditDefaultsOnly)
	class UAnimMontage* AttackAnimMontage;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
    float Direction = 0;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	float Speed = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bIsInAir = false;
};
