// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorHUD.h"
#include "Engine/Texture2D.h"
#include "CanvasItem.h"
#include "LoneSurvivorCharacter.h"
#include "TextureResource.h"
#include "UObject/ConstructorHelpers.h"

const float ALoneSurvivorHUD::MinHudScale = 0.5f;

ALoneSurvivorHUD::ALoneSurvivorHUD()
{
	NoAmmoFadeOutTime = 1.0f;
	HitNotifyDisplayTime = 0.75f;
	KillFadeOutTime = 2.0f;
	LastEnemyHitDisplayTime = 0.2f;
	NoAmmoNotifyTime = -NoAmmoFadeOutTime;
	LastKillTime = -KillFadeOutTime;
	LastEnemyHitTime = -LastEnemyHitDisplayTime;

	static ConstructorHelpers::FObjectFinder<UTexture2D> HitTextureOb(TEXT("/Game/UI/HUD/HitIndicator"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDMainTextureOb(TEXT("/Game/UI/HUD/HUDMain"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> HUDAssets02TextureOb(TEXT("/Game/UI/HUD/HUDAssets02"));
	static ConstructorHelpers::FObjectFinder<UTexture2D> LowHealthOverlayTextureOb(TEXT("/Game/UI/HUD/LowHealthOverlay"));

	// Fonts are not included in dedicated server builds.

	static ConstructorHelpers::FObjectFinder<UFont> BigFontOb(TEXT("/Game/UI/HUD/Roboto51"));
	static ConstructorHelpers::FObjectFinder<UFont> NormalFontOb(TEXT("/Game/UI/HUD/Roboto18"));
	BigFont = BigFontOb.Object;
	NormalFont = NormalFontOb.Object;

	HitNotifyTexture = HitTextureOb.Object;
	HUDMainTexture = HUDMainTextureOb.Object;
	HUDAssets02Texture = HUDAssets02TextureOb.Object;
	LowHealthOverlayTexture = LowHealthOverlayTextureOb.Object;

	HitNotifyIcon[EShooterHudPosition::Left] = UCanvas::MakeIcon(HitNotifyTexture, 158, 831, 585, 392);
	HitNotifyIcon[EShooterHudPosition::FrontLeft] = UCanvas::MakeIcon(HitNotifyTexture, 369, 434, 460, 378);
	HitNotifyIcon[EShooterHudPosition::Front] = UCanvas::MakeIcon(HitNotifyTexture, 848, 284, 361, 395);
	HitNotifyIcon[EShooterHudPosition::FrontRight] = UCanvas::MakeIcon(HitNotifyTexture, 1212, 397, 427, 394);
	HitNotifyIcon[EShooterHudPosition::Right] = UCanvas::MakeIcon(HitNotifyTexture, 1350, 844, 547, 321);
	HitNotifyIcon[EShooterHudPosition::BackRight] = UCanvas::MakeIcon(HitNotifyTexture, 1232, 1241, 458, 341);
	HitNotifyIcon[EShooterHudPosition::Back] = UCanvas::MakeIcon(HitNotifyTexture, 862, 1384, 353, 408);
	HitNotifyIcon[EShooterHudPosition::BackLeft] = UCanvas::MakeIcon(HitNotifyTexture, 454, 1251, 371, 410);

	KillsBg = UCanvas::MakeIcon(HUDMainTexture, 15, 16, 235, 62);
	TimePlaceBg = UCanvas::MakeIcon(HUDMainTexture, 262, 16, 255, 62);
	WeaponBg = UCanvas::MakeIcon(HUDMainTexture, 543, 17, 441, 81);

	DeathMessagesBg = UCanvas::MakeIcon(HUDMainTexture, 502, 177, 342, 187);
	HealthBar = UCanvas::MakeIcon(HUDAssets02Texture, 67, 212, 372, 50);
	HealthBarBg = UCanvas::MakeIcon(HUDAssets02Texture, 67, 162, 372, 50);

	HealthIcon = UCanvas::MakeIcon(HUDAssets02Texture, 78, 262, 28, 28);
	KillsIcon = UCanvas::MakeIcon(HUDMainTexture, 318, 93, 24, 24);
	KilledIcon = UCanvas::MakeIcon(HUDMainTexture, 425, 92, 38, 36);
	PlaceIcon = UCanvas::MakeIcon(HUDMainTexture, 250, 468, 21, 28);

	Crosshair[EShooterCrosshairDirection::Left] = UCanvas::MakeIcon(HUDMainTexture, 43, 402, 25, 9); // left
	Crosshair[EShooterCrosshairDirection::Right] = UCanvas::MakeIcon(HUDMainTexture, 88, 402, 25, 9); // right
	Crosshair[EShooterCrosshairDirection::Top] = UCanvas::MakeIcon(HUDMainTexture, 74, 371, 9, 25); // top
	Crosshair[EShooterCrosshairDirection::Bottom] = UCanvas::MakeIcon(HUDMainTexture, 74, 415, 9, 25); // bottom
	Crosshair[EShooterCrosshairDirection::Center] = UCanvas::MakeIcon(HUDMainTexture, 75, 403, 7, 7); // center

	Offsets[EShooterHudPosition::Left] = FVector2D(173, 0);
	Offsets[EShooterHudPosition::FrontLeft] = FVector2D(120, 125);
	Offsets[EShooterHudPosition::Front] = FVector2D(0, 173);
	Offsets[EShooterHudPosition::FrontRight] = FVector2D(-120, 125);
	Offsets[EShooterHudPosition::Right] = FVector2D(-173, 0);
	Offsets[EShooterHudPosition::BackRight] = FVector2D(-120, -125);
	Offsets[EShooterHudPosition::Back] = FVector2D(0, -173);
	Offsets[EShooterHudPosition::BackLeft] = FVector2D(120, -125);


	HitNotifyCrosshair = UCanvas::MakeIcon(HUDMainTexture, 54, 453, 50, 50);

	Offset = 20.0f;
	HUDLight = FColor(175, 202, 213, 255);
	HUDDark = FColor(110, 124, 131, 255);
	ShadowedFont.bEnableShadow = true;
}

void ALoneSurvivorHUD::DrawWeaponHUD()
{
	ALoneSurvivorCharacter* MyPlayer = CastChecked<ALoneSurvivorCharacter>(GetOwningPawn());
	if (MyPlayer)
	{
		FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
		TextItem.EnableShadow(FLinearColor::Black);

		//Draw the weapon
		const float WeaponOffsetY = 65;
		const float WeaponBoxWidth = 150;

		Canvas->SetDrawColor(FColor::White);
		const float WeaponBgPosY = Canvas->ClipY - Canvas->OrgY - (WeaponOffsetY + WeaponBg.VL + Offset) * ScaleUI;

		//Weapon Draw position
		const float WeaponPosX = Canvas->ClipX - Canvas->OrgX - ((WeaponBoxWidth + MyPlayer->WeaponIcon.UL) / 2.0f + 2 * Offset) * ScaleUI;
		const float WeaponPosY = Canvas->ClipY - Canvas->OrgY - (WeaponOffsetY + (WeaponBg.VL + MyPlayer->WeaponIcon.VL) / 2.0f + Offset) * ScaleUI;

		//Clip Draw Position
		const float ClipWidth = MyPlayer->ClipIcon.UL + MyPlayer->ClipIconOffset * (MyPlayer->AmmoIconsCount - 1);
		const float BoxWidth = 65.0f;
		const float ClipPosX = WeaponPosX - (BoxWidth + ClipWidth) * ScaleUI;
		const float ClipPosY = Canvas->ClipY - Canvas->OrgY - (WeaponOffsetY + (WeaponBg.VL + MyPlayer->ClipIcon.VL) / 2.f + Offset) * ScaleUI;

		const float LeftCornerWidth = 60;

		FCanvasTileItem TileItem(FVector2D(ClipPosX - Offset * ScaleUI, WeaponBgPosY), WeaponBg.Texture->Resource,
			FVector2D(LeftCornerWidth * ScaleUI, WeaponBg.VL * ScaleUI), FLinearColor::White);
		MakeUV(WeaponBg, TileItem.UV0, TileItem.UV1, WeaponBg.U, WeaponBg.V, LeftCornerWidth, WeaponBg.VL);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);

		const float RestWidth = Canvas->ClipX - ClipPosX - LeftCornerWidth * ScaleUI;
		TileItem.Position = FVector2D(ClipPosX - (Offset - LeftCornerWidth) * ScaleUI, WeaponBgPosY);
		TileItem.Size = FVector2D(RestWidth, WeaponBg.VL * ScaleUI);
		MakeUV(WeaponBg, TileItem.UV0, TileItem.UV1, WeaponBg.U + WeaponBg.UL - RestWidth / ScaleUI, WeaponBg.V, RestWidth / ScaleUI, WeaponBg.VL);
		Canvas->DrawItem(TileItem);

		//Draw the weapon and the ammo
		Canvas->DrawIcon(MyPlayer->WeaponIcon, WeaponPosX, WeaponPosY, ScaleUI);

		const float TextOffset = 12.f;
		float SizeX, SizeY;
		float TopTextHeight;

		FString Text = FString::FromInt(MyPlayer->GetCurrentClipAmmo());

		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		const float TopTextScale = 0.72f;
		const float TopTextPosX = Canvas->ClipX - Canvas->OrgX - (WeaponBoxWidth + Offset * 2 + (BoxWidth + SizeX  * TopTextScale) / 2.0f) * ScaleUI;
		const float TopTextPosY = Canvas->ClipY - Canvas->OrgY - (WeaponOffsetY + WeaponBg.VL + Offset - TextOffset / 2.0f) * ScaleUI;
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(TopTextScale * ScaleUI, TopTextScale * ScaleUI);
		TextItem.FontRenderInfo = ShadowedFont;
		Canvas->DrawItem(TextItem, TopTextPosX, TopTextPosY);

		TopTextHeight = TopTextScale * SizeY;
		Text = FString::FromInt(MyPlayer->GetPlayerAmmo());
		Canvas->StrLen(BigFont, Text, SizeX, SizeY);

		const float BottomTextScale = 0.49f;
		const float BottomTextPosX = Canvas->ClipX - Canvas->OrgX - (WeaponBoxWidth + Offset * 2 + (BoxWidth + SizeX * BottomTextScale) / 2.0f) * ScaleUI;
		const float BottomTextPosY = TopTextPosY + (TopTextHeight - 0.8f * TextOffset) * ScaleUI;
		TextItem.Text = FText::FromString(Text);
		TextItem.Scale = FVector2D(BottomTextScale * ScaleUI, BottomTextScale * ScaleUI);
		TextItem.FontRenderInfo = ShadowedFont;
		Canvas->DrawItem(TextItem, BottomTextPosX, BottomTextPosY);

		//Draw Clip Icons
		Canvas->SetDrawColor(FColor::White);

		const float AmmoPerIcon = MyPlayer->GetAmmoPerClip() / MyPlayer->AmmoIconsCount;
		for (int32 i = 0; i < MyPlayer->AmmoIconsCount; i++)
		{
			if ((i + 1) * AmmoPerIcon > MyPlayer->GetCurrentClipAmmo())
			{
				const float UsedPerIcon = (i + 1) * AmmoPerIcon - MyPlayer->GetCurrentClipAmmo();
				float PercentLeft = 0;
				if (UsedPerIcon < AmmoPerIcon)
				{
					PercentLeft = (AmmoPerIcon - UsedPerIcon) / AmmoPerIcon;
				}
				const int32 Color = 128 + 128 * PercentLeft;
				Canvas->SetDrawColor(Color, Color, Color, Color);
			}

			const float ClipOffset = MyPlayer->ClipIconOffset * ScaleUI * i;
			Canvas->DrawIcon(MyPlayer->ClipIcon, ClipPosX + ClipOffset, ClipPosY, ScaleUI);
		}
		Canvas->SetDrawColor(HUDDark);
	}
}

void ALoneSurvivorHUD::DrawHealth()
{
	ALoneSurvivorCharacter* MyPlayer = CastChecked<ALoneSurvivorCharacter>(GetOwningPawn());
	Canvas->SetDrawColor(FColor::White);

	const float HealthPosX = (Canvas->ClipX - HealthBarBg.UL * ScaleUI) / 2;
	const float HealthPosY = Canvas->ClipY - (Offset + HealthBarBg.VL) * ScaleUI;
	Canvas->DrawIcon(HealthBarBg, HealthPosX, HealthPosY, ScaleUI);
	const float HealthAmount = FMath::Min(1.0f, MyPlayer->GetPlayerHitPoints() / MyPlayer->GetPlayerMaxHitPoints());

	FCanvasTileItem TileItem(FVector2D(HealthPosX, HealthPosY), HealthBar.Texture->Resource,
		FVector2D(HealthBar.UL * HealthAmount * ScaleUI, HealthBar.VL * ScaleUI), FLinearColor::White);

	MakeUV(HealthBar, TileItem.UV0, TileItem.UV1, HealthBar.U, HealthBar.V, HealthBar.UL * HealthAmount, HealthBar.VL);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

	Canvas->DrawIcon(HealthIcon, HealthPosX + Offset * ScaleUI, HealthPosY + (HealthBar.VL - HealthIcon.VL) / 2.f * ScaleUI, ScaleUI);
}

void ALoneSurvivorHUD::MakeUV(FCanvasIcon& Icon, FVector2D& UV0, FVector2D& UV1, uint16 U, uint16 V, uint16 UL, uint16 VL)
{
	if (Icon.Texture)
	{
		const float Width = Icon.Texture->GetSurfaceWidth();
		const float Height = Icon.Texture->GetSurfaceHeight();
		UV0 = FVector2D(U / Width, V / Height);
		UV1 = UV0 + FVector2D(UL / Width, VL / Height);
	}
}

void ALoneSurvivorHUD::NotifyOutofAmmo()
{
	NoAmmoNotifyTime = GetWorld()->GetTimeSeconds();
}

void ALoneSurvivorHUD::ShowDeathMessage()
{
}

void ALoneSurvivorHUD::NotifyEnemyHit()
{
	LastEnemyHitTime = GetWorld()->GetTimeSeconds();
}

void ALoneSurvivorHUD::DrawHUD()
{
	Super::DrawHUD();

	if (Canvas == nullptr)
	{
		return;
	}
	ScaleUI = Canvas->ClipY / 1080.f;


	//Enforce minimum 
	ScaleUI = FMath::Max(ScaleUI, MinHudScale);

	//Draw Low Health Overlay
	ALoneSurvivorCharacter* MyPlayer = CastChecked<ALoneSurvivorCharacter>(GetOwningPawn());
	if (MyPlayer && MyPlayer->IsAlive() && MyPlayer->GetPlayerHitPoints() < MyPlayer->GetPlayerMaxHitPoints() * 0.3f)
	{
		const float AnimSpeedModifier = 1.0f + 5.0f * (1.0f - MyPlayer->GetPlayerHitPoints() / (MyPlayer->GetPlayerMaxHitPoints() * 0.3f));
		int32 EffectValue = 32 + 72 * (1.0f - MyPlayer->GetPlayerHitPoints() / (MyPlayer->GetPlayerMaxHitPoints() * 0.3f));
		PulseValue += GetWorld()->GetDeltaSeconds() * AnimSpeedModifier;
		float EffectAplha = FMath::Abs(FMath::Sin(PulseValue));

		float AlphaValue = (1.0f / 255.0f) * (EffectAplha * EffectValue);

		//FullScreen Low Health Overlay
		FCanvasTileItem TileItem(FVector2D(0, 0), LowHealthOverlayTexture->Resource, FVector2D(Canvas->ClipX, Canvas->ClipY), FLinearColor(1.0f, 0.0f, 0.0f, AlphaValue));
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
		Canvas->ApplySafeZoneTransform();
	}

	if (MyPlayer)
	{
		float TextScale = 1.2f;
		if (MyPlayer->IsAlive())
		{
			DrawHealth();
			DrawWeaponHUD();
		}
		else
		{
			FString Text = TEXT("LOADING FROM LAST CHECKPOINT");
			FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
			TextItem.EnableShadow(FLinearColor::Black);
			TextItem.Text = FText::FromString(Text);
			TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
			TextItem.FontRenderInfo = ShadowedFont;
			TextItem.SetColor(HUDLight);
			AddInfoString(TextItem);
		}

		DrawCrosshair();
		DrawHitIndicator();

		// No ammo message if required
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		if (CurrentTime - NoAmmoNotifyTime >= 0 && CurrentTime - NoAmmoNotifyTime <= NoAmmoFadeOutTime)
		{
			FString Text = FString();

			const float Alpha = FMath::Min(1.0f, 1 - (CurrentTime - NoAmmoNotifyTime) / NoAmmoFadeOutTime);
			Text = TEXT("NO AMMO");

			FCanvasTextItem TextItem(FVector2D::ZeroVector, FText::GetEmpty(), BigFont, HUDDark);
			TextItem.EnableShadow(FLinearColor::Black);
			TextItem.Text = FText::FromString(Text);
			TextItem.Scale = FVector2D(TextScale * ScaleUI, TextScale * ScaleUI);
			TextItem.FontRenderInfo = ShadowedFont;
			TextItem.SetColor(FLinearColor(0.75f, 0.125f, 0.125f, Alpha));
			AddInfoString(TextItem);
		}
	}

	float MessageOffset = (Canvas->ClipY / 4.0)* ScaleUI;

	// Render the info messages such as waiting to re spawn - these will be drawn below any 'killed player' message.
	ShowInfoItems(MessageOffset, 1.0f);
}

void ALoneSurvivorHUD::DrawCrosshair()
{
	ALoneSurvivorCharacter* MyPlayer = Cast<ALoneSurvivorCharacter>(GetOwningPawn());

	if (MyPlayer && !MyPlayer->IsRunning())
	{
		const float SpreadMulti = 300.0f;
		const float CurrentTime = GetWorld()->GetTimeSeconds();
		float CrossSpread = 2;

		float CenterX = Canvas->ClipX / 2.0f;
		float CenterY = Canvas->ClipY / 2.0f;
		Canvas->SetDrawColor(255, 255, 255, 192);

		FCanvasIcon* CurrentCrosshair[5];
		for (int32 i = 0; i < 5; i++)
		{
			CurrentCrosshair[i] = &Crosshair[i];
		}

		//Draw the laser dot if the player is aiming down the sight
		if (MyPlayer->IsAimingDownSight())
		{
			Canvas->SetDrawColor(255, 0, 0, 192);
			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Center],
				CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).UL * ScaleUI / 2.0f,
				CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).VL *ScaleUI / 2.0f, ScaleUI);
		}
		else
		{
			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Center],
				CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).UL * ScaleUI / 2.0f,
				CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Center]).VL * ScaleUI / 2.0f, ScaleUI);

			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Left],
				CenterX - 1 - (*CurrentCrosshair[EShooterCrosshairDirection::Left]).UL * ScaleUI - CrossSpread * ScaleUI,
				CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Left]).VL * ScaleUI / 2.0f, ScaleUI);

			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Right],
				CenterX + CrossSpread * ScaleUI,
				CenterY - (*CurrentCrosshair[EShooterCrosshairDirection::Right]).VL * ScaleUI / 2.0f, ScaleUI);

			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Top],
				CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Top]).UL * ScaleUI / 2.0f,
				CenterY - 1 - (*CurrentCrosshair[EShooterCrosshairDirection::Top]).VL * ScaleUI - CrossSpread * ScaleUI, ScaleUI);

			Canvas->DrawIcon(*CurrentCrosshair[EShooterCrosshairDirection::Bottom],
				CenterX - (*CurrentCrosshair[EShooterCrosshairDirection::Bottom]).UL * ScaleUI / 2.0f,
				CenterY + CrossSpread * ScaleUI, ScaleUI);
		}

		//Draw Hit Notification
		if (CurrentTime - LastEnemyHitTime >= 0 && CurrentTime - LastEnemyHitTime <= LastEnemyHitDisplayTime)
		{
			const float Alpha = FMath::Min(1.0f, 1 - (CurrentTime - LastEnemyHitTime) / LastEnemyHitDisplayTime);
			Canvas->SetDrawColor(255, 255, 255, 255 * Alpha);

			Canvas->DrawIcon(HitNotifyCrosshair,
				CenterX - HitNotifyCrosshair.UL * ScaleUI / 2.0f,
				CenterY - HitNotifyCrosshair.VL * ScaleUI / 2.0f, ScaleUI);
		}
	}
}

