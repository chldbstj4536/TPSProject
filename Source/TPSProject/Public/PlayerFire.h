// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBaseComponent.h"
#include "PlayerFire.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(PlayerComponent), meta = (BlueprintSpawnableComponent))
class TPSPROJECT_API UPlayerFire : public UPlayerBaseComponent
{
	GENERATED_BODY()
public:
	UPlayerFire();

	virtual void BeginPlay() override;

	virtual void SetupInputBinding(class UInputComponent* PlayerInputComponent) override;

    void Fire();
	void Zoom();
	void EquipGranadeGun();
	void EquipSniperGun();

public:
	UPROPERTY()
	class UCameraComponent* TPSCameraComponent;
	UPROPERTY()
	class USkeletalMeshComponent* GunMeshComponent;
	UPROPERTY()
	class UStaticMeshComponent* SniperGunComponent;

	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditDefaultsOnly, Category = SniperUI)
	TSubclassOf<class UUserWidget> SniperUIClass;
	class UUserWidget* SniperUI;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> CrosshairUIClass;
	class UUserWidget* CrosshairUI;

	UPROPERTY(EditDefaultsOnly)
	class UParticleSystem* HitParticle;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UCameraShakeBase> CameraShakeClass;

	UPROPERTY(EditDefaultsOnly)
    class USoundBase* FireSound;

	UPROPERTY(VisibleAnywhere)
	bool bIsZooming = false;

	UPROPERTY(VisibleAnywhere)
	bool bEquipGranadeGun = true;
};
