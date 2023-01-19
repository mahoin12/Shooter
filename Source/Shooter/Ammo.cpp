// Fill out your copyright notice in the Description page of Project Settings.


#include "Ammo.h"
#include "ShooterCharacter.h"
#include "Components/BoxComponent.h"

#pragma region Ali
// Sets default values
AAmmo::AAmmo()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AlternateMeshAsset(TEXT("/Game/Megascans/3D_Assets/Wooden_Military_Crate_ucooedgfa/S_Wooden_Military_Crate_ucooedgfa_lod3"));
	// Create the Root Component
	if (RootComponent == nullptr)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	}

	// Creating Box Component to represent collision
	Collision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Mesh"));
	// Attaching Box Component to Root Component
	Collision->SetupAttachment(RootComponent);
	Collision->SetBoxExtent(FVector(35.f, 35.f, 12.f));
	Collision->SetCollisionProfileName("Trigger");
	// Creating overlap event
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AAmmo::OnOverlapBegin);

	// Setting mesh component
	if (AlternateMeshAsset.Succeeded())
	{
		// Create the Static Mesh component
		AmmoMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
		// Attaching Static Mesh Component to Root Component
		AmmoMesh->SetupAttachment(RootComponent);
		// Settings for our static mesh
		AmmoMesh->SetRelativeLocation(FVector(0, 0, -13.f));
		AmmoMesh->SetStaticMesh(AlternateMeshAsset.Object);
		AmmoMesh->SetCollisionProfileName("OverlapOnlyPawn");
	}
}

#pragma endregion Ali

// Function for overlap
void AAmmo::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Check overlapped actor
	if (OtherActor != nullptr && OtherActor != this)
	{
		// For get and set ammo quantity after that destroy actor
		if (class AShooterCharacter* MyCharacter = Cast<AShooterCharacter>(OtherActor))
		{
			if (MyCharacter->GetAmmo() < MyCharacter->GetMaxAmmo())
			{
				MyCharacter->UpdateAmmo(Ammo);
				if (MyCharacter->PopUp)
				{
					MyCharacter->SetPopUp(false);
				}
				Destroy();
				//MyCharacter->Destroy();
			}
		}
	}
}
