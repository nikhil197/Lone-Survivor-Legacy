// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupObject.h"
#include <Components/StaticMeshComponent.h>
#include "Engine.h"

//Set Default values
APickupObject::APickupObject()
{
	//All pickups start active
	bIsActive = true;

	//Disable physics for the pickups
	GetMesh()->SetSimulatePhysics(false);
}

//Set the given pickup state
void APickupObject::SetActive(const bool NewPickupState)
{
	bIsActive = NewPickupState;
}

//Return the current pickup state
bool APickupObject::IsActive() const
{
	return bIsActive;
}

//Handling the collection of the object
void APickupObject::OnCollection_Implementation()
{
	FString message = TEXT("Pickup object ") + GetName();
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message);
}