// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnemyAIController.h"
#include "blackboard_keys.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UChasePlayer::UChasePlayer(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Chase Player");
}

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	// Get TargetLocation from blackboard via AI controller
	AEnemyAIController* const cont = Cast<AEnemyAIController>(owner_comp.GetAIOwner());
	FVector const target_location = cont->get_blackboard()->GetValueAsVector(bb_keys::target_location);

	// Move to the target location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(cont, target_location);

	// Finish with success
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
