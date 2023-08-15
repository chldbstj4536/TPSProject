// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FInputBindingDelegate, class UInputComponent*);

UCLASS()
class TPSPROJECT_API ATPSPlayer : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:	
	FInputBindingDelegate OnInputBindingDelegate;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* TPSCameraComponent;

	// ÃÑ ½ºÄÌ·¹Å»¸Þ½Ã
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* SniperGunComponent;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* PlayerMoveComponent;

	UPROPERTY(VisibleAnywhere, Category = Component)
	class UPlayerBaseComponent* PlayerFireComponent;

};
