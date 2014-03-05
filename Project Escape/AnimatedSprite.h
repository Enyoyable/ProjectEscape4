#pragma once

#include "stdafx.h"

namespace esc
{

	class AnimatedSprite : public sf::Sprite
	{

	public:

		struct Frame
		{
			sf::IntRect m_placement;
			sf::Texture *m_texture;
			float m_fDuration;
		};

	public:
		AnimatedSprite();

		void update(float p_fDeltaTime);

		void play();

		void pause();

		void addFrame(Frame *p_xFrame);

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setCurrentFrame(int p_iCurrentFrame);

		int getCurrentFrame();

		int getFramesCount();

		void setAnimationRange(int p_iIndex1, int p_iIndex2);

		void setFramesToAnimate(std::vector<int> p_vFrames);

		void resetAnimationConditions();

	private:
		bool m_bIsPlaying;

		std::vector<Frame*> m_vFrames;

		std::vector<Frame*> m_vTotalFrames;

		int m_iCurrentFrame;

		float m_fTimeSinceLastFrame;

		bool m_bSpecialCondition;

		std::vector<int> m_vFramesToAnimate;
	};

}