// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

private:
	UPROPERTY(transient)
	class UBlackboardComponent* ZombieBlackBoardComponent;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* ZombieBehaviorTreeComponent;

public:

	AZombieAIController();

	//To possess the pawn
	virtual void Possess(class APawn* InPawn) override;

	//To unpossess the pawn
	virtual void UnPossess() override;

	//To being the idle state
	virtual void BeginInactiveState() override;

	//Set the enemy of the zombie
	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void SetEnemy(class APawn* InPawn);

	//Get the enemy of the zombie i.e The player
	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	class ALoneSurvivorCharacter* GetEnemy() const;

	//Update the controller rotation to the walking direction
	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	virtual void UpdateControlRotation(float DeltaTime, bool bUpdatePawn = true) override;

	//To search for the enemy
	UFUNCTION(BlueprintCallable, Category = "Behaviour")
	void FindClosestEnemy();

	//Add PawnCanBeSeen method to set the distance up to which the zombie can see the pawn

protected:
	//Zombie id in the black board component
	int32 EnemyKeyID;
};
