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
}

void APhantomPawn::BeginPlay()
{
	Super::BeginPlay();
	FVector2D StartNode = TheGridGen->GetXYPositionByRelativeLocation(GetActorLocation());
	LastNode = TheGridGen->TileMap[StartNode];

	Player = Cast<APacmanPawn>(UGameplayStatics::GetActorOfClass(GetWorld(), APacmanPawn::StaticClass()));

	GameMode = (ATestGridGameMode*)(GetWorld()->GetAuthGameMode());
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

			if ((GameInstance->GetLives()) > 0) {
				//play pacman dead sound
				UGameplayStatics::PlaySound2D(this, PacmanDeadSound);
				//respawn starting postion of pawns
				GameMode->RespawnPositions();
			}

			//se player non ha più vite->destroy
			else
			{
				//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("GAME OVER! YOU ARE DEAD!!!")));
				GameMode->IsGameOver = true;
				Pacman->Destroy();
				
				GameMode->BlinkyPawn->Destroy();
				GameMode->InkyPawn->Destroy();
				GameMode->PinkyPawn->Destroy();
				GameMode->ClydePawn->Destroy();

				GameMode->ShowGameOverScreen();
			}
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
	//if (CurrentGridCoords == (FVector2D(17, 13)) || CurrentGridCoords == (FVector2D(17, 12)) || CurrentGridCoords == (FVector2D(17, 14)) && (LastInputDirection.X<0 || LastValidInputDirection.X<0))
	//if(CurrentGridCoords == (FVector2D(17, 13)) && (LastInputDirection.X < 0 || LastValidInputDirection.X < 0))
	if(CurrentGridCoords == (FVector2D(17, 13)))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("trying to enter")));
		// Permetti il transito
		if (this->IsDeadState())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("enter granted")));
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
	if ((CurrentGridCoords == (FVector2D(15, 13)) || CurrentGridCoords == (FVector2D(15, 12)) || CurrentGridCoords == (FVector2D(15, 14))) && (LastInputDirection.X>0 || LastValidInputDirection.X>0)) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("trying to exit")));
		
		// Permetti il transito
		if (this->IsChaseState() || this->IsScatterState())
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("exit granted")));
		}
		// Blocca il transito
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("exit denied")));
			LastNode = (*(GridGenTMap.Find(FVector2D(15, 13))));
			//torna indietro
			FVector OppositeDirection = this->GetLastValidDirection();
			SetNextNodeByDir(OppositeDirection, true);
			SetTargetNode(NextNode);
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

void APhantomPawn::SetSpeed(float Speed)
{
	CurrentMovementSpeed = Speed;
}

AGridBaseNode* APhantomPawn::GetPlayerRelativeTarget()
{
	return Player->GetLastNode();
}

void APhantomPawn::SetGhostTarget()
{
	//chase state allora insegue player
	if (this->IsChaseState())
	{
		const AGridBaseNode* Target = GetPlayerRelativeTarget();
		if (!Target)
		{
			Target = GetPlayer()->GetLastNode();
		}

		AGridBaseNode* PossibleNode = TheGridGen->GetClosestNodeFromMyCoordsToTargetCoords(this->GetLastNodeCoords(), Target->GetGridPosition(), -(this->GetLastValidDirection()));

		//const FVector Dimensions(60, 60, 20);
		//DrawDebugBox(GetWorld(), PossibleNode->GetTileCoordinates(), Dimensions, FColor::Red);

		if (PossibleNode)
		{
			this->SetNextNodeByDir(TheGridGen->GetThreeDOfTwoDVector(PossibleNode->GetGridPosition() - this->GetLastNodeCoords()), true);
		}
	}
	if (this->IsDeadState())
	{
		//override della casella home per ciascun ghost
		this->GoHome();
	}

	if (this->IsIdleState())
	{
		this->ExitGhostArea();
	}

	if (this->IsScatterState()) {

		this->GoToHisCorner();
	}

	if (this->IsFrightenedState())
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
}

//in override in ciascun ghost
void APhantomPawn::RespawnGhostStartingPosition()
{
}

//in override in ciascun ghost
void APhantomPawn::GoHome()
{
}

void APhantomPawn::ExitGhostArea()
{
	this->SetIdleState();
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
		if (CurrentGridCoords == FVector2D(17, 13)) {
			return;
		}
	}
}

//funzione che inverte la direzione del fantasma, con un certo ritardo casuale
void APhantomPawn::ReverseDirection() {

	// Genera un valore casuale per il ritardo tra 0 e 1 secondo
	float Delay = FMath::FRandRange(0.f, 1.f);

	// Calcola la direzione opposta rispetto all'ultima direzione valida
	FVector OppositeDirection = -GetLastValidDirection();

	GetWorld()->GetTimerManager().SetTimer(DelayReverse, [this, OppositeDirection]() {
		SetNextNodeByDir(OppositeDirection, true);
	}, Delay, false);
}

void APhantomPawn::GoToHisCorner()
{
}

void APhantomPawn::ClearTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

void APhantomPawn::AlternateScatterChase(int GhostIndex)
{
	int& sequencePoint = sequencePoints[GhostIndex];  // Ottiene il punto della sequenza per il fantasma chiamante

	switch (sequencePoint)
	{
	case 1:
		SetScatterState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex);}, 7.0f, false);
		break;

	case 2:
		SetChaseState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 20.0f, false);
		break;

	case 3:
		SetScatterState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 7.0f, false);
		break;

	case 4:
		SetChaseState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 20.0f, false);
		break;

	case 5:
		SetScatterState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 5.0f, false);
		break;

	case 6:
		SetChaseState();
		sequencePoint++;		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 20.0f, false);
		break;

	case 7:
		SetScatterState();
		sequencePoint++;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, GhostIndex]() {AlternateScatterChase(GhostIndex); }, 5.0f, false);
		break;

	case 8:
		SetChaseState();
		// Rimani nello stato di chase per sempre
		break;

	default:
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
	this->SetSpeed(NormalMovementSpeed);
}

bool APhantomPawn::IsChaseState()
{
	if (this->EEnemyState == Chase) return true;

	else return false;
}

void APhantomPawn::SetScatterState()
{
	if (this->IsChaseState()) {
		//change direction
		this->ReverseDirection();
	}

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Scatter mode")));
	StaticMesh->SetMaterial(2, DefaultSkin);
	this->EEnemyState = Scatter;
	this->SetSpeed(NormalMovementSpeed);
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
	this->SetSpeed(400.f);
}

bool APhantomPawn::IsFrightenedState()
{
	if (this->EEnemyState == Frightened) return true;

	else return false;
}

void APhantomPawn::SetIdleState()
{
	StaticMesh->SetMaterial(2, DefaultSkin);
	this->EEnemyState = Idle;
}

bool APhantomPawn::IsIdleState()
{
	if (this->EEnemyState == Idle) return true;

	else return false;
}

void APhantomPawn::SetDeadState()
{
	StaticMesh->SetMaterial(2, DeadSkin);
	this->EEnemyState = Dead;
	this->SetSpeed(NormalMovementSpeed);
}

bool APhantomPawn::IsDeadState()
{

	if (this->EEnemyState == Dead) return true;

	else return false;
}
