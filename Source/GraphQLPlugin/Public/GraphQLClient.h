#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpModule.h"

#include "GraphQLClient.generated.h"

UCLASS()
class UGraphQLClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGraphQLClient();

	void SendRequest(FString Query);

	void SetGraphQLURI(FString URI);

protected:
	void OnSendRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

private:
	FHttpModule* Http;

	FString GraphQLEndpoint;
};
