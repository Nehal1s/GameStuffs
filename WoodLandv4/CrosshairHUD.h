// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */
UCLASS()
class WOODLANDV4_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()
public:
	ACrosshairHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;
public:
	void DragCrossHair(float value, float value1);
private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;
};
