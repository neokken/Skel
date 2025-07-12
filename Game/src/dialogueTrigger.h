#pragma once
#include "levelTrigger.h"


class DialogueManager;

class DialogueTrigger : public LevelTrigger
{
public:
	DialogueTrigger() = delete;
	DialogueTrigger(const AABB& aabb, Level& level, Player& player, bool _crawlActivatable, DialogueManager* dialogueManager, const CodecDialogue* dialogue);

	void ActivateTrigger() override;

	void SetUsed();

private:
	bool used{ false };
	DialogueManager* dialogueManager{ nullptr };
	const CodecDialogue* dialogue{ nullptr };
};

