// Fill out your copyright notice in the Description page of Project Settings.


#include "GridPawn.h"
#include "GridPlayerController.h"
#include "TestGridGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGridPawn::AGridPawn()
{
	//
	// N.B il pawn viene spawnato automaticamente nella posizione del player start
	// dato che il pawn di default � stato impostato nei setting come BP_GridPawn
	//  
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Collider = CreateDefaultSubobject<UBoxComponent>(TEXT("Collider"));
	RootComponent = Collider;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(Collider);

	//// nodi
	LastNode = nullptr;
	TargetNode = nullptr;
	NextNode = nullptr;
}

void AGridPawn::SetVerticalInput(float AxisValue)
{
}

void AGridPawn::SetHorizontalInput(float AxisValue)
{
}

// Called when the game starts or when spawned
void AGridPawn::BeginPlay()
{
	Super::BeginPlay();
	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());
	TheGridGen = GameMode->GField;
	CurrentMovementSpeed = NormalMovementSpeed;
	CanMove = true;
	Collider->OnComponentBeginOverlap.AddDynamic(this, &AGridPawn::OnOverlapBegin);

}

void AGridPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

// Called every frame
void AGridPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CanMove)
	{
		HandleMovement();
	}
}

void AGridPawn::OnClick()
{
}

void AGridPawn::HandleMovement()
{
	MoveToCurrentTargetNode();
	if (TargetNode == nullptr)
	{
		if (NextNode != nullptr)
		{
			SetTargetNode(NextNode);
			SetNextNode(nullptr);
		}
	}
}

// Called to bind functionality to input
void AGridPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AGridPawn::MoveToCurrentTargetNode()
{
	if (TargetNode == nullptr) return;
	const float Dist = FMath::Abs(FVector::Dist2D(TargetNode->GetActorLocation(), GetActorLocation()));
	if (Dist <= AcceptedDistance)
	{
		OnNodeReached();
		return;
	}
	// funzione di interpolazione che fa muovere il pawn verso una nuova posizione data la posizione corrente
	const FVector2D StartVector = TheGridGen->GetTwoDOfVector(GetActorLocation());
	const FVector2D EndVector = TheGridGen->GetTwoDOfVector(TargetNode->GetActorLocation());
	const auto Pos = FMath::Vector2DInterpConstantTo(StartVector, EndVector, GetWorld()->GetDeltaSeconds(), CurrentMovementSpeed);
	const FVector Location(Pos.X, Pos.Y, GetActorLocation().Z);
	SetActorLocation(Location);
}

void AGridPawn::OnNodeReached()
{
	CurrentGridCoords = TargetNode->GetGridPosition();
	LastNode = TargetNode;
	SetTargetNode(nullptr);
}

void AGridPawn::SetTargetNode(AGridBaseNode* Node)
{
	TargetNode = Node;
}

void AGridPawn::SetNextNode(AGridBaseNode* Node)
{
	NextNode = Node;
}

void AGridPawn::SetNodeGeneric(const FVector Dir)
{
	const auto Node = TheGridGen->GetNextNode(CurrentGridCoords, Dir);
	if (TheGridGen->IsNodeValidForWalk(Node))
	{
		SetTargetNode(Node);
	}
}

void AGridPawn::SetNextNodeByDir(FVector InputDir, bool ForceLast)
{
	const FVector2D Coords = TargetNode ? TargetNode->GetGridPosition() : LastNode->GetGridPosition();
	const auto Node = GameMode->GField->GetNextNode(Coords, InputDir);
	if (GameMode->GField->IsNodeValidForWalk(Node))
	{
		SetNextNode(Node);
		SetLastValidDirection(InputDir);
	}
}

void AGridPawn::PowerModeOn()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Stepped on a power node")));
	CurrentMovementSpeed = 800.0f;
	float PowerModeTime = 5;
	// set timer to call UFUNCTION that resets speed to default value
	// // should also enter in frightened mode, will do later
	GetWorld()->GetTimerManager().SetTimer(PowerModeTimer, this, &AGridPawn::PowerModeOff, PowerModeTime, false);
}

void AGridPawn::PowerModeOff()
{
	CurrentMovementSpeed = 400.0f;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("End of power mode")));
}

FVector AGridPawn::GetLastValidDirection() const
{
	return LastValidInputDirection;
}

AGridBaseNode* AGridPawn::GetLastNode() const
{
	return LastNode;
}

AGridBaseNode* AGridPawn::GetTargetNode() const
{
	return TargetNode;
}

FVector2D AGridPawn::GetLastNodeCoords() const
{
	if (LastNode)
	{
		return LastNode->GetGridPosition();
	}
	return FVector2D(0, 0);
}

FVector2D AGridPawn::GetTargetNodeCoords() const
{
	if (TargetNode)
	{
		return TargetNode->GetGridPosition();
	}
	return FVector2D::ZeroVector;
}

void AGridPawn::SetLastValidDirection(FVector Dir)
{
	if (Dir == FVector::ZeroVector) return;
	LastValidInputDirection = Dir;
}