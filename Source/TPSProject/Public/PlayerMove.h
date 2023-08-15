// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerMove.generated.h"

/**
 * 
 */
UCLASS()
class TPSPROJECT_API UPlayerMove : public UPlayerBaseComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMove();
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

    void Turn(float value);
    void LookUp(float value);

    void Horizontal(float value);
    void Vertical(float value);
	void RunPressed();
	void RunReleased();
	void Jump();
	void Move();

public:
	UPROPERTY(VisibleAnywhere)
	FVector Direction;

	UPROPERTY(EditAnywhere)
	float WalkSpeed = 200.0f;

	UPROPERTY(EditAnywhere)
	float RunSpeed = 600.0f;

};
