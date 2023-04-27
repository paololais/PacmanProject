// Fill out your copyright notice in the Description page of Project Settings.

#include "PointNode.h"
#include "PacmanPawn.h"
#include "TestGridGameMode.h"


APointNode::APointNode()
{
    EEatId = NotEaten;

    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Collider;
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(Collider);
    FVector BoxDimension = FVector(40.f, 40.f, 120.f);
    Collider->SetBoxExtent(BoxDimension);
}

void APointNode::BeginPlay()
{
    Super::BeginPlay();
    //registrazione degli eventi di collisione attraverso AddDynamic
    Collider->OnComponentBeginOverlap.AddDynamic(this, &APointNode::OnBeginOverlap);

    //GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());
}


void APointNode::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{   /*
    if (this->CheckNotEaten())
    {
        if (OtherActor->IsA(APacmanPawn::StaticClass()))
        {
            this->setEaten();
            APlayerController* PacmanController = GetWorld()->GetFirstPlayerController();
            const auto Pacman = Cast<APacmanPawn>(PacmanController->GetPawn());

            //Score System
            int new_value = (GameMode->getScore()) + 10;

            GameMode->setScore(new_value);

            //GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Score: %d"), GameMode->getScore()));
        }
    }
    */
}
