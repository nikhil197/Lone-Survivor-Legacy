// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "LoneSurvivorHUD.generated.h"

struct FHitData
{
	/** Last hit time. */
	float HitTime;

	/** strength of hit icon */
	float HitPercentage;

	/** Initialize defaults. */
	FHitData()
	{
		HitTime = 0.0f;
		HitPercentage = 0.0f;
	}
};

UCLASS()
class LONESURVIVOR_API ALoneSurvivorHUD : public AHUD
{
	GENERATED_BODY()

public:
	/**HUD Constructor**/
	ALoneSurvivorHUD();

	/**Draw the HUD for Game**/
	virtual void DrawHUD() override;

	/**
	*Notify Hit
	* @param	DamageTaken		The amount of damage.
	* @param	DamageEvent		The actual damage event.
	* @param	PawnInstigator	The pawn that did the damage.
	**/
	void NotifyMeleeHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator);

	/**Showing no ammo**/
	void NotifyOutofAmmo();

	/**Show Death Message**/
	void ShowDeathMessage();

	/** Notifies we have hit the enemy. */
	void NotifyEnemyHit();

protected:
	/** Floor for automatic hud scaling. */
	static const float MinHudScale;

	/** Lighter HUD color. */
	FColor HUDLight;

	/** Darker HUD color. */
	FColor HUDDark;

	/** When we got last notice about out of ammo. */
	float NoAmmoNotifyTime;

	/** How long notice is fading out. */
	float NoAmmoFadeOutTime;

	/** Most recent hit time, used to check if we need to draw hit indicator at all. */
	float LastHitTime;

	/** How long till hit notify fades out completely. */
	float HitNotifyDisplayTime;

	/** When we last time hit the enemy. */
	float LastEnemyHitTime;

	/** How long till enemy hit notify fades out completely. */
	float LastEnemyHitDisplayTime;

	/** Icons for hit indicator. */
	UPROPERTY()
	FCanvasIcon HitNotifyIcon[8];

	/** kills background icon. */
	UPROPERTY()
	FCanvasIcon KillsBg;

	/** Match timer and player position background icon. */
	UPROPERTY()
	FCanvasIcon TimePlaceBg;

	/** Primary weapon background icon. */
	UPROPERTY()
	FCanvasIcon WeaponBg;

	/** Crosshair icons (left, top, right, bottom and center). */
	UPROPERTY()
	FCanvasIcon Crosshair[5];

	/** On crosshair indicator that we hit someone. */
	UPROPERTY()
	FCanvasIcon HitNotifyCrosshair;

	/** Death messages background icon. */
	UPROPERTY()
	FCanvasIcon DeathMessagesBg;

	/** Health bar background icon. */
	UPROPERTY()
	FCanvasIcon HealthBarBg;

	/** Health bar icon. */
	UPROPERTY()
	FCanvasIcon HealthBar;

	/** Health icon on the health bar. */
	UPROPERTY()
	FCanvasIcon HealthIcon;

	/** Kills icon. */
	UPROPERTY()
	FCanvasIcon KillsIcon;

	/** Bigger killed icon. */
	UPROPERTY()
	FCanvasIcon KilledIcon;

	/** Podium icon. */
	UPROPERTY()
	FCanvasIcon PlaceIcon;

	/** UI scaling factor for other resolutions than Full HD. */
	float ScaleUI;

	/** Current animation pulse value. */
	float PulseValue;

	/** FontRenderInfo enabling casting shadow.s */
	FFontRenderInfo ShadowedFont;

	/** Big "KILLED [Zombie Type]" message text above the crosshair. */
	FText CenteredKillMessage;

	/** last time we killed someone. */
	float LastKillTime;

	/** How long the message will fade out. */
	float KillFadeOutTime;

	/** Offsets to display hit indicator parts. */
	FVector2D Offsets[8];

	/** Texture for hit indicator. */
	UPROPERTY()
	UTexture2D* HitNotifyTexture;

	/** texture for HUD elements. */
	UPROPERTY()
	UTexture2D* HUDMainTexture;

	/** Texture for HUD elements. */
	UPROPERTY()
	UTexture2D* HUDAssets02Texture;

	/** Overlay shown when health is low. */
	UPROPERTY()
	UTexture2D* LowHealthOverlayTexture;

	/** Large font - used for ammo display etc. */
	UPROPERTY()
	UFont* BigFont;

	/** Normal font - used for death messages and such. */
	UPROPERTY()
	UFont* NormalFont;

	/** General offset for HUD elements. */
	float Offset;

	/** Runtime data for hit indicator. */
	FHitData HitNotifyData[8];

	/** Array of information strings to render (Waiting to re spawn etc) */
	TArray<FCanvasTextItem> InfoItems;

	/** Draws weapon HUD. */
	void DrawWeaponHUD();

	/** Draws kills information. */
	void DrawKills();

	/** Draw player's health bar. */
	void DrawHealth();

	/** Draws weapon crosshair. */
	void DrawCrosshair();

	/** Draws hit indicator. */
	void DrawHitIndicator();

	/** helper for getting uv coords in normalized top,left, bottom, right format */
	void MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL);

	/*
	* Add information string that will be displayed on the hud. They are added as required and rendered together to prevent overlaps
	*
	* @param InInfoString	InInfoString
	*/
	void AddInfoString(const FCanvasTextItem InfoItem);

	/*
	* Render the info messages.
	*
	* @param YOffset	YOffset from top of canvas to start drawing the messages
	* @param ScaleUI	UI Scale factor
	* @param TextScale	Text scale factor
	*
	* @returns The next Y position to draw any further strings
	*/
	float ShowInfoItems(float YOffset, float TextScale);
};

namespace EShooterCrosshairDirection
{
	enum Type
	{
		Left = 0,
		Right = 1,
		Top = 2,
		Bottom = 3,
		Center = 4
	};
}

namespace EShooterHudPosition
{
	enum Type
	{
		Left = 0,
		FrontLeft = 1,
		Front = 2,
		FrontRight = 3,
		Right = 4,
		BackRight = 5,
		Back = 6,
		BackLeft = 7,
	};
}
