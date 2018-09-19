// Fill out your copyright notice in the Description page of Project Settings.

#include "MyObject.h"
#include <Components/StaticMeshComponent.h>

// Sets default values
AMyObject::AMyObject()
	:AActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Create the mesh component
	ObjectMesh = CreateDefaultSubobject<class UStaticMeshComponent>(TEXT("ObjectMesh"));
	RootComponent = ObjectMesh;

	//Setting the object physics
	ObjectMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AMyObject::BeginPlay()
{
	AActor::BeginPlay();
}

// Called every frame
void AMyObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}