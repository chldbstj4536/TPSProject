// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/CharacterMovementComponent.h>
#include "PlayerBaseComponent.h"

// Sets default values for this component's properties
UPlayerBaseComponent::UPlayerBaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void UPlayerBaseComponent::InitializeComponent()
{
	Super::InitializeComponent();

	Owner = Cast<ATPSPlayer>(GetOwner());
	MoveComponent = Owner->GetCharacterMovement();

	Owner->OnInputBindingDelegate.AddUObject(this, &UPlayerBaseComponent::SetupInputBinding);
}

// Called when the game starts
void UPlayerBaseComponent::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void UPlayerBaseComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

