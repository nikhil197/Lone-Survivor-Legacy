// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Engine/Canvas.h"
#include "LoneSurvivorCharacter.generated.h"

/**
*
*/
UCLASS()
class LONESURVIVOR_API ALoneSurvivorCharacter : public ACharacter
{
	GENERATED_BODY()

	/**Character Mesh:1st person view(arms; only seen by self)*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* PlayerMesh;

	/**Gun Mesh: 1st person view(seen only by self)*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;

	/**Location where the gun projectiles(bullets) should spawn*/
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent* MuzzleLocation;

	/**First person camera component*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

public:
	// Sets default values for this character's properties
	ALoneSurvivorCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/**Base Turn Rate, in deg/sec*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/**Base Lookup rate, in deg/sec */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/**Gun Muzzle's Offset from the character*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;

protected:
	/**Fire the projectile(Bullets)*/
	void OnStartFire();

	/**Stop firing*/
	void OnStopFire();

	/**Reload the weapon**/
	void Reload();

	/**Stop reloading while running**/
	void StopReload();

	/**Aim Down the Sight**/
	void AimDownSight();

	/**Stop aiming down the sight**/
	void StopAimingDownSight();

	/**
	*	Handles the forward / backward movement
	*	@param Value - Amount by which character is to be moved
	*/
	void MoveForward(const float Value);

	/**
	*	Handles the forward / backward movement
	*	@param Value - Amount by which character is to be moved
	*/
	void MoveRight(const float Value);

	//Start Sprint
	void StartSprint();

	//Stop Sprint
	void StopSprint();

	/**Start jumping**/
	void OnStartJump();

	/**Stop Jumping**/
	void OnStopJump();

	/**
	*	Called to turn at a given rate
	*	@param Rate - Normalized Rate
	*/
	void TurnAtRate(const float Rate);

	/**
	*	Called to turn at a given rate
	*	@param Rate - Normalized Rate
	*/
	void LookUpAtRate(const float Rate);


public:
	/**Return the Player Mesh*/
	FORCEINLINE class USkeletalMeshComponent* GetPlayerMesh() const { return PlayerMesh; }

	/**Return the First Person Camera Component*/
	FORCEINLINE class UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	/**Total Health of the player**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
	float PlayerTotalHitPoints;

	/**Current Hit points of the player**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Health")
	float PlayerHitPoints;

	/**Status of the player, true if the player is alive**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Health")
	bool bIsAlive;

	/**Whether the player is running**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Movement")
	bool bIsRunning;

	/**Whether the player is moving**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Movement")
	bool bIsMovingForward;

	/**Whether the player is aiming down the sight**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Movement")
	bool bIsAiming;

	/**Whether the player is firing**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Weapon")
	bool bIsFiring;

	/**Whether the player is reloading**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Weapon")
	bool bIsReloading;

	/**Total ammo capacity**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Weapon")
	int32 PlayerAmmoCapacity;

	/**Current Ammo **/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Weapon")
	int32 PlayerAmmo;

	/**Ammo Per Clip**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Weapon")
	int32 AmmoPerClip;

	/**Clip Ammo**/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attributes|Weapon")
	int32 CurrentClipAmmo;

	/**Speed of the player while walking**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float PlayerWalkSpeed;

	/**Speed of the player while sprinting**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attributes|Movement")
	float PlayerSprintSpeed;

public:
	///////Player HUD//////////////
	/** icon displayed on the HUD when weapon is equipped as primary **/
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FCanvasIcon WeaponIcon;

	/** bullet icon used to draw current clip (left side) */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FCanvasIcon ClipIcon;

	/** how many icons to draw per clip */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	float AmmoIconsCount;

	/** defines spacing between primary ammo icons (left side) */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	int32 ClipIconOffset;

	/** crosshair parts icons (left, top, right, bottom and center) */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FCanvasIcon Crosshair[5];

	/** crosshair parts icons when targeting (left, top, right, bottom and center) */
	UPROPERTY(EditDefaultsOnly, Category = "HUD")
	FCanvasIcon AimingCrosshair[5];

public:
	/**Return the current status of the player**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	bool IsAlive() const { return bIsAlive; }

	/**Return the current movement state**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Movement")
	bool IsRunning() const { return bIsRunning; }

	/**Return whether the player is aiming down the sight**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Movement")
	bool IsAimingDownSight() const { return bIsAiming; }

	/**Return whether the player is firing**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Weapon")
	bool IsFiring() const { return bIsFiring; }

	/**Return the hit points of the player**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	float GetPlayerHitPoints() const { return PlayerHitPoints; }

	/**Return the total Hit points of the player**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Health")
	float GetPlayerMaxHitPoints() const { return PlayerTotalHitPoints; }

	/**Return the current Ammo**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Weapon")
	int32 GetPlayerAmmo() const { return PlayerAmmo; }

	/**Return Ammo Per Clip**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Weapon")
	int32 GetAmmoPerClip() const { return AmmoPerClip; }

	/**Return the current Clip Ammo**/
	UFUNCTION(BlueprintPure, Category = "Attributes|Weapon")
	int32 GetCurrentClipAmmo() const { return CurrentClipAmmo; }

	/**Set the state of the player
	*@param NewPlayerState - New state to be assigned to player
	**/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void SetPlayerState(const bool NewPlayerState);

	/**
	*Update Health
	* @param ChangeValue - Amount by which health is to be changed
	**/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateHealth(const float ChangeValue);

	/**
	*Update Ammo Capacity
	*@param ChangeValue - Amount by which ammo is to be changed
	**/
	UFUNCTION(BlueprintCallable, Category = "Attributes")
	void UpdateAmmo(const int32 ChangeValue);

	/**Collect Medipacks**/
	UFUNCTION(BlueprintCallable, Category = "Pickups")
	void CollectPickups();

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALoneSurvivorBullet> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	/**Animation Montage to play while reloading**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* ReloadAnimation;

	/**Reload Animation total play Time**/
	float ReloadAnimTime;

	/**Reload Animation play time**/
	float ReloadAnimPlayTime;
};