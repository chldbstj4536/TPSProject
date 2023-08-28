// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "EnemyFSM.h"
#include "EnemyAnim.h"
#include <GameFramework/CharacterMovementComponent.h>

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshFinder(TEXT("'/Game/Enemy/Model/vampire_a_lusth.vampire_a_lusth'"));
	if (MeshFinder.Succeeded())
	{
        GetMesh()->SetSkeletalMesh(MeshFinder.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88), FRotator(0, -90, 0));
		GetMesh()->SetRelativeScale3D(FVector(0.84));
		ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClass(TEXT("'/Script/Engine.AnimBlueprint'/Game/Blueprints/ABP_Enemy.ABP_Enemy_C'"));
		if (AnimInstanceClass.Succeeded())
		{
            GetMesh()->SetAnimInstanceClass(AnimInstanceClass.Class);
		}
	}

	FSM = CreateDefaultSubobject<UEnemyFSM>(TEXT("EnemyFSM"));
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

