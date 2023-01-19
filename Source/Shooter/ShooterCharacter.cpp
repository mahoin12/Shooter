// Copyright Epic Games, Inc. All Rights Reserved.

#include "ShooterCharacter.h"
#include "ShooterProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All)

//////////////////////////////////////////////////////////////////////////
// AShooterCharacter

AShooterCharacter::AShooterCharacter()
{
	static ConstructorHelpers::FObjectFinder<UAnimMontage> AnimObj(TEXT("AnimMontage'/Game/FirstPerson/Animations/FirstPersonFire_Montage.FirstPersonFire_Montage'"));

	OtherCharacter = nullptr;
	FireSound = nullptr;
	FireAnimation = AnimObj.Object;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	// Default offset from the character location for projectiles to spawn
	GunOffset = FVector(0.0f, 0.0f, 10.0f);
	
}

void AShooterCharacter::SetPopUp(bool val)
{
	PopUp=val;
}

void AShooterCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	Mesh1P->SetHiddenInGame(false, true);
}

//////////////////////////////////////////////////////////////////////////
// Input

void AShooterCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	
	// set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AShooterCharacter::OnFire);
	// Bind reload event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AShooterCharacter::ReloadWeapon);
	// Bind player switch event
	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AShooterCharacter::SwitchCharacter);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShooterCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AShooterCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AShooterCharacter::LookUpAtRate);
}

FText AShooterCharacter::GetAmmoIntText() const
{
	const FString ClipAmmoHUD = FString::FromInt(ClipAmmo);
	const FString TotalAmmoHUD = FString::FromInt(TotalAmmo);
	const FString AmmoHUD = ClipAmmoHUD + FString(TEXT("/")) + TotalAmmoHUD;
	FText AmmoText = FText::FromString(AmmoHUD);
	return AmmoText;
}

// Set totalAmmo
int AShooterCharacter::GetAmmo() const
{
	return TotalAmmo;
}

// Set maxTotalAmmo
int AShooterCharacter::GetMaxAmmo() const
{
	return MaxTotalAmmo;
}

// Update ammo quantity
void AShooterCharacter::UpdateAmmo(int const AmmoChange)
{
	TotalAmmo += AmmoChange;
	if (TotalAmmo > MaxTotalAmmo)
	{
		TotalAmmo = MaxTotalAmmo;
	}
}

// Update new maximum ammo limit 
void AShooterCharacter::UpdateMaxAmmo(int const AmmoChange)
{
	MaxTotalAmmo += AmmoChange;
}

void AShooterCharacter::OnFire()
{
	
	if (ClipAmmo > 0)
	{
		// try and fire a projectile
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{

				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = ((FP_MuzzleLocation != nullptr) ? FP_MuzzleLocation->GetComponentLocation() : GetActorLocation()) + SpawnRotation.RotateVector(GunOffset);

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

				// spawn the projectile at the muzzle
				World->SpawnActor<AShooterProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}

		// try and play the sound if specified
		if (FireSound != nullptr)
		{
			UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation());
		}

		// try and play a firing animation if specified
		if (FireAnimation != nullptr)
		{
			// Get the animation object for the arms mesh
			UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
			if (AnimInstance != nullptr)
			{
				AnimInstance->Montage_Play(FireAnimation, 1.f);
			}
		}
		ClipAmmo -= 1;
	}
	else if (TotalAmmo > 0)
	{
		ReloadWeapon();
	}
	else
	{
		PopUp = true;
	}
}

void AShooterCharacter::MoveForward(const float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Val);
	}
}

void AShooterCharacter::MoveRight(const float Val)
{
	if (Val != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Val);
	}
}

void AShooterCharacter::TurnAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookUpAtRate(const float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Reload algorithm
void AShooterCharacter::ReloadWeapon()
{
	if (ClipAmmo < MaxClipAmmo)
	{
		if (TotalAmmo - (MaxClipAmmo - ClipAmmo) > 0)
		{
			TotalAmmo -= MaxClipAmmo - ClipAmmo;
			ClipAmmo = MaxClipAmmo;
		}
		else
		{
			ClipAmmo += TotalAmmo;
			TotalAmmo = 0;
		}
	}
}

//Swithing between characters and disabling previus one
void AShooterCharacter::SwitchCharacter()
{
	if (OtherCharacter && GetController())
	{
		AController* Temp = GetController();
		if (Temp)
		{
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			Temp->UnPossess();
			OtherCharacter->SetActorHiddenInGame(false);
			OtherCharacter->SetActorEnableCollision(true);
			Temp->Possess(OtherCharacter);
		}
	}
}