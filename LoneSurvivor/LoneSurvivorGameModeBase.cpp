// Fill out your copyright notice in the Description page of Project Settings.

#include "LoneSurvivorGameModeBase.h"
#include "LoneSurvivorCharacter.h"
#include "LoneSurvivorHUD.h"
#include "UObject/ConstructorHelpers.h"

ALoneSurvivorGameModeBase::ALoneSurvivorGameModeBase()
	:Super()
{
	//Set default class to the blueprint character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/Characters/BP_LoneSurvivorCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// Set default hud class
	HUDClass = ALoneSurvivorHUD::StaticClass();
}