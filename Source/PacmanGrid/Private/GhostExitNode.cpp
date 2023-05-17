// Fill out your copyright notice in the Description page of Project Settings.


#include "GhostExitNode.h"
#include "PhantomPawn.h"
#include <GameFramework/CheatManager.h>

void AGhostExitNode::BeginPlay()
{
	Super::BeginPlay();
}

void AGhostExitNode::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor->IsA(APhantomPawn::StaticClass()))
    {
        const auto Ghost = Cast<APhantomPawn>(OtherActor);
        if (IsValid(Ghost))
        {
            // Ottieni il vettore di direzione del fantasma
            FVector GhostDirection = Ghost->GetLastValidDirection();

            // Controlla la direzione e lo stato del fantasma per determinare se permettere o bloccare il transito
            if (GhostDirection.X > 0) // Direzione verso l'alto
            {
                if (Ghost->IsChaseState() || Ghost->IsScatterState())
                {
                    // Permetti il transito
                    OverlappedComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                }
                else
                {
                    // Blocca il transito
                    OverlappedComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                }
            }
            else if (GhostDirection.X < 0) // Direzione verso il basso
            {
                if (Ghost->IsDeadState())
                {
                    // Permetti il transito
                    OverlappedComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                }
                else
                {
                    // Blocca il transito
                    OverlappedComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                }
            }
        }
    }
    else
    {
        // Attiva la collisione per gli altri attori/pawn
        OverlappedComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    }
}