// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	

public:

	AEnemyAIController(FObjectInitializer const& object_initializer = FObjectInitializer::Get());

	void BeginPlay() override;

	virtual void OnPossess(APawn* const Pawn) override;

	class UBlackboardComponent* get_blackboard() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "True"))
		class UBehaviorTreeComponent* behavior_tree_component;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "True"))
		class UBehaviorTree* btree;
	
	class UBlackboardComponent* Blackboard;
};
