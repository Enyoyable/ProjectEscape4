#pragma once

#include "stdafx.h"

namespace esc
{
	
	class AnimatedSprite;
	class SpriteManager;

	class Animator
	{
	public:
		Animator(SpriteManager *p_xSpriteManager, std::string p_sDirectoryPath);

		void loadAnimations(std::string p_sFilePath);

		void update(float p_fdeltaTime);

		AnimatedSprite *getCurrentAnimation();

		void playAnimationOnce(std::string p_sId);

		void queueAnimation(std::string p_sId);

		void setCurrentAnimation(std::string p_sId);

		std::string getCurrentAnimationID();

		AnimatedSprite *getAnimation(std::string p_sID);

		void setForcedAnimation(std::string p_sID);

	private:
		std::map<std::string, AnimatedSprite*> m_mAnimations;

		std::string m_sCurrentAnimation;

		std::vector<std::string> m_vQueuedAnimations;

		SpriteManager *m_xSpriteManager;

		std::string m_sDirectoryPath;

		AnimatedSprite *m_xTempAnim;

		std::string m_sDefaultAnimation;

		AnimatedSprite *m_xForcedAnimation;

		std::string m_sForcedAnimation;
	};

}