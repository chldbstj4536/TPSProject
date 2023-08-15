// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerMove.h"

UPlayerMove::UPlayerMove()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerMove::BeginPlay()
{
	Super::BeginPlay();

	MoveComponent->MaxWalkSpeed = WalkSpeed;
}

void UPlayerMove::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	Move();
}

void UPlayerMove::SetupInputBinding(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &UPlayerMove::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &UPlayerMove::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &UPlayerMove::Jump);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &UPlayerMove::Vertical);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &UPlayerMove::Horizontal);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &UPlayerMove::RunPressed);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &UPlayerMove::RunReleased);
}

void UPlayerMove::LookUp(float value)
{
	Owner->AddControllerPitchInput(value);
}

void UPlayerMove::Turn(float value)
{
	Owner->AddControllerYawInput(value);
}

void UPlayerMove::Horizontal(float value)
{
	Direction.Y = value;
}

void UPlayerMove::Vertical(float value)
{
	Direction.X = value;
}

void UPlayerMove::Jump()
{
	Owner->Jump();
}

void UPlayerMove::Move()
{
	FVector NewDirection = FTransform(Owner->GetActorRotation()).TransformVector(Direction);
	NewDirection.Normalize();
	Owner->AddMovementInput(NewDirection);
}

void UPlayerMove::RunPressed()
{
	MoveComponent->MaxWalkSpeed = RunSpeed;
}

void UPlayerMove::RunReleased()
{
	MoveComponent->MaxWalkSpeed = WalkSpeed;
}
