// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/STT_Log.h"
#include <AIController.h>
//#include <StateTreeExecutionContext.h>
//#include <PropertyBag.h>

EStateTreeRunStatus USTT_Log::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Enter state %s, %s, %s"), *GetClass()->GetName(), *Message,
		*Actor->GetName(), *AIController->GetName());
	return EStateTreeRunStatus::Succeeded;
}

void USTT_Log::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Exit state %s"), *GetClass()->GetName(), *Message);
}
