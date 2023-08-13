// Fill out your copyright notice in the Description page of Project Settings.

#include <Kismet/GameplayStatics.h>
#include <EngineUtils.h>
#include "Enemy.h"
#include "EnemyManager.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
	float CreateTime = FMath::RandRange(MinTime, MaxTime);
	FindSpawnPoints();
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, CreateTime);
}

void AEnemyManager::CreateEnemy()
{
	// 랜덤 위치 구하기
	int index = FMath::RandRange(0, SpawnPoints.Num() - 1);
	// 적 생성 및 배치하기
	GetWorld()->SpawnActor<AEnemy>(EnemyClass, SpawnPoints[index]->GetActorLocation(), FRotator(0));

	// 다시 랜덤 시간에 CreateEnemy 함수가 호출되도록 타이머 설정
	float CreateTime = FMath::RandRange(MinTime, MaxTime);
	GetWorld()->GetTimerManager().SetTimer(SpawnTimerHandle, this, &AEnemyManager::CreateEnemy, CreateTime);
}

void AEnemyManager::FindSpawnPoints()
{
	// TActorIterator<T>를 활용한 T 찾기
	for (TActorIterator<AActor> Iter(GetWorld()); Iter; ++Iter)
	{
		AActor* Spawn = *Iter;

		if (Spawn->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			SpawnPoints.Add(Spawn);
		}
	}

	// UGameplayStatics::GetAllActorsOfClass를 활용해 T클래스 찾기
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), AllActors);
	for (auto Actor : AllActors)
	{
		if (Actor->GetName().Contains(TEXT("BP_EnemySpawnPoint")))
		{
			SpawnPoints.Add(Actor);
		}
	}
}
