// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UCLASS()
class SHOOTER_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	//Setting and Getting health variable
	float GetHealth() const { return Health; }
	void SetHealth(const float Val) { Health = Val; }

	//	Function for implement damage
	UFUNCTION(BlueprintCallable)
	void TakingDamage(float Damage);

	//	Boolean for take damage animation 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool bTakenDamage = false;

	//	Boolean for dying animation 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enemy)
	bool bDead = false;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//	Pointer for front-side health bar
	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* HealthWidgetComp = nullptr;

	//	Pointer for backside health bar
	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* HealthWidgetComp1;

	// Variable for represent current health
	float Health;

	// Variable for represent maximum health
	float MaxHealth = 100;
};
