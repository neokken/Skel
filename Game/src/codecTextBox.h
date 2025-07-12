#pragma once
#include "UIBase.h"

class ResourceManager;

enum class CodecTextBoxState
{
	Closed,
	Opening,
	Open,
	Closing
};




class CodecTextBox : public UIBase
{
public:
	CodecTextBox() = default;

	void Init(const ResourceManager& resourceManager);

	void Update(float deltaTime) override;

	void Draw(Surface& target, int2 offset) const override;


	void Open(bool instant = false);
	void Close(bool instant = false);

	CodecTextBoxState GetCurrentState() const;

	void SetText(Characters character, const char* text);

	void SkipDialogue();

	bool FinishedText() const;

private:
	enum class OpennessState
	{
		Closed,
		Line,
		Open,
	};

	void SetClosedTarget(float time);
	void SetLineTarget(float time);
	void SetOpenTarget(float time);


	AnimatedTextBox textBox;

	CodecTextBoxState currentState{ CodecTextBoxState::Closed };
	OpennessState opennessState{ OpennessState::Closed };

	const char* character{ nullptr };
	int characterLength{ 0 };

	const char* text{ nullptr };
	int textLength{ 0 };
	int currentChar{ 0 };	

	float nextCharCooldown{ TIME_PER_CHARACTER };


};


