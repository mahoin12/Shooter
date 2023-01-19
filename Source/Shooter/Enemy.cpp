// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBar.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemy::AEnemy()
{
	// Setting each assets location
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AlternateMeshAsset(TEXT("SkeletalMesh'/Game/_Main/Anim/copzombie_l_actisdato.copzombie_l_actisdato'"));
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimObj(TEXT("AnimBlueprint'/Game/_Main/Anim/Animation/BP_ZombieAnim.BP_ZombieAnim'"));
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetObj(TEXT("/Game/_Main/BluePrints/Widgets/WBP_EnemyHealthBar"));

	// Setting root component
	if (RootComponent == nullptr)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	}

	// Setting mesh component
	if (AlternateMeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(AlternateMeshAsset.Object);
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
		GetMesh()->SetRelativeRotation(FRotator(FRotator(0.0f, -90.0f, 0.0f)));
		GetCapsuleComponent()->SetRelativeScale3D(FVector(1, 1, 1));
	}

	// Setting animation object
	if (AnimObj.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(AnimObj.Object->GeneratedClass);
	}

	// Setting widgets for health bar
	if (WidgetObj.Succeeded()) 
	{
		HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar"));
		HealthWidgetComp->SetupAttachment(RootComponent);
		HealthWidgetComp->SetRelativeLocation(FVector(0, 0, 120.f));
		HealthWidgetComp->SetWidgetClass(WidgetObj.Class);
		HealthWidgetComp->SetCollisionProfileName("NoCollision");
		HealthWidgetComp->SetDrawSize(FVector2D(300, 30));
		HealthWidgetComp1 = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBar1"));
		HealthWidgetComp1->SetupAttachment(RootComponent);
		HealthWidgetComp1->SetRelativeLocation(FVector(0, 0, 120.f));
		HealthWidgetComp1->SetRelativeRotation(FRotator(0 , 180.f, 0));
		HealthWidgetComp1->SetWidgetClass(WidgetObj.Class);
		HealthWidgetComp1->SetCollisionProfileName("NoCollision");
		HealthWidgetComp1->SetDrawSize(FVector2D(300, 30));
	}

	// Initial health equal to maxhealth
	Health = MaxHealth;

	// Settings for smooth chase player
	bUseControllerRotationYaw = false;
	// Second line is more effective
	// GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Setting owner of each healthbar which will be represent
	if (UEnemyHealthBar* HealthBar = Cast<UEnemyHealthBar>(HealthWidgetComp->GetUserWidgetObject()))
	{
		HealthBar->SetOwnerEnemy(this);
	}
	if (UEnemyHealthBar* HealthBar1 = Cast<UEnemyHealthBar>(HealthWidgetComp1->GetUserWidgetObject()))
	{
		HealthBar1->SetOwnerEnemy(this);
	}
}

// Take damage function for each bullets which carried random number between 5-10
void AEnemy::TakingDamage(const float Damage)
{
	Health -= Damage;
	if (Health < 1)
	{
		Health = 0;
		bDead = true;
	}
	else
	{
		bTakenDamage = true;
	}
}

