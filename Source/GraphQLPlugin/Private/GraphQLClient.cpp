#include "GraphQLClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"
#include <GraphQLRequestStruct.h>
#include <JsonObjectConverter.h>

UGraphQLClient::UGraphQLClient()
{
	Http = &FHttpModule::Get();
}

void UGraphQLClient::SendRequest(FString Query, FString Token)
{
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UGraphQLClient::OnSendRequestComplete);

	Request->SetURL(GraphQLEndpoint);
	Request->SetVerb("POST");
	Request->SetContentAsString(Query);
	Request->SetHeader(TEXT("User-Agent"), "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->SetHeader("Authorization", Token);

	Request->ProcessRequest();
}

void UGraphQLClient::SetGraphQLURI(FString URI)
{
	GraphQLEndpoint = URI;
}

void UGraphQLClient::OnSendRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	OnResponseEvent.Broadcast(*Response->GetContentAsString(), Response->GetResponseCode());
}

FString UGraphQLClient::MakeResponse(FString Response)
{
	TSharedPtr<FJsonObject> Json;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response);
	if (FJsonSerializer::Deserialize(Reader, Json))
	{
		TSharedPtr<FJsonObject> Data = Json->GetObjectField(FString("data"));
		FString Output;
		TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
		FJsonSerializer::Serialize(Data.ToSharedRef(), Writer);
		return Output;
	}
	return FString("");
}

FString UGraphQLClient::MakeRequest(FGraphQLRequestStruct Query, FString QueryName)
{
	/*TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
	Json->SetStringField(FString("query"), *LeftString + Data + *RightString);
	FString Output;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);*/
	FString Output;
	FJsonObjectConverter::UStructToJsonObjectString(Query, Output, 0, 0);
	return Output;
}