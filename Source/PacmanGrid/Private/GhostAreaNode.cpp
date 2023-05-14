// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostAreaNode.h"
#include <Kismet/GameplayStatics.h>


AGhostAreaNode::AGhostAreaNode()
{
}

void AGhostAreaNode::BeginPlay()
{
	Super::BeginPlay();

	APhantomPawn* PhantomPawn = Cast<APhantomPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APhantomPawn::StaticClass()));
	if (PhantomPawn)
	{
		Ghost = PhantomPawn;
		if (Ghost->IsIdleState() || Ghost->IsDeadState())
		{
			EWalkableId = Walkable;
		}
		else
		{
			EWalkableId = NotWalkable;
		}
	}
}

bool AGhostAreaNode::CanEnter()
{
	if (IsValid(Ghost) && (Ghost->IsIdleState() || Ghost->IsDeadState()))
	{
		return true;
	}
	else
	{
		return false;
	}
}
