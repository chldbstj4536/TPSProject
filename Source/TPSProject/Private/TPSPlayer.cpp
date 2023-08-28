// Fill out your copyright notice in the Description page of Project Settings.

#include "TPSPlayer.h"
#include <GameFramework/SpringArmComponent.h>
#include <Camera/CameraComponent.h>
#include <Kismet/GameplayStatics.h>
#include "PlayerMove.h"
#include "PlayerFire.h"
#include "TPSProject.h"

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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshFinder(TEXT("'/Game/AnimStarterPack/UE4_Mannequin/Mesh/SK_Mannequin.SK_Mannequin'"));
	if (SkeletalMeshFinder.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletalMeshFinder.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0, 0.0, -90.0), FRotator(0.0, -90.0, 0.0));
	}

	GunMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMeshComponent->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
    ConstructorHelpers::FObjectFinder<USkeletalMesh> GunMesh(TEXT("'/Game/Resources/FPWeapon/Mesh/SK_FPGun.SK_FPGun'"));
	if (GunMesh.Succeeded())
	{
        GunMeshComponent->SetSkeletalMesh(GunMesh.Object);
		GunMeshComponent->SetRelativeLocation(FVector(-17, 10, -3));
		GunMeshComponent->SetRelativeRotation(FRotator(0, 90, 0));
	}
	GunMeshComponent->SetVisibility(true);

	SniperGunComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SniperMesh"));
	SniperGunComponent->SetupAttachment(GetMesh(), TEXT("hand_rSocket"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> SniperMesh(TEXT("'/Game/Resources/SniperGun/sniper1.sniper1'"));
	if (SniperMesh.Succeeded())
	{
		SniperGunComponent->SetStaticMesh(SniperMesh.Object);
		SniperGunComponent->SetRelativeLocation(FVector(-42, 7, 1));
		SniperGunComponent->SetRelativeRotation(FRotator(0, 90, 0));
		SniperGunComponent->SetRelativeScale3D(FVector(0.15));
	}
	SniperGunComponent->SetVisibility(false);

	PlayerMoveComponent = CreateDefaultSubobject<UPlayerMove>(TEXT("PlayerMove"));
}

// Called when the game starts or when spawned
void ATPSPlayer::BeginPlay()
{
	Super::BeginPlay();

	CurrentHP = InitialHP;
}

// Called every frame
void ATPSPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	OnInputBindingDelegate.Broadcast(PlayerInputComponent);

}

void ATPSPlayer::OnHitEvent()
{
	if (--CurrentHP == 0)
	{
		PRINT_LOG(TEXT("Died"));
		OnGameOver();
	}
	else
	{
		PRINT_LOG(TEXT("Damaged"));
	}
}

void ATPSPlayer::OnGameOver_Implementation()
{
	UGameplayStatics::SetGamePaused(GetWorld(), true);

}
