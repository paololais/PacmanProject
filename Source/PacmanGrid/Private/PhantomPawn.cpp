// Fill out your copyright notice in the Description page of Project Settings.


#include "PhantomPawn.h"
#include "PacmanPawn.h"
#include "TestGridGameMode.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

APhantomPawn::APhantomPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// get the game instance reference
	GameInstance = Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

	DeadSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Engine/MapTemplates/Sky/M_BlackBackground.M_BlackBackground'"));

	VulnerableSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_Blue.M_Blue'"));

	FlashingSkin = LoadObject<UMaterialInterface>(nullptr, TEXT("Material'/Game/Materials/M_FlashSkin.M_FlashSkin'"));

	GhostLeaveTime = 4.0f;
}

void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();

	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	CurrentMovementSpeed = NormalGhostSpeed;

	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));

	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());

	GlobalCounter = 0;
}

void APhantomPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	const auto Pacman = Cast<APacmanPawn>(OtherActor);

	//chase state
	if (this->IsChaseState() || this->IsScatterState())
	{
		if (Pacman && IsValid(GameInstance)) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("I Reached you")));

			//decrementa vita di 1
			int newvalue = (GameInstance->GetLives()) - 1;
			GameInstance->SetLives(newvalue);

			GlobalCounter = 0;
			Pacman->OnPacmanDead();
		}
	}

	else if (this->IsFrightenedState()) {
		if (Pacman) {
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("you killed a ghost")));

			UGameplayStatics::PlaySound2D(this, GhostDeadSound);

			int killings = Pacman->GetNumberOfGhostsKilled() + 1;
			Pacman->SetNumberOfGhostsKilled(killings);

			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("ghosts killed: %d"), killings));

			this->SetDeadState();

			//score based on number of ghosts killed
			if (IsValid(GameInstance)) {
				int new_score = (GameInstance->getScore());

				switch (killings)
				{
				case 0:
					GameInstance->setScore(new_score);
					break;
				case 1:
					new_score += 400;
					GameInstance->setScore(new_score);
					break;
				case 2:
					new_score += 800;
					GameInstance->setScore(new_score);
					break;
				case 3:
					new_score += 1600;
					GameInstance->setScore(new_score);
					break;
				default:
					break;
				}
			}
		}
	}
}

void APhantomPawn::OnNodeReached()
{
	Super::OnNodeReached();

	//vuole entrare nella ghost area
	if (CurrentGridCoords == (FVector2D(17, 13)))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("trying to enter")));
		// Permetti il transito
		if (this->IsDeadState())
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("enter granted")));
		}
		//ghost is leaving ghost area
		else if (this->bIsLeaving)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, FString::Printf(TEXT("is leaving")));
		}

		//Non può entrare, prosegue per la sua direzione
		else
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("enter denied")));
			LastValidInputDirection = PreviousDirection;
			SetNextNodeByDir(LastValidInputDirection, true);
			SetTargetNode(NextNode);
		}
	}

	//vuole uscire dalla ghost area
	else if ((CurrentGridCoords == (FVector2D(15, 13)) || CurrentGridCoords == (FVector2D(15, 12)) || CurrentGridCoords == (FVector2D(15, 14))) && (LastInputDirection.X > 0 || LastValidInputDirection.X > 0))
		//if (CurrentGridCoords == (FVector2D(15, 13)) && (LastInputDirection.X>0 || LastValidInputDirection.X>0))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("trying to exit")));

		// Permetti il transito
		if ((IsChaseState() || IsScatterState()) && bIsLeaving)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("exit granted")));
			this->bIsHome = false;
		}
		// Blocca il transito
		else
		{
			if (CurrentGridCoords == FVector2D(15, 12)) {
				LastNode = (*(GridGenTMap.Find(FVector2D(15, 12))));
				//torna indietro
				FVector OppositeDirection = -GetLastValidDirection();
				SetNextNodeByDir(OppositeDirection, true);
				SetTargetNode(NextNode);
			}
			else if (CurrentGridCoords == FVector2D(15, 13)) {
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("exit denied")));
				LastNode = (*(GridGenTMap.Find(FVector2D(15, 13))));
				//torna indietro
				FVector OppositeDirection = -GetLastValidDirection();
				SetNextNodeByDir(OppositeDirection, true);
				SetTargetNode(NextNode);
			}
			
			else { // (CurrentGridCoords == FVector2D(15, 14))
				LastNode = (*(GridGenTMap.Find(FVector2D(15, 14))));
				//torna indietro
				FVector OppositeDirection = -GetLastValidDirection();
				SetNextNodeByDir(OppositeDirection, true);
				SetTargetNode(NextNode);
			}
		}
	}

	else if ((CurrentGridCoords == (FVector2D(17, 12)) || CurrentGridCoords == (FVector2D(17, 14))) && (this->bIsLeaving == true)) {
		this->bIsLeaving = false;
	}

	//tunnel speed
	//tunnel sx
	else if (CurrentGridCoords == FVector2D(14, 4))
	{
		//enters the tunnel
		if (LastValidInputDirection.Y < 0 )
		{
			this->SetSpeed(TunnelSpeed);
		}
		//leaves the tunnel
		else
		{
			//checks ghost state
			if (IsFrightenedState())
			{
				this->SetSpeed(FrightenedSpeed);
			}
			else
			{
				this->SetSpeed(NormalGhostSpeed);
			}
		}
	}
	//tunnel dx
	else if (CurrentGridCoords == FVector2D(14, 23))
	{
		//enters the tunnel
		if (LastValidInputDirection.Y > 0)
		{
			this->SetSpeed(TunnelSpeed);
		}
		//leaves the tunnel
		else
		{
			//checks ghost state
			if (IsFrightenedState())
			{
				this->SetSpeed(FrightenedSpeed);
			}
			else
			{
				this->SetSpeed(NormalGhostSpeed);
			}
		}
	}
}

void APhantomPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (this->GetTargetNode() == nullptr)
	{
		SetGhostTarget();
	}
}

APacmanPawn* APhantomPawn::GetPlayer() const
{
	return Player;
}

AGridBaseNode* APhantomPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();
}

void APhantomPawn::SetGhostTarget()
{
	//chase state allora insegue player
	if (this->IsChaseState() && !bIsLeaving && !bIsHome)
	{
		const AGridBaseNode* Target = GetPlayerRelativeTarget();
		if (!Target)
		{
			Target = GetPlayer()->GetLastNode();
		}

		AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
		}
	}
	//se morto va a casa
	else if (this->IsDeadState())
	{
		//override della casella home per ciascun ghost
		this->GoHome();
	}

	//se è a casa e non sta uscendo continua ad andare su e giù per la casa
	else if (bIsHome && !bIsLeaving)
	{
		this->UpAndDown();
	}

	else if (this->IsScatterState() && !bIsLeaving && !bIsHome) {
		this->GoToHisCorner();
	}

	else if (this->IsFrightenedState())
	{
		// Randomly select a target node for the ghost
		const TArray<AGridBaseNode*>& AllNodes = TheGridGen->GetTileArray();
		if (AllNodes.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, AllNodes.Num() - 1);
			AGridBaseNode* RandomNode = AllNodes[RandomIndex];
			
			AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), RandomNode->GetGridPosition(), -(this->GetLastValidDirection()));

			if (PossibleNode)
			{
				this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
			}
		}
	}

	else if (bIsLeaving && (IsScatterState() || IsChaseState() || IsFrightenedState()) )
	{
		this->ExitGhostArea();
	}
}

void APhantomPawn::RespawnGhostStartingPosition()
{
	GetWorld()->GetTimerManager().ClearTimer(GhostLeaveTimer);

	int LocationX = StartPosition[MyIndex()].X;
	int LocationY = StartPosition[MyIndex()].Y;

	const FVector Location(LocationX*100, LocationY*100, GetActorLocation().Z);

	LastNode = (*(GridGenTMap.Find(StartPosition[MyIndex()])));
	SetNextNode(*(GridGenTMap.Find(StartPosition[MyIndex()])));
	SetTargetNode(NextNode);

	SetActorLocation(Location);

	this->SetSpeed(NormalGhostSpeed);
	this->SetActorHiddenInGame(false);
}

void APhantomPawn::GoHome()
{
	this->SetDeadState();

	const AGridBaseNode* Target = *(GridGenTMap.Find(HomePosition[MyIndex()]));

	AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == HomePosition[MyIndex()])
	{
		this->AlternateScatterChase();
		this->bIsLeaving = true;
	}
}

void APhantomPawn::ExitGhostArea()
{	
	const AGridBaseNode* Target1 = *(GridGenTMap.Find(FVector2D(15, 13)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target1->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(15, 13))
	{
		//esci
		const AGridBaseNode* Target2 = *(GridGenTMap.Find(FVector2D(17, 13)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target2->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}
	}
}

//funzione che inverte la direzione del fantasma, con un certo ritardo casuale
void APhantomPawn::ReverseDirection() {

	// Genera un valore casuale per il ritardo tra 0 e 1 secondo
	float Delay = FMath::FRandRange(0.f, 1.f);

	// Calcola la direzione opposta rispetto all'ultima direzione valida
	FVector OppositeDirection = -GetLastValidDirection();

	SetNextNodeByDir(OppositeDirection, true);
}

void APhantomPawn::GoToHisCorner()
{
}

void APhantomPawn::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(GhostLeaveTimer);
}

void APhantomPawn::SetSequencePoint(int a)
{
	SequencePoint = a;
}

void APhantomPawn::AlternateScatterChase() {
	switch (SequencePoint)
	{
	case 1:
		SetScatterState();
		SetSequencePoint(2);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this, &APhantomPawn::AlternateScatterChase, 7.0f, false);
		break;

	case 2:
		SetChaseState();
		SetSequencePoint(3);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 20.0f, false);
		break;

	case 3:
		SetScatterState();
		SetSequencePoint(4);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 7.0f, false);
		break;

	case 4:
		SetChaseState();
		SetSequencePoint(5);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 20.0f, false);
		break;

	case 5:
		SetScatterState();
		SetSequencePoint(6);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 5.0f, false);
		break;

	case 6:
		SetChaseState();
		SetSequencePoint(7);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 20.0f, false);
		break;

	case 7:
		SetScatterState();
		SetSequencePoint(8);
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APhantomPawn::AlternateScatterChase, 5.0f, false);
		break;

		//stato di chase per sempre
	default:
		SetChaseState();
		break;
	}
}

