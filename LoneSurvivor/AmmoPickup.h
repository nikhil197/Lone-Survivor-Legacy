// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupObject.h"
#include "AmmoPickup.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API AAmmoPickup : public APickupObject
{
	GENERATED_BODY()

public:
	//Set Default values
	AAmmoPickup();

	//Handle the collection
	void OnCollection_Implementation() override;

	//Return the ammo capacity
	int32 GetAmmoCapacity() const;

protected:
	//Amount by which the player ammo is increased when this object is picked
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoCapacity;

};