// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Components/ArrowComponent.h>
#include <Blueprint/UserWidget.h>
#include <Particles/ParticleSystem.h>
#include <Kismet/GameplayStatics.h>
#include "Bullet.h"

// Sets default values
ATPSPlayer::ATPSPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->SetRelativeLocation(FVector(0, 70, 90));
	SpringArmComponent->TargetArmLength = 400;
	SpringArmComponent->bUsePawnControlRotation = true;

	TPSCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	TPSCameraComponent->SetupAttachment(SpringArmComponent);
	TPSCameraComponent->bUsePawnControlRotation = false;

	bUseControllerRotationYaw = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (SkeletalMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshFinder.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));
	}

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMeshComponent->SetupAttachment(GetMesh());
    ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("'/Game/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (GunMesh.Succeeded())
	{
        GunMeshComponent->SetSkeletalMesh(GunMesh.Object);
		GunMeshComponent->SetRelativeLocation(FVector(-14, 52, 120));
	}
	GunMeshComponent->SetVisibility(true);

	SniperGunComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMesh"));
	SniperGunComponent->SetupAttachment(GetMesh());
	ConstructorHelpers::FObjectFinder<UStaticMesh> SniperMesh(TEXT("'/Game/Resources/SniperGun/sniper1.sniper1'"));
	if (SniperMesh.Succeeded())
	{
		SniperGunComponent->SetStaticMesh(SniperMesh.Object);
		SniperGunComponent->SetRelativeLocation(FVector(-22, 55, 120));
		SniperGunComponent->SetRelativeScale3D(FVector(0.15));
	}
	SniperGunComponent->SetVisibility(false);
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	SniperUI = CreateWidget(GetWorld(), SniperUIClass);
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FTransform Transform(GetActorRotation());

	FVector NewDirection = Transform.TransformVector(Direction);
	NewDirection.Normalize();
	AddMovementInput(NewDirection);
}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ATPSPlayer::LookUp);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ATPSPlayer::Turn);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ATPSPlayer::Jump);
	PlayerInputComponent->BindAxis(TEXT("Vertical"), this, &ATPSPlayer::Vertical);
	PlayerInputComponent->BindAxis(TEXT("Horizontal"), this, &ATPSPlayer::Horizontal);
	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &ATPSPlayer::Fire);
	PlayerInputComponent->BindAction(TEXT("Zoom"), EInputEvent::IE_Pressed, this, &ATPSPlayer::Zoom);
	PlayerInputComponent->BindAction(TEXT("GranadeGun"), EInputEvent::IE_Pressed, this, &ATPSPlayer::EquipGranadeGun);
	PlayerInputComponent->BindAction(TEXT("SniperGun"), EInputEvent::IE_Pressed, this, &ATPSPlayer::EquipSniperGun);
}

void ATPSPlayer::LookUp(float value)
{
	AddControllerPitchInput(value);
}

void ATPSPlayer::Turn(float value)
{
	AddControllerYawInput(value);
}

void ATPSPlayer::Horizontal(float value)
{
	Direction.Y = value;
}

void ATPSPlayer::Vertical(float value)
{
	Direction.X = value;
}

void ATPSPlayer::Fire()
{
	if (bEquipGranadeGun)
	{
        GetWorld()->SpawnActor<ABullet>(BulletClass, GunMeshComponent->GetSocketTransform(TEXT("FirePosition")));
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
		Params.AddIgnoredActor(this);

		bool bHit = GetWorld()->LineTraceSingleByChannel(HitInfo, StartPosition, EndPosition, ECollisionChannel::ECC_Visibility, Params);

		if (bHit)
        {
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitParticle, HitInfo.ImpactPoint);
        }
	}
}

void ATPSPlayer::Zoom()
{
	bIsZooming = !bIsZooming;

	if (bIsZooming)
	{
		SniperUI->AddToViewport();
        TPSCameraComponent->SetFieldOfView(45);
	}
	else
	{
		SniperUI->RemoveFromParent();
        TPSCameraComponent->SetFieldOfView(90);
	}
}

void ATPSPlayer::EquipGranadeGun()
{
	if (bEquipGranadeGun)
		return;

	bEquipGranadeGun = true;
	GunMeshComponent->SetVisibility(true);
	SniperGunComponent->SetVisibility(false);
}

void ATPSPlayer::EquipSniperGun()
{
	if (!bEquipGranadeGun)
		return;

	bEquipGranadeGun = false;
	GunMeshComponent->SetVisibility(false);
	SniperGunComponent->SetVisibility(true);
}