void ALoneSurvivorHUD::DrawHitIndicator()
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastHitTime <= HitNotifyDisplayTime)
	{
		const float StartX = Canvas->ClipX / 2.0f;
		const float StartY = Canvas->ClipY / 2.0f;

		for (uint8 i = 0; i < 8; i++)
		{
			const float TimeModifier = FMath::Max(0.0f, 1.0f - ((CurrentTime - HitNotifyData[i].HitTime) / HitNotifyDisplayTime));
			const float Alpha = TimeModifier * HitNotifyData[i].HitPercentage;
			Canvas->SetDrawColor(255, 255, 255, FMath::Clamp(FMath::TruncToInt(Alpha * 255 * 1.5f), 0, 255));
			Canvas->DrawIcon(HitNotifyIcon[i],
				StartX + (HitNotifyIcon[i].U - HitNotifyTexture->GetSizeX() / 2 + Offsets[i].X) * ScaleUI,
				StartY + (HitNotifyIcon[i].V - HitNotifyTexture->GetSizeY() / 2 + Offsets[i].Y) * ScaleUI,
				ScaleUI);
		}
	}
}

void ALoneSurvivorHUD::AddInfoString(class FCanvasTextItem InInfoItem)
{
	InfoItems.Add(InInfoItem);
}

float ALoneSurvivorHUD::ShowInfoItems(float YOffset, float TextScale)
{
	float Y = YOffset;
	float CanvasCentre = Canvas->ClipX / 2.0f;

	for (int32 iItem = 0; iItem < InfoItems.Num(); iItem++)
	{
		float X = 0.0f;
		float SizeX, SizeY;
		Canvas->StrLen(InfoItems[iItem].Font, InfoItems[iItem].Text.ToString(), SizeX, SizeY);
		X = CanvasCentre - (SizeX * InfoItems[iItem].Scale.X) / 2.0f;
		Y += SizeY * InfoItems[iItem].Scale.Y;
	}

	return Y;
}

