// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "NavigationSystem.h"
#include "AI/Navigation/NavigationTypes.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionTypes.h"
#include "MYEnemyProjectCharacter.h"
#include "GameFramework/Controller.h"
#include "Crystal.h"
#include "Enemy.h"
#include "EnemyAnim.h"

const FName AEnemyAIController::CrysytalVector(TEXT("CrysytalVector"));
const FName AEnemyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AEnemyAIController::TargetKey(TEXT("Target"));
const FName AEnemyAIController::AttackTargetKey(TEXT("AttackTargetKey"));
const FName AEnemyAIController::State(TEXT("State"));


AEnemyAIController::AEnemyAIController()
{
	RepeatInterval = 3.0f;



	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Script/AIModule.BlackboardData'/Game/MyContent/Blueprints/AI/BB_Enemy.BB_Enemy'"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Script/AIModule.BehaviorTree'/Game/MyContent/Blueprints/AI/BT_Enemy.BT_Enemy'"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}

	//class UEnemyAnim* anim;
	
	

	SetPerceptionSystem();
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	GetWorld()->GetTimerManager().SetTimer(RepeatTimerHandle, this, &AEnemyAIController::OnRepeatTimer, RepeatInterval, true);

	//애님 인스턴드 불러오기
	me = Cast<AEnemy>(GetPawn());
	anim = Cast<UEnemyAnim>(me->GetMesh()->GetAnimInstance());
	if (anim != nullptr) {
		anim->animState = EEnemyState::Move;
	}
	

	if (UseBlackboard(BBAsset, BlackboardComp))
	{
		BlackboardComp->SetValueAsVector(CrysytalVector, FVector(0, 0, 0));
		if (!RunBehaviorTree(BTAsset))
		{
			return;
		}
		BlackboardComp->SetValueAsObject(TargetKey, nullptr);
	}

	BlackboardComp->SetValueAsEnum(State, static_cast<uint8>(EEnemyState::Move));

}

void AEnemyAIController::OnUnPossess()
{
	auto CurrentPawn = GetPawn();
	check(CurrentPawn != nullptr);
}

void AEnemyAIController::OnRepeatTimer()
{
}

void AEnemyAIController::SetPerceptionSystem()
{

	SightPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("SightPerception"));
	SightConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 500.0f;
	SightConfig->LoseSightRadius = SightConfig->SightRadius + 50.0f;
	SightConfig->PeripheralVisionAngleDegrees = 180.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightPerception->ConfigureSense(*SightConfig);
	SightPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);

	AttackPerception = CreateOptionalDefaultSubobject<UAIPerceptionComponent>(TEXT("AttackPerception"));
	AttackConfig = CreateOptionalDefaultSubobject<UAISenseConfig_Sight>(TEXT("AttackConfig"));
	AttackConfig->SightRadius = 300.f;
	AttackConfig->LoseSightRadius = AttackConfig->SightRadius + 50.0f;
	AttackConfig->PeripheralVisionAngleDegrees = 180.f;
	AttackConfig->DetectionByAffiliation.bDetectEnemies = true;
	AttackConfig->DetectionByAffiliation.bDetectNeutrals = true;
	AttackConfig->DetectionByAffiliation.bDetectFriendlies = true;
	AttackPerception->ConfigureSense(*AttackConfig);
	AttackPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetAttack);
	AttackPerception->OnPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetAttack2);

}

void AEnemyAIController::OnTargetDetected(AActor* actor, FAIStimulus const Stimulus)
{
	//만났을 때 Player일 경우 지속
	TArray<FName> ActorTags = actor->Tags;
	for (FName Tag : ActorTags) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("tag = %s"), Tag);
		if (Tag != "Player")
			return;
	}
	if (BlackboardComp->GetValueAsEnum(State) == 2) {
		return;
	}

	if (BlackboardComp->GetValueAsObject(TargetKey) != NULL && Stimulus.WasSuccessfullySensed()) 
	{
		//만약 타겟이 이미 있는 경우 액터를 발견했을 때
		UE_LOG(LogTemp, Warning, TEXT("already Target"));
		return;
	}
	else if (BlackboardComp->GetValueAsObject(TargetKey) == NULL && Stimulus.WasSuccessfullySensed()) 
	{
		//만약 타겟이 없은경우 액터를 발견했을 때
		BlackboardComp->SetValueAsObject(TargetKey, actor);
		return;
	}
	else if(BlackboardComp->GetValueAsObject(TargetKey) != NULL && !Stimulus.WasSuccessfullySensed())
	{
		//타겟이 있는 경우 놓쳤을때
		//BlackboardComp->SetValueAsObject(TargetKey, NULL);
		return;
	}
	else
	{

		BlackboardComp->SetValueAsObject(TargetKey, NULL);
		return;
	}
}

void AEnemyAIController::OnTargetAttack(AActor* actor, FAIStimulus const Stimulus)
{
	UE_LOG(LogTemp, Warning, TEXT("OnTargetAttack"));
	TArray<FName> ActorTags = actor->Tags;
	FName PlayerTag = "Player";
	for (FName Tag : ActorTags) {
		if (Tag == "Player") {
			BlackboardComp->SetValueAsEnum(State, static_cast<uint8>(EEnemyState::Attack));
			//BlackboardComp->SetValueAsObject(TargetKey, actor);
		}
		else
		{

		}
	}
	if (!Stimulus.WasSuccessfullySensed())
	{
		BlackboardComp->SetValueAsEnum(State, static_cast<uint8>(EEnemyState::Move));
	}
}

void AEnemyAIController::OnTargetAttack2(const TArray<AActor*>& UpdatedActors)
{
	int num = 0;
	for (AActor* actor : UpdatedActors)
	{
		TArray<FName> ActorTags = actor->Tags;
		for (FName Tag : ActorTags)
		{
			if (Tag == "Player")
				num++;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("%d"), num);
}
