// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyObject.generated.h"

UCLASS()
class LONESURVIVOR_API AMyObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Return the object mesh
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return ObjectMesh; }

private:
	//Static Mesh to represent the object in the world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* ObjectMesh;

};