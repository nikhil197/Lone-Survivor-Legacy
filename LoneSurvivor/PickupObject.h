// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyObject.h"
#include "PickupObject.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API APickupObject : public AMyObject
{
	GENERATED_BODY()

public:
	//Set default values for pickups
	APickupObject();

protected:
	//True if pickup object is active
	bool bIsActive;

public:
	//Return whether the object is active or not
	UFUNCTION(BlueprintPure, Category = "Pickup State")
	bool IsActive() const;

	//Set a new state for the pickup object
	UFUNCTION(BlueprintCallable, Category = "Pickup State")
	void SetActive(const bool NewPickupState);

	//Responding to the collection of the pickup
	UFUNCTION(BlueprintNativeEvent)
	void OnCollection();
	virtual void OnCollection_Implementation();

};