// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AShooterHUD::AShooterHUD()
{
	// Set the crosshair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	if (!ensure(CrosshairTexObj.Object != nullptr)) return;
	CrosshairTex = CrosshairTexObj.Object;

	// Set self created user widget location
	static ConstructorHelpers::FClassFinder<UUserWidget> CharacterHudObj(TEXT("/Game/_Main/BluePrints/Widgets/WBP_CharacterHud"));
	if (!ensure(CharacterHudObj.Class != nullptr)) return;
	HUDWidgetClass = CharacterHudObj.Class;

	// Attaching self created widget class to viewport
	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


void AShooterHUD::DrawHUD()
{
	Super::DrawHUD();

	// Draw very simple crosshair

	// find center of the Canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	// offset by half the texture's dimensions so that the center of the texture aligns with the center of the Canvas
	const FVector2D CrosshairDrawPosition( (Center.X), (Center.Y + 20.0f));

	// draw the crosshair
	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->GetResource(), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );
}

void AShooterHUD::BeginPlay()
{
	Super::BeginPlay();


}
