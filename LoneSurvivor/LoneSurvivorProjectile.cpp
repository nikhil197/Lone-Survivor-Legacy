// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorProjectile.h"
#include <Components/CapsuleComponent.h>
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ALoneSurvivorProjectile::ALoneSurvivorProjectile()
{
	// Set this actor not to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = false;

	//Use a Capsule to represent the 
	CollisionComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCapsule"));
	CollisionComponent->InitCapsuleSize(.5f, 3.f);
	CollisionComponent->BodyInstance.SetCollisionProfileName("Projectile");

	//Player cannot step on it
	CollisionComponent->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComponent->CanCharacterStepUpOn = ECB_No;

	//Set as root component
	RootComponent = CollisionComponent;

	//Create a projectile movement component tp govern the projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->UpdatedComponent = CollisionComponent;
	ProjectileMovement->InitialSpeed = 3600.f;
	ProjectileMovement->MaxSpeed = 3600.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	//Life span of the projectile
	InitialLifeSpan = 3.f;

	//Damage done by the projectile
	OnHitDamage = 100.0f;

	CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
}