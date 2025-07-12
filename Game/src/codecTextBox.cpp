#include "precomp.h"
#include "tileSheet.h"
#include "font.h"
#include "textBox.h"
#include "animatedTextBox.h"
#include "codecTextBox.h"

void CodecTextBox::Init(const ResourceManager& resourceManager)
{
	textBox.Init(resourceManager);
	SetClosedTarget(0.f);
}

void CodecTextBox::Update(float deltaTime)
{
	textBox.Update(deltaTime);

	if (currentState == CodecTextBoxState::Opening)
	{
		switch (opennessState)
		{
		case CodecTextBox::OpennessState::Closed:
			if (textBox.Finished())
			{
				SetLineTarget(.75f);
				opennessState = OpennessState::Line;
			}
			break;
		case CodecTextBox::OpennessState::Line:
			if (textBox.Finished())
			{
				SetOpenTarget(.5f);
				opennessState = OpennessState::Open;
			}
			break;
		case CodecTextBox::OpennessState::Open:
			if (textBox.Finished())
			{
				currentState = CodecTextBoxState::Open;
			}
			break;
		}
	


	}

	if (currentState == CodecTextBoxState::Closing)
	{
		switch (opennessState)
		{
		case CodecTextBox::OpennessState::Open:
			if (textBox.Finished())
			{
				SetLineTarget(1.f);
				opennessState = OpennessState::Line;
			}
			break;

		case CodecTextBox::OpennessState::Line:
			if (textBox.Finished())
			{
				SetClosedTarget(1.f);
				opennessState = OpennessState::Closed;
			}
			break;

		case CodecTextBox::OpennessState::Closed:
			if (textBox.Finished())
			{
				currentState = CodecTextBoxState::Closed;
			}
			break;
		}
	}

	if (currentState == CodecTextBoxState::Open && currentChar < textLength)
	{
		nextCharCooldown -= deltaTime;

		if (nextCharCooldown <= 0)
		{
			currentChar++;
			nextCharCooldown = TIME_PER_CHARACTER;
		}
	}

}

void CodecTextBox::Draw(Surface& target, int2 offset) const
{
	textBox.Draw(target, offset);


	if (currentState == CodecTextBoxState::Open)
	{
		int2 textOffset = { 2,3 };
		const Font* font = textBox.GetFont();

		if (character != nullptr && text != nullptr)
		{
			
			int2 charSize = font->GetCharacterSize();

			// draw 
			font->DrawCharacter(target, '[', offset + textOffset);

			font->DrawText(target, character, offset + textOffset + int2(charSize.x, 0));

			font->DrawCharacter(target, ']', offset + textOffset + int2(charSize.x * (1 + characterLength), 0));
		
			int2 startOffset = offset + textOffset + int2(charSize.x * (2 + characterLength),0);
			int2 dialogueTextOffset = int2(0, 0);
			for (int i = 0; i < currentChar; i++)
			{
				font->DrawCharacter(target, text[i], startOffset + dialogueTextOffset);
				if (text[i] == '\n')
				{
					dialogueTextOffset.x = 0;
					dialogueTextOffset.y += charSize.y+1;
				}
				else
				{
					dialogueTextOffset.x += charSize.x;
				}

			}
		}
		else
		{
			font->DrawText(target, "Dialogue Not Set", offset + textOffset);
		}
		
		if (currentChar == textLength)
		{
			font->DrawCharacter(target, '~', offset + int2(ROOM_WIDTH*TERRAIN_TILE_SIZE-10, 35));
		}

	}

}

void CodecTextBox::Open(bool instant)
{
	currentState = CodecTextBoxState::Opening;
	
	if (instant)
	{
		currentState = CodecTextBoxState::Open;
		opennessState = OpennessState::Open;
		SetOpenTarget(0.f);
	}

}

void CodecTextBox::Close(bool instant)
{
	currentState = CodecTextBoxState::Closing;

	if (instant)
	{
		currentState = CodecTextBoxState::Closed;
		opennessState = OpennessState::Closed;
		SetClosedTarget(0.f);
	}
}

CodecTextBoxState CodecTextBox::GetCurrentState() const
{
	return currentState;
}

void CodecTextBox::SetText(Characters _character, const char* _text)
{
	character = CharacterNames[static_cast<int>(_character)];
	characterLength = static_cast<int>(strlen(character));
	text = _text;
	textLength = static_cast<int>(strlen(text));
	currentChar = 0;

}

void CodecTextBox::SkipDialogue()
{
	currentChar = textLength;
}

bool CodecTextBox::FinishedText() const
{
	return textLength == currentChar;
}

void CodecTextBox::SetClosedTarget(float time)
{
	int2 position = int2(ROOM_WIDTH * TERRAIN_TILE_SIZE / 2, 0);
	textBox.SetTarget(position, position+int2(0,1), time);
}

void CodecTextBox::SetLineTarget(float time)
{
	textBox.SetTarget(int2(0, 0), int2(ROOM_WIDTH * TERRAIN_TILE_SIZE, 1), time);
}

void CodecTextBox::SetOpenTarget(float time)
{
	textBox.SetTarget(int2(0), int2(ROOM_WIDTH * TERRAIN_TILE_SIZE, 45), time);
}
