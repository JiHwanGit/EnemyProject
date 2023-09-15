// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "EnemyAIController.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Ible,
	Move,
	Attack,
	Damage,
	Die
};
/**
 * 
 */
UCLASS()

class MYENEMYPROJECT_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

public:	

	AEnemyAIController();

	UPROPERTY()
	class AEnemy* me;

	UPROPERTY()
	class UEnemyAnim* anim;

	//class AMYEnemyProjectCharacter* Player;

	//class ACrystal* Crystal;
	
	virtual void OnPossess(APawn* InPawn) override;

	static const FName CrysytalVector;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	static const FName AttackTargetKey;
	static const FName State;
	
	virtual void OnUnPossess() override;

private:
	void OnRepeatTimer();

	void SetPerceptionSystem();

	FTimerHandle RepeatTimerHandle;
	float RepeatInterval;

	UPROPERTY()
	class UBehaviorTree* BTAsset;

	UPROPERTY()
	class UBlackboardData* BBAsset;

	UBlackboardComponent* BlackboardComp = Blackboard;

public:

	//2 Perception
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAIPerceptionComponent* SightPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAIPerceptionComponent* AttackPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	class UAISenseConfig_Sight* AttackConfig;

	UFUNCTION()
	void OnTargetDetected(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnTargetAttack(AActor* actor, FAIStimulus const Stimulus);

	UFUNCTION()
	void OnTargetAttack2(const TArray<AActor*>& UpdatedActors);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	float AISightRadius = 500.f;

	//이미 본 표적을 볼 수 있는 최대 시거리.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	float AILoseSightRadius = 50.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	float AIFieldOfView = 90.f;

	//이 감각에 의해 생성된 자극이 잊혀진 후의 연령 제한을 지정합니다. 0은 "절대로"를 의미합니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	float AISightAge = 5.f;

	//InvalidRange(기본값)가 아닌 경우, 마지막으로 본 위치의 이 범위 내에 있으면 이미 본 대상을 항상 볼 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Perception)
	float AILastSeenLocation = 500.f;
	
};
