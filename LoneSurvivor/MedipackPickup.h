// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupObject.h"
#include "MedipackPickup.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API AMedipackPickup : public APickupObject
{
	GENERATED_BODY()

public:
	//Set Default values
	AMedipackPickup();

	//Handling the collection
	void OnCollection_Implementation() override;

	//Return the healing capacity
	float GetHealingCapacity() const;

protected:
	//Healing capacity of the pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Heal")
	float HealingCapacity;

};