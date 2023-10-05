#include "GraphQLRequest.h"

void UGraphQLRequest::Send()
{
}

void UGraphQLRequest::Create()
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
