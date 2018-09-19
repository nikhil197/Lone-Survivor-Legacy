// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LoneSurvivorProjectile.generated.h"

UCLASS()
class LONESURVIVOR_API ALoneSurvivorProjectile : public AActor
{
	GENERATED_BODY()

	/**Capsule Collision Component**/
	UPROPERTY(VisibleDefaultsOnly, Category = "Projectile")
	class UCapsuleComponent* CollisionComponent;

	/**Projectile Movement Component**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

public:
	// Sets default values for this actor's properties
	ALoneSurvivorProjectile();

	/**Returns the Collision component of the projectile**/
	FORCEINLINE class UCapsuleComponent* GetCollisionComponent() const { return CollisionComponent; }

	/**Returns the Projectile Movement Component**/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileComponent() const { return ProjectileMovement; }


protected:
	/**Total Damage done by the projectile on hit**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float OnHitDamage;
};