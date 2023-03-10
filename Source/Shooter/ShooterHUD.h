// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShooterHUD.generated.h"

UCLASS()
class AShooterHUD : public AHUD
{
	GENERATED_BODY()

public:
	AShooterHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:

	/** Crosshair asset pointer */
	UPROPERTY()
	class UTexture2D* CrosshairTex;

	// The userwidget to attach
	UPROPERTY(EditAnywhere, Category = "Ammo")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	// Userwidget pointer
	UPROPERTY(EditAnywhere, Category = "Ammo")
	class UUserWidget* CurrentWidget;

};

