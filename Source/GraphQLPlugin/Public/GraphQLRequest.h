#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphQLRequest.generated.h"

UCLASS()
class UGraphQLRequest : public UObject
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	void Send();

	UFUNCTION(BlueprintCallable)
	void Create();

protected:

private:
	FString GraphQLEndpoint{ TEXT("") };
	FString RequestText{ TEXT("") };

	class UGraphQLClient* GraphQLClient;
};
