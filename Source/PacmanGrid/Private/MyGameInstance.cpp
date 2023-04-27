// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::setScore(int newvalue)
{
	this->Score = newvalue;
}

int UMyGameInstance::getScore()
{
	return Score;
}


int UMyGameInstance::GetLives()
{
	return Lives;
}

void UMyGameInstance::SetLives(int newvalue)
{
	this->Lives = newvalue;
}
