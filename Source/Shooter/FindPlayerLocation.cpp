// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "EnemyAIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "blackboard_keys.h"
#include "Enemy.h"

UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const& Object_Initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& Owner_Comp, uint8* Node_Memory)
{
	// Getting player character and the NPC's controller
	const ACharacter* Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const Cont = Cast<AEnemyAIController>(Owner_Comp.GetAIOwner());

	// Getting player and enemy location
	FVector const Player_Location = Player->GetActorLocation();
	const AEnemy* EnemyRef = Cast<AEnemy>(Cont->GetPawn());
	FVector const Enemy_Location = EnemyRef->GetActorLocation();
	// Calculating distance between enemy and player
	float const Distance = FVector::Dist(Player_Location, Enemy_Location);

	// If character is not dead and player inside search_radius then will chase player
	if (Distance < Search_Radius && !EnemyRef->bDead )
	{
		Cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, Player_Location);
	}
	// Else destination point will be own location means not move
	else
	{
		Cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, Enemy_Location);
	}


	// Finish with success
	FinishLatentTask(Owner_Comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
