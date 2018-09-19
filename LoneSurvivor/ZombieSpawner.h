// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

UCLASS()
class LONESURVIVOR_API AZombieSpawner : public AActor
{
	GENERATED_BODY()

	/**Box component to specify where zombies should be spawned**/
	UPROPERTY(VisibleAnywhere, Category = "Spawning")
	class UBoxComponent* SpawnVolume;

public:
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**The Zombie to be spawned**/
	UPROPERTY(EditAnywhere, Category = "Spawing")
	TSubclassOf<class ALoneSurvivorZombie> Zombie;

	/**Timer handle**/
	FTimerHandle SpawnTimer;

	/**Minimum spawn delay**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawing|Timer")
	float SpawnDelayRangeLow;

	/**Maximum Spawn Delay**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawing|Timer")
	float SpawnDelayRangeHigh;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**Returns a random point inside the spawn volumne**/
	UFUNCTION(BlueprintPure, Category = "Spawing")
	FVector GetRandomPointInVolume();

	/**Return the spawn volume**/
	FORCEINLINE class UBoxComponent* GetSpawnVolume() const { return SpawnVolume; }

private:
	/**Handles the spawning of zombies**/
	void SpawnZombies();

	/**Actual / Current Spawn Delay**/
	float SpawnDelay;

	/**Number of Zombies spawned**/
	uint8 ZombieCount;
};