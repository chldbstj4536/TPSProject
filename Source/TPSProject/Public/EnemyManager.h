// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyManager.generated.h"

UCLASS()
class TPSPROJECT_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void CreateEnemy();
	void FindSpawnPoints();

public:
	// ���� �ð� ���� �ּڰ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float MinTime;
	// ���� �ð� ���� �ִ�
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	float MaxTime;
	// ������ ��ġ ���� �迭
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TArray<AActor*> SpawnPoints;
	// AEnemy Ÿ���� �������Ʈ �Ҵ���� ����
	UPROPERTY(EditAnywhere, Category = SpawnSettings)
	TSubclassOf<class AEnemy> EnemyClass;

	// ������ ���� �˶� Ÿ�̸�
	FTimerHandle SpawnTimerHandle;
};
