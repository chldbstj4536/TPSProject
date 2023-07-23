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
	UFUNCTION()
	void Zoom();
	UFUNCTION()
	void EquipGranadeGun();
	UFUNCTION()
	void EquipSniperGun();

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
	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class USkeletalMeshComponent* GunMeshComponent;

	UPROPERTY(VisibleAnywhere, Category = GunMesh)
	class UStaticMeshComponent* SniperGunComponent;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIClass;

	class UUserWidget* SniperUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> CrosshairUIClass;
	class UUserWidget* CrosshairUI;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(VisibleAnywhere)
	FVector Direction;

	UPROPERTY(VisibleAnywhere)
	bool bIsZooming = false;

	UPROPERTY(VisibleAnywhere)
	bool bEquipGranadeGun = true;
};
