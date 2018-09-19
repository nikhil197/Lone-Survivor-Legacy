// Fill out your copyright notice in the Description page of Project Settings.

#include "AmmoPickup.h"
#include "Engine.h"

//Set the default values
AAmmoPickup::AAmmoPickup()
{
	//Set the default values
	AmmoCapacity = 100;
}

//Handle the collection
void AAmmoPickup::OnCollection_Implementation()
{
	//Call the superclass implementation
	Super::OnCollection_Implementation();

	//Destroy the object
	Destroy();
}

//Return the ammo capacity of the pickup
int32 AAmmoPickup::GetAmmoCapacity() const
{
	return AmmoCapacity;
}