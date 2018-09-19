// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LoneSurvivorZombie.generated.h"

UCLASS()
class LONESURVIVOR_API ALoneSurvivorZombie : public ACharacter
{
	GENERATED_BODY()

	/**Trigger component for the body**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* BodyTriggerComponent;

	/**Trigger Component for the head**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
	class USphereComponent* HeadTriggerComponent;

	/**Trigger component for the right arm**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* RightArmTriggerComponent;

	/**Trigger Component for the left arm**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Triggers", meta = (AllowPrivateAccess = "true"))
	class UCapsuleComponent* LeftArmTriggerComponent;

public:
	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* ZombieBehaviorTree;

	// Sets default values for this character's properties
	ALoneSurvivorZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	/**Zombie max Health**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
	float TotalHitPoints;

	/**Zombie Current Health**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
	float HitPoints;

	/**Zombie Attack Range**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Attack")
	float AttackRange;

	/**Zombie Attack Damage**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Attack")
	float AttackDamage;

	/**Whether zombie is alive or dead**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
	bool bIsDead;

	/**Whether zombie is attacking**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Attack")
	bool bIsAttacking;

	/**Sensing component to sense other pawns**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	class UPawnSensingComponent* PawnSensor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UAnimMontage* AttackAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UAnimMontage* DeathAnimation;

public:
	/** Perform Attack**/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnPerformAttack();

	/**Before attacking**/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnPreAttack();

	/**After Attacking**/
	UFUNCTION(BlueprintCallable, Category = "AI")
	void OnPostAttack();

	/**Hear Player using the sensing component**/
	UFUNCTION()
	void OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume);

	/**See the player using sensing component**/
	UFUNCTION()
	void OnSeePawn(APawn* OtherPawn);

	/**Update the health of the zombie**/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(const float Amount);

	/**Function to handle the hit to player**/
	UFUNCTION()
	void OnHandTriggerOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**Return whether the zombie is alive or dead**/
	FORCEINLINE bool IsDead() const { return bIsDead; }

	/**Set whether the zombie is dead or not**/
	UFUNCTION(BlueprintCallable, Category = "Health")
	void SetNewDeadState(const bool NewDeadState);

	/**Return the body trigger**/
	FORCEINLINE class UCapsuleComponent* GetBodyTriggerComponent() const { return BodyTriggerComponent; }

	/**Return the head Trigger**/
	FORCEINLINE class USphereComponent* GetHeadTriggerComponent() const { return HeadTriggerComponent; }

protected:
	virtual void PostInitializeComponents() override;

	virtual void FaceRotation(FRotator NewRotation, float DeltaTime = 0.f) override;
};
