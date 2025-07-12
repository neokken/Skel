#include "precomp.h"
#include "resourceManager.h"

#include "tilesheet.h"
#include "font.h"

#include "codec.h"

void Codec::Init(const ResourceManager& resourceManager)
{
	codecSprite = resourceManager.getTileSheet(TileSheets::Codec);
	recvsendSprites = resourceManager.getTileSheet(TileSheets::CodecRecvSend);

	connectionSprites = resourceManager.getTileSheet(TileSheets::CodecConnection);

	frequencyFont = resourceManager.getFont(Fonts::CodecFrequency);
		
}

void Codec::Update(float deltaTime)
{

	float connectionTime = .5f;

	if (connectionState == ConnectionState::Connecting)
	{
		connectionProgress += deltaTime * (1 / connectionTime);
		
		if (connectionProgress >= 1.f)
		{
			connectionProgress = 1.f;
			connectionState = ConnectionState::Connected;
		}

	}
	else if (connectionState == ConnectionState::Disconnecting)
	{
		connectionProgress -= deltaTime * (1 / connectionTime);

		if (connectionProgress <= 0.f)
		{
			connectionProgress = 0.f;
			connectionState = ConnectionState::Disconnected;
		}
	}

}

void Codec::Draw(Surface& target, int2 offset) const
{
	codecSprite->DrawTile(target, offset, 0);


	int connectionFrame = static_cast<int>(connectionProgress * 9);
	if (connectionFrame != 0)
		connectionSprites->DrawTile(target, offset + int2(9,58), connectionFrame - 1);



	recvsendSprites->DrawTile(target, offset + int2(29, 64), isRecv ? 0 : 1);



	// render frequency
	 
	int tens = frequency / 10;
	int ones = frequency % 10;

	frequencyFont->DrawTile(target, tens, offset+int2(37, 73));
	frequencyFont->DrawTile(target, ones, offset+int2(45, 73));

}

void Codec::SetConnected()
{
	connectionState = ConnectionState::Connected;
	connectionProgress = 1.f;
}

void Codec::SetDisconnected()
{
	connectionState = ConnectionState::Disconnected;
	connectionProgress = 0.f;
}

void Codec::SetConnectDeconnectTime(float time)
{
	connectDeconnectTime = time;
}

void Codec::SetSending()
{
	isRecv = false;
}

void Codec::SetReceiving()
{
	isRecv = true;
}

void Codec::SetFrequency(int _frequency)
{
	frequency = _frequency;
}

ConnectionState Codec::GetConnectionState() const
{
	return connectionState;
}

int Codec::GetFrequency() const
{
	return frequency;
}

void Codec::Connect()
{
	connectionState = ConnectionState::Connecting;
}

void Codec::Disconnect()
{
	connectionState = ConnectionState::Disconnecting;
}