#include "GraphQLClient.h"
#include "HttpModule.h"
#include "Interfaces/IHttpResponse.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

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

FString UGraphQLClient::MakeRequest(FString Query, TMap<FString, FString> Variables)
{
	FString Data = FString();
	for (auto& Variable : Variables) {
		if (!Data.IsEmpty()) {
			Data = Data.Append(FString(", "));
		}
		Data = Data.Append(Variable.Key + FString(": \"") + Variable.Value + FString("\""));
	}
	FString LeftString, RightString;
	Query.Split(FString("VARIABLES"), &LeftString, &RightString);
	TSharedPtr<FJsonObject> Json = MakeShareable(new FJsonObject);
	Json->SetStringField(FString("query"), *LeftString + Data + *RightString);
	FString Output;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Output);
	FJsonSerializer::Serialize(Json.ToSharedRef(), Writer);
	return Output;
}


