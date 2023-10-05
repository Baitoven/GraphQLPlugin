#include "GraphQLRequest.h"
#include "GraphQLClient.h"

UGraphQLRequest::UGraphQLRequest()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UGameInstance* GameInstance = World->GetGameInstance();
		if (GameInstance)
		{
			GraphQLClient = GameInstance->GetSubsystem<UGraphQLClient>();
		}
	}
}

void UGraphQLRequest::Send()
{
	GraphQLClient->SendRequest(FString(TEXT("")));
}
