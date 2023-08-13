// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyFSM.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle,
	Move,
	Attack,
	Damage,
	Die
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TPSPROJECT_API UEnemyFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnemyFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void IdleState();
	void MoveState();
	void AttackState();
	void DamageState();
	void DieState();

	void OnDamageProcess();
	bool GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Destination);

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::Idle;

	UPROPERTY()
    class UEnemyAnim* EnemyAnim;

	UPROPERTY(EditDefaultsOnly, Category=FSM)
	float IdleDelayTime = 2;
	float CurrentTime = 0;

	UPROPERTY(EditAnywhere, Category=FSM)
	float AttackRange = 150;

	UPROPERTY(EditAnywhere, Category=FSM)
	float AttackDelayTime = 2;

	UPROPERTY(EditAnywhere, Category=FSM)
	float DamageDelayTime = 1;

	UPROPERTY(EditAnywhere, Category=FSM)
	float DieSpeed = 50;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	int32 HP = 10.0f;

	UPROPERTY(VisibleAnywhere, Category=FSM)
	class ATPSPlayer* Target = nullptr;
	
	UPROPERTY()
    class AEnemy* Me = nullptr;
	
	UPROPERTY()
	class AAIController* AIController;

	FVector RandomPosition;
};
