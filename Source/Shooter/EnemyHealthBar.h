// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enemy.h"
#include "EnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()

public:

	// Setting owner reference
	void SetOwnerEnemy(AEnemy* InEnemy) { OwnerEnemy = InEnemy; }

protected:

	// Function for update variables
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// Weak-pointer Enemy class for prevent crashing
	TWeakObjectPtr<AEnemy> OwnerEnemy;

	//Binding progress bar as HealthBar
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HealthBar;

	//Binding text as CurrentHealthLabel
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHealthLabel;
};
