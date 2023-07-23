// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFSM.h"
#include "TPSProject.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UEnemyFSM::UEnemyFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyFSM::BeginPlay()
{
	Super::BeginPlay();

	Target = Cast<ATPSPlayer>(UGameplayStatics::GetActorOfClass(GetWorld(), ATPSPlayer::StaticClass()));
	Me = Cast<AEnemy>(GetOwner());
}


// Called every frame
void UEnemyFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	switch (EnemyState)
	{
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Damage:
		DamageState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void UEnemyFSM::IdleState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

	if (IdleDelayTime <= CurrentTime)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Move;
	}
}
void UEnemyFSM::MoveState()
{
    if (FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation()) <= AttackRange)
	{
		EnemyState = EEnemyState::Attack;
	}
	else
    {
        Me->AddMovementInput((Target->GetActorLocation() - Me->GetActorLocation()).GetSafeNormal());
    }
}
void UEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

    if (FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation()) > AttackRange)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Move;
	}
	
    else if (CurrentTime < AttackDelayTime)
	{
		CurrentTime -= AttackDelayTime;
		PRINT_LOG(TEXT("Attack"));
	}

}
void UEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

	if (DamageDelayTime <= CurrentTime)
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Idle;
	}
}
void UEnemyFSM::DieState()
{
	Me->AddMovementInput(FVector::DownVector, DieSpeed * GetWorld()->GetDeltaSeconds());

	if (Me->GetActorLocation().Z < -200.0f)
	{
        Me->Destroy();
	}
}

void UEnemyFSM::OnDamageProcess()
{
	if (--HP == 0)
    {
		CurrentTime = 0;
        Me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		EnemyState = EEnemyState::Die;
	}

	else
	{
		CurrentTime = 0;
		EnemyState = EEnemyState::Damage;
	}
}
