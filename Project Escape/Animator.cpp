#include "Animator.h"
#include "SpriteManager.h"
#include "AnimatedSprite.h"
#include <fstream>

namespace esc
{

	Animator::Animator(SpriteManager *p_xSpriteManager, std::string p_sDirectoryPath)
	{
		m_xSpriteManager = p_xSpriteManager;
		m_sDirectoryPath = p_sDirectoryPath;
	}

	void Animator::loadAnimations(std::string p_sFilePath)
	{
		std::fstream stream(m_sDirectoryPath + p_sFilePath);

		int i = 0;
		while (stream.is_open())
		{
			std::string currentAnim;

			stream >> currentAnim;

			if (currentAnim.compare("") == 0)
				break;

			if (i++ == 0)
				m_sDefaultAnimation = currentAnim;

			AnimatedSprite *currentAnimation = m_xSpriteManager->loadAnimatedSprite(currentAnim);

			m_mAnimations.insert(std::pair<std::string, AnimatedSprite*>(currentAnim, currentAnimation));
		}
	}

	void Animator::update(float p_fDeltaTime)
	{

		if (m_vQueuedAnimations.size() > 0)
		{
			AnimatedSprite *currentAnimation = m_mAnimations.find(m_sCurrentAnimation)->second;

			if (currentAnimation->onLastFrame())
			{
				currentAnimation->update(p_fDeltaTime);

				if (!currentAnimation->onLastFrame())
				{
					m_sCurrentAnimation = m_vQueuedAnimations[0];

					m_vQueuedAnimations.erase(m_vQueuedAnimations.begin() + 1);
				}
			}
		}
		else if (m_mAnimations.find(m_sCurrentAnimation) != m_mAnimations.end())
		{
			m_mAnimations.find(m_sCurrentAnimation)->second->update(p_fDeltaTime);
		}
		else
		{
			m_mAnimations.find(m_sDefaultAnimation)->second->update(p_fDeltaTime);
		}
	}

	AnimatedSprite *Animator::getCurrentAnimation()
	{
		if (m_sCurrentAnimation.compare("") == 0)
			return m_mAnimations.find(m_sDefaultAnimation)->second;
		else
			return m_mAnimations.find(m_sCurrentAnimation)->second;
	}

	void Animator::playAnimationOnce(std::string p_sId)
	{
		m_xTempAnim = m_mAnimations.find(p_sId)->second;
	}

	void Animator::queueAnimation(std::string p_sId)
	{
		m_vQueuedAnimations.push_back(p_sId);
	}

	void Animator::setCurrentAnimation(std::string p_sId)
	{
		m_sCurrentAnimation = p_sId;
	}

}