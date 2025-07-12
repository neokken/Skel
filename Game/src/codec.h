#pragma once
#include "UIBase.h" // whoawhaahjhahaha

class ResourceManager;
class TileSheet;
class Font;


enum class ConnectionState
{
	Disconnected,
	Connecting,
	Connected,
	Disconnecting
};


class Codec : public UIBase
{
public:
	Codec() = default;

	void Init(const ResourceManager& resourceManager);

	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offset) const override;

	void SetConnected();
	void SetDisconnected();

	void SetConnectDeconnectTime(float time);
	void SetSending();
	void SetReceiving();

	void SetFrequency(int frequency);
	int GetFrequency() const;

	void Connect();
	void Disconnect();



	ConnectionState GetConnectionState() const;

private:
	TileSheet* codecSprite{ nullptr };

	TileSheet* recvsendSprites{ nullptr };

	TileSheet* connectionSprites{ nullptr };

	Font* frequencyFont{ nullptr };


	bool isRecv{false};
	
	float connectDeconnectTime{ .5f };

	ConnectionState connectionState{ ConnectionState::Disconnected };
	float connectionProgress{ 0.f }; // goes from 0 till 1;

	int frequency{ 0 };
};

