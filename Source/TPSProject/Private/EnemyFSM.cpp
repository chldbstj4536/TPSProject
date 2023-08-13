// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyFSM.h"
#include "TPSProject.h"
#include "Enemy.h"
#include "TPSPlayer.h"
#include "EnemyAnim.h"
#include <Components/CapsuleComponent.h>
#include <Kismet/GameplayStatics.h>
#include <AIController.h>
#include <NavigationSystem.h>

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
	EnemyAnim = Cast<UEnemyAnim>(Me->GetMesh()->GetAnimInstance());

    AIController = Cast<AAIController>(Me->GetController());
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
        GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPosition);
        EnemyAnim->AnimState = EnemyState = EEnemyState::Move;
	}
}
void UEnemyFSM::MoveState()
{
    if (FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation()) <= AttackRange)
	{
        AIController->StopMovement();
        EnemyAnim->AnimState = EnemyState = EEnemyState::Attack;
		EnemyAnim->bAttackPlay = true;
		CurrentTime = AttackDelayTime;
	}
	else
    {
		//AIController->MoveToLocation(Target->GetActorLocation());
		auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
		FPathFindingQuery Query;
		FAIMoveRequest Request;
		Request.SetAcceptanceRadius(3.0f);
		Request.SetGoalLocation(Target->GetActorLocation());
		AIController->BuildPathfindingQuery(Request, Query);
		FPathFindingResult Result = ns->FindPathSync(Query);

		if (Result.IsSuccessful())
		{
			AIController->MoveToLocation(Target->GetActorLocation());
		}
		else
		{
            auto MoveResult = AIController->MoveToLocation(RandomPosition);
			if (MoveResult == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPosition);
			}
		}
        //Me->AddMovementInput((Target->GetActorLocation() - Me->GetActorLocation()).GetSafeNormal());
    }
}
void UEnemyFSM::AttackState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

    if (FVector::Distance(Target->GetActorLocation(), Me->GetActorLocation()) > AttackRange)
	{
		CurrentTime = 0;
        EnemyAnim->AnimState = EnemyState = EEnemyState::Move;
        GetRandomPositionInNavMesh(Me->GetActorLocation(), 500, RandomPosition);
	}
	
    else if (CurrentTime >= AttackDelayTime)
	{
		CurrentTime -= AttackDelayTime;
		EnemyAnim->bAttackPlay = true;
	}

}
void UEnemyFSM::DamageState()
{
	CurrentTime += GetWorld()->GetDeltaSeconds();

	if (DamageDelayTime <= CurrentTime)
	{
		CurrentTime = 0;
        EnemyAnim->AnimState = EnemyState = EEnemyState::Idle;
	}
}
void UEnemyFSM::DieState()
{
	Me->AddMovementInput(FVector::RightVector, DieSpeed * GetWorld()->GetDeltaSeconds());

	PRINT_LOG(TEXT("%f"), DieSpeed * GetWorld()->GetDeltaSeconds());

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
        EnemyAnim->AnimState = EnemyState = EEnemyState::Die;
		EnemyAnim->PlayDamageAnim(TEXT("Die"));
	}

	else
	{
		CurrentTime = 0;
        EnemyAnim->AnimState = EnemyState = EEnemyState::Damage;
		FString SectionName = FString::Printf(TEXT("Damage%d"), FMath::RandRange(0, 1));
        EnemyAnim->PlayDamageAnim(FName(*SectionName));
	}

	AIController->StopMovement();
}

bool UEnemyFSM::GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& Destination)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(CenterLocation, Radius, loc);
    Destination = loc.Location;
	return result;

}
