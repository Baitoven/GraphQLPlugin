#include "GraphQLClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"

UGraphQLClient::UGraphQLClient()
{
	Http = &FHttpModule::Get();
}

void UGraphQLClient::SendRequest(FString Query)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGraphQLClient::OnSendRequestComplete);

	Request->SetURL(GraphQLEndpoint);
	Request->SetVerb("POST");
	Request->SetContentAsString(Query);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));

	Request->ProcessRequest();
	UE_LOG(LogTemp, Warning, TEXT("Request sent"));
}

void UGraphQLClient::SetGraphQLURI(FString URI)
{
	GraphQLEndpoint = URI;
}

void UGraphQLClient::OnSendRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("The response code is {%i}"), Response->GetResponseCode());
	FString ResponseString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("The response is {%s}"), *ResponseString);
}


