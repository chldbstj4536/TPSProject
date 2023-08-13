// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyFSM.h"
#include "EnemyAnim.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UEnemyAnim : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, Category=FSMEvent)
	void OnEndAttackAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayDamageAnim(FName SectionName);

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
    EEnemyState AnimState;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly);
	bool bAttackPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bDieDone;
};
