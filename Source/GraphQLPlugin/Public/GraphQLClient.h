#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HttpModule.h"

#include "GraphQLClient.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResponseEvent, FString, ResponseBody, int32, ResponseCode);

UCLASS()
class UGraphQLClient : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UGraphQLClient();

	UFUNCTION(BlueprintCallable)
	void SendRequest(FString Query);

	UFUNCTION(BlueprintCallable)
	void SetGraphQLURI(FString URI);

	UFUNCTION(BlueprintCallable)
	FString MakeRequest(FString Query, TMap<FString, FString> Variables);

	UFUNCTION(BlueprintCallable)
	FString MakeResponse(FString Response);

protected:
	void OnSendRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	UPROPERTY(BlueprintAssignable)
	FOnResponseEvent OnResponseEvent;

private:
	FHttpModule* Http;

	FString GraphQLEndpoint;
};
