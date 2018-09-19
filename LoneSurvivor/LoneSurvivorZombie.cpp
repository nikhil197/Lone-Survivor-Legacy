// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorZombie.h"
#include "ZombieAIController.h"
#include <Perception/PawnSensingComponent.h>
#include <Components/SphereComponent.h>
#include <Components/CapsuleComponent.h>
#include <Components/SceneComponent.h>
#include <Components/SkeletalMeshComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include "LoneSurvivorCharacter.h"

// Sets default values
ALoneSurvivorZombie::ALoneSurvivorZombie()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//set the controller class
	AIControllerClass = AZombieAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Set the attributes
	TotalHitPoints = 1000.0f;
	HitPoints = TotalHitPoints;
	AttackDamage = 200.0f;
	AttackRange = 100.0f;
	bIsDead = false;
	bIsAttacking = false;

	//Add the controller rotation to the zombie
	bUseControllerRotationYaw = true;

	//Set the mesh as the root component
	GetMesh()->SetupAttachment(RootComponent);

	//Build the sensing component
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensor"));
	PawnSensor->SetSensingInterval(.25f);
	PawnSensor->bOnlySensePlayers = true;
	PawnSensor->SetPeripheralVisionAngle(85.f);

	/**Create triggers**/

	//TODO: Add sockets to the zombie

	//Body Trigger
	BodyTriggerComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyTriggerComp"));
	BodyTriggerComponent->InitCapsuleSize(35.0f, 48.f);
	BodyTriggerComponent->SetupAttachment(GetMesh());		// Add socket name

															//Head Trigger
	HeadTriggerComponent = CreateDefaultSubobject<USphereComponent>(TEXT("HeadTriggerComp"));
	HeadTriggerComponent->SetSphereRadius(27.f);
	HeadTriggerComponent->SetupAttachment(BodyTriggerComponent);

	FColor HandsTriggerColor = FColor(0, 0, 255, 255);

	//Right hand Trigger
	RightArmTriggerComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("RightArmTriggerComp"));
	RightArmTriggerComponent->InitCapsuleSize(10.0f, 48.f);
	RightArmTriggerComponent->SetupAttachment(BodyTriggerComponent);
	RightArmTriggerComponent->ShapeColor = HandsTriggerColor;
	RightArmTriggerComponent->SetGenerateOverlapEvents(0);

	//Left hand trigger
	LeftArmTriggerComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("LeftArmTriggerComp"));
	RightArmTriggerComponent->InitCapsuleSize(10.0f, 48.f);
	LeftArmTriggerComponent->SetupAttachment(BodyTriggerComponent);
	LeftArmTriggerComponent->ShapeColor = HandsTriggerColor;
	LeftArmTriggerComponent->SetGenerateOverlapEvents(0);

	//Avoid zombie interactions with each other as much as possible
	GetCharacterMovement()->GetNavAgentPropertiesRef().AgentRadius = 200.0f;
	GetCharacterMovement()->SetUpdateNavAgentWithOwnersCollisions(false);

}

// Called when the game starts or when spawned
void ALoneSurvivorZombie::BeginPlay()
{
	Super::BeginPlay();

	BodyTriggerComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("BodyTrigger"));

	HeadTriggerComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("HeadTrigger"));

	RightArmTriggerComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("RightArmTrigger"));

	LeftArmTriggerComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("LeftArmTrigger"));
}

// Called every frame
void ALoneSurvivorZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead)
	{
		if (DeathAnimation != NULL)
		{
			UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
			if (AnimInstance != NULL)
			{
				float AnimDuration;
				AnimDuration = AnimInstance->Montage_Play(DeathAnimation, 1.0f);
				this->SetLifeSpan(AnimDuration);
			}
		}
	}

}

// Called to bind functionality to input
void ALoneSurvivorZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALoneSurvivorZombie::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	PawnSensor->OnSeePawn.AddDynamic(this, &ALoneSurvivorZombie::OnSeePawn);
	PawnSensor->OnHearNoise.AddDynamic(this, &ALoneSurvivorZombie::OnHearNoise);

	LeftArmTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ALoneSurvivorZombie::OnHandTriggerOverlap);
	RightArmTriggerComponent->OnComponentBeginOverlap.AddDynamic(this, &ALoneSurvivorZombie::OnHandTriggerOverlap);
}

void ALoneSurvivorZombie::OnHearNoise(APawn* OtherPawn, const FVector& Location, float Volume)
{

}

void ALoneSurvivorZombie::OnSeePawn(APawn* OtherPawn)
{

}

void ALoneSurvivorZombie::OnPerformAttack()
{
	if (AttackAnimation != NULL)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Play(AttackAnimation, 1.0f);
		}
	}
}

//handle pre attack events
void ALoneSurvivorZombie::OnPreAttack()
{
	LeftArmTriggerComponent->SetGenerateOverlapEvents(1);
	RightArmTriggerComponent->SetGenerateOverlapEvents(1);
}

//Handle post attack events
void ALoneSurvivorZombie::OnPostAttack()
{
	LeftArmTriggerComponent->SetGenerateOverlapEvents(0);
	RightArmTriggerComponent->SetGenerateOverlapEvents(0);
}

void ALoneSurvivorZombie::UpdateHealth(const float Amount)
{
	if (Amount != 0)
	{
		if (HitPoints + Amount <= TotalHitPoints)
		{
			HitPoints = 0;
			bIsDead = true;
		}
		else
		{
			HitPoints += Amount;
		}
	}
}

void ALoneSurvivorZombie::OnHandTriggerOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	ALoneSurvivorCharacter* const Temp_Player = Cast<ALoneSurvivorCharacter>(OtherActor);
	if (Temp_Player)
	{
		// TODO::::Send the notification to the HUD

		//Decrease the player health
		Temp_Player->UpdateHealth(-AttackDamage);

		//Disable the overlap events
		LeftArmTriggerComponent->SetGenerateOverlapEvents(0);
		RightArmTriggerComponent->SetGenerateOverlapEvents(0);
	}

}

void ALoneSurvivorZombie::SetNewDeadState(const bool NewDeadState)
{
	bIsDead = NewDeadState;
}

void ALoneSurvivorZombie::FaceRotation(FRotator NewRotation, float DeltaTime)
{
	FRotator CurrentRotation = FMath::RInterpTo(GetActorRotation(), NewRotation, DeltaTime, 8.0f);

	Super::FaceRotation(CurrentRotation, DeltaTime);
}