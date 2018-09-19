// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorCharacter.h"
#include "LoneSurvivorBullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "PickupObject.h"
#include "MedipackPickup.h"
#include "AmmoPickup.h"
#include "Engine.h"

// Sets default values
ALoneSurvivorCharacter::ALoneSurvivorCharacter()
{
	//Set Actor to tick every frame
	PrimaryActorTick.bCanEverTick = true;

	//Initialize the collision capsule 
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	//Set up turn rates
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Initial Ammo and Health
	PlayerTotalHitPoints = 2000.f;
	PlayerAmmoCapacity = 630;
	AmmoPerClip = 30;
	CurrentClipAmmo = AmmoPerClip;
	PlayerHitPoints = PlayerTotalHitPoints;
	PlayerAmmo = PlayerAmmoCapacity;

	//Initialize Player Speed
	PlayerWalkSpeed = 1.5f * (GetCharacterMovement()->MaxWalkSpeed);
	PlayerSprintSpeed = 1.5f * PlayerWalkSpeed;

	//Player starts alive
	bIsAlive = true;

	//Player is not sprinting in the beginning
	bIsRunning = false;
	bIsMovingForward = false;

	//Player is neither firing nor aiming down the sight
	bIsFiring = false;
	bIsAiming = false;

	/******************Set up the camera component***************************/
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));	//Create the camera component
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());				//Attach the camera to the collision capsule
	FirstPersonCameraComponent->bUsePawnControlRotation = true;						//Bind the camera rotation with the pawn rotation
	FirstPersonCameraComponent->RelativeLocation = FVector(0.f, 0.f, 50.f + BaseEyeHeight);	//Position the camera 

	/****************Setup the player mesh********************/
	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PlayerMesh"));	//Create the mesh 
	PlayerMesh->SetOnlyOwnerSee(true);													//Only owner can see the mesh
	PlayerMesh->SetupAttachment(FirstPersonCameraComponent);							//Attach the Mesh to the camera
	PlayerMesh->bCastDynamicShadow = false;
	PlayerMesh->CastShadow = false;
	PlayerMesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	PlayerMesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);

	GetMesh()->SetOwnerNoSee(true);

	/*********************Setup the Gun***************************/
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));		//Create the mesh
	GunMesh->SetOnlyOwnerSee(true);													//Only Owner can see the gun
	GunMesh->bCastDynamicShadow = false;
	GunMesh->CastShadow = false;
	GunMesh->SetupAttachment(RootComponent);

	/*******************Projectile spawn location****************/
	MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	MuzzleLocation->SetupAttachment(GunMesh);
	MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	//Default spawn location for projectiles
	GunOffset = FVector(100.f, 0.f, 10.f);

	//Set default Player movement attributes
	GetCharacterMovement()->MaxWalkSpeed = GetCharacterMovement()->MaxWalkSpeed * PlayerWalkSpeed;

	//Set Overlapping events
	GetCapsuleComponent()->SetGenerateOverlapEvents(1);

	//Set the default reload animation time
	if (ReloadAnimation != NULL)
	{
		ReloadAnimTime = ReloadAnimation->GetPlayLength();
		ReloadAnimPlayTime = 0.0f;
	}
	else
	{
		ReloadAnimTime = ReloadAnimPlayTime = 0.0f;
	}
}

// Called when the game starts or when spawned
void ALoneSurvivorCharacter::BeginPlay()
{
	//Call the base class 
	Super::BeginPlay();

	//Attach the gun mesh component to Skeleton
	GunMesh->AttachToComponent(PlayerMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	PlayerMesh->SetHiddenInGame(false, true);
}

//Called every frame
void ALoneSurvivorCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ALoneSurvivorCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Set up game play key bindings
	check(PlayerInputComponent);

	/**Binding the actions**/

	//Bind Jump action with the Character class implementation
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartJump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ALoneSurvivorCharacter::OnStopJump);

	//Bind the fire action with OnFire function
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ALoneSurvivorCharacter::OnStartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ALoneSurvivorCharacter::OnStopFire);

	//Bind the Aim the down sight
	PlayerInputComponent->BindAction("AimDownSight", IE_Pressed, this, &ALoneSurvivorCharacter::AimDownSight);
	PlayerInputComponent->BindAction("AimDownSight", IE_Released, this, &ALoneSurvivorCharacter::StopAimingDownSight);

	//Bind the collection key
	PlayerInputComponent->BindAction("CollectPickups", IE_Pressed, this, &ALoneSurvivorCharacter::CollectPickups);

	//Bind the sprint
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALoneSurvivorCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALoneSurvivorCharacter::StopSprint);

	//Bind Reload
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ALoneSurvivorCharacter::Reload);

	/**Binding the Axis**/

	//Bind the movement
	PlayerInputComponent->BindAxis("MoveForward", this, &ALoneSurvivorCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALoneSurvivorCharacter::MoveRight);

	//Bind the Turning and looking up
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnAtRate", this, &ALoneSurvivorCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpAtRate", this, &ALoneSurvivorCharacter::LookUpAtRate);
}

