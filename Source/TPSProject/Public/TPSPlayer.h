// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TPSPlayer.generated.h"

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

	UFUNCTION()
    void LookUp(float value);
	UFUNCTION()
    void Turn(float value);
	UFUNCTION()
    void Horizontal(float value);
	UFUNCTION()
    void Vertical(float value);
	UFUNCTION()
    void Fire();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, Category = Camera)
	class UCameraComponent* TPSCameraComponent;

	// ÃÑ ½ºÄÌ·¹Å»¸Þ½Ã
	UPROPERTY(VisibleAnywhere, Category = GunMesh);
	class USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ABullet> BulletClass;

	//float Speed;
	FVector Direction;
};
