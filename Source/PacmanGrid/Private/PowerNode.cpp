// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerNode.h"
#include "PacmanPawn.h"


APowerNode::APowerNode()
{
    EEatId = NotEaten;
    Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
    RootComponent = Collider;
    UStaticMeshComponent* MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(Collider);
    FVector BoxDimension = FVector(40.f, 40.f, 120.f);
    Collider->SetBoxExtent(BoxDimension);
}




void APowerNode::BeginPlay()
{
    Super::BeginPlay();
    //registrazione degli eventi di collisione attraverso AddDynamic
    Collider->OnComponentBeginOverlap.AddDynamic(this, &APowerNode::OnBeginOverlap);
}

void APowerNode::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    //check se non è mai stato mangiato
    if (this->CheckNotEaten())
    {
        if (OtherActor->IsA(AGridPawn::StaticClass())) //check collisione con pacman
        {
            //  debug string
            GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, FString::Printf(TEXT("collision powernode")));
            APlayerController* PacmanController = GetWorld()->GetFirstPlayerController();
            const auto Pacman = Cast<APacmanPawn>(PacmanController->GetPawn());
            Pacman->PowerModeOn(); //mode powered On
            this->setEaten();
            SetActorHiddenInGame(true);
        }
    }

}





