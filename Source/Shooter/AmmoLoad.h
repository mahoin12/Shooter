// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoLoad.generated.h"

class UStaticMeshComponent;
UCLASS()
class SHOOTER_API AAmmoLoad : public AActor
{
	GENERATED_BODY()


public:	
	// Sets default values for this actor's properties
	AAmmoLoad();

	// Box Component to represent collision
	UPROPERTY(VisibleInstanceOnly, Category = "Tile")
		class UBoxComponent* Collision;

	// Function for overlap
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// Presetted variable for increase max ammo load
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Weapon)
		int Ammo = 10;

protected:

	// Static mesh to represent the pickup in the level
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Tile")
		UStaticMeshComponent* AmmoMesh;
};
