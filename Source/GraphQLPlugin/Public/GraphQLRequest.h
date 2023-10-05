#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphQLRequest.generated.h"

UCLASS()
class UGraphQLRequest : public UObject
{
	GENERATED_BODY()
public:
	UGraphQLRequest();

	UFUNCTION(BlueprintCallable)
	void Send();

protected:

private:
	FString GraphQLEndpoint{ TEXT("") };
	FString RequestText{ TEXT("") };

	UGraphQLClient* GraphQLClient;
};
