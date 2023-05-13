// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAreaNode.h"
#include <Kismet/GameplayStatics.h>

bool AGhostAreaNode::CanEnter()
{
	Ghost = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APhantomPawn::StaticClass()));
	if (Ghost->IsIdleState() || Ghost->IsDeadState())
	{
		return true;
	}
	return false;
}
