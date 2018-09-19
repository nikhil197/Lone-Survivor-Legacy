// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_FindPointNearEnemy.h"
#include "ZombieAIController.h"
#include "LoneSurvivorCharacter.h"
#include "AI/NavigationSystemBase.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTTask_FindPointNearEnemy::UBTTask_FindPointNearEnemy()
	:Super()
{
}

EBTNodeResult::Type UBTTask_FindPointNearEnemy::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AZombieAIController* ZombieController = Cast<AZombieAIController>(OwnerComp.GetAIOwner());
	if (ZombieController == NULL)
	{
		return EBTNodeResult::Failed;
	}

	APawn* Zombie = ZombieController->GetPawn();
	ALoneSurvivorCharacter* Enemy = ZombieController->GetEnemy();
	if (Enemy && Zombie)
	{
		const float SearchRadius = 200.0f;
		const FVector SearchOrigin = Enemy->GetActorLocation() + 600.0f * (Zombie->GetActorLocation() - Enemy->GetActorLocation()).GetSafeNormal();
		FVector Location(0);
		UNavigationSystem::K2_GetRandomReachablePointInRadius(ZombieController, SearchOrigin, Location, SearchRadius);
		if (Location != FVector::ZeroVector)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BlackboardKey.GetSelectedKeyID(), Location);
			return EBTNodeResult::Succeeded;
		}
	}

	return EBTNodeResult::Failed;
}