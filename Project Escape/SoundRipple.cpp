#include "SoundRipple.h"
#include "Guard.h"

namespace esc
{
	SoundRipple::SoundRipple(sf::Vector2f p_v2fPosiiton, float p_fMinRadious, float p_fmaxRadious, float p_fDuration) : GameObject(p_v2fPosiiton, sf::Vector2f(0, 0), false, 999999, SOUNDRIPPLE)
	{
		m_fMinRadious = p_fMinRadious;
		m_fMaxRadious = p_fmaxRadious;
		m_fMaxDuration = p_fDuration;

		m_v2fOriginPosition = p_v2fPosiiton;

		m_v2fCurrentPos = new sf::Vector2f(p_v2fPosiiton);

		m_fCurrentDuration = 0;
		m_fCurrentRadious = m_fMinRadious;

		m_bIsUpdating = true;

		m_xRipple = nullptr;
	}

	SoundRipple::~SoundRipple()
	{
		delete m_xRipple;
		m_xRipple = nullptr;
	}

	void SoundRipple::update(float p_fDeltaTime)
	{
		if (!m_bIsUpdating)
			return;

		m_fCurrentDuration += p_fDeltaTime;

		float fMinMaxRadDelta = m_fMaxRadious - m_fMinRadious;

		float fCurrentDurationDiff = m_fCurrentDuration / m_fMaxDuration;

		float fCurrentRadDiff = fMinMaxRadDelta * fCurrentDurationDiff;

		m_fCurrentRadious = fCurrentRadDiff + m_fMinRadious;

		sf::Vector2f newPos(m_v2fOriginPosition - sf::Vector2f(m_fCurrentRadious, m_fCurrentRadious));

		delete m_v2fCurrentPos;

		m_v2fCurrentPos = new sf::Vector2f(newPos);

		if (m_fCurrentDuration > m_fMaxDuration || m_fCurrentRadious > m_fMaxRadious)
			destroy();

		setSize(sf::Vector2f(m_fCurrentRadious * 2, m_fCurrentRadious * 2));

		setOrigin(sf::Vector2f(m_fCurrentRadious, m_fCurrentRadious));

		delete m_xRipple;
		m_xRipple = nullptr;

		m_xRipple = new sf::CircleShape;

		m_xRipple->setRadius(m_fCurrentRadious);
		m_xRipple->setOutlineThickness(3.f);

		sf::Color fadingColor = sf::Color::Green;
		fadingColor.a = 1 - 255 * (m_fCurrentRadious) / m_fMaxRadious;

		m_xRipple->setOutlineColor(fadingColor);
		m_xRipple->setFillColor(sf::Color(0, 0, 0, 0));
		m_xRipple->setPointCount(100);

		float offset = sqrtf((m_fCurrentRadious * m_fCurrentRadious) / 2);

		m_v2fRipplePosition = sf::Vector2f(m_v2fOriginPosition - sf::Vector2f(m_fCurrentRadious, m_fCurrentRadious));

		m_xRipple->setPosition(m_v2fRipplePosition);
		
	}

	void SoundRipple::destroy()
	{
		m_bIsUpdating = false;
	}

	void SoundRipple::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		if (!m_bIsUpdating)
			return;
		
		if (m_xRipple == nullptr)
			return;
		

		target.draw(*m_xRipple);

		
	}

	void SoundRipple::HandleCollision(GameObject *p_xGameObject)
	{
		if (p_xGameObject->getType() == PATROLLINGGUARD || p_xGameObject->getType() == STATIONARYGUARD)
		{
			
		}
	}

	sf::Vector2f SoundRipple::getRipplePosition()
	{
		return m_v2fRipplePosition + sf::Vector2f(m_fCurrentRadious, m_fCurrentRadious);
	}

	float SoundRipple::getCurrentRadious()
	{
		return m_fCurrentRadious;
	}
}

