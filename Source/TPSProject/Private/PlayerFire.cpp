// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "EnemyFSM.h"
#include "TPSPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "Bullet.h"
#include <Blueprint/UserWidget.h>
#include <Camera/CameraComponent.h>
#include "PlayerAnim.h"
#include "PlayerFire.h"
#include "TPSProject.h"

UPlayerFire::UPlayerFire()
{
    ConstructorHelpers::FObjectFinder<USoundBase> FireSoundObject(TEXT("'/Game/Resources/SniperGun/Rifle.Rifle'"));
	if (FireSoundObject.Succeeded())
	{
		FireSound = FireSoundObject.Object;
	}
}

void UPlayerFire::BeginPlay()
{
	Super::BeginPlay();

	TPSCameraComponent = Owner->TPSCameraComponent;
	GunMeshComponent = Owner->GunMeshComponent;
    SniperGunComponent = Owner->SniperGunComponent;

	SniperUI = CreateWidget(GetWorld(), SniperUIClass);
	CrosshairUI = CreateWidget(GetWorld(), CrosshairUIClass);
	CrosshairUI->AddToViewport();
}

void UPlayerFire::SetupInputBinding(UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &UPlayerFire::Fire);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &UPlayerFire::Zoom);
	PlayerInputComponent->BindAction(TEXT("GranadeGun"), EInputEvent::IE_Pressed, this, &UPlayerFire::EquipGranadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), EInputEvent::IE_Pressed, this, &UPlayerFire::EquipSniperGun);
}

void UPlayerFire::Fire()
{
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(CameraShakeClass);
	Cast<UPlayerAnim>(Owner->GetMesh()->GetAnimInstance())->PlayAttackAnim();
	UGameplayStatics::PlaySound2D(GetWorld(), FireSound);

	if (bEquipGranadeGun)
	{
        GetWorld()->SpawnActor<ABullet>(BulletClass, Owner->GunMeshComponent->GetSocketTransform(TEXT("FirePosition")));
    }
    else
	{
		FVector StartPosition = TPSCameraComponent->GetComponentLocation();
		FVector EndPosition = TPSCameraComponent->GetForwardVector() * 5000 + StartPosition;

		// LineTrace의 충돌 정보를 담을 변수
		FHitResult HitInfo;
		// 충돌 옵션 설정 변수
		FCollisionQueryParams Params;
		// 자기 자신(플레이어)는 충돌에서 제외
		Params.AddIgnoredActor(Owner);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPosition, EndPosition, ECollisionChannel::ECC_Visibility, Params);

		if (bHit)
        {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitInfo.ImpactPoint);
			UPrimitiveComponent* HitComponent = HitInfo.GetComponent();
			if (HitComponent != nullptr && HitComponent->IsSimulatingPhysics())
			{
				HitComponent->AddForce(-HitInfo.ImpactNormal * HitComponent->GetMass() * 500000);
			}

			AEnemy* Enemy = Cast<AEnemy>(HitInfo.GetActor());
			if (Enemy != nullptr)
			{
				Enemy->FSM->OnDamageProcess();
                PRINT_LOG(TEXT("Enemy is not null"));
			}
			else
			{
                PRINT_LOG(TEXT("Enemy is null"));
			}
        }
	}
}

void UPlayerFire::Zoom()
{
	bIsZooming = !bIsZooming;

	if (bIsZooming)
	{
		CrosshairUI->RemoveFromParent();
		SniperUI->AddToViewport();
        TPSCameraComponent->SetFieldOfView(45);
	}
	else
	{
		SniperUI->RemoveFromParent();
		CrosshairUI->AddToViewport();
        TPSCameraComponent->SetFieldOfView(90);
	}
}

void UPlayerFire::EquipGranadeGun()
{
	if (bEquipGranadeGun)
		return;

	bEquipGranadeGun = true;
	GunMeshComponent->SetVisibility(true);
	SniperGunComponent->SetVisibility(false);

	Owner->OnUsingGrenade(true);
}

void UPlayerFire::EquipSniperGun()
{
	if (!bEquipGranadeGun)
		return;

	bEquipGranadeGun = false;
	GunMeshComponent->SetVisibility(false);
	SniperGunComponent->SetVisibility(true);

	Owner->OnUsingGrenade(false);
}