void APhantomPawn::SetChaseState()
{
	if (this->IsScatterState()) {
		//change direction
		this->ReverseDirection();
	}

	StaticMesh->SetMaterial(2, DefaultSkin);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Chase mode")));
	this->EEnemyState = Chase;
	this->SetSpeed(NormalGhostSpeed);
}

bool APhantomPawn::IsChaseState()
{
	if (this->EEnemyState == Chase) return true;

	else return false;
}

void APhantomPawn::SetScatterState()
{
	if (IsChaseState()) {
		//change direction
		ReverseDirection();
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Scatter mode")));
	StaticMesh->SetMaterial(2, DefaultSkin);
	this->EEnemyState = Scatter;
	this->SetSpeed(NormalGhostSpeed);
}

bool APhantomPawn::IsScatterState()
{
	if (this->EEnemyState == Scatter) return true;

	else return false;
}

void APhantomPawn::SetFrightenedState()
{
	if (this->IsDeadState())
	{
		return;
	}
	//change direction
	this->ReverseDirection();
	StaticMesh->SetMaterial(2, VulnerableSkin);
	this->EEnemyState = Frightened;
	this->SetSpeed(FrightenedSpeed);
}

bool APhantomPawn::IsFrightenedState()
{
	if (this->EEnemyState == Frightened) return true;

	else return false;
}

void APhantomPawn::SetDeadState()
{
	StaticMesh->SetMaterial(2, DeadSkin);
	this->EEnemyState = Dead;
	this->SetSpeed(DeadSpeed);
}

bool APhantomPawn::IsDeadState()
{

	if (this->EEnemyState == Dead) return true;

	else return false;
}

void APhantomPawn::UpAndDown() {
	FVector2D center = HomePosition[MyIndex()];
	//nodo alto dx
	const AGridBaseNode* Up = *(GridGenTMap.Find(FVector2D(center.X+1, center.Y)));

	AGridBaseNode* PossibleNode1 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Up->GetGridPosition(), -(this->GetLastValidDirection()));

	if (PossibleNode1)
	{
		this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode1->GetGridPosition() - this->GetLastNodeCoords()), true);
	}

	if (CurrentGridCoords == FVector2D(center.X + 1, center.Y))
	{
		//nodo basso dx
		const AGridBaseNode* Down = *(GridGenTMap.Find(FVector2D(center.X - 1, center.Y)));

		AGridBaseNode* PossibleNode2 = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Down->GetGridPosition(), -(this->GetLastValidDirection()));

		if (PossibleNode2)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode2->GetGridPosition() - this->GetLastNodeCoords()), true);
		}

		if (CurrentGridCoords == FVector2D(center.X - 1, center.Y))
		{
			this->UpAndDown();
		}
	}
}

void APhantomPawn::CanExitHouse()
{
	//use each ghost counter
	if ((GameInstance->GetLives()) == 3)
	{
		this->IncrementPointCounter();

		int counter = this->PointGhostCounter();

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("ghost Counter = %d"), counter));
		int limit = this->PointGhostLimit();
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("ghost limit = %d"), limit));
		
		if (this->PointGhostCounter() >= this->PointGhostLimit())
		{
			this->bIsLeaving = true;
			GetWorld()->GetTimerManager().ClearTimer(this->GhostLeaveTimer);
			this->ResetPointCounter();
			Player->SetNextPreferredGhost();
		}
	}
	//use global counter
	else
	{
		this->GlobalCounter++;
		int counter = this->GlobalCounter;

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("global Counter = %d"), counter));
		int limit = this->GlobalLimits[this->MyIndex()];
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, FString::Printf(TEXT("my global limit = %d"), limit));
		
		if (counter >= limit)
		{
			this->bIsLeaving = true;
			GetWorld()->GetTimerManager().ClearTimer(this->GhostLeaveTimer);
			Player->SetNextPreferredGhost();
		}
	}
}

void APhantomPawn::ExitOnTimer()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Exit on timer")));
	GetWorld()->GetTimerManager().ClearTimer(this->GhostLeaveTimer);
	this->bIsLeaving = true;

	Player->SetNextPreferredGhost();
	if(IsValid((Player->CurrentPreferredGhost)))  {
		GetWorld()->GetTimerManager().SetTimer(Player->CurrentPreferredGhost->GhostLeaveTimer, [this]() {
			Player->CurrentPreferredGhost->ExitOnTimer();
			}, Player->CurrentPreferredGhost->GhostLeaveTime, false);
	}
}


void APhantomPawn::FlashSkin() {
	if (IsFrightenedState()) {
		StaticMesh->SetMaterial(2, FlashingSkin);
	}
}