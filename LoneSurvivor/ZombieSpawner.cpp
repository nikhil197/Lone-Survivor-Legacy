// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieSpawner.h"
#include "LoneSurvivorZombie.h"
#include "ZombieAIController.h"
#include "TimerManager.h"
#include <Components/BoxComponent.h>
#include <Kismet/KismetMathLibrary.h>

// Sets default values
AZombieSpawner::AZombieSpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the spawn volume
	SpawnVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnVolume"));
	RootComponent = SpawnVolume;

	//Set the spawn delay
	SpawnDelayRangeLow = 1.0f;
	SpawnDelayRangeHigh = 5.0f;

	ZombieCount = 0;
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();

	//Set the current time delay
	SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AZombieSpawner::SpawnZombies, SpawnDelay, false);

}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//Return a random point in the spawn volume
FVector AZombieSpawner::GetRandomPointInVolume()
{
	FVector SpawnOrigin = SpawnVolume->Bounds.Origin;
	FVector SpawnExtent = SpawnVolume->Bounds.BoxExtent;

	return UKismetMathLibrary::RandomPointInBoundingBox(SpawnOrigin, SpawnExtent);
}

//Spawns the zombies
void AZombieSpawner::SpawnZombies()
{
	//Check if there is something to spawn
	if (Zombie != NULL)
	{
		ZombieCount++;

		//Check if the world is valid	
		UWorld* const World = GetWorld();
		if (World)
		{
			//Set the spawn Parameters
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

			//Get a random position to spawn the zombie
			FVector SpawnLocation = GetRandomPointInVolume();

			//Give a rotation to zombie
			FRotator SpawnRotation(0);

			//Spawn the zombie
			ALoneSurvivorZombie* const SpawnedZombie = World->SpawnActor<ALoneSurvivorZombie>(Zombie, SpawnLocation, SpawnRotation, SpawnParams);
			SpawnedZombie->AIControllerClass = AZombieAIController::StaticClass();

			//Set the timer for next spawn
			SpawnDelay = FMath::FRandRange(SpawnDelayRangeLow, SpawnDelayRangeHigh);
			GetWorldTimerManager().SetTimer(SpawnTimer, this, &AZombieSpawner::SpawnZombies, SpawnDelay, false);
		}

		if(ZombieCount == 10)
		{
			Destroy();
		}

	}
}