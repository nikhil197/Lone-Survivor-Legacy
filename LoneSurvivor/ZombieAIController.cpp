// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "LoneSurvivorZombie.h"
#include "LoneSurvivorCharacter.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"

AZombieAIController::AZombieAIController()
{
	//Create the blackboard and behavior tree component
	ZombieBlackBoardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("ZombieBlackBoardCompoenent"));

	ZombieBehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("ZombieBehaviorTreeCompoenent"));

	bWantsPlayerState = true;
}

void AZombieAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	ALoneSurvivorZombie* Zombie = Cast<ALoneSurvivorZombie>(InPawn);

	//Start the behavior tree
	if (Zombie && Zombie->ZombieBehaviorTree)
	{
		if (Zombie->ZombieBehaviorTree->BlackboardAsset)
		{
			ZombieBlackBoardComponent->InitializeBlackboard(*Zombie->ZombieBehaviorTree->BlackboardAsset);
		}

		//Store the enemy blackboard ID and store it to access that blackboard later
		EnemyKeyID = ZombieBlackBoardComponent->GetKeyID("Enemy");
		ZombieBehaviorTreeComponent->StartTree(*Zombie->ZombieBehaviorTree);

	}
}

void AZombieAIController::UnPossess()
{
	Super::UnPossess();

	ZombieBehaviorTreeComponent->StopTree();
}

void AZombieAIController::BeginInactiveState()
{
	Super::BeginInactiveState();

}

void AZombieAIController::SetEnemy(class APawn* InPawn)
{
	if (ZombieBlackBoardComponent)
	{
		ZombieBlackBoardComponent->SetValue<UBlackboardKeyType_Object>(EnemyKeyID, InPawn);
		SetFocus(InPawn);
	}
}

class ALoneSurvivorCharacter* AZombieAIController::GetEnemy() const
{
	if (ZombieBlackBoardComponent)
	{
		return Cast<ALoneSurvivorCharacter>(ZombieBlackBoardComponent->GetValue<UBlackboardKeyType_Object>(EnemyKeyID));
	}
	return NULL;
}

void AZombieAIController::UpdateControlRotation(float DeltaTime, bool bUpdatePawn)
{
	//Facing towards the player
	FVector Center = GetFocalPoint();
	if (!Center.IsZero() && GetPawn())
	{
		FVector Direction = Center - GetPawn()->GetActorLocation();
		FRotator NewRotation = Direction.Rotation();

		NewRotation.Yaw = FRotator::ClampAxis(NewRotation.Yaw);

		SetControlRotation(NewRotation);

		APawn* const Temp_Pawn = GetPawn();
		if (Temp_Pawn && bUpdatePawn)
		{
			Temp_Pawn->FaceRotation(NewRotation, DeltaTime);
		}
	}
}

void AZombieAIController::FindClosestEnemy()
{
	APawn* Zombie = GetPawn();
	if (Zombie != NULL)
	{
		const FVector Location = Zombie->GetActorLocation();
		float BestDistance = MAX_FLT;
		ALoneSurvivorCharacter* PlayerPawn = NULL;

		//Iterator over all the pawns in the world
		for (FConstPawnIterator Itr = GetWorld()->GetPawnIterator(); Itr; Itr++)
		{
			ALoneSurvivorCharacter* TestPawn = Cast<ALoneSurvivorCharacter>(*Itr);

			if (TestPawn && TestPawn->IsAlive())
			{
				const float DistanceSq = (TestPawn->GetActorLocation() - Location).SizeSquared();
				if (DistanceSq < BestDistance)
				{
					BestDistance = DistanceSq;
					PlayerPawn = TestPawn;
				}
			}

		}

		if (PlayerPawn)
		{
			SetEnemy(PlayerPawn);
		}

	}
}