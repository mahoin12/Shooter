// Fill out your copyright notice in the Description page of Project Settings.


#include "AmmoLoad.h"
#include "ShooterCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AAmmoLoad::AAmmoLoad()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AlternateMeshAsset(TEXT("/Game/Megascans/3D_Assets/Military_Wooden_Box_ud4nfhofa/S_Military_Wooden_Box_ud4nfhofa_lod3_Var1"));
	// Create the Root Component
	if (RootComponent == nullptr)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	}

	// Creating Box Component to represent collision
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Mesh"));
	// Attaching Box Component to Root Component
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(30.f, 25.f, 12.f));
	Collision->SetCollisionProfileName("Trigger");
	// Creating overlap event
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAmmoLoad::OnOverlapBegin);

	// Setting mesh component
	if (AlternateMeshAsset.Succeeded())
	{
		// Create the Static Mesh Component
		AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
		// Attaching Static Mesh Component to Root Component
		AmmoMesh->SetupAttachment(RootComponent);
		// Settings for our static mesh
		AmmoMesh->SetRelativeLocation(FVector(0, 0, -13.f));
		AmmoMesh->SetStaticMesh(AlternateMeshAsset.Object);
		AmmoMesh->SetCollisionProfileName("OverlapOnlyPawn");
		AmmoMesh->SetRelativeScale3D(FVector(2, 2, 2));
	}
}

// Function for overlap
void AAmmoLoad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check overlapped actor
	if (OtherActor != nullptr && OtherActor != this)
	{
		// For get and set max ammo quantity after that destroy actor
		class AShooterCharacter* MyCharacter = Cast<AShooterCharacter>(OtherActor);
		if (MyCharacter)
		{
			MyCharacter->UpdateMaxAmmo(Ammo);
			Destroy();
		}
	}
}

