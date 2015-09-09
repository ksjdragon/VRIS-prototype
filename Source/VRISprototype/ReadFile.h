#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadFile.generated.h"

UCLASS()
class VRISPROTOTYPE_API UReadFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY() public:
		UFUNCTION(BlueprintCallable, meta = (DisplayName = "Read File", Keywords = "Reads from a file"), Category = Utilities)
		static void ReadFile(FString Directory, FString& Value);
};
