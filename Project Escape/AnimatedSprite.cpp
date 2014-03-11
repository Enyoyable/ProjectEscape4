#include "AnimatedSprite.h"

namespace esc
{

	AnimatedSprite::AnimatedSprite()
	{
		m_fTimeSinceLastFrame = 0;
		m_bIsPlaying = true;
		m_iCurrentFrame = 0;
	}

	void AnimatedSprite::update(float p_fDeltaTime)
	{
		if (m_bIsPlaying)
		{
			m_fTimeSinceLastFrame += p_fDeltaTime;

			if (m_fTimeSinceLastFrame > m_vFrames[m_iCurrentFrame]->m_fDuration)
			{
				m_iCurrentFrame = (m_iCurrentFrame + 1) % m_vFrames.size();
				m_fTimeSinceLastFrame = 0;
			}
		}
	}

	void AnimatedSprite::play()
	{
		m_bIsPlaying = true;
	}

	void AnimatedSprite::pause()
	{
		m_bIsPlaying = false;
	}

	void AnimatedSprite::addFrame(Frame *p_xFrame)
	{
		m_vFrames.push_back(p_xFrame);
		m_vTotalFrames.push_back(p_xFrame);
	}

	void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Sprite sprite(*m_vFrames[m_iCurrentFrame]->m_texture, m_vFrames[m_iCurrentFrame]->m_placement);
		states.transform *= getTransform();
		target.draw(sprite, states);
	}

	void AnimatedSprite::setCurrentFrame(int p_iCurrentFrame)
	{
		m_iCurrentFrame = p_iCurrentFrame;
	}

	int AnimatedSprite::getCurrentFrame()
	{
		return m_iCurrentFrame;
	}

	int AnimatedSprite::getFramesCount()
	{
		return m_vFrames.size();
	}

	void AnimatedSprite::setAnimationRange(int p_iIndex1, int p_iIndex2)
	{
		m_vFrames.clear();

		for (int i = p_iIndex1; i <= p_iIndex2; i++)
		{
			m_vFrames.push_back(m_vTotalFrames[i]);
		}
	}

	void AnimatedSprite::setFramesToAnimate(std::vector<int> p_vFrames)
	{
		m_vFrames.clear();

		for (auto frame : p_vFrames)
		{
			m_vFrames.push_back(m_vTotalFrames[frame]);
		}
	}

	void AnimatedSprite::resetAnimationConditions()
	{
		m_vFrames = m_vTotalFrames;
	}

	bool AnimatedSprite::onLastFrame()
	{
		return m_iCurrentFrame == (m_vFrames.size() - 1);
	}

}