//Firing the bullets
void ALoneSurvivorCharacter::OnStartFire()
{
	if (!bIsFiring)
	{
		bIsFiring = true;

		//decrease the ammo
		if (CurrentClipAmmo > 0)
		{
			CurrentClipAmmo--;
			StopReload();
		}
		else
		{
			Reload();
			return;
		}

		//try and fire the projectile
		if (ProjectileClass != NULL)
		{
			UWorld* const World = GetWorld();
			if (World != NULL)
			{
				const FRotator SpawnRotation = GetControlRotation();

				//Muzzle offset is in camera space, transform it to world location before offsetting from the character location to find the final muzzle location
				const FVector SpawnLocation = ((MuzzleLocation != nullptr) ? MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set spawn collision override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				//spawn the actor at muzzle location
				ALoneSurvivorBullet* Bullet;
				Bullet = World->SpawnActor<ALoneSurvivorBullet>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				if (Bullet)
				{
					FVector LaunchDirection = SpawnRotation.Vector();

				}
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != NULL)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
	}

	// try and play a firing animation if specified
	if (FireAnimation != NULL)
	{
		float AnimDuration;
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimDuration = AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

//Stop firing
void ALoneSurvivorCharacter::OnStopFire()
{
	if (bIsFiring)
	{
		bIsFiring = false;

		UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			AnimInstance->Montage_Stop(FireAnimation->GetPlayLength(), FireAnimation);
		}
	}
}

//Aiming down the sight
void ALoneSurvivorCharacter::AimDownSight()
{
	if (!bIsAiming)
	{
		bIsAiming = true;
	}
}

//Stop Aiming down the sight
void ALoneSurvivorCharacter::StopAimingDownSight()
{
	bIsAiming = false;
}

/**Reload The Weapon**/
void ALoneSurvivorCharacter::Reload()
{
	if (CurrentClipAmmo == AmmoPerClip)
		return;

	if (!bIsRunning && !bIsReloading)
	{
		OnStopFire();

		bIsReloading = true;
		int32 Diff = 30 - CurrentClipAmmo;
		if (Diff != 0)
		{
			CurrentClipAmmo = 30;
			PlayerAmmo -= Diff;

			//try to play the reload animation if the animation montage is not null
			if (ReloadAnimation != NULL)
			{
				//get the animation instance of the player mesh
				UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
				if (AnimInstance != NULL)
				{
					ReloadAnimPlayTime = AnimInstance->Montage_Play(ReloadAnimation);
				}
			}

		}
	}
}

//Stop reloading if weapon is reloading
void ALoneSurvivorCharacter::StopReload()
{
	if (bIsReloading)
	{
		bIsReloading = false;

		UAnimInstance* AnimInstance = PlayerMesh->GetAnimInstance();
		if (AnimInstance != NULL)
		{
			this->StopAnimMontage(ReloadAnimation);
		}
	}
}

//Start jumping
void ALoneSurvivorCharacter::OnStartJump()
{
	if (bIsReloading)
		StopReload();

	bPressedJump = true;
}

//Stop jumping
void ALoneSurvivorCharacter::OnStopJump()
{
	bPressedJump = false;
	StopJumping();
}

//Start Sprint
void ALoneSurvivorCharacter::StartSprint()
{
	if (bIsReloading)
	{
		StopReload();
	}
	if (bIsFiring)
	{
		OnStopFire();
	}
	if (bIsMovingForward)
		bIsRunning = true;
}

//Stop sprinting
void ALoneSurvivorCharacter::StopSprint()
{
	bIsRunning = false;
}

//Turn the character at a given rate
void ALoneSurvivorCharacter::TurnAtRate(const float Rate)
{
	//Calculate the delta for this frame (Degree's Moved)
	this->AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

//Lookup at a given rate
void ALoneSurvivorCharacter::LookUpAtRate(const float Rate)
{
	//Calculate the delta for this frame (Degree's moved)
	this->AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

//Move the character forward / backward
void ALoneSurvivorCharacter::MoveForward(const float Value)
{
	if (Value != 0.f)
	{
		if (Value > 0.0f)
			bIsMovingForward = true;
		else
			bIsMovingForward = false;

		//Set the character movement speed
		if (bIsRunning)
			GetCharacterMovement()->MaxWalkSpeed = PlayerSprintSpeed;
		else
			GetCharacterMovement()->MaxWalkSpeed = PlayerWalkSpeed;

		//Adding the forward movement if the value is not zero
		this->AddMovementInput(GetActorForwardVector(), Value);
	}
}

//Move the character left / right
void ALoneSurvivorCharacter::MoveRight(const float Value)
{
	if (Value != 0.f)
	{
		//Set the character movement speed
		if (bIsRunning) {
			GetCharacterMovement()->MaxWalkSpeed = PlayerSprintSpeed;
			GetCharacterMovement()->MaxCustomMovementSpeed = PlayerSprintSpeed;
		}
		else {
			GetCharacterMovement()->MaxWalkSpeed = PlayerWalkSpeed;
			GetCharacterMovement()->MaxCustomMovementSpeed = PlayerWalkSpeed;
		}

		bIsMovingForward = false;

		//Adding the right movement if the value is not zero
		this->AddMovementInput(GetActorRightVector(), Value);
	}
}

//Set a new Player state
void ALoneSurvivorCharacter::SetPlayerState(const bool NewPlayerState)
{
	bIsAlive = NewPlayerState;
}

//Update the health of the player
void ALoneSurvivorCharacter::UpdateHealth(const float ChangeAmount)
{
	if (PlayerHitPoints + ChangeAmount < 0.f)		//If the health falls below zero, set it to zero 
	{
		PlayerHitPoints = 0.f;
	}
	else if (PlayerHitPoints + ChangeAmount > PlayerTotalHitPoints)   //If the Health increases more than the total health, set it to max health
	{
		PlayerHitPoints = PlayerTotalHitPoints;
	}
	else			//otherwise add the value to the hit points
	{
		PlayerHitPoints += ChangeAmount;
	}

	//Set whether the player is alive
	bIsAlive = PlayerHitPoints > 0.f ? true : false;
}

//Update Ammo
void ALoneSurvivorCharacter::UpdateAmmo(const int32 ChangeAmount)
{

	if (PlayerAmmo + ChangeAmount < 0)		//If the health falls below zero, set it to zero 
	{
		PlayerAmmo = 0;
	}
	else if (PlayerAmmo + ChangeAmount > PlayerAmmoCapacity)		//If Health is above 100, set it to 100
	{
		PlayerAmmo = PlayerAmmoCapacity;
	}
	else			//otherwise add the value to the hit points
	{
		PlayerAmmo += ChangeAmount;
	}
}

//Collecting the overlapping pickups
void ALoneSurvivorCharacter::CollectPickups()
{
	//Array to store the overlapping object
	TArray<AActor*> OverlappingActors;

	//Get overlapping objects
	GetCapsuleComponent()->GetOverlappingActors(OverlappingActors);

	for (int32 num = 0; num < OverlappingActors.Num(); num++)
	{
		//Cast the actor to pickup objects
		APickupObject* const TempObject = Cast<APickupObject>(OverlappingActors[num]);

		//if the cast is successful and the object is active and the object is not pending kill i.e. the object is not destroyed yet
		if (TempObject && !TempObject->IsPendingKill() && TempObject->IsActive())
		{
			//casting the pickup object to a medipack pickup
			AMedipackPickup* const TempMedipack = Cast<AMedipackPickup>(TempObject);
			if (TempMedipack)
			{
				if (PlayerHitPoints < PlayerTotalHitPoints)
				{
					TempMedipack->OnCollection_Implementation();

					int32 HitPointsDifference = (PlayerTotalHitPoints - PlayerHitPoints) > 150 ? 150 : PlayerTotalHitPoints - PlayerHitPoints;

					//Display the message
					FString message = TEXT("Hit Points + ");
					message.FString::AppendInt(HitPointsDifference);

					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, message, true, FVector2D(2.f, 2.f));

					//Update Player Health
					UpdateHealth(TempMedipack->GetHealingCapacity());

					//Disable the pickup
					TempObject->SetActive(false);
				}
				else
				{
					FString message = TEXT("Player Health Full");
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message, true, FVector2D(2.f, 2.f));
				}
			}

			//Casting the pickup object to an ammo pickup
			AAmmoPickup* const TempAmmo = Cast<AAmmoPickup>(TempObject);
			if (TempAmmo)
			{
				if (PlayerAmmo < PlayerAmmoCapacity)
				{
					TempAmmo->OnCollection_Implementation();

					int32 AmmoDifference = (PlayerAmmoCapacity - PlayerAmmo) > 100 ? 100 : PlayerAmmoCapacity - PlayerAmmo;

					//Display the message
					FString message = TEXT("Ammo + ");
					message.FString::AppendInt(AmmoDifference);
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, message, true, FVector2D(2.f, 2.f));

					//Update the player ammo
					UpdateAmmo(TempAmmo->GetAmmoCapacity());

					//Disable the pickup
					TempObject->SetActive(false);
				}
				else
				{
					//Display the message
					FString message = TEXT("Player Ammo Full");
					GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, message, true, FVector2D(2.f, 2.f));
				}
			}
		}
	}
}