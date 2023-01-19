// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHealthBar.h"
#include <Components/ProgressBar.h>
#include <Components/TextBlock.h>


void UEnemyHealthBar::NativeTick(const FGeometry& MyGeometry, const float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Checking
	if (!OwnerEnemy.IsValid())
		return;

	// Setting progress bar percentage
	HealthBar->SetPercent(OwnerEnemy->GetHealth() / 100);

	// Throwing the fractions after zero
	FNumberFormattingOptions Opts;
	Opts.SetMaximumFractionalDigits(0);

	// Setting text which represent ammo status 
	CurrentHealthLabel->SetText(FText::AsNumber(OwnerEnemy->GetHealth(), &Opts));
}