void ALoneSurvivorHUD::NotifyMeleeHit(float DamageTaken, struct FDamageEvent const& DamageEvent, class APawn* PawnInstigator)
{
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	ALoneSurvivorCharacter* MyPlayer = (PlayerOwner) ? Cast<ALoneSurvivorCharacter>(PlayerOwner->GetPawn()) : NULL;
	if (MyPlayer)
	{
		if ((CurrentTime - LastHitTime) > HitNotifyDisplayTime)
		{
			for (uint8 i = 0; i < 8; i++)
			{
				HitNotifyData[i].HitPercentage = 0;
			}
		}

		FVector ImpulseDir;
		FHitResult Hit;
		DamageEvent.GetBestHitInfo(this, PawnInstigator, Hit, ImpulseDir);

		//check hit vector against direction vectors - left, right , front, back
		const FVector HitVector = FRotationMatrix(PlayerOwner->GetControlRotation()).InverseTransformVector(-ImpulseDir);

		FVector Dirs2[8] = {
			FVector(0,-1,0) /*left*/,
			FVector(1,-1,0) /*front left*/,
			FVector(1,0,0) /*front*/,
			FVector(1,1,0) /*front right*/,
			FVector(0,1,0) /*right*/,
			FVector(-1,1,0) /*back right*/,
			FVector(-1,0,0), /*back*/
			FVector(-1,-1,0) /*back left*/
		};
		int32 DirIndex = -1;
		float HighestModifier = 0;

		for (uint8 i = 0; i < 8; i++)
		{
			//Normalize our direction vectors
			Dirs2[i].Normalize();
			const float DirModifier = FMath::Max(0.0f, FVector::DotProduct(Dirs2[i], HitVector));
			if (DirModifier > HighestModifier)
			{
				DirIndex = i;
				HighestModifier = DirModifier;
			}
		}
		if (DirIndex > -1)
		{
			const float DamageTakenPercentage = (DamageTaken / MyPlayer->GetPlayerHitPoints());
			HitNotifyData[DirIndex].HitPercentage += DamageTakenPercentage * 2;
			HitNotifyData[DirIndex].HitPercentage = FMath::Clamp(HitNotifyData[DirIndex].HitPercentage, 0.0f, 1.0f);
			HitNotifyData[DirIndex].HitTime = CurrentTime;
		}

	}

	LastHitTime = CurrentTime;
}

void ALoneSurvivorHUD::DrawKills()
{

}