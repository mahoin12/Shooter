// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

UCLASS(config=Game)
class AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AShooterCharacter();

	void SetPopUp(bool val);

protected:
	virtual void BeginPlay() override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	float BaseLookUpRate;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector GunOffset;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category= "Projectile")
	TSubclassOf<class AShooterProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	UAnimMontage* FireAnimation;

	//The total amount of ammo that can be carried for the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxTotalAmmo = 0;

	//The total amount of ammo that can be in the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxClipAmmo = 0;

	//The total amount of ammo being carried for the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo = 0;

	//The amount of ammo in the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ClipAmmo = 0;

	//The time it takes to reload the weapon.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ReloadTime = 1.0f;

	/** Get Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool PopUp = false;

	/** Get Ammo quantity as Text */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	FText GetAmmoIntText() const;

	/** Get Ammo quantity */
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int GetAmmo() const;

	/** Get Max Ammo quantity*/
	UFUNCTION(BlueprintPure, Category = "Ammo")
	int GetMaxAmmo() const;

	/** Update Ammo quantity*/
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void UpdateAmmo(int AmmoChange);

	/** Increase Max Ammo quantity*/
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void UpdateMaxAmmo(int AmmoChange);

protected:
	
	/** Fires a projectile. */
	void OnFire();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles stafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	//Reloads the current weapon
	void ReloadWeapon();

	// Function for switch character when we press "P"
	void SwitchCharacter();

	// Pointer for other character which we choose in editor
	UPROPERTY(EditAnywhere, Category = "Possession")
	AShooterCharacter* OtherCharacter;

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
};

