// Copyright Epic Games, Inc. All Rights Reserved.


#include "WoodLandv4GameModeBase.h"
#include "/JellyPlane/JellyPlane.h"
#include "CrosshairHUD.h"
#include "UObject/ConstructorHelpers.h"


AWoodLandv4GameModeBase::AWoodLandv4GameModeBase() : Super()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/JellyPlane"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACrosshairHUD::StaticClass();
}
