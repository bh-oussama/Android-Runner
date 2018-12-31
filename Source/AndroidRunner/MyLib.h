// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EndlessGM.h"

/**
 * 
 */
class ANDROIDRUNNER_API MyLib
{
public:
	MyLib();
	~MyLib();

	static AEndlessGM* GetEndlessGM(UWorld* world);

};
