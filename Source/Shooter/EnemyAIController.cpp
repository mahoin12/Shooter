// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Uobject/ConstructorHelpers.h"
#include "Engine/World.h"


AEnemyAIController::AEnemyAIController(FObjectInitializer const& object_initializer)
{
	// Setting location of BehaviorTree
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>obj(TEXT("BehaviorTree'/Game/_Main/BluePrints/Behavior/BT_Enemy.BT_Enemy'"));
	if (obj.Succeeded())
	{
		btree = obj.Object;
	}
	// Creating behaviour tree component
	behavior_tree_component = object_initializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorComp"));
	// Creating blackboard
	Blackboard = object_initializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));
}
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();
	// At start start behavior tree
	RunBehaviorTree(btree);
	behavior_tree_component->StartTree(*btree);
}

//  To possess a pawn
void AEnemyAIController::OnPossess(APawn* const Pawn1)
{
	Super::OnPossess(Pawn1);
	// Initialize blackboard
	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*btree->BlackboardAsset);
	}
}

// For get blackboard
UBlackboardComponent* AEnemyAIController::get_blackboard() const
{
	return Blackboard;
}