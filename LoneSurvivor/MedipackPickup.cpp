// Fill out your copyright notice in the Description page of Project Settings.

#include "MedipackPickup.h"
#include "Engine.h"

//Set the default values
AMedipackPickup::AMedipackPickup()
{
	//Default value for healing capacity
	HealingCapacity = 100.f;
}

//Handle the collection
void AMedipackPickup::OnCollection_Implementation()
{
	//Call the superclass implementation
	Super::OnCollection_Implementation();

	//Destroy the object
	Destroy();
}

//Return the healing capacity
float AMedipackPickup::GetHealingCapacity() const
{
	return HealingCapacity;
}