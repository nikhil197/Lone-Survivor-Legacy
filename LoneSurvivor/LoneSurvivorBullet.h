// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LoneSurvivorProjectile.h"
#include "LoneSurvivorBullet.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API ALoneSurvivorBullet : public ALoneSurvivorProjectile
{
	GENERATED_BODY()

public:
	ALoneSurvivorBullet();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	//Get damage done by the bullet
	UFUNCTION(BlueprintPure, Category = "Attack")
	float GetBulletDamage() const;


	void FireInDirection(const FVector& ShootDirection);
};
