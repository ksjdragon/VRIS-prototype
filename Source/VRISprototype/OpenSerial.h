// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "OpenSerial.generated.h"

UCLASS()
class VRISPROTOTYPE_API UOpenSerial : public UBlueprintFunctionLibrary
{
	GENERATED_BODY() public:
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Open Serial", Keywords = "Opens serial port"), Category = Utilities)
		static void OpenSerial(FString Port, FString Baud);
	
	
	
};
