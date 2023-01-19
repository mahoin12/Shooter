// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "FindPlayerLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SHOOTER_API UFindPlayerLocation:public UBTTask_BlackboardBase
{
	GENERATED_BODY()


public:

	// Initialize BTTask
	UFindPlayerLocation(FObjectInitializer const& Object_Initializer);

	// ExecuteTask carried by BTTask
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& Owner_Comp, uint8* Node_Memory) override;

protected:

	// Editor editable radius variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowProtectedAccess = "true"))
	float Search_Radius = 1200.0f;
};
