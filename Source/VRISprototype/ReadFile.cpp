#include "VRISprototype.h"
#include "ReadFile.h"
#include "CoreMisc.h"

void UReadFile::ReadFile(FString Filename, FString& Result)
{
	FFileHelper::LoadFileToString(Result, *Filename);
}




